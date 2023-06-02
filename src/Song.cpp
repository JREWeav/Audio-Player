#include "Song.h"

//==============================================================================
Song::Song(String _name, URL _url, int _timeInSecs) : name(_name), url(_url), timeInSecs(_timeInSecs), time(timeFromSecs(_timeInSecs))
{
}

Song::~Song()
{
}

String Song::getName()
{
    return name;
}

URL Song::getURL()
{
    return url;
}

String Song::getTime()
{
    return time;
}

int Song::getTimeInSecs()
{
    return timeInSecs;
}

String Song::timeFromSecs(int secs)
{
    String minString;
    String secString;
    int secondsLeft = secs % 60;
    int mins = (secs - secondsLeft) / 60;
    if (mins < 10)
    {
        minString = "0" + (String)mins;
    }
    else
    {
        minString = (String)mins;
    }
    if (secondsLeft < 10)
    {
        secString = "0" + (String)secondsLeft;
    }
    else
    {
        secString = (String)secondsLeft;
    }
    return minString + ":" + secString;
}