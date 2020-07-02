#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ThaiBasilAudioProcessor::ThaiBasilAudioProcessor()
    : AudioProcessor(BusesProperties().withInput("Input", AudioChannelSet::stereo())
        .withOutput("Output", AudioChannelSet::stereo())),
    //compressorProc(44100),
    vts(*this, nullptr, Identifier("Parameters"), createParameterLayout()),
    oversampling(2, 3, dsp::Oversampling<float>::filterHalfBandPolyphaseIIR)
{
    limiter.setThreshold(-3.0f);
    limiter.setRelease(100.0f);

    //WaveFolder Param Tree Pointers
    freqParam = vts.getRawParameterValue("freq");
    depthParam = vts.getRawParameterValue("depth");
    //ffParam = vts.getRawParameterValue("feedforward");
    fbParam = vts.getRawParameterValue("feedback");
    satParam = vts.getRawParameterValue("sat");
    waveParam = vts.getRawParameterValue("wave"); 
    
    // Subharmonic Generator Param Tree Pointers
    shgPreCutoffParam = vts.getRawParameterValue("shgPreCutoff");
    shgPostCutoffParam = vts.getRawParameterValue("shgPostCutoff");
    //shgMainGainParam = vts.getRawParameterValue("shgMainGain");
    //shgSideGainParam = vts.getRawParameterValue("shgSideGain");
    outGainParam = vts.getRawParameterValue("outGain");
    //shgAttackParam = vts.getRawParameterValue("shgAttack");
    //shgReleaseParam = vts.getRawParameterValue("shgRelease");

    // Compressor Parameters
    thresholdDBParameter = vts.getRawParameterValue("compThreshold");
    ratioParameter = vts.getRawParameterValue("compRatio");
    outputGainDBParameter = vts.getRawParameterValue("compOutputGain");

    //Stereo Effect Param Tree Pointers
    stereoWidthParam = vts.getRawParameterValue("stereoWidth");
    //stereoOnParam = vts.getRawParameterValue("stereoOn");
}

