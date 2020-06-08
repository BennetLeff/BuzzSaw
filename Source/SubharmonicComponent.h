/*
  ==============================================================================

    SubharmonicComponent.h
    Created: 19 May 2020 3:29:01pm
    Author:  harpe

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;
using ButtonAttachment = AudioProcessorValueTreeState::ButtonAttachment;
using ComboBoxAttachment = AudioProcessorValueTreeState::ComboBoxAttachment;

//==============================================================================
/*
*/
class SubharmonicComponent : public Component,
    private Slider::Listener
{
public:
    SubharmonicComponent(AudioProcessorValueTreeState&);
    ~SubharmonicComponent();

    void paint(Graphics&) override;
    void resized() override;

    void sliderValueChanged(Slider* slider) override;
private:
    
    Slider preCutoffSlide;
    std::unique_ptr<SliderAttachment> preCutoffAttach;

    Slider postCutoffSlide;
    std::unique_ptr<SliderAttachment> postCutoffAttach;

    
    Slider outGainSlide;
    std::unique_ptr<SliderAttachment> outGainAttach;

    Slider blendSlide; 
    std::unique_ptr<SliderAttachment> blendAttach;

    //Slider sideGainSlide;
    //std::unique_ptr<SliderAttachment> sideGainAttach;
    
    Slider attackSlide;
    std::unique_ptr<SliderAttachment> attackAttach;

    Slider releaseSlide;
    std::unique_ptr<SliderAttachment> releaseAttach;

    Slider driveSlide;
    std::unique_ptr<SliderAttachment> driveAttach;

    ToggleButton stereoOnButton;
    std::unique_ptr<ButtonAttachment> stereoOnAttach;






    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SubharmonicComponent)
};
