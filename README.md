Purpose
-------

This is an example of how to use CMake to build C++ executables
that link to [Aquila DSP library](http://aquila-dsp.org/).

Prerequisites
-------------

Download, compile and install the library as
[described here](http://aquila-dsp.org/download/). On Linux, this usually
creates a static library `libAquila.a` and installs it to `/usr/local/lib/`.
When installing on Windows, set `CMAKE_INSTALL_PREFIX` to a directory where
you want Aquila to be installed. This root path will be required later.

Building and running the example
--------------------------------

First, get the source code of the example from GitHub. Following the best
practices, we will also create a separate directory for out-of-source build:

    git clone https://github.com/Sound-drop/standAloneAquila.git
    cd standAloneAquila
    vim Makefile

Modify the path to your aquila-src path

https://github.com/Sound-drop/standAloneAquila/blob/master/Makefile#L4

Next steps differ a little, depending on your operating system.


On Linux the following should *just work*:

    make
    ./test

