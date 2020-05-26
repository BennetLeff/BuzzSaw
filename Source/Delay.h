/*
  ==============================================================================

    Delay.h
    Created: 23 May 2020 5:47:22pm
    Author:  harpe

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

//used to create stereo effect 

class Delay {
public:
    Delay();
    ~Delay();

    void initialize(double sampleRate);
    void processBlock(float* buffer, const int numSamples);
    void setDelaySec(float ds);
    void setFeedback(float fb);
    void setWetLevel(float wl);
    void setDryLevel(float dl);
    void setActive(bool state);



    //params (potentially slider controlled)
    float delaySec;
    float feedback;
    float dryLevel;
    float wetLevel;
    bool isActive;



private:

    AudioSampleBuffer delayBuffer;
    int delayBufferLength;
    int delayReadPosition, delayWritePosition;




};
