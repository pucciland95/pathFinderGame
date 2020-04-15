# Find the path game
This repository contains a little game written in C++ where the player needs to do ABSOLUTELY nothing except watching the computer play.
It is just to see it play.

The code uses windows.h header to handle the windows console.

The game is structured as follows:

- The game

- The player

- The path finding algorithm

The first update the console depending by the actions chosen by the algorithm.

The second is straighfoward what it is.

The third is basically the Probabilistic road map algorithm that runs in background and tells the player where to go from the starting position
(where the player is at) to the final position (where the player needs to go that in our case is the position of the 'food' that our player needs to eat).

So, the purpose of the player is simply to get to the food position by itself. Stop.

Note that it does not find the shortest path but one path. 
Sometime it takes a lot of time to work but at the end it always succedes (there must be a bug somewhere).

Feel free to change the code as you want.

PS: I am not a good programmer. 




