# Percolation

## What is this?

This is an implementation of the [Disjoint-set](https://en.wikipedia.org/wiki/Disjoint-set_data_structure) algorithm to check if the grid [Percolates](https://en.wikipedia.org/wiki/Percolation_theory). The program launches a GUI that allows you to poke holes in a grid, these holes will be represented as white cells. Once there is a pass through from the top to the bottom of the grid, the program will notify that the grid percolates. Diagonals do not count as a pass through.

<h2>Random Mode</h2>
<img src="./screenshots/Percolation Random.gif" alt="Percolation User Mode" style="width: 400px;" />

<h2>User Mode</h2>
<img src="./screenshots/Percolation User.gif" alt="Percolation User Mode" style="width: 400px;" />

## Usage

⚠ Warning: The grid size cannot be above 1/2 of the window size, otherwise the grid will not render. If you wish to test a larger grid, please increase the window size to at least 2x the desired grid size in the rendering.c file.

User Mode: `bin user [grid size]`\
Random Mode: `bin random [milliseconds interval] [grid size]`

The `milliseconds interval` can be set to zero fastest possible simulation.
You can also change the parameters in the Makefile and call `make run`.

## Setup

⚠ I have currently setup the configuration for my Windows Mingw environment. Feel free to play around to get this working for your system, and PRs are welcome!

If you are using Mingw on windows:

- Install the SDL source code for the mingw environment
- Create a `/src` folder in the root of your project
- CD into either `/i686-w64-mingw32` or `/x86_64-w64-mingw32`
- Copy the `/include` and `/lib` folders into your project's `/src` folder
- CD into the `/bin` folder of your chosen source folder
- Copy the `SDL2.dll` file into the root of your project
- Run `make` to build
- Run `make run` to run the program
