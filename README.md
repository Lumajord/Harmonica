App that listens to the microphone and detects the pitch. If the frequency matches a harmonica note, the corresponding hole is highlighted.

usage on Linux:  
install glew, glu, and portaudio.  
cmake src/  
make  
./Harmonica

Pitch detection algorithm was inspired by: Fast, Accurate Pitch Detection Tools for Music Analysis by Philip McLeod (PhD thesis, 2008)

Project uses OpenGL, kissFFT, DspFilters, and portaudio.

![alt text](https://github.com/Lumajord/Harmonica/blob/main/Harmonica.png "Playing the fourth hole of a G diatonic harmonica.")
