#ifndef CONFIG
#define CONFIG

#include <X11/Xlib.h>
#include <X11/keysym.h>

typedef struct wm_key {
    int mod;
    int keysym;
} wm_keybinding_t;

#define MODKEY Mod4Mask

static wm_keybinding_t wm_keybindings[] = {
    {MODKEY, XK_q},
};
static const int NUM_OF_KEYBINDINGS = 1;

#endif
