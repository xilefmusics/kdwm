#include "config.h"

// error handler
int wm_err_detect_other(Display *display, XErrorEvent *event) {
    fprintf(stderr, "ERROR: another windowmanager is already started\n");
    exit(EXIT_FAILURE);
}

// event handler
void wm_on_map_request(XMapRequestEvent *event) {
    wm_client_manage(event->window);
}

void wm_on_configure_request(XConfigureEvent *event) {
    XWindowChanges changes;
    changes.x = event->x;
    changes.y = event->y;
    changes.width = event->width;
    changes.height = event->height;
    XConfigureWindow(wm_global.display, event->window, 15, &changes);
}

void wm_on_unmap_notify(XUnmapEvent *event) {
    if (!event->send_event) {
        return;
    }
    wm_client_unmanage(event->window);
}

void wm_on_destroy_notify(XDestroyWindowEvent *event) {
    wm_client_unmanage(event->window);
}

void wm_on_key_press(XKeyEvent *event) {
    int keysym = XKeycodeToKeysym(wm_global.display, event->keycode, 0);
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
            return;
        }
    }
}

// client
void wm_client_add(Window window) {
    wm_client_t *client = wm_client_find(window);
    if (client) {
        return;
    }
    wm_client_t *new = malloc(sizeof(wm_client_t));
    new->window = window;
    new->prev = NULL;
    new->tag_mask = wm_global.tag_mask;
    if (wm_global.client_list.head_client) {
        wm_global.client_list.head_client->prev = new;
    }
    new->next = wm_global.client_list.head_client;
    wm_global.client_list.head_client = new;
    wm_global.client_list.size++;
}

void wm_client_delete(wm_client_t *client) {
    if (client->next) {
        client->next->prev = client->prev;
    }
    if (client->prev) {
        client->prev->next = client->next;
    }
    if (client == wm_global.client_list.head_client) {
        wm_global.client_list.head_client = client->next;
    }
    free(client);
    wm_global.client_list.size--;
}

void wm_client_swap(wm_client_t *client1, wm_client_t *client2) {
    if (client1 == wm_global.client_list.head_client) {
        wm_global.client_list.head_client = client2;
    }

    wm_client_t *buffer = client1->prev;
    client1->prev = client2;
    client2->prev = buffer;

    buffer = client2->next;
    client2->next = client1;
    client1->next = buffer;

    if (client2->prev) {
        client2->prev->next = client2;
    }
    if (client1->next) {
        client1->next->prev = client1;
    }
}

void wm_client_rehead(wm_client_t *client) {
    if (!client || client == wm_global.client_list.head_client) {
        return;
    }
    if (client->next) {
        client->next->prev = client->prev;
    }
    if (client->prev) {
        client->prev->next = client->next;
    }
    client->prev = NULL;
    client->next = wm_global.client_list.head_client;
    wm_global.client_list.head_client->prev = client;
    wm_global.client_list.head_client = client;
}

wm_client_t *wm_client_get_next(wm_client_t *client) {
    if (!client) {
        client = wm_global.client_list.head_client;
        if (client && client->tag_mask & wm_global.tag_mask) {
            return client;
        }
    }
    while (client) {
        client = client->next;
        if (client && client->tag_mask & wm_global.tag_mask) {
            return client;
        }
    }
    return NULL;
}

wm_client_t *wm_client_get_prev(wm_client_t *client) {
    while (client) {
        client = client->prev;
        if (!client || client->tag_mask & wm_global.tag_mask) {
            return client;
        }
    }
    return NULL;
}

void wm_client_focus(wm_client_t *client) {
    if (client) {
        wm_global.client_list.active_client = client;
        XSetInputFocus(wm_global.display, client->window, RevertToPointerRoot, CurrentTime);
        XRaiseWindow(wm_global.display, client->window);
    }
}

void wm_client_find_new_focus(wm_client_t *client) {
    wm_client_t *next = wm_client_get_next(client);
    wm_client_t *prev = wm_client_get_prev(client);

    if (next){
        wm_client_focus(next);
    } else if (prev) {
        wm_client_focus(prev);
    } else {
        wm_global.client_list.active_client = NULL;
    }
}

void wm_client_send_XEvent(wm_client_t *client, Atom atom) {
    XEvent event;
    event.type = ClientMessage;
    event.xclient.window = client->window;
    event.xclient.message_type = wm_global.atoms[WM_PROTOCOLS];
    event.xclient.format = 32;
    event.xclient.data.l[0] = atom;
	event.xclient.data.l[1] = CurrentTime;
    XSendEvent(wm_global.display, client->window, false, NoEventMask, &event);
}

int wm_clients_count(){
    int result = 0;
    wm_client_t *client = NULL;
    while (client = wm_client_get_next(client)) {
        result++;
    }
    return result;
}

void wm_clients_arrange() {
    (*layouts[wm_global.current_layout])();
}

void wm_clients_map() {
    wm_client_t *client = wm_global.client_list.head_client;
    if (!client) {
        return;
    }
    do {
        if (client->tag_mask & wm_global.tag_mask) {
            XMapWindow(wm_global.display, client->window);
        }
    } while (client = client->next);
}

void wm_clients_unmap() {
    wm_client_t *client = wm_global.client_list.head_client;
    if (!client) {
        return;
    }
    do {
        if (client->tag_mask & wm_global.tag_mask) {
            XUnmapWindow(wm_global.display, client->window);
        }
    } while (client = client->next);
}

