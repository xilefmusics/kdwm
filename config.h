#include "definitions.h"

// MODULES (.h)
#include "modules/basic_event_handling/basic_event_handling.h"
#include "modules/basic_key_handling/basic_key_handling.h"
#include "modules/basic_user_control/basic_user_control.h"
#include "modules/pertag/pertag.h"
#include "modules/multimon/multimon.h"
#include "modules/cancer/cancer.h"
// #include "modules/kdwmc_server/kdwmc_server.h"
// #include "modules/autostart/autostart.h"

// VARIABLES
static const int MASTER_WIDTH = 50;
static const int BORDER_WIDTH = 1;
static const int OFFSET_TOP = 0;
static const int OFFSET_BOTTOM = 0;
static const int OFFSET_LEFT = 0;
static const int OFFSET_RIGHT = 0;
static const int GAPS = 0;
static const char* BORDER_COLOR_ACTIVE = "#cc241d";
static const char* BORDER_COLOR_PASSIVE = "#262626";
static const char* DO_NOT_HANDLE[] = {};

// MODULE VARIABLES
#define NUM_OF_TAGS 9

// LAYOUTS
#include "layouts/masterstack.c"
#include "layouts/monocle.c"
#include "layouts/fullscreen.c"
enum {MASTERSTACK, MONOCLE, FULLSCREEN};
static const void (*layouts[])(int x, int y, int w, int h) =  {masterstack, monocle, fullscreen};

// AUTOSTART
// static const char *autostart_commands[] = {};

// INSERT FUNCTIONS
static const void (*wm_on_init[])() = {
  basic_event_handling_init,
  basic_key_handling_init,
//  kdwmc_server_start,
//  autostart
};
static const void (*wm_on_tini[])() = {
//  kdwmc_server_stop
};
static const void (*wm_on_retag[])(int tag_mask) = {
  multimon_on_retag,
  pertag_configure
};
static const void (*wm_on_add_tag_to_tag_mask[])(int tag) = {
  pertag_configure
};
static const void (*wm_on_arrange[])() = {
//  kdwmc_server_subscribe
};

// KEYBINDINGS
// #define MODKEY Mod1Mask // ALT
#define MODKEY Mod4Mask // Super
static const basic_key_handling_keybinding_t basic_key_handling_keybindings[] = {
  // stop
  {MODKEY|ShiftMask, XK_q, wm_stop, NONE, NULL},
  // audio
  {0, XF86XK_AudioLowerVolume, wm_spawn, STRING, "amixer set Master 5%-"},
  {0, XF86XK_AudioRaiseVolume, wm_spawn, STRING, "amixer set Master 5%+"},
  {0, XF86XK_AudioMute, wm_spawn, STRING, "amixer set Master toggle"},
  // brightness
  {0, XF86XK_MonBrightnessUp, wm_spawn, STRING, "xib +5"},
  {0, XF86XK_MonBrightnessDown, wm_spawn, STRING, "xib -5"},
  // spawn
  {MODKEY, XK_b, wm_spawn, STRING, "qutebrowser"},
  {MODKEY|ShiftMask, XK_b, wm_spawn, STRING, "chromium"},
  {MODKEY|ShiftMask, XK_t, wm_spawn, STRING, "telegram-desktop"},
  {MODKEY, XK_r, wm_spawn, STRING, "dmenu_run"},
  {MODKEY|ControlMask, XK_d, wm_spawn, STRING, "dmenu_monitor"},
  {MODKEY, XK_Return, wm_spawn, STRING, "st"},
  {MODKEY, XK_e, wm_spawn, STRING, "st -e lf"},
  {MODKEY|ShiftMask, XK_m, wm_spawn, STRING, "st -e cmus"},
  // system
  {MODKEY|ControlMask, XK_l, wm_spawn, STRING, "i3lock -c 000000 && systemctl suspend"},
  {MODKEY|ControlMask, XK_s, wm_spawn, STRING, "systemctl suspend"},
  // keyboardlayouts
  {MODKEY|ControlMask, XK_u, wm_spawn, STRING, "setxkbmap us"},
  {MODKEY|ControlMask, XK_g, wm_spawn, STRING, "setxkbmap de"},
  // monitors
  {MODKEY|ControlMask, XK_m, multimon_update, NONE, NULL},
  {MODKEY, XK_u, multimon_move_tag_mask_to_next_monitor, NONE, NULL},
  {MODKEY, XK_i, multimon_move_tag_mask_to_prev_monitor, NONE, NULL},
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
  {MODKEY, XK_f, wm_change_layout, INTEGER, "2"},
  // change master width
  {MODKEY, XK_h, wm_change_master_width, INTEGER, "-5"},
  {MODKEY, XK_l, wm_change_master_width, INTEGER, "5"},
  {MODKEY, XK_g, wm_change_master_width, INTEGER, "0"},
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

// MODULES (.c)
#include "modules/basic_event_handling/basic_event_handling.c"
#include "modules/basic_key_handling/basic_key_handling.c"
#include "modules/basic_user_control/basic_user_control.c"
#include "modules/pertag/pertag.c"
#include "modules/multimon/multimon.c"
#include "modules/cancer/cancer.c"
// #include "modules/kdwmc_server/kdwmc_server.c"
// #include "modules/autostart/autostart.c"
