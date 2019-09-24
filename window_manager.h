#ifndef WINDOW_MANAGER
#define WINDOW_MANAGER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

#include <X11/Xlib.h>

// makros
#define numlockmask 0
#define CLEANMASK(mask)         (mask & ~(numlockmask|LockMask) & (ShiftMask|ControlMask|Mod1Mask|Mod2Mask|Mod3Mask|Mod4Mask|Mod5Mask))
#define LENGTH(X)               (sizeof X / sizeof X[0])

// definitions
typedef enum {NONE, STRING, INTEGER, DOUBLE} wm_arg_types;
typedef struct wm_global {
    Display *display;
    Window root_window;
    int screen;
    int screen_width;
    int screen_height;
    bool running;
    FILE *log_fp;
} wm_global_t;

typedef struct wm_key {
    int mod;
    int keysym;
    void (*func)();
    wm_arg_types arg_type;
    char *arg;
} wm_keybinding_t;

// basic functions
void wm_init();
void wm_start();
void wm_stop();
void wm_tini();

// user controll of wm

#endif
