/*
  ==============================================================================

    WavefolderProcessor.h
    Created: 15 May 2020 4:32:57pm
    Author:  Bennet

  ==============================================================================
*/

#pragma once

#include "Saturators.h"
#include "Waves.h"

class WavefolderProcessor
{
public:
    WavefolderProcessor();

    void reset(float sampleRate);
    void processBlock(float* buffer, int numSamples);

    void setFreq(float newFreq) { freq.setTargetValue(newFreq); }
    void setDepth(float newDepth) { depth.setTargetValue(newDepth); }
    void setFF(float newFF) { feedforward.setTargetValue(newFF); }
    void setFB(float newFB) { feedback.setTargetValue(newFB); }
    void setSatType(SatType type) { ffSat = Saturators::getSaturator(type); }
    void setWaveType(WaveType type) { wave = Waves::getWave(type); }

private:
    float fs = 44100.0f;
    SatFunc ffSat = Saturators::getSaturator(SatType::ahypsin); //removed box
    WaveFunc wave = Waves::getWave(WaveType::zero); //removed box (no waves, could set this to something else later)

    SmoothedValue<float, ValueSmoothingTypes::Linear> freq = 0.5f;//removed slider
    SmoothedValue<float, ValueSmoothingTypes::Linear> depth = 0.1f; //removed slider
    SmoothedValue<float, ValueSmoothingTypes::Linear> feedforward = 1.0f; //doesnt need to be a smoothed value, we removed its slider
    SmoothedValue<float, ValueSmoothingTypes::Linear> feedback = 0.0f; //removed slider

    float y1 = 0.0f;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WavefolderProcessor)
};