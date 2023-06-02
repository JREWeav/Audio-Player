#pragma once

// CMake builds don't use an AppConfig.h, so it's safe to include juce module headers
// directly. If you need to remain compatible with Projucer-generated builds, and
// have called `juce_generate_juce_header(<thisTarget>)` in your CMakeLists.txt,
// you could `#include <JuceHeader.h>` here instead, to make all your module headers visible.
#include <JuceHeader.h>
#include "Song.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

class TrackPlaylist : public juce::Component,
                      public juce::TableListBoxModel,
                      public juce::Button::Listener,
                      public juce::ChangeBroadcaster
{
public:
    //==============================================================================
    TrackPlaylist();
    ~TrackPlaylist();
    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

    // Table Stuff
    int getNumRows() override;
    void paintRowBackground(Graphics &g,
                            int rowNumber,
                            int width, int height,
                            bool rowIsSelected) override;
    void paintCell(Graphics &g,
                   int rowNumber,
                   int columnId,
                   int width, int height,
                   bool rowIsSelected) override;

    Component *refreshComponentForCell(int rowNumber,
                                       int columnId,
                                       bool isRowSelected,
                                       Component *existingComponentToUpdate) override;

    // Listener
    void buttonClicked(Button *) override;

    // Files
    void addSong(Song *song);

    // Interaction
    void removeFirstSong();
    String getFirstSongName();
    URL getFirstSongURL();

private:
    //==============================================================================
    // Your private member variables go here...
    TableListBox table;

    TextButton nextBtn{"NEXT"};

    FileChooser fileChooser{"LOAD"};

    std::vector<Song *> songs;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackPlaylist)
};