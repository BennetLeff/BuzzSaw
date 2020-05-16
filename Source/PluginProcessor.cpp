/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ThaiBasilAudioProcessor::ThaiBasilAudioProcessor()
     : AudioProcessor(BusesProperties().withInput("Input", AudioChannelSet::stereo()) 
		 .withOutput("Output", AudioChannelSet::stereo())),
    vts(*this, nullptr, Identifier("Parameters"), createParameterLayout()),
    oversampling(2, 3, dsp::Oversampling<float>::filterHalfBandPolyphaseIIR)
{
	// addParameter(gain = new AudioParameterFloat("Gain", "Gain", 0, 6.0, 1.0));
	gain = 1.0;
    preGain = 1.0;

    freqParam = vts.getRawParameterValue("freq");
    depthParam = vts.getRawParameterValue("depth");
    ffParam = vts.getRawParameterValue("feedforward");
    fbParam = vts.getRawParameterValue("feedback");
    satParam = vts.getRawParameterValue("sat");
    waveParam = vts.getRawParameterValue("wave");
}

AudioProcessorValueTreeState::ParameterLayout ThaiBasilAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;

    params.push_back(std::make_unique<AudioParameterFloat>("freq", "Freq", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<AudioParameterFloat>("depth", "Depth", 0.0f, 0.5f, 0.1f));
    params.push_back(std::make_unique<AudioParameterFloat>("feedback", "Feedback", 0.0f, 0.9f, 0.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("feedforward", "Feedforward", 0.0f, 1.0f, 1.0f));

    params.push_back(std::make_unique<AudioParameterInt>("sat", "Saturator", SatType::none, SatType::ahypsin, SatType::none));
    params.push_back(std::make_unique<AudioParameterInt>("wave", "Wave", WaveType::zero, WaveType::sine, WaveType::zero));

    return { params.begin(), params.end() };
}

ThaiBasilAudioProcessor::~ThaiBasilAudioProcessor()
{
}

//==============================================================================
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

//==============================================================================
void ThaiBasilAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    oversampling.initProcessing(samplesPerBlock);

    wfProc[0].reset((float)sampleRate * (float)oversampling.getOversamplingFactor());
    wfProc[1].reset((float)sampleRate * (float)oversampling.getOversamplingFactor());
}

void ThaiBasilAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    oversampling.reset();
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

void ThaiBasilAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;

    dsp::AudioBlock<float> block(buffer);
    dsp::AudioBlock<float> osBlock(buffer);

    osBlock = oversampling.processSamplesUp(block);

    float* ptrArray[] = { osBlock.getChannelPointer(0), osBlock.getChannelPointer(1) };
    AudioBuffer<float> osBuffer(ptrArray, 2, static_cast<int> (osBlock.getNumSamples()));

    for (int ch = 0; ch < osBuffer.getNumChannels(); ++ch)
    {
        wfProc[ch].setFreq(*freqParam);
        wfProc[ch].setDepth(*depthParam);
        wfProc[ch].setFF(*ffParam);
        wfProc[ch].setFB(*fbParam);
        wfProc[ch].setSatType(static_cast<SatType> ((int)*satParam));
        wfProc[ch].setWaveType(static_cast<WaveType> ((int)*waveParam));

        wfProc[ch].processBlock(osBuffer.getWritePointer(ch), osBuffer.getNumSamples());
    }

    oversampling.processSamplesDown(block);
}

//==============================================================================
bool ThaiBasilAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ThaiBasilAudioProcessor::createEditor()
{
    return new ThaiBasilAudioProcessorEditor (*this);
}

//==============================================================================
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

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ThaiBasilAudioProcessor();
}
