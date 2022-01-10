PROG2002 Graphics Programming 
ASSIGNMENT 2

Group 32
Members:
	- Anders Brunsberg Mariendal
	- Ole Kristian Elnæs

How to run (visual studio): 
	1. Copy repository web link
	2. Open Visual Studio
	3. Select "Clone a repository"
	4. Add repository link to "repository location" and click "clone"
	5. When is finished CMake configuring the project, select "pacman_3D.exe" as
	   startup item, then run it
	6. Now, the program should run, and a playable 3d version of pacman should
	   should pop up on your screen

FUNCTIONALITY:
	* The program shuts down when:
		a. All pellets are collected, or
		b. When pacman collides with a ghost.
	* The ghosts have the appearance of round, flying balls/polygons.
	  with a pixelated red texture
	* Everything turns redder when pacman is closer to a ghost, as a way to warn the 
	  player. We chose this instead of a minimap.
	* Pellets are collected as the pacman strolls over them.
	* Walls cannot be penetrated(1)
		(1) See KNOWN BUGS for an exception.
	* When the pacman walks out of the map on either side, in the middle, where there is 
	  in opening, the pacman is teleported to the other side. 


CONTROLS:
ESC: exit the game
W: move forward
S: move backwards
A: move left
D: move right
Mouse movement: changes view

KNOWN BUGS:
	* Sometimes the light turns more red than should be possible, seemingly out of the blue.
	* One roof tile and one floor tile is missing somewhere in the middle of the map
	* Player can get stuck in wall corners if attempted


VERSIONING: 
	* We did not have access to the group folder on gitlab for some time (we weren't able 
	to request access either) so we pushed versions to a private github repo instead. 
	

COMMITS, commit messages:
1: Initial commit
2: Shader class added
3: GameRenderer class added
4: Walls and pellets are rendering with textures
5: Updated with more textures; floor and wall tiles are now rendering
6: Added lighting, fixed y position and some gameplay methods added
7: Pellets disappear when collected, ghosts now have a simple direction selection algorithm
8: 3D object rendering and gameplay improved
