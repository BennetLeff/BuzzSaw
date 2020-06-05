/*
  ==============================================================================

    WavefolderComponent.h
    Created: 18 May 2020 2:24:32pm
    Author:  Bennet

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;
using ComboBoxAttachment = AudioProcessorValueTreeState::ComboBoxAttachment;

//==============================================================================
/*
*/
class WavefolderComponent    : public Component,
    private Slider::Listener
{
public:
    WavefolderComponent(AudioProcessorValueTreeState&);
    ~WavefolderComponent();

    void paint (Graphics&) override;
    void resized() override;

    void sliderValueChanged(Slider* slider) override;
private:

    //Slider driveSlide;
    //std::unique_ptr<SliderAttachment> driveAttach;

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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavefolderComponent)
};
