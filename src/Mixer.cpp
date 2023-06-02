#include "Mixer.h"

//==============================================================================
Mixer::Mixer(AudioPlayer *_audPlay1, AudioPlayer *_audPlay2) : audPlay1(_audPlay1), audPlay2(_audPlay2)
{
    addAndMakeVisible(mixSlider);
    mixSlider.setRange(0.0f, 1.0f);
    mixSlider.setNumDecimalPlacesToDisplay(2);
    mixSlider.setValue(0.5f);
    mixSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    mixSlider.addListener(this);
}

Mixer::~Mixer()
{
}

//==============================================================================
void Mixer::paint(juce::Graphics &g)
{
    g.fillAll(Colours::purple);
}

void Mixer::resized()
{
    mixSlider.setBounds(getLocalBounds());
}

void Mixer::sliderValueChanged(Slider *slider)
{
    if (slider == &mixSlider)
    {
        audPlay1->setMixGain(1 - mixSlider.getValue());
        audPlay2->setMixGain(mixSlider.getValue());
    }
}
