#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

#include <X11/Xlib.h>
#include <X11/keysym.h>

// makros
#define LENGTH(X)               (sizeof X / sizeof X[0])

// definitions
typedef enum {NONE, STRING, INTEGER, DOUBLE} wm_arg_types;

enum {WM_PROTOCOLS, WM_DELETE_WINDOW};

typedef struct wm_client wm_client_t;
struct wm_client {
    Window window;
    wm_client_t *next;
    wm_client_t *prev;
    int tag_mask;
};

typedef struct wm_client_list {
    wm_client_t *head_client;
    wm_client_t *active_client;
    int size;
} wm_client_list_t;

typedef struct wm_global {
    Display *display;
    Window root_window;
    int screen;
    int screen_width;
    int screen_height;
    bool running;
    FILE *log_fp;
    int tag_mask;
    float master_width;
    Atom atoms[2];
    wm_client_list_t client_list;
} wm_global_t;

typedef struct wm_keybinding {
    int mod;
    int keysym;
    void (*func)();
    wm_arg_types arg_type;
    char *arg;
} wm_keybinding_t;

// error handler
int wm_err_detect_other(Display * display, XErrorEvent *event);

// event handler
void wm_on_map_request(XMapRequestEvent *event);
void wm_on_destroy_notify(XDestroyWindowEvent *event);
void wm_on_key_press(XKeyEvent *event);

// user controll of wm
void wm_focus_prev();
void wm_focus_next();
void wm_focus_head();
void wm_kill_active_client();
void wm_spawn(char *name);

// window functions
void wm_windows_arrange();
void wm_windows_map();
void wm_windows_unmap();

// client list
void wm_client_add(Window window);
void wm_client_delete(wm_client_t *client);
void wm_client_swap(wm_client_t *client1, wm_client_t *client2);
void wm_client_rehead(wm_client_t *client);
wm_client_t *wm_client_get_next(wm_client_t *client);
wm_client_t *wm_client_get_prev(wm_client_t *client);
void wm_client_focus(wm_client_t *client);
void wm_client_send_XEvent(wm_client_t *client, Atom atom);


// basic functions
void wm_run();
void wm_init();
void wm_start();
void wm_stop();
void wm_tini();

