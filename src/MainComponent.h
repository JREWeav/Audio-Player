#pragma once

// CMake builds don't use an AppConfig.h, so it's safe to include juce module headers
// directly. If you need to remain compatible with Projucer-generated builds, and
// have called `juce_generate_juce_header(<thisTarget>)` in your CMakeLists.txt,
// you could `#include <JuceHeader.h>` here instead, to make all your module headers visible.
#include <JuceHeader.h>
#include "DeckGUI.h"
#include "AudioPlayer.h"
#include "Mixer.h"
#include "PlaylistComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    // Audio
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
    LookAndFeel_V4 tex;

    MixerAudioSource mixer;

    AudioFormatManager formatManager;
    AudioThumbnailCache thumbCache{1000};

    AudioPlayer audPlayer1{formatManager};
    AudioPlayer audPlayer2{formatManager};

    DeckGUI trackOne{&audPlayer1, formatManager, thumbCache};
    DeckGUI trackTwo{&audPlayer2, formatManager, thumbCache};

    Mixer mix{&audPlayer1, &audPlayer2};

    PlaylistComponent playlist{formatManager, trackOne, trackTwo};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
