#include "window_manager.h"

// stores to global configuration of the windowmanager
typedef struct wm_global {
    Display *display;
    bool running;
    FILE *log_fp;
} wm_global_t;

static wm_global_t wm_global;



// error handler
void wm_err_detect_other(Display *display, XErrorEvent *e) {
    fprintf(stderr, "ERROR: an other windowmanager is already started\n");
    exit(EXIT_FAILURE);
}



// event handler
static wm_on_map_request(XMapRequestEvent *event) {

}



// user controll



// basic functions
static void wm_run() {
    XEvent event;
    XSync(wm_global.display, False);
    while (!XNextEvent(wm_global.display, &event) && wm_global.running) {
        switch(event.type) {
            case MapRequest:
                fprintf(log_fp, "Handle MapRequest\n");
                fflush(log_fp);
                wm_on_map_request(&event.xmaprequest);
                break;
            default:
                fprintf(log_fp, "Got not handled request from X-Server: %d\n", event.type);
                fflush(log_fp);
        }
    }
}

void wm_init() {
    // initialize global variables
    wm_global.running = false;
    wm_global.log_fp = fopen("/home/xilef/Git/kdwm/log.txt", "w");

    // open display (connection to X-Server)
    wm_global.display = XOpenDisplay(NULL);

    // init screen
    int screen = DefaultScreen(wm_global.display);
    int screen_width = DisplayWidth(wm_global.display, screen);
    int screen_height = DisplayHeight(wm_global.display, screen);
    Window root_window = RootWindow(wm_global.display, screen);

    // set error handler
    XSetErrorHandler(&wm_err_detect_other);

    // tell X-Server to handle the root_window
    XSelectInput(wm_global.display, root_window, SubstructureRedirectMask | SubstructureNotifyMask);
}

void wm_start() {
    wm_global.running = true;
    wm_run();
}

void wm_stop() {
    wm_global.running = false;
}

void wm_tini() {
    // close display (connections to X-Server)
    XCloseDisplay(wm_global.display);

    // close logging
    fclose(wm_global.log_fp);
}
