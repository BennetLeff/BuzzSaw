/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
buzzsawAudioProcessorEditor::buzzsawAudioProcessorEditor (buzzsawAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    subharmonicComponent= std::make_unique<SubharmonicComponent>(p.getVTS());
    //wavefolderComponent= std::make_unique<WavefolderComponent>(p.getVTS());
 
    addAndMakeVisible(*subharmonicComponent);
    //addAndMakeVisible(*wavefolderComponent);

    setSize(620, 400);
}

buzzsawAudioProcessorEditor::~buzzsawAudioProcessorEditor()
{
}

//==============================================================================
void buzzsawAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    Image background = ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);
    g.drawImageWithin(background,0,0,getWidth(),getHeight(), RectanglePlacement::stretchToFit);

    g.setColour (Colours::white);
    g.setFont (15.0f);

    //background
    //Image background = ImageCache::getFromMemory(BinaryData::buzzsawtest_png, BinaryData::buzzsawtest_pngSize);
    //g.drawImageAt(background, 0, 0);

    subharmonicComponent->paint(g);
    //wavefolderComponent->paint(g);

}

void buzzsawAudioProcessorEditor::resized()
{
    subharmonicComponent->setBounds(0, 0, getWidth(), getHeight());
    //wavefolderComponent->setBounds(0, getHeight()/2, getWidth(), getHeight()/2);
}

void buzzsawAudioProcessorEditor::sliderValueChanged(Slider* slider)
{

}