/*
  ==============================================================================

    WavefolderComponent.cpp
    Created: 18 May 2020 2:24:32pm
    Author:  Bennet

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WavefolderComponent.h"

//==============================================================================
WavefolderComponent::WavefolderComponent(AudioProcessorValueTreeState& valueTreeState)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    auto setupSlider = [this](Slider& slider, AudioProcessorValueTreeState& vts, String paramID,
        std::unique_ptr<SliderAttachment>& attachment, String name = {},
        std::function<void()> onValueChange = {}, std::function<String(double)> textFromValue = {},
        std::function<double(String)> valueFromText = {})
    {
        addAndMakeVisible(slider);
        attachment.reset(new SliderAttachment(vts, paramID, slider));

        slider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        slider.setName(name);
        slider.textFromValueFunction = textFromValue;
        slider.valueFromTextFunction = valueFromText;
        slider.setNumDecimalPlacesToDisplay(2);
        slider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 15);
        slider.setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);
        slider.onValueChange = onValueChange;
    };

    setupSlider(driveSlide, valueTreeState, "drive", driveAttach, "Drive", [this] { /*nlViewer.updateCurve();*/ });
    setupSlider(freqSlide, valueTreeState, "freq", freqAttach, "Freq", [this] { /*nlViewer.updateCurve();*/ });
    setupSlider(depthSlide, valueTreeState, "depth", depthAttach, "Depth", [this] { /*nlViewer.updateCurve();*/ });
    setupSlider(ffSlide, valueTreeState, "feedforward", ffAttach, "Feedforward", [this] {/* nlViewer.updateCurve();*/ });
    setupSlider(fbSlide, valueTreeState, "feedback", fbAttach, "Feedback", [this] { /*nlViewer.updateCurve();*/ });

    auto setupBox = [this](ComboBox& box, AudioProcessorValueTreeState& vts, String paramID,
        std::unique_ptr<ComboBoxAttachment>& attachment, StringArray choices,
        std::function<void()> onChange = {})
    {
        addAndMakeVisible(box);
        box.addItemList(choices, 1);
        box.setSelectedItemIndex(0);
        box.onChange = onChange;

        attachment.reset(new ComboBoxAttachment(vts, paramID, box));
    };

    setupBox(satBox, valueTreeState, "sat", satBoxAttach, satChoices, [this] {/* nlViewer.updateCurve();*/ });
    setupBox(waveBox, valueTreeState, "wave", waveBoxAttach, waveChoices, [this] { /*nlViewer.updateCurve();*/ });
}

WavefolderComponent::~WavefolderComponent()
{
}

void WavefolderComponent::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
   // g.drawText ("WavefolderComponent", getLocalBounds(),
               // Justification::centred, true);   // draw some placeholder text

    auto makeName = [this, &g](Component& comp, String name)
    {
        const int height = 20;
        Rectangle<int> nameBox(comp.getX(), comp.getY() - 13, comp.getWidth(), height);
        g.drawFittedText(name, nameBox, Justification::centred, 1);
    };

    makeName(driveSlide, "Drive");
    makeName(freqSlide, "Frequency");
    makeName(depthSlide, "Depth");
    makeName(ffSlide, "FF");
    makeName(fbSlide, "FB");
    makeName(satBox, "Saturators");
    makeName(waveBox, "Waves");
}

void WavefolderComponent::resized()
{
    freqSlide.setBounds(-10, this->getHeight() / 4, 90, 80);
    depthSlide.setBounds(freqSlide.getRight() - 30, this->getHeight() / 4, 90, 80);
    ffSlide.setBounds(depthSlide.getRight() - 30, this->getHeight() / 4, 90, 80);
    fbSlide.setBounds(ffSlide.getRight() - 30, this->getHeight() / 4, 90, 80);
    satBox.setBounds(fbSlide.getRight() - 10, this->getHeight() / 4, 70, 20);
    waveBox.setBounds(satBox.getRight() + 10, this->getHeight() / 4, 70, 20);
    driveSlide.setBounds(waveBox.getRight() - 10, this->getHeight() / 4, 90, 80);
}

void WavefolderComponent::sliderValueChanged(Slider* slider)
{
    //
}