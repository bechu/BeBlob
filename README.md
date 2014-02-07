# BeBlob

BeBlob is a basic strategy video game based on the original idea :
http://www.blobsallad.se/

![width:200px](images/welcome.png?raw=true)
![width:200px](images/ingame.png?raw=true)

## How to play
## Install

### For linux users :

You need to install libzip :
 * sudo apt-get install libzip-dev

and SFML 2.0 or higher :
 * http://www.sfml-dev.org

To compile by yourself :

You need a recent compiler (gcc or clang) because beblob was write in c++11.

 * git clone ..
 * cd source
 * mkdir build && cd build
 * cmake ..
 * make

Warning, to run the game need to be in same directory of blob.pack file !
to put the blob.pack (from assets directory) in the same folder of you bin
and launch it !

## Licence
The source code of BeBlob is released under GPL v3.0.

## About pieces of software

for the graphics tool, I used SFML :
http://www.sfml-dev.org/

for Particles, I used a part of the thor library :
http://www.bromeon.ch/libraries/thor/

for the entity/component system, i clone and update entityx :
https://github.com/alecthomas/entityx

for the event system I used simple signal :
https://testbit.eu/2013/cpp11-signal-system-performance/

Light engine was inspire from this tutorial :
http://gregouar.developpez.com/tutoriels/jeux/moteur-lumieres-dynamiques-2d/

and finaly to generate the world I used this perlin noise implementation :
https://github.com/sol-prog/Perlin_Noise

