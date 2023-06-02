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
class AudioPlayer : public juce::AudioSource
{
public:
    //==============================================================================
    AudioPlayer(AudioFormatManager &formatManagerToUse);

    // Audio
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;

    // Interact with
    bool loadURL(URL audioFile);
    void setGain(float _gain);
    void setMixGain(float mgain);
    void setSpeed(float speed);
    void setRelativePosition(float position);
    void start();
    void stop();

    double getRelativePosition();
    double getResampleSpeed();

private:
    //==============================================================================
    // Your private member variables go here...

    // Audio Files Loader
    std::unique_ptr<AudioFormatReaderSource> audSource;
    AudioTransportSource audTransport;

    AudioFormatManager &formatManager;

    // Audio Speed Set
    ResamplingAudioSource audResample{&audTransport, false};

    float gain;
    float mixGain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPlayer)
};
