/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BuzzSawAudioProcessor::BuzzSawAudioProcessor()
     : AudioProcessor(BusesProperties().withInput("Input", AudioChannelSet::stereo()) 
		 .withOutput("Output", AudioChannelSet::stereo())),
    vts(*this, nullptr, Identifier("Parameters"), createParameterLayout()),
    oversampling(2, 3, dsp::Oversampling<float>::filterHalfBandPolyphaseIIR)
{
    
    //Standard Param Tree Pointers
    mainBlend = vts.getRawParameterValue("mainBlend"); //blend % is same as wet level, dry level is (100 - wet level)
    driveParam = vts.getRawParameterValue("drive");

    //WaveFolder Param Tree Pointers
    freqParam = vts.getRawParameterValue("freq");
    depthParam = vts.getRawParameterValue("depth");
    //ffParam = vts.getRawParameterValue("feedforward");
    fbParam = vts.getRawParameterValue("feedback");
    satParam = vts.getRawParameterValue("sat");
    waveParam = vts.getRawParameterValue("wave"); 
    
    //WaveFolder Param Tree Pointers
    shgPreCutoffParam = vts.getRawParameterValue("shgPreCutoff");
    shgPostCutoffParam = vts.getRawParameterValue("shgPostCutoff");
    outGainParam = vts.getRawParameterValue("outGain");


    //Stereo Effect Param Tree Pointers
    stereoWidthParam = vts.getRawParameterValue("stereoWidth");
}

