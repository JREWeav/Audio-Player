echo "Starting"

cmake -B build

cmake --build build

START ./build/DJApp_artefacts/Debug/"DJing for Gamers.exe"

PAUSE

TASKKILL /F /IM "DJing for Gamers.exe"