#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize(1500, 900);
    setAudioChannels(0, 2); // no inputs, two outputs

    addAndMakeVisible(trackOne);
    addAndMakeVisible(trackTwo);

    addAndMakeVisible(mix);

    addAndMakeVisible(playlist);

    trackOne.setLookAndFeel(&tex);
    trackTwo.setLookAndFeel(&tex);

    mixer.addInputSource(&audPlayer1, false);
    mixer.addInputSource(&audPlayer2, false);

    formatManager.registerBasicFormats();

    playlist.loadSongs();
}

MainComponent::~MainComponent()
{
    trackOne.setLookAndFeel(nullptr);
    trackTwo.setLookAndFeel(nullptr);
}

// audio

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    audPlayer1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    audPlayer2.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixer.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
    mixer.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    audPlayer1.releaseResources();
    audPlayer2.releaseResources();
    mixer.releaseResources();
}

//==============================================================================
void MainComponent::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::skyblue);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    int rowH = getHeight() / 12;
    int colW = getWidth() / 3;

    // 1 Block
    playlist.setBounds(0, 0, colW, getHeight());

    // TRACKS

    // Mixer
    mix.setBounds(colW, 0, colW * 2, rowH);

    // 2 Blocks
    trackOne.setBounds(colW, rowH, colW, getHeight());

    // 3 Blocks
    trackTwo.setBounds(colW * 2, rowH, colW, getHeight());
}