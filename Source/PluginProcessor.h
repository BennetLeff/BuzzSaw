/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include"WavefolderProcessor.h"
#include "SubharmonicProcessor.h"
#include "EQFilter.h"
#include "Gain.h"
#include "Delay.h"

//==============================================================================
/**
*/
class ThaiBasilAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    ThaiBasilAudioProcessor();
    ~ThaiBasilAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    AudioProcessorValueTreeState& getVTS() { return vts; }
    static AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    void updateParams();

    //old gain controls
	//float gain;
    //float preGain;
private:
    //==============================================================================
    AudioProcessorValueTreeState vts;
    
    /*
    std::atomic<float>* freqParam;
    std::atomic<float>* depthParam;
    std::atomic<float>* ffParam;
    std::atomic<float>* fbParam;
    std::atomic<float>* satParam;
    std::atomic<float>* waveParam;
    */

    //Subharmonic Generator Params
    std::atomic<float>* shgPreCutoffParam;
    std::atomic<float>* shgPostCutoffParam;
    std::atomic<float>* shgMainGainParam;
    std::atomic<float>* shgSideGainParam;
    std::atomic<float>* shgAttackParam;
    std::atomic<float>* shgReleaseParam;
    
    //stereo params
    std::atomic<float>* stereoOnParam;

    const float butterQs[3] = { 0.51763809f, 0.70710678f, 1.93185165f };

    //stereo effect comes from 2 slightly out of sync channels
    const float leftDelayTime = 0.04;
    const float rightDelayTime = 0.07;
    const float feedback = 0.1;
    const float wetLevel = 0.5;
    const float dryLevel = 0.5;

    AudioBuffer<float> sidechainBuffer;

    SubharmonicProcessor subProc[2];
    EQFilter preEQ[2];
    EQFilter postEQ[3][2];
    EQFilter dcBlocker[2];
    Gain mainGain[2];
    Gain sideGain[2];
    Delay delay[2];






    

    dsp::Oversampling<float> oversampling;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThaiBasilAudioProcessor)
};
