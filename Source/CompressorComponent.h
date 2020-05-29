/*
  ==============================================================================

    CompressorComponent.h
    Created: 29 May 2020 12:38:46pm
    Author:  Bennet

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;

class CompressorComponent : public Component,
    private Slider::Listener
{
public:
    CompressorComponent(AudioProcessorValueTreeState&);

    void paint(Graphics&) override;
    void resized() override;

    void sliderValueChanged(Slider* slider) override;
private:
    Slider thresholdSlider;
    std::unique_ptr<SliderAttachment> thresholdAttachment;

    Slider ratioSlider;
    std::unique_ptr<SliderAttachment> ratioAttachment;

    Slider gainSlider;
    std::unique_ptr<SliderAttachment> gainAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CompressorComponent)
};