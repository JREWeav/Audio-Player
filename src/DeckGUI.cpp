#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(AudioPlayer *audPlayerToUse, AudioFormatManager &formatManagerToUse, AudioThumbnailCache &cacheToUse) : audPlayer(audPlayerToUse), waveForm(formatManagerToUse, cacheToUse)
{
    // Add Buttons and Button Listeners
    addAndMakeVisible(playButton);

    playButton.addListener(this);

    vinylScratch.addChangeListener(this);

    playButton.setToggleable(true);

    // Add Sliders, set slider settings and add slider listeners

    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(gainLabel);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(waveForm);
    addAndMakeVisible(vinylScratch);
    addAndMakeVisible(curSong);
    addAndMakeVisible(playlist);

    playlist.addChangeListener(this);

    volSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalDrag);
    volSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxRight, false, 40, 20);
    volSlider.setValue(0.1);
    volSlider.setRange(0, 1);
    volSlider.setNumDecimalPlacesToDisplay(2);
    volSlider.addListener(this);

    speedSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    speedSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxLeft, false, 40, 20);
    speedSlider.setValue(1);
    speedSlider.setRange(0, 5);
    speedSlider.setNumDecimalPlacesToDisplay(2);
    speedSlider.addListener(this);

    gainLabel.setText("GAIN", dontSendNotification);
    gainLabel.setJustificationType(Justification::centred);
    gainLabel.setInterceptsMouseClicks(false, false);

    speedLabel.setText("SPEED", dontSendNotification);
    speedLabel.setInterceptsMouseClicks(false, false);
    speedLabel.setJustificationType(Justification::centred);

    lastClick = 0;

    songIsLoaded = false;

    startTimer(50);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

//==============================================================================
void DeckGUI::paint(juce::Graphics &g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::blueviolet);
    g.fillCheckerBoard(getLocalBounds().toFloat(), getWidth() / 10, getWidth() / 10, juce::Colours::blueviolet, juce::Colours::purple);
}

void DeckGUI::resized()
{
    int rowH = getHeight() / 8;

    // 1 Block
    playButton.setBounds(0, 0, getWidth(), rowH);
    // 2 Blocks
    vinylScratch.setBounds(0, rowH, getWidth() / 4, rowH);
    curSong.setBounds(getWidth() / 4, rowH, 3 * getWidth() / 4, rowH);
    // 4 Blocks
    volSlider.setBounds(0, rowH * 2, getWidth() / 2, rowH * 2);
    speedSlider.setBounds(getWidth() / 2, rowH * 2, getWidth() / 2, rowH * 2);
    gainLabel.setBounds(0, rowH * 2, ((getWidth() / 2) - 40), rowH * 2);
    speedLabel.setBounds(getWidth() / 2, rowH * 2, (getWidth() / 2) + 40, rowH * 2);
    // 5 Blocks
    waveForm.setBounds(0, rowH * 4, getWidth(), rowH);
    // 8 Blocks
    playlist.setBounds(0, rowH * 5, getWidth(), rowH * 3);
}

void DeckGUI::buttonClicked(Button *button)
{
    if (button == &playButton)
    {
        if (button->getToggleState())
        {
            audPlayer->stop();
            playButton.setButtonText("STOPPED");
            button->setToggleState(false, NotificationType::dontSendNotification);
        }
        else
        {
            audPlayer->start();
            playButton.setButtonText("PLAYING");
            button->setToggleState(true, NotificationType::dontSendNotification);
        }
    }
}

void DeckGUI::sliderValueChanged(Slider *slider)
{
    if (slider == &volSlider)
    {
        audPlayer->setGain((float)slider->getValue());
    }
    if (slider == &speedSlider)
    {
        audPlayer->setSpeed((float)slider->getValue());
    }
}

void DeckGUI::timerCallback()
{
    if (lastClick != waveForm.getLastRelativeClick())
    {
        lastClick = waveForm.getLastRelativeClick();
        audPlayer->setRelativePosition((float)lastClick);
    }
    if (waveForm.isLooping())
    {
        if (audPlayer->getRelativePosition() >= (waveForm.getLastRelativeClick() + waveForm.getRelativeLoopLength()))
        {
            audPlayer->setRelativePosition((float)lastClick);
        }
        else if (audPlayer->getRelativePosition() <= (waveForm.getLastRelativeClick()))
        {
            audPlayer->setRelativePosition((float)lastClick);
        }
    }

    waveForm.setRelativePosition(audPlayer->getRelativePosition());
}

void DeckGUI::changeListenerCallback(ChangeBroadcaster *source)
{
    if (source == &vinylScratch)
    {
        if (vinylScratch.isSpinning())
        {
            audPlayer->setSpeed(vinylScratch.getRelativeValue());
            if (audPlayer->getRelativePosition() >= 1)
            {
                audPlayer->setRelativePosition(0.001);
            }
            else if (audPlayer->getRelativePosition() <= 0)
            {
                audPlayer->setRelativePosition(1.001);
            }
        }
        else
        {
            audPlayer->setSpeed(speedSlider.getValue());
        }
    }

    if (source == &playlist)
    {
        loadSong(playlist.getFirstSongName(), playlist.getFirstSongURL());
        playlist.removeFirstSong();
    }
}

void DeckGUI::loadSong(String songName, URL songUrl)
{
    playButton.setToggleState(false, NotificationType::dontSendNotification);
    playButton.setButtonText("STOPPED");
    audPlayer->loadURL(songUrl);
    waveForm.loadAudio(new URLInputSource(songUrl));
    curSong.setText(songName, NotificationType::dontSendNotification);
    songIsLoaded = true;
}

void DeckGUI::addToPlaylist(Song *song)
{
    playlist.addSong(song);
}

bool DeckGUI::isLoaded()
{
    return songIsLoaded;
}