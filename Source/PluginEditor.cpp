/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ThaiBasilAudioProcessorEditor::ThaiBasilAudioProcessorEditor (ThaiBasilAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setSize(400, 500);

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

    setupSlider(freqSlide, p.getVTS(), "freq", freqAttach, "Freq", [this] { /*nlViewer.updateCurve();*/ });
    setupSlider(depthSlide, p.getVTS(), "depth", depthAttach, "Depth", [this] { /*nlViewer.updateCurve();*/ });
    setupSlider(ffSlide, p.getVTS(), "feedforward", ffAttach, "Feedforward", [this] {/* nlViewer.updateCurve();*/ });
    setupSlider(fbSlide, p.getVTS(), "feedback", fbAttach, "Feedback", [this] { /*nlViewer.updateCurve();*/ });

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

    setupBox(satBox, p.getVTS(), "sat", satBoxAttach, satChoices, [this] {/* nlViewer.updateCurve();*/ });
    setupBox(waveBox, p.getVTS(), "wave", waveBoxAttach, waveChoices, [this] { /*nlViewer.updateCurve();*/ });
 
}

ThaiBasilAudioProcessorEditor::~ThaiBasilAudioProcessorEditor()
{
}

//==============================================================================
void ThaiBasilAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);

    auto makeName = [this, &g](Component& comp, String name)
    {
        const int height = 20;
        Rectangle<int> nameBox(comp.getX(), 402, comp.getWidth(), height);
        g.drawFittedText(name, nameBox, Justification::centred, 1);
    };

    makeName(freqSlide, "Frequency");
    makeName(depthSlide, "Depth");
    makeName(ffSlide, "FF");
    makeName(fbSlide, "FB");
    makeName(satBox, "Saturators");
    makeName(waveBox, "Waves");
}

void ThaiBasilAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    freqSlide.setBounds(-10, 415, 90, 80);
    depthSlide.setBounds(freqSlide.getRight() - 30, 415, 90, 80);
    ffSlide.setBounds(depthSlide.getRight() - 30, 415, 90, 80);
    fbSlide.setBounds(ffSlide.getRight() - 30, 415, 90, 80);
    satBox.setBounds(fbSlide.getRight() - 10, 440, 70, 20);
    waveBox.setBounds(satBox.getRight() + 10, 440, 70, 20);

}

void ThaiBasilAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
	processor.gain = static_cast<float>(gainControl.getValue());
    processor.preGain = static_cast<float>(preGainControl.getValue());
}