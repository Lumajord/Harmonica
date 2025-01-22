App that listens to the microphone and detects the pitch. If the frequency matches a harmonica note, the corresponding hole is highlighted.

usage on Linux:  
install [glew](https://glew.sourceforge.net/install.html), [glut](https://freeglut.sourceforge.net/), and [portaudio](https://www.portaudio.com/).  
[cmake](https://cmake.org/download/) src/  
make  
./Harmonica

Pitch detection algorithm was inspired by: Fast, Accurate Pitch Detection Tools for Music Analysis by Philip McLeod ([PhD thesis, 2008](https://www.cs.otago.ac.nz/research/publications/oucs-2008-03.pdf))

Project uses [OpenGL](https://www.opengl.org/), [kissFFT](https://github.com/mborgerding/kissfft), [DspFilters](https://github.com/vinniefalco/DSPFilters), and [portaudio](https://www.portaudio.com/).

![alt text](https://github.com/Lumajord/Harmonica/blob/main/Harmonica.png "Playing the fourth hole of a G diatonic harmonica.")
