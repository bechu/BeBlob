# BeBlob

BeBlob is a basic strategy video game based on the original idea :
http://www.blobsallad.se/

![](images/welcome.png?raw=true)
![](images/ingame.png?raw=true)

## How to play

First select a seed and a color, then click on Play !

Next you can control your blob, the concept of the game is to protect your home (the house with same color of you blob)
and trying to destruct the home of the AI.

You can :
 * Eat : click on the Blob and right click on a flower, the blob eat the flower and when the blob have enough eat, it is able to split itself;
 * Build : click on the Blob and right click on somewere block is empty, the blob build a block in place;
 * Destruct : click on the Blob and right click on wall, the blob will hit the block.
 * Move : click on the Blob, it follow your cursor.


Remark : Each blob bring a light and you house have a light too.

Second remark : Fireworks still visible on the top of dark (so you can see when the ai come to destruct you house !

## Install

### For linux users :

You need to install libzip :
 * sudo apt-get install libzip-dev

and SFML 2.0 or higher :
 * http://www.sfml-dev.org

To compile by yourself :

You need a recent compiler (gcc or clang) because beblob was written in c++11.

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

