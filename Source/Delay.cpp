/*
  ==============================================================================

    Delay.cpp
    Created: 23 May 2020 5:47:22pm
    Author:  harpe

  ==============================================================================
*/

#include "Delay.h"

Delay::Delay() : delayWritePosition(0), delayReadPosition(0)
{
    
}

Delay::~Delay() {

}

void Delay::initialize(double sampleRate) { //is a double ok here?
    // Allocate and zero the delay buffer (size will depend on current sample rate)
// Sanity check the result so we don't end up with any zero-length calculations
    delayBufferLength = static_cast<int>(2.0 * sampleRate);
    if (delayBufferLength < 1) delayBufferLength = 1;
    delayBuffer.setSize(1, delayBufferLength);
    delayBuffer.clear();

    // This method gives us the sample rate. Use this to figure out what the delay position
    // offset should be (since it is specified in seconds, and we need to convert it to a number
    // of samples)
    delayReadPosition = static_cast<int>(delayWritePosition - (delaySec * sampleRate)
        + delayBufferLength) % delayBufferLength;

    //DBG("Delay Write: " << delayWritePosition << " Delay Read: " << delayReadPosition);
}

//process a single channel
void Delay::processBlock(float* buffer, const int numSamples) {
    //bypassed
    if (!isActive) {
        return;
    }

    //temps for write and read positions
    int dpr = delayReadPosition;
    int dpw = delayWritePosition;

    //assuming a single channel so i'll just use the left 
    float* delayData = delayBuffer.getWritePointer(0);

    //

    for (int i = 0; i < numSamples; i++) {
        const float in = buffer[i];
        float out = 0.0;

        //output blended
        //DBG("DPR: " << dpr);
        out = (dryLevel * in) + (wetLevel * delayData[dpr]);
        //DBG("Read delayData: " << delayData[dpr]);

        delayData[dpw] = in + (delayData[dpr] * feedback);

        //increment + wrap r/w positions
        if (++dpr >= delayBufferLength) dpr = 0;
        if (++dpw >= delayBufferLength) dpw = 0;

        buffer[i] = out;

    }

    //write back r/w positions
    delayReadPosition = dpr;
    delayWritePosition = dpw;


}

void Delay::setDelaySec(float ds) {
    delaySec = ds;
}

void Delay::setFeedback(float fb) {
    feedback = fb;
}

void Delay::setWetLevel(float wl) {
    wetLevel = wl;
}

void Delay::setDryLevel(float dl) {
    dryLevel = dl;
}

void Delay::setActive(bool state) {
    isActive = state;
    //clear buffer when disabled
    if (!state) {
        delayBuffer.clear();
    }
}