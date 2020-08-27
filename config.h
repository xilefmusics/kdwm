#include "definitions.h"

// VARIABLES
static const int MASTER_WIDTH = 50;
static const int BORDER_WIDTH = 1;
/* static const char* BORDER_COLOR_ACTIVE = "#83a598"; */
static const char* BORDER_COLOR_ACTIVE = "#FF0000";
static const char* BORDER_COLOR_PASSIVE = "#282828";

// MODULE VARIABLES
#define NUM_OF_TAGS 9

// LAYOUTS
#include "layouts/masterstack.c"
#include "layouts/monocle.c"
enum {MASTERSTACK, MONOCLE};
static void (*layouts[])() =  {masterstack, monocle};

// MODULES 1
#include "modules/basic_key_handling/basic_key_handling.h"
#include "modules/basic_event_handling/basic_event_handling.h"
#include "modules/basic_user_control/basic_user_control.h"
#include "modules/pertag.c"
#include "modules/cancer.c"


// KEYBINDINGS
#define MODKEY Mod1Mask // ALT
//#define MODKEY Mod4Mask // Super
static basic_key_handling_keybinding_t basic_key_handling_keybindings[] = {
    // stop
    {MODKEY|ShiftMask, XK_q, wm_stop, NONE, NULL},
    // spawn
    {MODKEY, XK_Return, wm_spawn, STRING, "st"},
    {MODKEY, XK_r, wm_spawn, STRING, "dmenu_run"},
    // monitors
    {MODKEY|ControlMask, XK_m, wm_monitor_update, NONE, NULL},
    {MODKEY, XK_u, move_tag_mask_to_next_monitor, NONE, NULL},
    {MODKEY, XK_i, move_tag_mask_to_prev_monitor, NONE, NULL},
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
    {MODKEY, XK_g, wm_change_master_width, INTEGER, "0"},
    // switch tag
    {MODKEY, XK_1, pertag_retag, INTEGER, "1"},
    {MODKEY, XK_2, pertag_retag, INTEGER, "2"},
    {MODKEY, XK_3, pertag_retag, INTEGER, "4"},
    {MODKEY, XK_4, pertag_retag, INTEGER, "8"},
    {MODKEY, XK_5, pertag_retag, INTEGER, "16"},
    {MODKEY, XK_6, pertag_retag, INTEGER, "32"},
    {MODKEY, XK_7, pertag_retag, INTEGER, "64"},
    {MODKEY, XK_8, pertag_retag, INTEGER, "128"},
    {MODKEY, XK_9, pertag_retag, INTEGER, "256"},
    {MODKEY, XK_0, pertag_retag, INTEGER, "-1"},
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
    {MODKEY|ControlMask, XK_1, pertag_add_tag_to_tag_mask, INTEGER, "1"},
    {MODKEY|ControlMask, XK_2, pertag_add_tag_to_tag_mask, INTEGER, "2"},
    {MODKEY|ControlMask, XK_3, pertag_add_tag_to_tag_mask, INTEGER, "4"},
    {MODKEY|ControlMask, XK_4, pertag_add_tag_to_tag_mask, INTEGER, "8"},
    {MODKEY|ControlMask, XK_5, pertag_add_tag_to_tag_mask, INTEGER, "16"},
    {MODKEY|ControlMask, XK_6, pertag_add_tag_to_tag_mask, INTEGER, "32"},
    {MODKEY|ControlMask, XK_7, pertag_add_tag_to_tag_mask, INTEGER, "64"},
    {MODKEY|ControlMask, XK_8, pertag_add_tag_to_tag_mask, INTEGER, "128"},
    {MODKEY|ControlMask, XK_9, pertag_add_tag_to_tag_mask, INTEGER, "256"},
    {MODKEY|ControlMask, XK_0, pertag_add_tag_to_tag_mask, INTEGER, "-1"},
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

// MODULES 2
#include "modules/basic_event_handling/basic_event_handling.c"
#include "modules/basic_key_handling/basic_key_handling.c"
#include "modules/basic_user_control/basic_user_control.c"

// ON_START AND ON_STOP
static void (*wm_on_init[])() = {basic_event_handling_init, basic_key_handling_init};
static void (*wm_on_tini[])() = {};
