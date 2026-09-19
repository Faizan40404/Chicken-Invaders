# Chicken Invaders (Console Edition)

A text-based take on the classic Chicken Invaders, built entirely in C++ and running straight in the terminal — no graphics library, just characters moving around a grid.

You're `$`, the chickens are `&`, your bullets are `*`, and their eggs are `O`. Shoot the chickens before they reach the bottom or hit you, dodge the eggs, survive, level up, try to land a spot on the leaderboard.

## How to play

- `W` / `A` / `S` / `D` to move
- `Space` to fire
- Killing a chicken = 10 points
- Getting hit by a chicken or an egg costs you a life (start with 3)
- If a chicken makes it all the way to the bottom row, you lose 3 lives instantly
- Score enough and you level up — levels get faster and throw more chickens at you
- Reach level 4 and you've cleared the game

## Menu

On launch it asks for your name (used for the leaderboard), then drops you into a simple menu:

1. Start Game
2. Tutorial — quick rundown of controls
3. Leader Board — top 10 scores, read from/written to `HighScore.txt`
4. Exit

## How it's built

Everything runs on a fixed-size character grid (20 rows x 40 columns) that gets redrawn every frame. Player, bullets, chickens, and eggs are all just tracked as arrays of row/col positions with "is this slot active" flags, and each game loop iteration:

- reads your input and moves you / fires a bullet
- moves existing bullets up
- spawns and moves chickens
- randomly drops eggs from chickens
- moves existing eggs down
- checks all the collisions
- redraws the whole grid

Difficulty scales through a `level` variable — it changes chicken spawn rate, egg drop odds, and how fast the loop runs (shorter sleep = faster game).

## High scores

Top 10 scores get saved to `HighScore.txt` sitting next to the executable, sorted highest to lowest, and get reloaded and updated after every game so the leaderboard persists between sessions.
