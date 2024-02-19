# kdwm

A simple keyboard driven windowmanager for X

# Dependencies

Only the X-Server (including Xlib and Xinerama)

# Installation

In order to install kdwm you need to:

1. clone the kdwm repository
2. navigate to the cloned repository
3. execute the following two commands

```bash
make
make install
```

4. and edit your .xinitrc such, that it executes the produced binary (usually kdwm) upon startup of the X-Server (if done manually with the command `startx`)

# Usage

Kdwm is a list based window manager, which restricts the ways the clients can be arranged to the order, in which they are encountered while iterating over the list.
Layouts arrange the clients on the screen based on the aforementioned order (the master stack layout places the first encountered client on the left and all the rest in a stack on the right).
Before you first start kdwm, you should familiarize yourself with the keybindings found in the config.h, or change them.
Furthermore you should make sure to substitute any command (like opening a terminal) with a command present on your machine.

# Configuration

Kdwm doesn't make use of a config file, modifications to the default behavior must therefore be made to the source code. 
Afterwards the code has to be recompiled (the steps 2 and 3 of the Installation). 
The most common modifications, such as changing keybindings and including modules (from the modules folder), can be made by editing the config.h file.
For further Information on the modules take a look at the according .md files (work in progress).

## License

[![Apache License 2.0](https://img.shields.io/badge/License-Apache_2.0-blue.svg)](LICENSE)
