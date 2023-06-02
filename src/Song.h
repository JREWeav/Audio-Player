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
class Song
{
public:
    Song(String _name, URL _url, int _timeInSecs);
    ~Song();
    //==============================================================================
    String getName();

    URL getURL();

    String getTime();

    int getTimeInSecs();

private:
    //==============================================================================
    // Your private member variables go here...

    String name;
    URL url;
    int timeInSecs;
    String time;

    String timeFromSecs(int secs);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Song)
};
