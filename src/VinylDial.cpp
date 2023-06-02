#include "VinylDial.h"

//==============================================================================
VinylDial::VinylDial()
{
    rotInRad = 0;
    lastDist = 0;
    storeDist = 0;
    spinning = false;
    img = ImageCache::getFromFile(File("./img/vinyl.png"));
    startTimer(100);
}

VinylDial::~VinylDial()
{
    stopTimer();
}

//==============================================================================
void VinylDial::paint(juce::Graphics &g)
{
    float dia = std::min((float)getWidth() - 10, (float)getHeight() - 10);
    g.setColour(Colours::slategrey);
    g.fillEllipse(((float)getWidth() / 2) - dia / 2, ((float)getHeight() / 2) - dia / 2, dia, dia);
    g.setColour(Colours::black);
    g.drawEllipse(((float)getWidth() / 2) - dia / 2, ((float)getHeight() / 2) - dia / 2, dia, dia, 3);
    g.drawEllipse(((float)getWidth() / 2) - dia / 1.3 / 2, ((float)getHeight() / 2) - dia / 1.3 / 2, dia / 1.3, dia / 1.3, 2);
    g.drawEllipse(((float)getWidth() / 2) - dia / 2 / 2, ((float)getHeight() / 2) - dia / 2 / 2, dia / 2, dia / 2, 2);
    g.setColour(Colours::yellow);
    g.fillEllipse(((float)getWidth() / 2) - dia / 3 / 2, ((float)getHeight() / 2) - dia / 3 / 2, dia / 3, dia / 3);
    g.setColour(Colours::white);
    g.fillEllipse(((float)getWidth() / 2) - dia / 6 / 2, ((float)getHeight() / 2) - dia / 6 / 2, dia / 6, dia / 6);
    g.setColour(Colours::orangered);
    g.addTransform(trans.rotation(rotInRad, (float)getWidth() / 2, (float)getHeight() / 2));
    g.drawRect((float)getWidth() / 2, (float)getHeight() / 2 - 2, dia / 2, 4.0f);

    // Image stuff
    //  float scale = std::min((float)getWidth() - 10, (float)getHeight() - 10) / img.getWidth();
    //  trans.rotation(rotInRad, (float)getWidth() / 2, (float)getHeight() / 2);
    //  g.drawImageTransformed(img, trans.scale(scale) + trans.rotation(rotInRad, (float)getWidth() / 2, (float)getHeight() / 2), false);
}

void VinylDial::resized()
{
    float scale = std::min((float)getWidth() - 10, (float)getHeight() - 10) / img.getWidth();
    trans.scale(scale);
}

void VinylDial::mouseDown(const MouseEvent &event)
{
}

void VinylDial::mouseDrag(const MouseEvent &event)
{
    spinning = true;
    int rot = event.getDistanceFromDragStartX() % 36;
    rot *= 10;
    rotInRad = degreesToRadians((double)rot);
    value = ((double)event.getDistanceFromDragStartX() - lastDist) / 5000;
    repaint();

    storeDist = event.getDistanceFromDragStartX();
}

void VinylDial::timerCallback()
{
    if (spinning)
    {
        value = storeDist - lastDist;
        lastDist = storeDist;
        value /= 10;
        sendChangeMessage();
    }
}

void VinylDial::mouseUp(const MouseEvent &event)
{
    value = 0;
    lastDist = 0;
    spinning = false;
    sendChangeMessage();
}

double VinylDial::getRelativeValue()
{
    return value;
}

bool VinylDial::isSpinning()
{
    return spinning;
}
