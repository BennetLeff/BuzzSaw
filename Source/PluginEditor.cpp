/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BuzzSawAudioProcessorEditor::BuzzSawAudioProcessorEditor (BuzzSawAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    subharmonicComponent= std::make_unique<SubharmonicComponent>(p.getVTS());
    //wavefolderComponent= std::make_unique<WavefolderComponent>(p.getVTS());
 
    addAndMakeVisible(*subharmonicComponent);
    //addAndMakeVisible(*wavefolderComponent);

    setSize(620, 400);
}

BuzzSawAudioProcessorEditor::~BuzzSawAudioProcessorEditor()
{
}

//==============================================================================
void BuzzSawAudioProcessorEditor::paint (Graphics& g)
{
    Image background = ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);
    g.drawImageWithin(background,0,0,getWidth(),getHeight(), RectanglePlacement::stretchToFit);

    g.setColour (Colours::white);
    g.setFont (15.0f);


    subharmonicComponent->paint(g);
    //wavefolderComponent->paint(g);

}

void BuzzSawAudioProcessorEditor::resized()
{
    subharmonicComponent->setBounds(0, 0, getWidth(), getHeight());
    //wavefolderComponent->setBounds(0, getHeight()/2, getWidth(), getHeight()/2);
}

void BuzzSawAudioProcessorEditor::sliderValueChanged(Slider* slider)
{

}