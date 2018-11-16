This project is based on the well known game, Tetris.

You can play it here: https://tetris.com/play-tetris
And you can read about it here: https://en.wikipedia.org/wiki/Tetris
 
Command line arguments:
-aps : prints the actions per second (updates/draws to screen)
-l   : skips the loading screen

Controls:
    Singleplayer or player one:
        Left arrow key  - move piece left
        Right arrow key - move piece right
        Up arrow key    - rotate piece
        Down arrow key  - fast-falling
        Full stop       - instant drop
        Enter           - hold piece
    Player two:
        A               - move piece left
        D               - move piece right
        W               - rotate piece
        S               - fast-falling
        Space bar       - instant drop
        E               - hold piece

Required libraries: SDL2, SDL_ttf, SDL_image, math.h

Uses the following fonts:
    Roboto     (under the Apache license - http://www.apache.org/licenses/LICENSE-2.0.txt)
    Diskoteque (freeware for personal use)

Important points:
    -redrawing
    -testing
