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
	/*gainControl.setSliderStyle(Slider::LinearBarVertical);
	gainControl.setRange(0.0, 127.0, 1.0);
	gainControl.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	gainControl.setPopupDisplayEnabled(true, false, this);
	gainControl.setTextValueSuffix(" Gain");
	gainControl.setValue(1.0);*/

    //pregain control
    preGainControl.setSliderStyle(Slider::Rotary);
    preGainControl.setRange(0, 1, 0.01);
    preGainControl.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    preGainControl.setTextValueSuffix(" Pre-Gain");
    preGainControl.setValue(processor.getPregain());
    
    /////

    distKnob.setSliderStyle(Slider::Rotary);
    distKnob.setRange(0, 1, 0.01);
    distKnob.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    distKnob.setTextValueSuffix(" Distortion");
    distKnob.setValue(processor.distortion);

    brightKnob.setSliderStyle(Slider::Rotary);
    brightKnob.setRange(0.01, 0.99, 0.01);
    brightKnob.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    brightKnob.setTextValueSuffix(" Brightness");
    brightKnob.setValue(processor.brightness);

    warmthKnob.setSliderStyle(Slider::Rotary);
    warmthKnob.setRange(0, 1, 0.01);
    warmthKnob.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    warmthKnob.setTextValueSuffix(" Warmth");
    warmthKnob.setValue(processor.warmth);

    /////




	// this function adds the slider to the editor
	addAndMakeVisible(&gainControl);
    addAndMakeVisible(&preGainControl);

    addAndMakeVisible(&distKnob);
    addAndMakeVisible(&brightKnob);
    addAndMakeVisible(&warmthKnob);

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

    g.drawSingleLineText("Pre-gain", edgeBound + (0.5 * knobSize),
        110, Justification::horizontallyCentred);
    g.drawSingleLineText("Distortion", edgeBound + (1.5 * knobSize),
        20, Justification::horizontallyCentred);
    g.drawSingleLineText("Brightness", edgeBound + (2.5 * knobSize),
        110, Justification::horizontallyCentred);
    g.drawSingleLineText("Warmth", edgeBound + (3.5 * knobSize),
        20, Justification::horizontallyCentred);
}

void ThaiBasilAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	// gainControl.setBounds(40, 30, 20, getHeight() - 60);
    // preGainControl.setBounds(70, 30, 20, getHeight() - 60);
   
    preGainControl.setBounds(edgeBound + (0 * knobSize), 60, knobSize, knobSize);
    distKnob.setBounds(edgeBound   + (1 * knobSize), 30, knobSize, knobSize);
    brightKnob.setBounds(edgeBound + (2 * knobSize), 60, knobSize, knobSize);
    warmthKnob.setBounds(edgeBound + (3 * knobSize), 30, knobSize, knobSize);
}

void ThaiBasilAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
	processor.gain = static_cast<float>(gainControl.getValue());
    processor.preGain = static_cast<float>(preGainControl.getValue());
}