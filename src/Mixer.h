#pragma once

// CMake builds don't use an AppConfig.h, so it's safe to include juce module headers
// directly. If you need to remain compatible with Projucer-generated builds, and
// have called `juce_generate_juce_header(<thisTarget>)` in your CMakeLists.txt,
// you could `#include <JuceHeader.h>` here instead, to make all your module headers visible.
#include <JuceHeader.h>
#include "DeckGui.h"
#include "AudioWaveform.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class Mixer : public juce::Component,
              public juce::Slider::Listener
{
public:
    //==============================================================================
    Mixer(AudioPlayer* _audPlay1, AudioPlayer *_audPlay2);
    ~Mixer();
    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;
    void drawWaveform();

    //Listeners
    void sliderValueChanged(Slider *) override;

private:
    //==============================================================================
    // Your private member variables go here...

    //Sliders and the values they change
    Slider mixSlider;

    AudioPlayer *audPlay1;
    AudioPlayer *audPlay2;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Mixer)
};
