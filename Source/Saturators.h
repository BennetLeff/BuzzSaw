/*
  ==============================================================================

    Saturators.h
    Created: 15 May 2020 4:34:21pm
    Author:  Bennet

  ==============================================================================
*/

#pragma once


#include "../JuceLibraryCode/JuceHeader.h"
#include <math.h>

static const int LUTSize = 400, LUTHalfSize = LUTSize/2, LUTMaxIndex = LUTSize-1;
static float aSinHlookupTable[LUTSize];

enum SatType
{
    none,
    hard,
    soft,
    hyptan,
    ahypsin,
};

using SatFunc = std::function<float(float)>;

class Saturators
{
public:

    static SatFunc getSaturator(SatType type)
    {
        if (type == SatType::hard)
            return [](float x) { return hardClip(x); };

        if (type == SatType::soft)
            return [](float x) { return softClip(x); };

        if (type == SatType::hyptan)
            return [](float x) { return tanhClip(x); };

        if (type == SatType::ahypsin)
            return [](float x) { return aSinhClip(x); };
            //return [](float x) { return aSinhClipLUT(x); };

        // None
        return [](float x) { return x; };
    }

    static inline float hardClip(float x)
    {
        if (x > 1.0f)
            return 1.0f;

        if (x < -1.0f)
            return -1.0f;

        return x;
    }

    static inline float softClip(float x)
    {
        if (x > 1.0f)
            return 2.0f / 3.0f;

        if (x < -1.0f)
            return -2.0f / 3.0f;

        return x - x * x * x / 3.0f;
    }

    static inline float tanhClip(float x)
    {
        return tanhf(x);
    }

    static inline float aSinhClip(float x)
    {
        return asinhf(x);
        
    }    
    
    static inline float aSinhClipLUT(float x)
    {
        float pi = MathConstants<float>::pi;
        int inputToIndex = jmax(0, static_cast<int>(LUTHalfSize + floor(LUTHalfSize * x / pi)));
        //x=pi -> inputToIndex = 400 (out of range)
        //x=-pi -> inputToIndex = 0
        //x=0 ->inputToIndex= 200
        int LUTIndex = jmin(LUTMaxIndex, inputToIndex);
        return aSinHlookupTable[LUTIndex];
        
    }

    //fills table with asinh values 
    static void fillLUT() {
        int minVal = LUTSize / -2;
        //int maxVal = LUTSize / 2 - 1;

        for (int i = 0; i < LUTSize; i++) {
            float piRangeFromIndex = ((minVal + i) * MathConstants<float>::pi) / (LUTSize / 2);
            aSinHlookupTable[i] = asinhf(piRangeFromIndex);
            //DBG(i<<": " << aSinHlookupTable[i]);
        }

    }



};