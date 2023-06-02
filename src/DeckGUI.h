#pragma once

// CMake builds don't use an AppConfig.h, so it's safe to include juce module headers
// directly. If you need to remain compatible with Projucer-generated builds, and
// have called `juce_generate_juce_header(<thisTarget>)` in your CMakeLists.txt,
// you could `#include <JuceHeader.h>` here instead, to make all your module headers visible.
#include <JuceHeader.h>
#include "AudioPlayer.h"
#include "AudioWaveform.h"
#include "VinylDial.h"
#include "TrackPlaylist.h"
#include "Song.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class DeckGUI : public juce::Component,
                public juce::Button::Listener,
                public juce::Slider::Listener,
                public juce::Timer,
                public juce::ChangeListener
{
public:
    //==============================================================================
    DeckGUI(AudioPlayer *audPlayerToUse, AudioFormatManager &formatManagerToUse, AudioThumbnailCache &cacheToUse);
    ~DeckGUI();
    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;
    void drawWaveform();

    // Listeners
    void buttonClicked(Button *) override;
    void sliderValueChanged(Slider *) override;
    void timerCallback() override;
    void changeListenerCallback(ChangeBroadcaster *source) override;
    void loadSong(String songName, URL songUrl);
    bool isLoaded();

    // Add to Playlist
    void addToPlaylist(Song *song);

private:
    //==============================================================================
    // Your private member variables go here...

    // Our Two Buttons and a Bool for Playing and Pausing
    TextButton playButton{"PLAY"};

    // Sliders and the values they change
    Slider volSlider;
    Slider speedSlider;
    VinylDial vinylScratch;

    // Audio Source
    FileChooser fileChooser{"LOAD"};
    AudioPlayer *audPlayer;

    // WaveForm
    AudioWaveform waveForm;
    double lastClick;

    // Label
    Label curSong;

    // Playlist
    TrackPlaylist playlist;

    // Is song loaded
    bool songIsLoaded;

    // Labels
    Label gainLabel{"VOLUME"};
    Label speedLabel{"SPEED"};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};
