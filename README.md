FOCP END SEMESTER PROJECT

TEAM BEE:
Ali Ahmad-504418
Farjad Nawaz– 515336
Haqiq Azeem– 519071
Anas Bin Waheed – 502924



BUZZBOMBER  -    A C++ GAME USING SFML LIBRARY.
The basic idea of the game is that the player controls a spray can to kill the bees moving above by spraying them continuously as bees escape the spray by moving around. The bees try to get down and escape screen. If it gets hit by bullet, user gets points and bee turns to a honeycomb. Killing all bees completes each level as difficulty increases with every next level.

The detail of each component of game is as under.

Spray Can is controlled by player that moves with arrow keys on both sides and shoots spray with space bar. You will have 50 bullets. You get extra 25 bullets after each 50 points and maximum bullets remain 50.points.

Bees fly in tiers and move down to lower tier as they hit the corner on screen. If bees move down and escape screen, game over.

Two types of bees;
•	Worker or normal bees are slower and give 10 points per kill.
•	Killer or fast bees are fast and give 20 points per kill.


Honeycombs form when bees die. Your spray disappears the honeycomb. They give extra 50 points at end of each level so don’t kill them.


3 levels of game:
•	First Level:.3 normal and 1 fast bee
•	Second Level: 4 normal and 2 fast
•	Third Level: 5 normal and 3 fast
•	If you kill all bees, you will get points and move to next level. Points display on screen once you finish game or game over in between.

The game’s basic layers are as under;
The game begins as you open the window made in SFML,
Menu page has 2 buttons.
Play and Quit.
Audio starts playing when window opens.
Quit closes the program.
Enters takes you to play.
--------------------------------
Game starts at level 1. 
Once you clear level 1, you move to the next level. If you lose you see your points and go to menu page.
	--------------------------------------
If you finish game on level 3, it displays your total points and then takes you to menu page. You can see your bullets on left and points on right corner respectively. Control player by keys and space bar,
-------------------------------.

Project architecture displayed in slides as well as assignment three is also mentioned here.
It includes the distribution of modules between team members. 
Project architecture Modules.
1. Main Module; initializes all other modules and starts the game loop.
2. Input Module; listens for inputs and turns into actions through game logic.
3. Game Logic; processes inputs and updates the game state in other modules.
4. Movement Module; ensures accurate movement and detects collisions
5. Graphics Module; renders updated visuals to the screen.
6. Audio Module; plays corresponding sounds or music.
7. Resource Manager; ensures textures, fonts, and sounds are loaded efficiently.
8. User Interfaces Module; displays important gameplay information and menus

Task distribution among team members of modules:
•	Ali Ahmad: Main module and Audio module programme.
•	Anas Bin Waheed: User interface and Graphics module programme.
•	Farjad Nawaz: Game logic and Movement module programme.
•	Haqiq Azeem: Graphics module and Resource manager module.
