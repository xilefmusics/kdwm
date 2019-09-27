#include "config.h"


// stores to global configuration of the windowmanager
static wm_global_t wm_global;


// error handler
void wm_err_detect_other(Display *display, XErrorEvent *e) {
    fprintf(stderr, "ERROR: an other windowmanager is already started\n");
    exit(EXIT_FAILURE);
}



// event handler
static wm_on_map_request(XMapRequestEvent *event) {
    XMapWindow(wm_global.display, event->window);
    XWindowChanges changes;
    changes.x = 0;
    changes.y = 0;
    changes.width = wm_global.screen_width;
    changes.height = wm_global.screen_height;
    XConfigureWindow(wm_global.display, event->window, 15, &changes);
    XFlush(wm_global.display);
}

static wm_on_key_press(XKeyEvent *event) {
    int keysym = XKeycodeToKeysym(wm_global.display, event->keycode, 0);
    fprintf(wm_global.log_fp, "Key Pressed: %d %d\n", keysym, event->state);
    fflush(wm_global.log_fp);

    for (int i = 0; i < LENGTH(wm_keybindings); ++i) {
        if (wm_keybindings[i].keysym == keysym && wm_keybindings[i].mod == event->state) {
            switch (wm_keybindings[i].arg_type) {
                case NONE:
                    wm_keybindings[i].func();
                    break;
                case STRING:
                    wm_keybindings[i].func(wm_keybindings[i].arg);
                    break;
                case INTEGER:
                    wm_keybindings[i].func(atoi(wm_keybindings[i].arg));
                    break;
            }
        }
    }
}


// user controll



// basic functions
static void wm_run() {
    XEvent event;
    XSync(wm_global.display, False);
    while (!XNextEvent(wm_global.display, &event) && wm_global.running) {
        switch(event.type) {
            case MapRequest:
                fprintf(wm_global.log_fp, "Handle MapRequest\n");
                fflush(wm_global.log_fp);
                wm_on_map_request(&event.xmaprequest);
                break;
            case KeyPress:
                fprintf(wm_global.log_fp, "Handle KeyPress\n");
                fflush(wm_global.log_fp);
                wm_on_key_press(&event.xkey);
                break;
            case CreateNotify:
                fprintf(wm_global.log_fp, "Got unhandled CreateNotify\n");
                fflush(wm_global.log_fp);
                break;
            case MappingNotify:
                fprintf(wm_global.log_fp, "Got unhandled MappingNotify\n");
                fflush(wm_global.log_fp);
                break;
            default:
                fprintf(wm_global.log_fp, "Got not handled request from X-Server: %d\n", event.type);
                fflush(wm_global.log_fp);
        }
    }
}

void wm_init() {
    // initialize global variables
    wm_global.running = false;
    char path_buffer[256];
    char *homedir = getenv("HOME");
    strcpy(path_buffer, homedir);
    strcat(path_buffer, "/.kdwm/log.txt");
    wm_global.log_fp = fopen(path_buffer, "w");

    // open display (connection to X-Server)
    wm_global.display = XOpenDisplay(NULL);

    // init screen
    wm_global.screen = DefaultScreen(wm_global.display);
    wm_global.screen_width = DisplayWidth(wm_global.display, wm_global.screen);
    wm_global.screen_height = DisplayHeight(wm_global.display, wm_global.screen);
    fprintf(wm_global.log_fp, "dimension: %dx%d\n", wm_global.screen_width, wm_global.screen_height);
    fflush(wm_global.log_fp);

    // init root window
    wm_global.root_window = RootWindow(wm_global.display, wm_global.screen);

    // set error handler
    XSetErrorHandler(&wm_err_detect_other);

    // tell X-Server to handle the root_window
    XSelectInput(wm_global.display, wm_global.root_window, SubstructureRedirectMask | SubstructureNotifyMask | KeyPressMask);

    // grab all key events while MODKEY is pressed
    XGrabKey(wm_global.display, AnyKey, MODKEY, wm_global.root_window, true, GrabModeAsync, GrabModeAsync);
}

void wm_start() {
    wm_global.running = true;
    wm_run();
}

void wm_stop() {
    wm_global.running = false;
    wm_tini();
}

void wm_tini() {
    // close display (connections to X-Server)
    XCloseDisplay(wm_global.display);

    // close logging
    fclose(wm_global.log_fp);
}



// main function
int main(int argc, char *argv[]) {
    wm_init();
    wm_start();
    return 0;
}
