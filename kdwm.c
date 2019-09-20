#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>

#include "config.h"

typedef struct global {
    Display *display;
} global_t;
global_t global;


void error_handler(Display *display, XErrorEvent *e) {
    fprintf(stderr, "ERROR: an other windowmanager is already started\n");
    exit(EXIT_FAILURE);
}

void on_map_request(XEvent *event) {
    static XWindowAttributes wa;
    XMapRequestEvent *ev = &event->xmaprequest;

    if (!XGetWindowAttributes(global.display, ev->window, &wa)) {
        return;
    }
    if (wa.override_redirect) {
        return;
    }
    //TODO remember window
}

int main(int argc, char *argv[]) {
    // open display (connection to X-Server)
    global.display = XOpenDisplay(NULL);

    // init screen
    int screen = DefaultScreen(global.display);
    int screen_width = DisplayWidth(global.display, screen);
    int screen_height = DisplayHeight(global.display, screen);
    Window root_window = RootWindow(global.display, screen);

    // set error handle
    XSetErrorHandler(&error_handler);

    // ich denke mal, dass man sich so für das root window einträgt
    XSelectInput(global.display, root_window, SubstructureRedirectMask | SubstructureNotifyMask);

    // event loop
    FILE *fp = fopen("/home/xilef/Git/kdwm/event_log.txt", "w");
    XEvent event;
    XSync(global.display, False);
    while (!XNextEvent(global.display, &event)) {
        switch(event.type) {
            case CreateNotify:
                fprintf(fp, "CreateNotify\n");
                fflush(fp);
                on_create_notify(&event);
                break;
            case DestroyNotify:
                fprintf(fp, "DestroyNotify\n");
                fflush(fp);
                break;
            case ReparentNotify:
                fprintf(fp, "ReparentNotify\n");
                fflush(fp);
                break;
            case MapRequest:
                fprintf(fp, "MapRequest\n");
                fflush(fp);
                on_map_request(&event);
            case ButtonPress:
                fprintf(fp, "ButtonPress\n");
                fflush(fp);
            case ClientMessage:
                fprintf(fp, "ClientMessage\n");
                fflush(fp);
            case ConfigureRequest:
                fprintf(fp, "ConfigureRequest\n");
                fflush(fp);
            case ConfigureNotify:
                fprintf(fp, "ConfigureNotify\n");
                fflush(fp);
            case EnterNotify:
                fprintf(fp, "EnterNotify\n");
                fflush(fp);
            case Expose:
                fprintf(fp, "Expose\n");
                fflush(fp);
            case FocusIn:
                fprintf(fp, "FocusIn\n");
                fflush(fp);
            case KeyRelease:
                fprintf(fp, "KeyRelease\n");
                fflush(fp);
            case KeyPress:
                fprintf(fp, "KeyPress\n");
                fflush(fp);
            case MappingNotify:
                fprintf(fp, "MappingNotify\n");
                fflush(fp);
            case MotionNotify:
                fprintf(fp, "MotionNotify\n");
                fflush(fp);
            case PropertyNotify:
                fprintf(fp, "PropertyNotify\n");
                fflush(fp);
            case UnmapNotify:
                fprintf(fp, "UnmapNotify\n");
                fflush(fp);
            default:
                fprintf(fp, "%d\n", event.type);
                fflush(fp);
        }
    }
    close(fp);

    // close display
    XCloseDisplay(global.display);

    return 0;
}
