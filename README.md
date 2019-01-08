# Mortal Kombat SNES

A C++ Term Project for CS-454( Development of Intelligent Interfaces and Games).

[Tutorial for SDL Library](http://lazyfoo.net/tutorials/SDL/index.php?fbclid=IwAR2hwyeCbnVHh-ox0zPzSzhnLEGnWcGhVAI9YtB1F6KZPcjbdlgg1rItjek)

Project Presentation ( In Greek ) [Link to Project](https://drive.google.com/open?id=1_n5jHUqKk16nBuf6OLlZ4vMkL-UAMaYm).

## Screenshots

![Game Screenshot] (https://lh3.googleusercontent.com/CfhSBBvfE75q862z_wm4C7enJSunkFVzve3OtsGglHfCM0nF9wJAIDfr0RWI5nnVJi-1o3HV_Z3vP7UznWyEjsYHIHLEYEci5YI5jMzaP70xRpEFIhDyeGLIGcEqrYmrKR7RPiFK=w2400 "Starting Screen")
![Game Screenshot] (https://lh3.googleusercontent.com/hlb-BhncY7BEh68B0RW70QhsgY_2diUWTwupljfky8IyKWK76AsdrMYvDqm0do__md6C4tfniEqanjfSQA4-rqx9tF0mU2DWwIeBmqJQ-3-VDrAsj7_F99u1sNvHwfvTYECQOx-g=w2400 "In Game")
![Game Screenshot] (https://lh3.googleusercontent.com/RSMPDVwljTIGYSgD9I4KMWpouizrowVawxizW250ng9ggNb8EzcWNV6-WVMn2unFWKh-S1bwep8ncqiHfzr9YvRKDcZzFOHWQuGhXEgMulYeTYxKbXI5igHocODCbbm3jrCpxtSE=w2400 "End of Game")

## Dependencies

Developed with Visual Studio 2017 (Debug x86 option at visual)

### Libraries
These libraries must be connected to the project ( [Tutorial Lesson 6](http://lazyfoo.net/tutorials/SDL/06_extension_libraries_and_loading_other_image_formats/index.php) on how to add libraries of SDL to VS Project) [VS Libs](https://drive.google.com/open?id=16KXC0-F-YL5EojR4VP8VPiTfx7kc_KPs)

And these dll files should be inserted to the MortalCombat project file [Project dll](https://drive.google.com/open?id=1Dle_k91aa4KPUmrSsZ_fKfRseakXk5Ul).
### Media 
The Game uses some images and sounds.[Here](https://drive.google.com/open?id=1NsByBXEabQzJ3YbzW0uFtkHMhTeCUBfO) you can find them. Add them named as media inside the Mortal Kombat file of the project.

## Game
This Mortal Kombat version is player VS  player. There are two characters implemented subzero and the scorpion.

##### Subzero Input
>PUNCH: Z 
>KICK: X
>BLOCK: C
>DUCK: S
>JUMP: W
>FWD: D
>BACK: A
>UPERRCUT: S + Z
>LOWKICK: S+X
>LOWBLOCK: S+C
>DOWNFLIP FWD: S+D
>DOWNFLIP BACK: S+A
>UPFLIP FWD: W+D
>UPFLIP BACK: W+A
>FREEZE COMBO: S+ (Q+A) COMBINED

##### Scorpion Input
>PUNCH: M
>KICK: N
>BLOCK: B
>DUCK: K
>JUMP: I
>FWD: J
>BACK: L
>UPERRCUT: K+ M
>LOWKICK: K+N
>LOWBLOCK: K+N
>DOWNFLIP FWD: K+J
>DOWNFLIP BACK: K+L
>UPFLIP FWD: I+J
>UPFLIP BACK: I+L
>FREEZE COMBO: K+ (L+O) COMBINED

