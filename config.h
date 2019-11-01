#include "definitions.h"

// VARIABLES
static const int MASTER_WIDTH = 50;
static const int BORDER_WIDTH = 1;
static const char* BORDER_COLOR_ACTIVE = "#61afef";
static const char* BORDER_COLOR_PASSIVE = "#21252b";
static const int FULLSCREEN_TAG_MASK = 512;

// LAYOUTS
#include "layouts/masterstack.c"
#include "layouts/monocle.c"
enum {MASTERSTACK, MONOCLE};
static void (*layouts[])() =  {masterstack, monocle};

// MODULES
#include "modules/logger.c"
#include "modules/default_user_control.c"
#include "modules/open_windows_on_start.c"

// ON_START AND ON_STOP
static void (*wm_on_init[])() = {open_windows_on_start};
static void (*wm_on_tini[])() = {};

// KEYBINDINGS
#define MODKEY Mod4Mask
static wm_keybinding_t wm_keybindings[] = {
    {0, XF86XK_AudioLowerVolume,     wm_spawn, STRING, "amixer set Master 5%-"},
    {0, XF86XK_AudioRaiseVolume,     wm_spawn, STRING, "amixer set Master 5%+"},
    {0, XF86XK_AudioMute,        wm_spawn, STRING, "amixer set Master toggle"},
    {MODKEY, XK_s, wm_spawn, STRING, "supertuxkart"},
    {MODKEY, XK_p, log_state, NONE, NULL},
    // stop
    {MODKEY|ShiftMask, XK_q, wm_stop, NONE, NULL},
    // spawn
    {MODKEY, XK_Return, wm_spawn, STRING, "st"},
    {MODKEY, XK_r, wm_spawn, STRING, "dmenu_run"},
    // clients
    {MODKEY, XK_j, wm_focus_next, NONE, NULL},
    {MODKEY, XK_k, wm_focus_prev, NONE, NULL},
    {MODKEY|ShiftMask, XK_j, wm_client_down, NONE, NULL},
    {MODKEY|ShiftMask, XK_k, wm_client_up, NONE, NULL},
    {MODKEY|ShiftMask, XK_Return, wm_rehead, NONE, NULL},
    {MODKEY, XK_q, wm_kill_active_client, NONE, NULL},
    // layouts
    {MODKEY, XK_t, wm_change_layout, INTEGER, "0"},
    {MODKEY, XK_m, wm_change_layout, INTEGER, "1"},
    // change master width
    {MODKEY, XK_h, wm_change_master_width, INTEGER, "-5"},
    {MODKEY, XK_l, wm_change_master_width, INTEGER, "5"},
    {MODKEY, XK_z, wm_change_master_width, INTEGER, "0"},
    // switch tag
    {MODKEY, XK_1, wm_retag, INTEGER, "1"},
    {MODKEY, XK_2, wm_retag, INTEGER, "2"},
    {MODKEY, XK_3, wm_retag, INTEGER, "4"},
    {MODKEY, XK_4, wm_retag, INTEGER, "8"},
    {MODKEY, XK_5, wm_retag, INTEGER, "16"},
    {MODKEY, XK_6, wm_retag, INTEGER, "32"},
    {MODKEY, XK_7, wm_retag, INTEGER, "64"},
    {MODKEY, XK_8, wm_retag, INTEGER, "128"},
    {MODKEY, XK_9, wm_retag, INTEGER, "256"},
    {MODKEY, XK_0, wm_retag, INTEGER, "-1"},
    {MODKEY, XK_f, wm_retag, INTEGER, "512"},
    // move client to tag
    {MODKEY|ShiftMask, XK_1, wm_set_tag_mask_of_focused_client, INTEGER, "1"},
    {MODKEY|ShiftMask, XK_2, wm_set_tag_mask_of_focused_client, INTEGER, "2"},
    {MODKEY|ShiftMask, XK_3, wm_set_tag_mask_of_focused_client, INTEGER, "4"},
    {MODKEY|ShiftMask, XK_4, wm_set_tag_mask_of_focused_client, INTEGER, "8"},
    {MODKEY|ShiftMask, XK_5, wm_set_tag_mask_of_focused_client, INTEGER, "16"},
    {MODKEY|ShiftMask, XK_6, wm_set_tag_mask_of_focused_client, INTEGER, "32"},
    {MODKEY|ShiftMask, XK_7, wm_set_tag_mask_of_focused_client, INTEGER, "64"},
    {MODKEY|ShiftMask, XK_8, wm_set_tag_mask_of_focused_client, INTEGER, "128"},
    {MODKEY|ShiftMask, XK_9, wm_set_tag_mask_of_focused_client, INTEGER, "256"},
    {MODKEY|ShiftMask, XK_0, wm_set_tag_mask_of_focused_client, INTEGER, "-1"},
    // display multiple tags
    {MODKEY|ControlMask, XK_1, wm_add_tag_to_tag_mask, INTEGER, "1"},
    {MODKEY|ControlMask, XK_2, wm_add_tag_to_tag_mask, INTEGER, "2"},
    {MODKEY|ControlMask, XK_3, wm_add_tag_to_tag_mask, INTEGER, "4"},
    {MODKEY|ControlMask, XK_4, wm_add_tag_to_tag_mask, INTEGER, "8"},
    {MODKEY|ControlMask, XK_5, wm_add_tag_to_tag_mask, INTEGER, "16"},
    {MODKEY|ControlMask, XK_6, wm_add_tag_to_tag_mask, INTEGER, "32"},
    {MODKEY|ControlMask, XK_7, wm_add_tag_to_tag_mask, INTEGER, "64"},
    {MODKEY|ControlMask, XK_8, wm_add_tag_to_tag_mask, INTEGER, "128"},
    {MODKEY|ControlMask, XK_9, wm_add_tag_to_tag_mask, INTEGER, "256"},
    {MODKEY|ControlMask, XK_0, wm_add_tag_to_tag_mask, INTEGER, "-1"},
    // add multiple tags to client
    {MODKEY|ControlMask|ShiftMask, XK_1, wm_add_tag_to_client, INTEGER, "1"},
    {MODKEY|ControlMask|ShiftMask, XK_2, wm_add_tag_to_client, INTEGER, "2"},
    {MODKEY|ControlMask|ShiftMask, XK_3, wm_add_tag_to_client, INTEGER, "4"},
    {MODKEY|ControlMask|ShiftMask, XK_4, wm_add_tag_to_client, INTEGER, "8"},
    {MODKEY|ControlMask|ShiftMask, XK_5, wm_add_tag_to_client, INTEGER, "16"},
    {MODKEY|ControlMask|ShiftMask, XK_6, wm_add_tag_to_client, INTEGER, "32"},
    {MODKEY|ControlMask|ShiftMask, XK_7, wm_add_tag_to_client, INTEGER, "64"},
    {MODKEY|ControlMask|ShiftMask, XK_8, wm_add_tag_to_client, INTEGER, "128"},
    {MODKEY|ControlMask|ShiftMask, XK_9, wm_add_tag_to_client, INTEGER, "256"},
    {MODKEY|ControlMask|ShiftMask, XK_0, wm_add_tag_to_client, INTEGER, "-1"},
};
