#include "config.h"


// stores to global configuration of the windowmanager
static wm_global_t wm_global;


// error handler
void wm_err_detect_other(Display *display, XErrorEvent *e) {
    fprintf(stderr, "ERROR: an other windowmanager is already started\n");
    exit(EXIT_FAILURE);
}



// event handler
void wm_on_map_request(XMapRequestEvent *event) {
    // unmap old window
    if (wm_global.client_list.active_client) {
        XUnmapWindow(wm_global.display, wm_global.client_list.active_client->window);
    }

    // make new window fullscreen
    XWindowChanges changes;
    changes.x = 0;
    changes.y = 0;
    changes.width = wm_global.screen_width;
    changes.height = wm_global.screen_height;
    XConfigureWindow(wm_global.display, event->window, 15, &changes);
    XFlush(wm_global.display);

    // map new window
    XMapWindow(wm_global.display, event->window);

    // add client and make new client active
    wm_client_add(event->window);
    wm_global.client_list.active_client = wm_global.client_list.head_client;
}

void wm_on_key_press(XKeyEvent *event) {
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
void wm_focus_next() {
    fprintf(wm_global.log_fp, "wm_focus_next");
    fflush(wm_global.log_fp);

    // if no next client return
    if (wm_global.client_list.active_client->next->window == NULL) {
        return;
    }
    // unmap current window
    if (wm_global.client_list.active_client) {
        XUnmapWindow(wm_global.display, wm_global.client_list.active_client->window);
    }
    // change current windwo
    wm_global.client_list.active_client = wm_global.client_list.active_client->next;
    // map new current window
    XMapWindow(wm_global.display, wm_global.client_list.active_client->window);
}

void wm_focus_prev() {
    fprintf(wm_global.log_fp, "wm_focus_prev");
    fflush(wm_global.log_fp);

    // if no next client return
    if (wm_global.client_list.active_client->prev == NULL) {
        return;
    }
    // unmap current window
    if (wm_global.client_list.active_client) {
        XUnmapWindow(wm_global.display, wm_global.client_list.active_client->window);
    }
    // change current windwo
    wm_global.client_list.active_client = wm_global.client_list.active_client->prev;
    // map new current window
    XMapWindow(wm_global.display, wm_global.client_list.active_client->window);
}

void wm_focus_head() {

}



// client list
void wm_client_add(Window window) {
    wm_client_t *new = malloc(sizeof(wm_client_t));
    new->window = window;
    new->prev = NULL;
    new->tag_mask = wm_global.tag_mask;
    wm_global.client_list.head_client->prev = new;
    new->next = wm_global.client_list.head_client;
    wm_global.client_list.head_client = new;
    wm_global.client_list.size++;
}

void wm_client_delete(wm_client_t *client) {
    client->next->prev = client->prev;
    if (client->prev) {
        client->prev->next = client->next;
    }
    free(client);
    wm_global.client_list.size--;
}

void wm_client_swap(wm_client_t *client1, wm_client_t *client2) {
    wm_client_t *buffer = client1->prev;
    client1->prev = client2->prev;
    client2->prev = buffer;

    buffer = client1->next;
    client1->next = client2->next;
    client2->next = buffer;

    if (client1->prev) {
        client1->prev->next = client2;
    }
    client1->next->prev = client2;

    if (client2->prev) {
        client2->prev->next = client1;
    }
    client2->next->prev = client1;
}

void wm_client_rehead(wm_client_t *client) {
    client->next->prev = client->prev;
    if (client->prev) {
        client->prev->next = client->next;
    }
    client->prev = NULL;
    client->next = wm_global.client_list.head_client;
    wm_global.client_list.head_client = client;
}


// basic functions
void wm_run() {
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
    wm_global.tag_mask = 1;
    wm_global.client_list.size = 0;
    wm_global.client_list.active_client = NULL;
    wm_global.client_list.head_client = malloc(sizeof(wm_client_t));
    wm_global.client_list.head_client->window = NULL;
    wm_global.client_list.head_client->next = NULL;
    wm_global.client_list.head_client->prev = NULL;
    wm_global.client_list.head_client->tag_mask = 0;


    // init logging
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
