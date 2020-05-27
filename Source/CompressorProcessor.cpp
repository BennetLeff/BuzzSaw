/*
  ==============================================================================

    CompressorProcessor.cpp
    Created: 27 May 2020 2:16:32pm
    Author:  Bennet

  ==============================================================================
*/

#include "CompressorProcessor.h"


CompressorProcessor::CompressorProcessor(float sampleRate)
{
    envelope.setSampleRate(sampleRate);

    // Set the ADSR parameters via struct from the ADSR class.
    ADSR::Parameters adsrParameters{this->attack, 0.001f, 1.0f, this->release};
    envelope.setParameters(adsrParameters);
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
    for (int i = 0; i < numSamples; i++)
    {
        if (buffer[i] >= threshold)
        {
            envelope.noteOn();
        }

        else if (buffer[i] < threshold)
        {
            envelope.noteOff();
        }

        auto amountOverThreshold = buffer[i] - threshold;

        // At the beginning of the attack, the envelope should be at 1.
        // At the end of the release the envelope should be at 0.
        // We'll have to keep an eye on this to ensure there's no floating point
        // arithmetic errors resulting in a negative value.
        auto inverseEnvelope = 1.0001 - (envelope.getNextSample() + 0.0001);

        buffer[i] = threshold + ((amountOverThreshold / ratio) * inverseEnvelope);

    }
}

void CompressorProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer&)
{

}