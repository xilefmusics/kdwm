#include "definitions.h"

#define MODKEY Mod4Mask

static wm_keybinding_t wm_keybindings[] = {
    {MODKEY|ShiftMask, XK_q, wm_stop, NONE, NULL},
    {MODKEY, XK_Return, system, STRING, "st &"},
    {MODKEY, XK_r, system, STRING, "dmenu_run &"},
};
