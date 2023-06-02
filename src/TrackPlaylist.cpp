#include "TrackPlaylist.h"

//==============================================================================
TrackPlaylist::TrackPlaylist()
{
    table.getHeader().addColumn("Song Name", 0, 275);
    table.getHeader().addColumn("Time", 1, 50);
    table.getHeader().addColumn("", 2, 50);
    table.getHeader().addColumn("", 3, 50);
    table.getHeader().addColumn("", 4, 75);

    table.setModel(this);

    addAndMakeVisible(table);
    addAndMakeVisible(nextBtn);
    nextBtn.addListener(this);
}

TrackPlaylist::~TrackPlaylist()
{
}

//==============================================================================
void TrackPlaylist::paint(juce::Graphics &g)
{
    g.drawText("This is playlist", getLocalBounds(), Justification::centred, false);
}

void TrackPlaylist::resized()
{
    int rowH = getHeight() / 5;
    nextBtn.setBounds(0, 0, getWidth(), rowH);
    table.setBounds(0, rowH, getWidth(), rowH * 3);

    table.getHeader().setColumnWidth(0, 0.55 * getWidth());
    table.getHeader().setColumnWidth(1, 0.10 * getWidth());
    table.getHeader().setColumnWidth(2, 0.10 * getWidth());
    table.getHeader().setColumnWidth(3, 0.10 * getWidth());
    table.getHeader().setColumnWidth(4, 0.15 * getWidth());
}

int TrackPlaylist::getNumRows()
{
    return (int)songs.size();
}

void TrackPlaylist::paintRowBackground(Graphics &g,
                                       int rowNumber,
                                       int width, int height,
                                       bool rowIsSelected)
{
    if (rowIsSelected)
    {
        if (rowNumber % 2 == 0)
        {
            g.fillAll(Colours::purple.darker(0.7));
        }
        else
        {
            g.fillAll(Colours::purple.darker(0.5));
        }
    }
    else
    {
        if (rowNumber % 2 == 0)
        {
            g.fillAll(Colours::purple.darker(0.2));
        }
        else
        {
            g.fillAll(Colours::purple);
        }
    }
}

void TrackPlaylist::paintCell(Graphics &g,
                              int rowNumber,
                              int columnId,
                              int width, int height,
                              bool rowIsSelected)
{
    g.setColour(Colours::aliceblue);
    if (columnId == 0)
    {
        g.drawText(String(rowNumber + 1) + ". " + songs[rowNumber]->getName(),
                   10, 0,
                   width, height,
                   Justification::centredLeft,
                   true);
    }
    else if (columnId == 1)
    {
        g.drawText(songs[rowNumber]->getTime(),
                   10, 0,
                   width, height,
                   Justification::centredLeft,
                   true);
    }
}

Component *TrackPlaylist::refreshComponentForCell(int rowNumber,
                                                  int columnId,
                                                  bool isRowSelected,
                                                  Component *existingComponentToUpdate)
{
    if (columnId == 2)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton *button = new TextButton{"UP"};
            button->addListener(this);
            button->setComponentID("AA" + String(rowNumber));
            existingComponentToUpdate = button;
        }
    }
    if (columnId == 3)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton *button = new TextButton{"DOWN"};
            button->addListener(this);
            button->setComponentID("AB" + String(rowNumber));
            existingComponentToUpdate = button;
        }
    }
    if (columnId == 4)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton *button = new TextButton{"REMOVE"};
            button->addListener(this);
            button->setComponentID("AC" +
                                   String(rowNumber));
            existingComponentToUpdate = button;
        }
    }
    return existingComponentToUpdate;
}

void TrackPlaylist::buttonClicked(Button *button)
{
    String stringID = button->getComponentID();

    if (stringID.startsWith("A"))
    {

        int songId = button->getComponentID().trimCharactersAtStart("ABC").getIntValue();

        if (stringID.startsWith("AA"))
        {
            if (songId > 0)
            {
                std::cout << "UP " + songs[songId]->getName() << std::endl;
                // Move Song Up in List
                Song *tempSong = songs[songId];
                songs[songId] = songs[songId - 1];
                songs[songId - 1] = tempSong;
                table.updateContent();
                table.repaint();
            }
        }
        else if (stringID.startsWith("AB"))
        {
            if (songId < songs.size() - 1)
            {
                std::cout << "DOWN " + songs[songId]->getName() << std::endl;
                // Move Song Down in List
                Song *tempSong = songs[songId];
                songs[songId] = songs[songId + 1];
                songs[songId + 1] = tempSong;
                table.updateContent();
                table.repaint();
            }
        }
        else
        {
            std::cout << "REMOVE " + songs[songId]->getName() << std::endl;
            // Remove Song from list
            songs.erase(songs.begin() + songId);
            table.updateContent();
            table.repaint();
        }
    }

    if (button == &nextBtn)
    {
        // Tell DeckGUI to load next song
        if (songs.size() > 0)
        {
            sendChangeMessage();
        }
    }
}

void TrackPlaylist::addSong(Song *song)
{
    songs.push_back(song);
    table.updateContent();
    table.repaint();
}

void TrackPlaylist::removeFirstSong()
{
    if (songs.size() > 0)
    {
        songs.erase(songs.begin());
        table.updateContent();
        table.repaint();
        std::cout << String{songs.size()} << std::endl;
    }
}

String TrackPlaylist::getFirstSongName()
{
    return songs[0]->getName();
}

URL TrackPlaylist::getFirstSongURL()
{
    return songs[0]->getURL();
}