AudioProcessorValueTreeState::ParameterLayout BuzzSawAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;

    //standard params
    params.push_back(std::make_unique<AudioParameterFloat>("drive", "Drive", 0.0f, 100.0f, 0.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("mainBlend", "Blend", 0.0f, 100.0f, 50.0f));

    
    //Subharmonic Params

    NormalisableRange<float> freqRange(20.0f, 20000.0f);
    freqRange.setSkewForCentre(1000.0f);

    NormalisableRange<float> attackRange(0.1f, 1000.0f);
    attackRange.setSkewForCentre(10.0f);

    NormalisableRange<float> releaseRange(1.0f, 3000.0f);
    releaseRange.setSkewForCentre(100.0f);

    params.push_back(std::make_unique<AudioParameterFloat>("shgPreCutoff", "PreCutoff", freqRange, 500.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("shgPostCutoff", "PostCutoff", freqRange, 20000.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("outGain", "OutGain", -50.0f, 20.0f, 0.0f));

    params.push_back(std::make_unique<AudioParameterFloat>("stereoWidth", "StereoWidth", 0.0, 3, 0.0));//10x the actual range (for step size)
   
    return { params.begin(), params.end() };
}

BuzzSawAudioProcessor::~BuzzSawAudioProcessor()
{
}

const String BuzzSawAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BuzzSawAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BuzzSawAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BuzzSawAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BuzzSawAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BuzzSawAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BuzzSawAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BuzzSawAudioProcessor::setCurrentProgram (int index)
{
}

const String BuzzSawAudioProcessor::getProgramName (int index)
{
    return {};
}

void BuzzSawAudioProcessor::changeProgramName (int index, const String& newName)
{
}

void BuzzSawAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    limiter.prepare({ sampleRate, static_cast<juce::uint32>(samplesPerBlock), 2 });
    limiter.setThreshold(-3.0f);
    limiter.setRelease(100.0f);

    //Oversampling (NOT CURRENTLY IMPLEMENTED)
    //oversampling.initProcessing(samplesPerBlock);
    //auto oversampledRate = (float)sampleRate * (float)oversampling.getOversamplingFactor();

    for (int ch = 0; ch < 2; ++ch)
    {
        //Subharmonic Processing
        subProc[ch].reset(sampleRate);
        wfProc[ch].reset(sampleRate);

        drive[ch].prepare();
        dryGain[ch].prepare();
        wetGain[ch].prepare();
        outGain[ch].prepare();

        dcBlocker[ch].reset(sampleRate);
        dcBlocker[ch].setEqShape(EqShape::highPass);
        dcBlocker[ch].setFrequency(35.0f);
        dcBlocker[ch].setQ(0.7071f);

        //delay
        delay[ch].initialize(sampleRate);
        delay[ch].setWetLevel(delayWetLevel);
        delay[ch].setDryLevel(delayDryLevel);
        delay[ch].setFeedback(feedback);

        //Biquad version of filtering block
        /*for (int i = 0; i < 3; ++i)
        {
            postEQ[i][ch].reset(sampleRate);
            postEQ[i][ch].setEqShape(EqShape::lowPass);
            postEQ[i][ch].toggleOnOff(true);
        }*/

        //built-in IIR vesion of filtering block
        postEQ[ch].reset();
        postEQ[ch].setCoefficients(IIRCoefficients::makeLowPass(sampleRate, 20000)); //lowpass at max cutoff
    }

    //other delay settings
    delay[0].setDelaySec(leftDelayTime);
    delay[1].setDelaySec(rightDelayTime);



    sidechainBuffer.setSize(2, samplesPerBlock);
}

void BuzzSawAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    oversampling.reset();
    limiter.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BuzzSawAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void BuzzSawAudioProcessor::updateParams()
{
    for (int ch = 0; ch < 2; ++ch)
    {


         mainWetLevel = (*mainBlend)/100.0f;
         mainDryLevel = (1 - mainWetLevel);

        drive[ch].setGain(Decibels::decibelsToGain(driveParam->load()));
        //need a multiplier on levels?
        dryGain[ch].setGain(mainDryLevel);//Decibels::decibelsToGain(shgMainGainParam->load()));
        wetGain[ch].setGain(mainWetLevel);//Decibels::decibelsToGain(shgSideGainParam->load()));
        outGain[ch].setGain(Decibels::decibelsToGain(outGainParam->load()));
        
        //Static attack and release
        subProc[ch].setDetector(shgAttack, shgRelease);


        //Biquad version of filtering block
        /*
        for (int i = 0; i < 3; ++i)
        {
            postEQ[i][ch].setFrequency(*shgPostCutoffParam);
            postEQ[i][ch].setQ(butterQs[i]);
        }*/
        
        //built-in IIR version of filtering block
        postEQ[ch].setCoefficients(IIRCoefficients::makeLowPass(getSampleRate(), *shgPostCutoffParam));
    }

    //set L & R delay times slightly apart from each other
    delay[0].setDelaySec(*stereoWidthParam/100);
    delay[1].setDelaySec((*stereoWidthParam /100)*0.9);
}

void BuzzSawAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;

    sidechainBuffer.makeCopyOf(buffer, true);

    //Oversampling processing (NOT CURRENTLY WORKING)
    //sidechainBuffer = buffer;
    //dsp::AudioBlock<float> block(buffer);   //sidechainBuffer);
    //dsp::AudioBlock<float> osBlock(buffer);         //sidechainBuffer);
    //osBlock = oversampling.processSamplesUp(block);
    //float* ptrArray[] = { osBlock.getChannelPointer(0), osBlock.getChannelPointer(1) };
    //AudioBuffer<float> osSideBuffer(ptrArray, 2, static_cast<int> (osBlock.getNumSamples()));
    
    updateParams();
    const int numSamples = buffer.getNumSamples(); 

    for (int ch = 0; ch < buffer.getNumChannels();ch++) 
    {
        
        auto main = buffer.getWritePointer(ch);
        auto side = sidechainBuffer.getWritePointer(ch); 

        //Subharmonic
        subProc[ch].processBlock(side, numSamples);

        drive[ch].processBlock(side, numSamples);

        //balance dry/wet volumes
        if (*driveParam < 30) {
            drive[ch].processBlock(main, numSamples);
        } else {
            for (int i = 0; i < numSamples; i++) {
                main[i] *= Decibels::decibelsToGain(30.f);
            }
        }


        //wavefolder
        wfProc[ch].processBlock(side,numSamples);

        //Biquad version
       /*
        for (int i = 0; i < 3; ++i) {
            postEQ[i][ch].processBlock(side, numSamples);
        }*/

        //built-in IIR version
        postEQ[ch].processSamples(side, numSamples);

        //delay stereo effect
        delay[ch].processBlock(side, numSamples);

        dcBlocker[ch].processBlock(side, numSamples);

        //balance
        wetGain[ch].processBlock(side, numSamples);
        dryGain[ch].processBlock(main, numSamples);


        //sum
        buffer.addFrom(ch, 0, sidechainBuffer, ch, 0, numSamples);
        outGain[ch].processBlock(buffer.getWritePointer(ch),numSamples);

    }
    
    //Limit (will add some extra distortion)
    juce::dsp::AudioBlock<float> block(buffer, 0);
    juce::dsp::ProcessContextReplacing<float> context(block);
    limiter.process(context);

    //oversampling.processSamplesDown(block);

}

bool BuzzSawAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* BuzzSawAudioProcessor::createEditor()
{
    return new BuzzSawAudioProcessorEditor (*this);
}

void BuzzSawAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = vts.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void BuzzSawAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(vts.state.getType()))
            vts.replaceState(ValueTree::fromXml(*xmlState));
}

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BuzzSawAudioProcessor();
}