void wm_client_draw(wm_client_t *client, int x, int y, int w, int h, bool border) {
    client->x = x;
    client->y = y;
    client->w = w;
    client->h = h;
    XWindowChanges changes;
    changes.x = x;
    changes.y = y;
    if (border) {
        changes.width = w-2*wm_global.border_width;
        changes.height = h-2*wm_global.border_width;
        changes.border_width = wm_global.border_width;
        XConfigureWindow(wm_global.display, client->window, 31, &changes);
        wm_client_set_border_color(client);
    } else {
        changes.width = w;
        changes.height = h;
        changes.border_width = 0;
        XConfigureWindow(wm_global.display, client->window, 31, &changes);
    }
}

void wm_client_manage(Window window) {
    XMapWindow(wm_global.display, window);
    wm_client_add(window);
    wm_client_focus(wm_global.client_list.head_client);
    wm_global.client_list.active_client->tag_mask = wm_global.tag_mask;
    wm_clients_arrange();
}

void wm_client_unmanage(Window window) {
    wm_client_t *client = wm_client_find(window);
    if (!client) {
        return;
    }
    wm_client_find_new_focus(client);
    wm_client_delete(client);
    wm_clients_arrange();
}

wm_client_t *wm_client_find(Window window) {
    wm_client_t *client = wm_global.client_list.head_client;
    while (client && client->window != window) {
        client = client->next;
    }
    if (!client) {
        return NULL;
    }
    return client;
}

void wm_client_set_border_color(wm_client_t *client) {
    if (client) {
        if (client == wm_global.client_list.active_client) {
            XSetWindowBorder(wm_global.display, client->window, wm_global.border_color_active.pixel);
        } else {
            XSetWindowBorder(wm_global.display, client->window, wm_global.border_color_passive.pixel);
        }
    }
}


// basic functions
void wm_keys_grab() {
    KeyCode keycode;
    XUngrabButton(wm_global.display, AnyKey, AnyModifier, wm_global.root_window);
    for (int i = 0; i < LENGTH(wm_keybindings); i++) {
        if (keycode = XKeysymToKeycode(wm_global.display, wm_keybindings[i].keysym)) {
            XGrabKey(wm_global.display, keycode,  wm_keybindings[i].mod, wm_global.root_window, true, GrabModeAsync, GrabModeAsync);
        }
    }
}

void wm_run() {
    XEvent event;
    XSync(wm_global.display, False);
    while (!XNextEvent(wm_global.display, &event) && wm_global.running) {
        switch(event.type) {
            case MapRequest:
                wm_on_map_request(&event.xmaprequest);
                break;
            case UnmapNotify:
                wm_on_unmap_notify(&event.xunmap);
                break;
            case DestroyNotify:
                wm_on_destroy_notify(&event.xdestroywindow);
                break;
            case KeyPress:
                wm_on_key_press(&event.xkey);
                break;
            case ConfigureRequest:
                wm_on_configure_request(&event.xconfigure);
                break;
            default:
                logsi("EVENT", event.type);
        }
    }
}

void wm_init() {
    logs("START");
    wm_global.running = false;
    wm_global.tag_mask = 1;
    wm_global.client_list.size = 0;
    wm_global.client_list.head_client = NULL;
    wm_global.master_width = MASTER_WIDTH;
    wm_global.border_width = BORDER_WIDTH;
    wm_global.tag_mask = 1;
    wm_global.current_layout = MASTERSTACK;

    // open display (connection to X-Server)
    wm_global.display = XOpenDisplay(NULL);

    // init screen
    wm_global.screen = DefaultScreen(wm_global.display);
    wm_global.screen_width = DisplayWidth(wm_global.display, wm_global.screen);
    wm_global.screen_height = DisplayHeight(wm_global.display, wm_global.screen);

    // init root window
    wm_global.root_window = RootWindow(wm_global.display, wm_global.screen);

    // set error handler
    XSetErrorHandler(&wm_err_detect_other);

    // tell X-Server to handle the root_window
    XSelectInput(wm_global.display, wm_global.root_window, SubstructureRedirectMask | SubstructureNotifyMask | KeyPressMask);

    // grab all key events while MODKEY is pressed
    wm_keys_grab();

    // get atoms
    wm_global.atoms[WM_PROTOCOLS] = XInternAtom(wm_global.display, "WM_PROTOCOLS", false);
	wm_global.atoms[WM_DELETE_WINDOW] = XInternAtom(wm_global.display, "WM_DELETE_WINDOW", false);

    // init colors
    wm_global.colormap = XCreateColormap(wm_global.display, wm_global.root_window, XDefaultVisual(wm_global.display, wm_global.screen), AllocNone);
    XAllocNamedColor(wm_global.display, wm_global.colormap, BORDER_COLOR_ACTIVE, &wm_global.border_color_active, &wm_global.border_color_active);
    XAllocNamedColor(wm_global.display, wm_global.colormap, BORDER_COLOR_PASSIVE, &wm_global.border_color_passive, &wm_global.border_color_passive);

    // init modules
    for (int i = 0; i < LENGTH(wm_on_init); i++) {
        wm_on_init[i]();
    }
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
    // tini modules
    for (int i = 0; i < LENGTH(wm_on_tini); i++) {
        wm_on_tini[i]();
    }
    // cloes connection to X-Server
    XCloseDisplay(wm_global.display);
}

// main function
int main(int argc, char *argv[]) {
    wm_init();
    wm_start();
    return 0;
}
