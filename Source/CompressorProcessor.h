/*
  ==============================================================================

    CompressorProcessor.h
    Created: 27 May 2020 2:16:32pm
    Author:  Bennet

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class CompressorProcessor
{
public:
    CompressorProcessor(float sampleRate);

    void reset(float sampleRate);
    void processBlock(float* buffer, int numSamples);
    void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages);
    
    void setAttack(float attack) { attack = attack; }
    void setRelease(float release) { release = release; }
    void setRatio(float ratio) { ratio = ratio;  }
    void setThreshold(float threshold) { threshold = threshold;  }
private:
    // in seconds
    float attack = 0.1;
    float release = 0.25;

    // Any samples above this volume will be compressed. 
    float threshold = -3.0;

    // The ratio determines the factor of gain reduction.
    // A ratio of n or n:1 will cause a sample n dB over the threshold
    // to be attenuated to 1 dB over the threshold.
    // For a ratio of 4:1, a sample 4 dB over the threshold is attenuated
    // to 1 dB over the threshold. A sample of 8 dB will be attenuated to
    // 3 dB over the threshold.
    float ratio = 4.0;

    float sampleRate = 44100.0f;

    // Controls the attack and release of the compressor.
    // Attack begins when the first sample above the threshold is reached.
    // No decay. Sustain while above threshold.
    // Release once the first sample below the threshold is reached.
    ADSR envelope;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CompressorProcessor)
};