/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "SubharmonicComponent.h"
#include "WavefolderComponent.h"

using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;
using ComboBoxAttachment = AudioProcessorValueTreeState::ComboBoxAttachment;

//==============================================================================
/**
*/
class BuzzSawAudioProcessorEditor  : public AudioProcessorEditor,
									   private Slider::Listener
{
public:
    BuzzSawAudioProcessorEditor (BuzzSawAudioProcessor&);
    ~BuzzSawAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
	void sliderValueChanged(Slider* slider) override;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    BuzzSawAudioProcessor& processor;

	//Slider gainControl;
    //Slider preGainControl;

    std::unique_ptr<SubharmonicComponent> subharmonicComponent;
    //std::unique_ptr<WavefolderComponent> wavefolderComponent;


    //In Case we want to add any of these controls back one day
    //Slider freqSlide;
    //std::unique_ptr<SliderAttachment> freqAttach;

    //Slider depthSlide;
    //std::unique_ptr<SliderAttachment> depthAttach;

    //Slider ffSlide;
    //std::unique_ptr<SliderAttachment> ffAttach;

    //Slider fbSlide;
    //std::unique_ptr<SliderAttachment> fbAttach;

    //ComboBox satBox;
    //std::unique_ptr<ComboBoxAttachment> satBoxAttach;
    //const StringArray satChoices = StringArray({ "None", "Hard", "Soft", "Tanh", "ASinh" });

    //ComboBox waveBox;
    //std::unique_ptr<ComboBoxAttachment> waveBoxAttach;
    //const StringArray waveChoices = StringArray({ "Zero", "Tri", "Sine" });


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BuzzSawAudioProcessorEditor)
};
