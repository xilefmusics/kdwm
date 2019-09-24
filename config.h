#ifndef CONFIG
#define CONFIG

#include <X11/Xlib.h>
#include <X11/keysym.h>

#include "window_manager.h"

#define MODKEY Mod4Mask

static wm_keybinding_t wm_keybindings[] = {
    {MODKEY, XK_q, wm_stop, NONE, NULL},
    {MODKEY, XK_Return, system, STRING, "st &"},
};

#endif
