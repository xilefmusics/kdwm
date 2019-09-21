#ifndef WINDOW_MANAGER
#define WINDOW_MANAGER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <X11/Xlib.h>
#include <X11/keysym.h>

#include "config.h"

// basic functions
void wm_init();
void wm_start();
void wm_stop();
void wm_tini();

// user controll of wm

#endif
