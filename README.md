# Geometry Wars Game

This repository contains the implementation of a kind of geometry wars in C++ using SFML library and IMGUI interface. The game is inspired by David's Churchill assignment on YouTube [here](https://www.youtube.com/watch?v=DLlGY1Po6Bk&list=PL_xRyXins84_Jf-aCh7chj47HR4oZLPwK&index=8).
The idea is to learn games development. From the video Description:

"_This is a course for students interested in learning the fundamentals of game programming and game engine architecture. Topics include an introduction to: vector math for games, rendering, animation, and artificial intelligence, collision detection, game physics, and user-interfaces. Students will be writing fully functional games using an ECS (Entities, Components, Systems) architecture, using the C++ programming language and the SFML graphics library._"

## Game Description

Geometry Wars is a 2D shooter game where the player controls a geometric figure that shoots bullets to destroy other polygons spawning on the screen. The objective is to survive as long as possible by avoiding collisions with the enemy polygons and destroying them.

##

In the *main* branch you will have nothing implemented, is the skeleton (more or less) of the assignment. In the *assignment_starts* branch you will see my attempt to make a game :) . Feel free to check it out and give me some better coding practices tips! Or maybe give the repo a star if you are just lazy to make the skeleton by yourself :D. 

## Prerequisites

To compile and run the game, you need to have the following dependencies installed:

- C++ compiler (e.g., GCC or Clang)
- SFML library
- IMGUI library

## Installation

1. Clone the repository:
```
   bash
   git clone https://github.com/nad-garraz/2dFigureWars-game.git
```

2. Install SFML library following the instructions for your operating system: [SFML Installation Guide](https://www.sfml-dev.org/tutorials/2.5/start-linux.php).

3. Install IMGUI library following the instructions provided by the library documentation: [IMGUI GitHub Repository](https://github.com/ocornut/imgui).

## Usage

1. Navigate to the project directory:
   ```
   bash
   cd 2dFigureWars-game
   ```

2. Compile the game using the provided Makefile:
   ```
   bash
   make
   ```

3. Run the game:
   ```
   bash
   make run
   ```

4. Use a, s, d, w keys to control the player figure and the mouse to shoot bullets. Try the right click to make a gravity trap.

## Cleaning Up

To remove the compiled binaries and object files, you can use the following command:
   ```
   bash
   make clean
   ```


## Contributing

Contributions to this project are welcome. If you find any issues or have suggestions for improvements, feel free to open an issue or submit a pull request.

## Acknowledgments

- David's Churchill tutorial on YouTube for the inspiration and guidance in building this game.
- SFML library for providing the necessary tools for game development.
- IMGUI library for the user interface implementation.

Enjoy playing and coding this Geometry Wars!
