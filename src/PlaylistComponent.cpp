#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(AudioFormatManager &formatManagerToUse, DeckGUI &_trackOne, DeckGUI &_trackTwo) : formatManager(formatManagerToUse), trackOne(_trackOne), trackTwo(_trackTwo)
{
    table.getHeader().addColumn("Song Name", 0, 225);
    table.getHeader().addColumn("Time", 1, 50);
    table.getHeader().addColumn("", 2, 75);
    table.getHeader().addColumn("", 3, 75);
    table.getHeader().addColumn("", 4, 75);

    addAndMakeVisible(table);
    table.setModel(this);

    addAndMakeVisible(loadBtn);
    loadBtn.addListener(this);
}

PlaylistComponent::~PlaylistComponent()
{
}

//==============================================================================
void PlaylistComponent::paint(juce::Graphics &g)
{
    g.drawText("This is playlist", getLocalBounds(), Justification::centred, false);
}

void PlaylistComponent::resized()
{
    int rowH = getHeight() / 10;
    loadBtn.setBounds(0, 0, getWidth(), rowH);
    table.setBounds(0, rowH, getWidth(), rowH * 9);

    table.getHeader().setColumnWidth(0, 0.5 * getWidth());
    table.getHeader().setColumnWidth(1, 0.1 * getWidth());
    table.getHeader().setColumnWidth(2, 0.15 * getWidth());
    table.getHeader().setColumnWidth(3, 0.15 * getWidth());
    table.getHeader().setColumnWidth(4, 0.1 * getWidth());
}

int PlaylistComponent::getNumRows()
{
    return (int)songs.size();
}

void PlaylistComponent::paintRowBackground(Graphics &g,
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

void PlaylistComponent::paintCell(Graphics &g,
                                  int rowNumber,
                                  int columnId,
                                  int width, int height,
                                  bool rowIsSelected)
{
    g.setColour(Colours::aliceblue);

    if (columnId == 0)
    {
        g.drawText(songs[rowNumber]->getName(),
                   0, 0,
                   width, height,
                   Justification::centred,
                   true);
    }
    else if (columnId == 1)
    {
        g.drawText(songs[rowNumber]->getTime(),
                   0, 0,
                   width, height,
                   Justification::centred,
                   true);
    }
}

Component *PlaylistComponent::refreshComponentForCell(int rowNumber,
                                                      int columnId,
                                                      bool isRowSelected,
                                                      Component *existingComponentToUpdate)
{
    if (columnId == 2)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton *button = new TextButton{"Track One"};
            button->addListener(this);
            button->setComponentID("AA" + String(rowNumber));
            existingComponentToUpdate = button;
        }
    }
    if (columnId == 3)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton *button = new TextButton{"Track Two"};
            button->addListener(this);
            button->setComponentID("AB" +
                                   String(rowNumber));
            existingComponentToUpdate = button;
        }
    }
    if (columnId == 4)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton *button = new TextButton{"Delete"};
            button->addListener(this);
            button->setComponentID("AC" +
                                   String(rowNumber));
            existingComponentToUpdate = button;
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(Button *button)
{
    String stringID = button->getComponentID();

    if (stringID.startsWith("A"))
    {

        int songId = button->getComponentID().trimCharactersAtStart("ABC").getIntValue();

        if (stringID.startsWith("AA"))
        {
            std::cout << "Track 1 " + songs[songId]->getName() << std::endl;
            if (!trackOne.isLoaded())
            {
                trackOne.loadSong(songs[songId]->getName(), songs[songId]->getURL());
            }
            else
            {
                trackOne.addToPlaylist(songs[songId]);
            }
        }
        else if (stringID.startsWith("AB"))
        {
            std::cout << "Track 2 " + songs[songId]->getName() << std::endl;
            if (!trackTwo.isLoaded())
            {
                trackTwo.loadSong(songs[songId]->getName(), songs[songId]->getURL());
            }
            else
            {
                trackTwo.addToPlaylist(songs[songId]);
            }
        }
        else
        {
            std::cout << "DELETE " + songs[songId]->getName() << std::endl;
            songs.erase(songs.begin() + songId);

            std::ifstream songFileRead("songList.txt");
            std::ofstream tmpFileWrite("tmp.txt", std::ios::app);
            if (songFileRead.is_open())
            {
                int i = 0;
                std::string line;
                while (std::getline(songFileRead, line))
                {
                    if (i != songId)
                    {
                        tmpFileWrite << line << std::endl;
                    }
                    i++;
                }
                songFileRead.close();
                tmpFileWrite.close();
                remove("songList.txt");
                rename("tmp.txt", "songList.txt");
                table.updateContent();
                table.repaint();
            }
        }
    }
    if (button == &loadBtn)
    {
        fileChooser.browseForFileToOpen();
        loadFile(fileChooser.getURLResult());
        std::cout << fileChooser.getResult().loadFileAsString() << std::endl;
    }
}

bool PlaylistComponent::isInterestedInFileDrag(const StringArray &files)
{
    if (files[0].containsAnyOf(".wav") || files[0].containsAnyOf(".mp3"))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void PlaylistComponent::filesDropped(const StringArray &files, int x, int y)
{
    for (int i = 0; i < files.size(); i++)
    {
        std::cout << files[i] << std::endl;
        loadFile(URL{File{files[i]}});
    }
}

bool PlaylistComponent::loadFile(URL fileURL, bool skipDuplicate)
{
    // Test if it is audio file
    AudioFormatReader *reader = formatManager.createReaderFor(fileURL.createInputStream(false));
    if (reader != nullptr)
    {
        // Open File
        std::ofstream songFileWrite("songList.txt", std::ios::app);
        // If File is open

        if (songFileWrite.is_open())
        {
            bool duplicate = false;
            // Open file for reading
            std::ifstream songFileRead("songList.txt");
            std::string line;

            // Test if it is a duplicate
            while (std::getline(songFileRead, line))
            {
                if (line == fileURL.toString(false))
                {
                    duplicate = true;
                    break;
                }
                else
                {
                    duplicate = false;
                }
            }
            songFileRead.close();

            // If not Duplicate OR Overridden
            if (!duplicate || skipDuplicate)
            {
                // Get Length
                int time = (reader->lengthInSamples / reader->sampleRate);

                String name = fileURL.getFileName().replace("%20", " ");
                // Remove File Suffix
                while (!name.endsWithChar('.'))
                {
                    name = name.dropLastCharacters(1);
                }
                name = name.dropLastCharacters(1);

                // Add to Array of Songs
                songs.push_back(new Song{name, fileURL, time});

                // Update Table
                table.updateContent();
                table.repaint();

                // ONLY if not duplicate
                if (!duplicate)
                {
                    // Add to File
                    songFileWrite << fileURL.toString(true) << std::endl;
                    songFileWrite.close();

                    // Change button text
                    loadBtn.setButtonText("LOADED!");
                }

                return true;
            }
            else
            {
                loadBtn.setButtonText("DUPLICATE FILE");
                return false;
            }
        }
        else
        {
            loadBtn.setButtonText("COULD NOT OPEN SONG LIST");
            return false;
        }
    }
    else
    {
        loadBtn.setButtonText("LOAD FAILED");
        return false;
    }
}

void PlaylistComponent::loadSongs()
{
    std::ifstream songFileRead("songList.txt");
    std::string line;
    if (songFileRead.is_open())
    {
        while (std::getline(songFileRead, line))
        {
            loadFile(URL{line}, true);
        }
        songFileRead.close();
    }
}