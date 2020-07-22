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

constexpr static const int LUTSize = 400, LUTHalfSize = LUTSize/2, LUTMaxIndex = LUTSize-1;
static float aSinHlookupTable[LUTSize];

enum SatType
{
    None,
    Hard,
    Soft,
    HypTan,
    AHypSin,
};

using SatFunc = std::function<float(float)>;

class Saturators
{
public:
    static SatFunc getSaturator(SatType type)
    {
        if (type == Hard)
            return [](auto x) { return hardClip(x); };

        if (type == Soft)
            return [](auto x) { return softClip(x); };

        if (type == HypTan)
            return [](auto x) { return tanhClip(x); };

        if (type == AHypSin)
            return [](auto x) { return aSinhClip(x); };

        // None
        return [](auto x) { return x; };
    }

	template <typename Numeric>
    constexpr static inline auto hardClip(Numeric x)
    {
        if (x > 1.0f)
            return 1.0f;

        if (x < -1.0f)
            return -1.0f;

        return x;
    }

	template <typename Numeric>
    constexpr static inline float softClip(Numeric x)
    {
        if (x > 1.0f)
            return 2.0f / 3.0f;

        if (x < -1.0f)
            return -2.0f / 3.0f;

        return x - x * x * x / 3.0f;
    }

	template <typename Numeric>
    static inline float tanhClip(Numeric x)
    {
        return tanhf(x);
    }

	template <typename Numeric>
    static inline float aSinhClip(Numeric x)
    {
        return asinhf(x);
    }    
    
	template <typename Numeric>
    static inline float aSinhClipLUT(Numeric x)
    {
        float pi = MathConstants<float>::pi;
        int inputToIndex = jmax(0, static_cast<int>(LUTHalfSize + floor(LUTHalfSize * x / pi)));

        int LUTIndex = jmin(LUTMaxIndex, inputToIndex);
        return aSinHlookupTable[LUTIndex];
    }

    //fills table with asinh values 
	template <typename Numeric>
    constexpr static void fillLUT() {
        auto minVal = LUTSize / -2;

        for (auto i = 0; i < LUTSize; i++) {
            auto piRangeFromIndex = ((minVal + i) * MathConstants<Numeric>::pi) / (LUTSize / 2);
            aSinHlookupTable[i] = asinhf(piRangeFromIndex);
        }
    }
};