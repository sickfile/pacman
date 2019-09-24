# pacman
Pacman clone for win32 using SDL in one file pure imperative C.
https://www.libsdl.org/

The game is not complete, only level 1.
This is a one file source less then 1800 lines of code.
It is split into several include files only for editing purposes, there are no separate compilation units.

To build the executable use the Free Borland C++ Command-Line Tools.

bcc32 -tW pacman.c

Will do the job, all the libs and dlls are already on the repo.

The SDL development library used for this project is:
https://www.libsdl.org/release/SDL-devel-1.2.15-VC.zip

