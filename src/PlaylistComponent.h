#pragma once

// CMake builds don't use an AppConfig.h, so it's safe to include juce module headers
// directly. If you need to remain compatible with Projucer-generated builds, and
// have called `juce_generate_juce_header(<thisTarget>)` in your CMakeLists.txt,
// you could `#include <JuceHeader.h>` here instead, to make all your module headers visible.
#include <JuceHeader.h>
#include "DeckGUI.h"
#include "Song.h"
#include <iostream>
#include <fstream>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

class PlaylistComponent : public juce::Component,
                          public juce::TableListBoxModel,
                          public juce::Button::Listener,
                          public juce::FileDragAndDropTarget,
                          public juce::ChangeBroadcaster
{
public:
    //==============================================================================
    PlaylistComponent(AudioFormatManager &formatManagerToUse, DeckGUI &_trackOne, DeckGUI &_trackTwo);
    ~PlaylistComponent();
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

    // Drag and Drop
    bool isInterestedInFileDrag(const StringArray &files) override;
    void filesDropped(const StringArray &files, int x, int y) override;

    // Allow MainComponent to load previous songs from disc
    void loadSongs();

    // Files
    bool loadFile(URL fileURL, bool skipDuplicate = false);

private:
    //==============================================================================
    // Your private member variables go here...
    TableListBox table;

    TextButton loadBtn{"LOAD"};

    FileChooser fileChooser{"LOAD"};

    AudioFormatManager &formatManager;

    DeckGUI &trackOne;
    DeckGUI &trackTwo;

    std::vector<Song *> songs;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};
