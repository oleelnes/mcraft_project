# mcraft_project

A game LIKE (but neither identical to nor a direct copy of) MineCraft (by MOJANG, go buy it) 


LIKECRAFT
by Ole Kristian Elnæs



Version 0.1.2: 
* Whole world is not rendered at once anymore.
* Fixed some major flaws in the algorithm that finds visible blocks.
* + Failed attempts at various things.

Version 0.1 is done when these things are implemented:
* Storing game data on computer.
* Placing and destroying blocks.
* Humidity- and height noise maps.
* "Infinite" world generation

Plans for 0.2:
* Biomes.
* Lakes.
* Oceans.
* Rivers(?).
* Collision and gravity mechanics.




Controls:
W: Forward
S: Backward
A: Left
D: Right
Q: Up
E: Down
Mouse movement: look around; set direction.



Textures:
All textures are self-made, but obiously inspired by Minecraft by Mojang.



Versions:
0.1.2: Current version -- see details above.

0.1.1: 
* Improved illumination. 
* Made it possible for blocks to be transparent.
	- Added water block (but not its functionality)
* Improved terrain generation.
* Set the framerate to 30 fps (I think?)
* Some code tidying; though the state of the program/code still is abhorrent by conventional standards.
 
0.1.0: 
* Added PerlinNoise-like algorithm to calculate terrain (top points).
* Added algorithms to cover the remaining terrain region with dirt blocks without grass (so that there are no holes in the terrain).

First commit: 
* Purely experimental -- only goal was to get a couple of hundred blocks on screen with limited 
abstraction. 
* Most of the essential structs and usages of std::vector in this version will be thrown away.
