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
- `Ctrl+Q` — quit the program, exit menu
- `delete` - while in a menu, exit menu
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
0. Prequisites
Build tools: 
- autoconf
- automake
- libtool


1. install curl dev library with headers

```sh
apt-get install libcurl4-openssl-dev
```

2. install ncurses dev library with headers

```sh
apt-get install libncurses5-dev libncursesw5-dev
```
3. install jansson
https://jansson.readthedocs.io/en/latest/gettingstarted.html#compiling-and-installing-jansson
required version of 2.15.1


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

This project is licensed under the MIT License. See [LICENSE](LICENSE).

Third-party software and license information are listed in [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).
