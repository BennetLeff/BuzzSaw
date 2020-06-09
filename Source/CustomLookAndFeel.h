/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 8 Jun 2020 2:51:48pm
    Author:  harpe

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"


class CustomLookAndFeel : public LookAndFeel_V4 {

public:

    CustomLookAndFeel() {
        //setColour(Slider::backgroundColourId, dialColour);
        setColour(ResizableWindow::backgroundColourId, bgColour);

    }

    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
    {

        auto radius = jmin(width / 2, height / 2);// - 4.0f;
        auto centreX = x + width * 0.5f;
        auto centreY = y + height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        // fill
        g.setColour(dialColour);
        g.fillEllipse(rx, ry, rw, rw);

        // outline
        g.setColour(Colours::black);
        g.drawEllipse(rx, ry, rw, rw, 1.0f);


        //dial marker
        Path p;
        auto pointerLength = radius * 0.5f;
        auto pointerThickness = 2.0f;
        p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));

        g.setColour(Colours::black);
        g.fillPath(p);
    }

private:
    const Colour dialColour = Colour(196, 196, 196);
    const Colour bgColour = Colour(48, 48, 48);

};