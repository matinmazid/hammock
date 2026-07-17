*********** NB *********************
this repo is not ready for pull requests

# hammock

A terminal-based REST client built with `ncurses` and `libcurl`.

## Overview

`hammock` opens a full-screen text UI with three panels:
- `URL` input bar at the top
- `LEFT` window for request headers/body or input content
- `RIGHT` window for REST response output

### Run

```sh
make hammock
./bin/hammock
```

## Controls
- `Ctrl+Q` — quit the program
- `Ctrl+L` — clear and redraw the screen
- `Ctrl+C` — clear content in the current active window
- `Ctrl+H` — open the headers/menu interface
- `Ctrl+Tab` — switch the active window
- `Up` / `Down` in the `URL` bar — cycle HTTP methods
- `Enter` in the `URL` bar or `Ctrl+E` — execute the REST call
- `Backspace` — delete the last character from the active window input
- Printable characters — append to the active window content
The program runs a single event loop inside `src/gui.c`. It redraws the UI each iteration, reads user input with `getch()`, and dispatches commands until the user exits.


### Active window model

The active window is tracked by `activeWindowPtr % 3` and cycles through:
1. `URL`
2. `LEFT`
3. `RIGHT`

When `URL` is active, Enter submits the current URL and selected HTTP method.
When `LEFT` or `RIGHT` is active, arrow keys scroll the content if needed.


## Build and run


1. install curl dev library with headers

```sh
apt-get install libcurl4-openssl-dev
```

2. install ncurses dev library with headers

```sh
apt-get install libncurses5-dev libncursesw5-dev
```

### Compile

```sh
git clone https://github.com/matinmazid/hammock.git
cd hammock
make
```


## Notes

- `src/gui.c` contains `main()` and the primary event loop.
- `src/scratch.c` is a toy program used for debugging and diagnostics.
- The current event loop is implemented with `ncurses` input handling and a redraw cycle.
- The `RIGHT` window displays the REST response body.

### Event loop behavior

The event loop in `src/gui.c`:
- redraws all windows every cycle
- reads keyboard input from the terminal
- handles window resize events
- supports navigation, scrolling, and REST execution
- exits cleanly on `Ctrl+Q`
## Licenses

MIT License (Modified)

Copyright (c) [2023] [Matin Mazid]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions.

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

You try to be a good person.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

## Additional licences

NCURSES Licence
https://en.wikipedia.org/wiki/Ncurses
Text and history of the ncurses library are found here:
https://invisible-island.net/ncurses/ncurses-license.html

libCurl Licence
https://invisible-island.net/ncurses/ncurses-license.html

log.c
https://github.com/rxi/log.c.git
