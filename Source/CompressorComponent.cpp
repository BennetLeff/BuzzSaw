/*
  ==============================================================================

    CompressorComponent.cpp
    Created: 29 May 2020 12:38:46pm
    Author:  Bennet

  ==============================================================================
*/

#include "CompressorComponent.h"


CompressorComponent::CompressorComponent(AudioProcessorValueTreeState& state)
{
    auto setupSlider = [this](Slider& slider, AudioProcessorValueTreeState& vts, String paramID,
        std::unique_ptr<SliderAttachment>& attachment, String name = {},
        std::function<void()> onValueChange = {}, std::function<String(double)> textFromValue = {},
        std::function<double(String)> valueFromText = {})
    {
        addAndMakeVisible(slider);
        attachment.reset(new SliderAttachment(vts, paramID, slider));

        slider.setSliderStyle(Slider::LinearHorizontal);
        slider.setName(name);
        slider.textFromValueFunction = textFromValue;
        slider.valueFromTextFunction = valueFromText;
        slider.setNumDecimalPlacesToDisplay(2);
        slider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 15);
        slider.setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);
        slider.onValueChange = onValueChange;
    };

    setupSlider(thresholdSlider, state, "compThreshold", thresholdAttachment, "Threshold", [this] { /* */ });
    setupSlider(ratioSlider, state, "compRatio", ratioAttachment, "Ratio", [this] { /* */ });
    setupSlider(gainSlider, state, "compOutputGain", gainAttachment, "Gain", [this] { /* */ });
}

void CompressorComponent::paint(Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(Colours::white);
    g.setFont(14.0f);
    g.drawText("Compressor Component", getLocalBounds(),
        Justification::centred, true);   // draw some placeholder text

    auto makeName = [this, &g](Component& comp, String name)
    {
        const int height = 20;
        Rectangle<int> nameBox(comp.getX(), 402, comp.getWidth(), height);
        g.drawFittedText(name, nameBox, Justification::centred, 1);
    };

    makeName(thresholdSlider, "Threshold");
    makeName(ratioSlider, "Ratio");
    makeName(gainSlider, "Gain");
}

void CompressorComponent::resized()
{
    auto x = 10;
    auto y = 10;
    auto w = 300;
    auto h = 40;
    thresholdSlider.setBounds(x, y, w, h);
    ratioSlider.setBounds(x, y + 40, w, h);
    gainSlider.setBounds(x, y + 80, w, h);
}

void CompressorComponent::sliderValueChanged(Slider* slider)
{

}