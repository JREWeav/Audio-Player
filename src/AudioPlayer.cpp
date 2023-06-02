#include "AudioPlayer.h"

//==============================================================================
AudioPlayer::AudioPlayer(AudioFormatManager &formatManagerToUse) : formatManager(formatManagerToUse)
{
    mixGain = 0.5;
    gain = 0.1;
}

// Audio
void AudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    audTransport.prepareToPlay(samplesPerBlockExpected, sampleRate);
    audResample.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void AudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
    audResample.getNextAudioBlock(bufferToFill);
}

void AudioPlayer::releaseResources()
{
    audTransport.releaseResources();
    audResample.releaseResources();
}

void AudioPlayer::start()
{
    audTransport.start();
}

void AudioPlayer::stop()
{
    audTransport.stop();
}

void AudioPlayer::setGain(float _gain)
{
    gain = _gain;
    audTransport.setGain(gain * mixGain);
}

void AudioPlayer::setSpeed(float speed)
{
    audResample.setResamplingRatio(speed);
}

void AudioPlayer::setRelativePosition(float position)
{
    float newPos = position * audTransport.getLengthInSeconds();
    audTransport.setPosition(newPos);
}

bool AudioPlayer::loadURL(URL audioURL)
{
    AudioFormatReader *reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr)
    {
        std::unique_ptr<AudioFormatReaderSource> newSrc(new AudioFormatReaderSource(reader, true));
        audTransport.setSource(newSrc.get(), 0, nullptr, reader->sampleRate);
        audSource.reset(newSrc.release());
        return true;
    }
    else
    {
        return false;
    }
}

double AudioPlayer::getRelativePosition()
{
    return (audTransport.getCurrentPosition() / audTransport.getLengthInSeconds());
}

double AudioPlayer::getResampleSpeed()
{
    return audResample.getResamplingRatio();
}

void AudioPlayer::setMixGain(float mgain)
{
    mixGain = mgain;
    setGain(gain);
}