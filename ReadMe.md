
*********** NB *********************
this repo is not ready for pull requests
Key Actions:

This software is available for use under the below terms and conditions. 
The terms and conditions may change at any time with out notice and are applicable to contemporary version and future versions of the software, until the terms and conditions changes.

MIT License (Modified)

Copyright (c) [2023] [Matin Mazid]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

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


NCURSES Licence
https://en.wikipedia.org/wiki/Ncurses
Text and history of the ncurses library are found here:
https://invisible-island.net/ncurses/ncurses-license.html

libCurl Licence
https://invisible-island.net/ncurses/ncurses-license.html

log.c
https://github.com/rxi/log.c.git

Dev environment setup
1. install curl dev library with headers
eg
```
apt-get install libcurl4-openssl-dev
```

2. install ncures dev library with  headers
eg
```
 apt-get install libncurses5-dev libncursesw5-dev
```

test server
https://httpbin.org/


gui.c contains main()

gdb -p `ps -ef|grep hammock |grep -v grep |sed 's/\s\+/ /g'| cut -d' ' -f2`