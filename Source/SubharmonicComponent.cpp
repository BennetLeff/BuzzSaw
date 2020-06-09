/*
  ==============================================================================

    SubharmonicComponent.cpp
    Created: 19 May 2020 3:29:01pm
    Author:  harpe

  ==============================================================================
*/


#include <JuceHeader.h>
#include "SubharmonicComponent.h"

//==============================================================================
SubharmonicComponent::SubharmonicComponent(AudioProcessorValueTreeState& valueTreeState)
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
        slider.setLookAndFeel(&customLookAndFeel); //all sliders will use the same look and feel
    };

    auto setupToggleButton = [this](ToggleButton& button, AudioProcessorValueTreeState& vts, String paramID,
        std::unique_ptr<ButtonAttachment>& attachment, String name = {},
        std::function<void()> onValueChange = {}, std::function<String(double)> textFromValue = {},
        std::function<double(String)> valueFromText = {})
    {
        addAndMakeVisible(button);
        attachment.reset(new ButtonAttachment(vts, paramID, button));

        //button.setButtonStyle(DrawableButton::ImageAboveTextLabel);
        button.setName(name);
        //button.textFromValueFunction = textFromValue;
        //slider.valueFromTextFunction = valueFromText;
        //slider.setNumDecimalPlacesToDisplay(2);
        //button.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 15);
        button.setColour(DrawableButton::backgroundColourId, Colours::transparentBlack);
        button.setColour(DrawableButton::backgroundOnColourId, Colours::peru);

        //button.onValueChange = onValueChange;
    };

    setupSlider(preCutoffSlide, valueTreeState, "shgPreCutoff", preCutoffAttach, "PreCutoff", [this] { /*nlViewer.updateCurve();*/ });
    setupSlider(postCutoffSlide, valueTreeState, "shgPostCutoff", preCutoffAttach, "PostCutoff", [this] { /*nlViewer.updateCurve();*/ });
    setupSlider(outGainSlide, valueTreeState, "outGain", outGainAttach, "outGain", [this] { /*nlViewer.updateCurve();*/ });
    setupSlider(blendSlide, valueTreeState, "mainBlend", blendAttach, "Blend", [this] { /*nlViewer.updateCurve();*/ });
    setupSlider(driveSlide, valueTreeState, "drive", driveAttach, "Drive", [this] { /*nlViewer.updateCurve();*/ });
    //setupSlider(sideGainSlide, valueTreeState, "shgSideGain", sideGainAttach, "SideGain", [this] { /*nlViewer.updateCurve();*/ });
    //setupSlider(attackSlide, valueTreeState, "shgAttack", attackAttach, "Attack", [this] { /*nlViewer.updateCurve();*/ });
    //setupSlider(releaseSlide, valueTreeState, "shgRelease", releaseAttach, "Release", [this] { /*nlViewer.updateCurve();*/ });
    
    setupToggleButton(stereoOnButton, valueTreeState, "stereoOn", stereoOnAttach, "Widen", [this] {});

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

    //WaveFolder boxes
    //setupBox(satBox, valueTreeState, "sat", satBoxAttach, satChoices, [this] {/* nlViewer.updateCurve();*/ });
    //setupBox(waveBox, valueTreeState, "wave", waveBoxAttach, waveChoices, [this] { /*nlViewer.updateCurve();*/ });
}

SubharmonicComponent::~SubharmonicComponent()
{
}

void SubharmonicComponent::paint(Graphics& g)
{
    g.fillAll(customLookAndFeel.findColour(ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(Colours::white);
    g.setFont(14.0f);
    //g.drawText("SubharmonicComponent", getLocalBounds(),
        //Justification::centred, true);   // draw some placeholder text

    auto makeName = [this, &g](Component& comp, String name)
    {
        const int height = 20;
        //Rectangle<int> nameBox(comp.getX(), 402, comp.getWidth(), height);
        Rectangle<int> nameBox(comp.getX(), comp.getY()-13, comp.getWidth(), height);
        g.drawFittedText(name, nameBox, Justification::centred, 1);
    };

    makeName(preCutoffSlide, "PreCutoff");
    makeName(postCutoffSlide, "PostCutoff");
    makeName(outGainSlide, "Gain");
    makeName(driveSlide, "Drive");
   // makeName(sideGainSlide, "SideGain");
    makeName(blendSlide, "Blend");
    makeName(attackSlide, "Attack");
    makeName(releaseSlide, "Release");

    makeName(stereoOnButton, "Widen");
}

void SubharmonicComponent::resized()
{
    float width = this->getWidth();
    float height = this->getHeight();
    float nameHeightPad = 20; //need to tweak this value here and in makeName
    float bigDiam = width / 6;
    float smallDiam = width / 10;
    float spacing = 3;


    preCutoffSlide.setBounds(10, height/4, bigDiam, bigDiam+nameHeightPad);
    postCutoffSlide.setBounds(preCutoffSlide.getRight() + spacing, height / 4, bigDiam, bigDiam+nameHeightPad);
    driveSlide.setBounds(postCutoffSlide.getRight() + spacing, height / 4, bigDiam, bigDiam + nameHeightPad);
    outGainSlide.setBounds(driveSlide.getRight() + spacing, height / 4, bigDiam, bigDiam + nameHeightPad);
   // sideGainSlide.setBounds(mainGainSlide.getRight() - 30, this->getHeight() / 4, 90, 80);
    blendSlide.setBounds(outGainSlide.getRight() + spacing, height / 4, smallDiam, smallDiam+nameHeightPad);
    //attackSlide.setBounds(blendSlide.getRight() + spacing, height / 4, smallDiam, smallDiam + nameHeightPad);
    //releaseSlide.setBounds(attackSlide.getRight() + spacing, height / 4, smallDiam, smallDiam + nameHeightPad);

    stereoOnButton.setBounds(80, height*(0.8), 40, 20);
}

void SubharmonicComponent::sliderValueChanged(Slider* slider)
{
    //
}