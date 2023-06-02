#pragma once

// CMake builds don't use an AppConfig.h, so it's safe to include juce module headers
// directly. If you need to remain compatible with Projucer-generated builds, and
// have called `juce_generate_juce_header(<thisTarget>)` in your CMakeLists.txt,
// you could `#include <JuceHeader.h>` here instead, to make all your module headers visible.
#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class VinylDial : public juce::Component,
                  public juce::ChangeBroadcaster,
                  public juce::Timer
{
public:
    //==============================================================================
    VinylDial();
    ~VinylDial();
    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;
    void drawWaveform();

    double getRelativeValue();

    void timerCallback() override;
    bool isSpinning();

    // Listeners
    void mouseDown(const MouseEvent &event) override;
    void mouseDrag(const MouseEvent &event) override;
    void mouseUp(const MouseEvent &event) override;

private:
    //==============================================================================
    // Your private member variables go here...

    // Sliders and the values they change

    Image img;
    AffineTransform trans;

    double value;
    double rotInRad;
    double curVal;
    double lastDist;
    double storeDist;
    bool spinning;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VinylDial)
};
