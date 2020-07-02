/*
  ==============================================================================

    CompressorProcessor.cpp
    Created: 27 May 2020 2:16:32pm
    Author:  Bennet

  ==============================================================================
*/

#include "CompressorProcessor.h"


CompressorProcessor::CompressorProcessor(AudioProcessorValueTreeState& state, float sampleRate)
    : state(state)
{
    envelope.setSampleRate(sampleRate);

    // Set the ADSR parameters via struct from the ADSR class.
    ADSR::Parameters adsrParameters{this->attack, 0.001f, 1.0f, this->release};
    envelope.setParameters(adsrParameters);

    threshold = state.getRawParameterValue("compThreshold");
    ratio = state.getRawParameterValue("compRatio");
    outputGain = state.getRawParameterValue("compOutputGain");
}

void CompressorProcessor::reset(float sampleRate)
{
    sampleRate = sampleRate;
    envelope.reset();
    envelope.setSampleRate(sampleRate);
}

/*
 * processBlock takes a buffer of length numSamples
 * It performs dynamic range compression on the buffer.
 * Dynamic Range Compression reduces the amplitude of samples
 * which are above a certain threshold volume.
 */
void CompressorProcessor::processBlock(float* buffer, int numSamples) 
{
    
}

void CompressorProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer&)
{

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        const int numSamples = buffer.getNumSamples();
        const float peak = buffer.getMagnitude(channel, 0, numSamples);
        const float peakDB = Decibels::gainToDecibels<float>(peak);

        // Peak Sensing
        // attack time is 0 (FIXME)
        // release time is the time it takes to play the buffer (FIXME)
        if (peakDB > *threshold) {
            if (!envelope.isActive())
            {
                envelope.noteOn();
            }

            const float cs = 1.0f - (1.0f / *ratio);
            float gainDB = cs * (*threshold - peakDB);
            gainDB = jmin<float>(0, gainDB);
            const float gain = Decibels::decibelsToGain<float>(gainDB) * envelope.getNextSample();
            buffer.applyGain(channel, 0, numSamples, gain);
        }

        if (peakDB < *threshold)
        {
            envelope.noteOff();
        }


    }

    const float outputGainFromDB = Decibels::decibelsToGain<float>(*outputGain);
    buffer.applyGain(outputGainFromDB);

}