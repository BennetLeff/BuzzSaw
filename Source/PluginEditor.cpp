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
    //postgain
	gainControl.setSliderStyle(Slider::LinearBarVertical);
	gainControl.setRange(0.0, 127.0, 1.0);
	gainControl.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	gainControl.setPopupDisplayEnabled(true, false, this);
	gainControl.setTextValueSuffix(" Gain");
	gainControl.setValue(1.0);

    //pregain control
    preGainControl.setSliderStyle(Slider::LinearBarVertical);
    preGainControl.setRange(0.0, 127.0, 1.0);
    preGainControl.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    preGainControl.setPopupDisplayEnabled(true, false, this);
    preGainControl.setTextValueSuffix(" Gain");
    preGainControl.setValue(1.0);

	// this function adds the slider to the editor
	addAndMakeVisible(&gainControl);
    addAndMakeVisible(&preGainControl);


    preGainControl.addListener(this);
	gainControl.addListener(this);
    



    // this function adds the slider to the editor

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(800, 600);

 
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
    g.drawFittedText ("Distortion Effect Unit", getLocalBounds(), Justification::centred, 1);
}

void ThaiBasilAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	gainControl.setBounds(40, 30, 20, getHeight() - 60);
    preGainControl.setBounds(70, 30, 20, getHeight() - 60);

}

void ThaiBasilAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
	processor.gain = static_cast<float>(gainControl.getValue());
    processor.preGain = static_cast<float>(preGainControl.getValue());
}