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
    subharmonicComponent= std::make_unique<SubharmonicComponent>(p.getVTS());
    //wavefolderComponent= std::make_unique<WavefolderComponent>(p.getVTS());
 
    addAndMakeVisible(*subharmonicComponent);
    //addAndMakeVisible(*wavefolderComponent);

    setSize(620, 400);
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

    //background
    //Image background = ImageCache::getFromMemory(BinaryData::thaibasiltest_png, BinaryData::thaibasiltest_pngSize);
    //g.drawImageAt(background, 0, 0);

    subharmonicComponent->paint(g);
    //wavefolderComponent->paint(g);

}

void ThaiBasilAudioProcessorEditor::resized()
{
    subharmonicComponent->setBounds(0, 0, getWidth(), getHeight());
    //wavefolderComponent->setBounds(0, getHeight()/2, getWidth(), getHeight()/2);
}

void ThaiBasilAudioProcessorEditor::sliderValueChanged(Slider* slider)
{

}