#include "definitions.h"

#define MODKEY Mod4Mask

static wm_keybinding_t wm_keybindings[] = {
    {MODKEY|ShiftMask, XK_q, wm_stop, NONE, NULL},
    {MODKEY, XK_Return, wm_spawn, STRING, "st"},
    {MODKEY, XK_r, wm_spawn, STRING, "dmenu_run"},
    {MODKEY, XK_j, wm_focus_next, NONE, NULL},
    {MODKEY, XK_k, wm_focus_prev, NONE, NULL},
};
