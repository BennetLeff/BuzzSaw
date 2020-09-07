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

void Delay::initialize(double sampleRate) { 
    // Allocate and zero the delay buffer (size will depend on current sample rate)
    delayBufferLength = static_cast<int>(2.0 * sampleRate);
    //sanity check length
    if (delayBufferLength < 1) delayBufferLength = 1;
    delayBuffer.setSize(1, delayBufferLength);
    delayBuffer.clear();

    this->sampleRate = sampleRate;

    delayReadPosition = static_cast<int>(delayWritePosition - (delaySec * sampleRate)
        + delayBufferLength) % delayBufferLength;

    setActive(true);
}

//process a single channel
void Delay::processBlock(float* buffer, const int numSamples) {
    //bypassed
    if (!isActive || delaySec<=0) {
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
        out = (dryLevel * in) + (wetLevel * delayData[dpr]);

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
    //just trying this to parameterize delaySec
    delayReadPosition = static_cast<int>(delayWritePosition - (delaySec * sampleRate)
        + delayBufferLength) % delayBufferLength;
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