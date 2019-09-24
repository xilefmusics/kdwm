#ifndef CONFIG
#define CONFIG

#include <X11/Xlib.h>
#include <X11/keysym.h>

void wm_stop();
void spawn_term();


typedef struct wm_key {
    int mod;
    int keysym;
    void (*func)();
} wm_keybinding_t;

#define MODKEY Mod4Mask

static wm_keybinding_t wm_keybindings[] = {
    {MODKEY, XK_q, wm_stop},
    {MODKEY, XK_Return, spawn_term},
};

#endif