AudioProcessorValueTreeState::ParameterLayout ThaiBasilAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;

    // WaveFolder Params
    /*params.push_back(std::make_unique<AudioParameterFloat>("freq", "Freq", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<AudioParameterFloat>("depth", "Depth", 0.0f, 0.5f, 0.1f));
    params.push_back(std::make_unique<AudioParameterFloat>("feedback", "Feedback", 0.0f, 0.9f, 0.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("feedforward", "Feedforward", 0.0f, 1.0f, 1.0f));

    //params.push_back(std::make_unique<AudioParameterInt>("sat", "Saturator", SatType::none, SatType::ahypsin, SatType::none));
    //params.push_back(std::make_unique<AudioParameterInt>("wave", "Wave", WaveType::zero, WaveType::sine, WaveType::zero));
    
    // Subharmonic Params
    NormalisableRange<float> freqRange(20.0f, 20000.0f);
    freqRange.setSkewForCentre(1000.0f);

    NormalisableRange<float> attackRange(0.1f, 1000.0f);
    attackRange.setSkewForCentre(10.0f);

    NormalisableRange<float> releaseRange(1.0f, 3000.0f);
    releaseRange.setSkewForCentre(100.0f);

    params.push_back(std::make_unique<AudioParameterFloat>("shgPreCutoff", "PreCutoff", freqRange, 500.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("shgPostCutoff", "PostCutoff", freqRange, 20000.0f));
    //params.push_back(std::make_unique<AudioParameterFloat>("shgMainGain", "MainGain", -60.0f, 30.0f, -4.0f));
    //params.push_back(std::make_unique<AudioParameterFloat>("shgSideGain", "SideGain", -60.0f, 30.0f, -4.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("outGain", "OutGain", -50.0f, 20.0f, 0.0f));
    //params.push_back(std::make_unique<AudioParameterFloat>("shgAttack", "Attack", attackRange, 10.0f));
    //params.push_back(std::make_unique<AudioParameterFloat>("shgRelease", "Release", releaseRange, 100.0f));

    // Compressor Params
    NormalisableRange<float> thresholdRange(-60.0f, 6.0f);
    NormalisableRange<float> ratioRange(1.0f, 60.0f);
    NormalisableRange<float> outputGainRange(0.0f, 20.0f);
    
    params.push_back(std::make_unique<AudioParameterFloat>("compThreshold", "Threshold", thresholdRange, 1.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("compRatio", "Ratio", ratioRange, 1.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("compOutputGain", "Output Gain", outputGainRange, 1.0f));

    // Stereo Effect Parameter
    params.push_back(std::make_unique<AudioParameterBool>("stereoOn", "Widen",false));
   
    return { params.begin(), params.end() };
}

ThaiBasilAudioProcessor::~ThaiBasilAudioProcessor()
{
}

const String ThaiBasilAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ThaiBasilAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ThaiBasilAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ThaiBasilAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ThaiBasilAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ThaiBasilAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ThaiBasilAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ThaiBasilAudioProcessor::setCurrentProgram (int index)
{
}

const String ThaiBasilAudioProcessor::getProgramName (int index)
{
    return {};
}

void ThaiBasilAudioProcessor::changeProgramName (int index, const String& newName)
{
}

void ThaiBasilAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    //DBG("50db to gain is "<<Decibels::decibelsToGain(50.f));
    oversampling.initProcessing(samplesPerBlock);

    limiter.prepare({sampleRate, static_cast<juce::uint32>(samplesPerBlock), 2});

    //WaveFolder Processing
    /*wfProc[0].reset((float)sampleRate * (float)oversampling.getOversamplingFactor());
    wfProc[1].reset((float)sampleRate * (float)oversampling.getOversamplingFactor());*/

    //not used at the moment
    auto oversampledRate = (float)sampleRate * (float)oversampling.getOversamplingFactor();

    for (int ch = 0; ch < 2; ++ch)
    {
        //Subharmonic Processing
        subProc[ch].reset(sampleRate);
        wfProc[ch].reset(sampleRate);

        drive[ch].prepare();
        dryGain[ch].prepare();
        wetGain[ch].prepare();
        outGain[ch].prepare();

        //preEQ[ch].reset(sampleRate);
        //preEQ[ch].setEqShape(EqShape::lowPass);
        //preEQ[ch].toggleOnOff(true);

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

void ThaiBasilAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    oversampling.reset();
    limiter.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ThaiBasilAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void ThaiBasilAudioProcessor::updateParams()
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

        //WaveFolder Param Updates
        
       // wfProc[ch].setFreq(*freqParam);
        //wfProc[ch].setDepth(*depthParam);
        //wfProc[ch].setFF(ffParam); //we finalized this at 1.0f
        //wfProc[ch].setFB(*fbParam);
        //wfProc[ch].setSatType(static_cast<SatType> ((int)*satParam));
        //wfProc[ch].setWaveType(static_cast<WaveType> ((int)*waveParam));

        

        //subProc[ch].setDetector(*shgAttackParam, *shgReleaseParam);
        subProc[ch].setDetector(shgAttack, shgRelease);

        //preEQ[ch].setFrequency(*shgPreCutoffParam);
        //preEQ[ch].setQ(butterQs[1]);

        //delay[ch].setActive(*stereoOnParam);

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

void ThaiBasilAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;

    sidechainBuffer.makeCopyOf(buffer, true);
    //sidechainBuffer = buffer; //just gonna point straight to original buffer to test oversampling

    //Oversample only sidechain buffer
    //dsp::AudioBlock<float> block(buffer);   //sidechainBuffer);
    //dsp::AudioBlock<float> osBlock(buffer);         //sidechainBuffer);
    //osBlock = oversampling.processSamplesUp(block);
    //float* ptrArray[] = { osBlock.getChannelPointer(0), osBlock.getChannelPointer(1) };
    //AudioBuffer<float> osSideBuffer(ptrArray, 2, static_cast<int> (osBlock.getNumSamples()));
    
    updateParams();
    //sidechainBuffer.makeCopyOf(buffer, true);
    const int numSamples = buffer.getNumSamples(); //not sure if this is using right buffer

    //for (int ch = 0; ch < buffer.getNumChannels();ch++) //osBuffer.getNumChannels(); ++ch)
    //{

        
        //Subharmonic
        //auto main = buffer.getWritePointer(ch);
        //auto side = sidechainBuffer.getWritePointer(ch);

        //preEQ[ch].processBlock(side, numSamples);
        //subProc[ch].processBlock(side, numSamples);
        //for (int i = 0; i < 3; ++i)
        //    postEQ[i][ch].processBlock(side, numSamples);
        ////delay stereo effect

        //delay[ch].processBlock(side, numSamples);

        //dcBlocker[ch].processBlock(side, numSamples);

        //mainGain[ch].processBlock(main, numSamples);
        //sideGain[ch].processBlock(side, numSamples);



        //buffer.addFrom(ch, 0, sidechainBuffer, ch, 0, numSamples);
    //}

    //oversampling.processSamplesDown(block);

    juce::dsp::AudioBlock<float> block(buffer, 0);

    juce::dsp::ProcessContextReplacing<float> context(block);
    limiter.process(context);
}

bool ThaiBasilAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ThaiBasilAudioProcessor::createEditor()
{
    return new ThaiBasilAudioProcessorEditor (*this);
}

void ThaiBasilAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = vts.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void ThaiBasilAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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
    return new ThaiBasilAudioProcessor();
}
