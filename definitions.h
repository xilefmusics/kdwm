#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

#include <X11/Xlib.h>
#include <X11/extensions/Xinerama.h>
#include <X11/keysym.h>
#include <X11/XF86keysym.h>
#include <X11/Xatom.h>

// makros
#define LENGTH(X)               (sizeof X / sizeof X[0])

// definitions

enum {WM_PROTOCOLS, WM_DELETE_WINDOW, _NET_WM_NAME, _NET_SUPPORTING_WM_CHECK, UTF8_STRING};

typedef struct wm_client {
    Window window;
    struct wm_client *next;
    struct wm_client *prev;
    int tag_mask;
    int x, y, w, h;
} wm_client_t;

typedef struct wm_client_list {
    wm_client_t *head_client;
    wm_client_t *active_client;
    int size;
} wm_client_list_t;

typedef struct wm_monitor {
    int x, y, w, h;
    int tag_mask;
    int active_tag_mask;
    struct wm_monitor *next;
    struct wm_monitor *prev;
} wm_monitor_t;

typedef struct wm_monitor_list {
    wm_monitor_t *head_monitor;
    int size;
} wm_monitor_list_t;

typedef struct wm_global {
    Display *display;
    Window root_window;
    int screen;
    int screen_width, screen_height;
    bool running;
    int tag_mask;
    int master_width;
    int border_width;
    Atom atoms[5];
    wm_client_list_t client_list;
    wm_monitor_list_t monitor_list;
    int current_layout;
    Colormap colormap;
    XColor border_color_active, border_color_passive;
    void (*event_handler[LASTEvent])(XEvent *);
} wm_global_t;
static wm_global_t wm_global;


// error handler
int wm_err_detect_other(Display * display, XErrorEvent *event);

// client
void wm_client_add(Window window);
void wm_client_delete(wm_client_t *client);
void wm_client_swap(wm_client_t *client1, wm_client_t *client2);
void wm_client_rehead(wm_client_t *client);
wm_client_t *wm_client_get_next(wm_client_t *client);
wm_client_t *wm_client_get_prev(wm_client_t *client);
void wm_client_focus(wm_client_t *client);
void wm_client_find_new_focus(wm_client_t *client);
void wm_client_send_XEvent(wm_client_t *client, Atom atom);
int wm_clients_count();
void wm_clients_arrange();
void wm_clients_map();
void wm_clients_unmap(wm_monitor_t *monitor);
void wm_client_draw(wm_client_t *client, int x, int y, int w, int h, bool border);
wm_client_t *wm_client_find(Window window);
void wm_client_manage(Window window);
void wm_client_unmanage(Window window);
void wm_client_set_border_color(wm_client_t *client);

// monitor
wm_monitor_t *wm_get_monitor(int tag_mask);
void wm_monitor_update();

// basic functions
void wm_run();
void wm_init();
void wm_start();
void wm_stop();
void wm_tini();
