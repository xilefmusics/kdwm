#include "config.h"

// stores to global configuration of the windowmanager
static wm_global_t wm_global;

// logging
#include "log.c"

// error handler
int wm_err_detect_other(Display *display, XErrorEvent *event) {
    fprintf(stderr, "ERROR: another windowmanager is already started\n");
    exit(EXIT_FAILURE);
}



// event handler
void wm_on_map_request(XMapRequestEvent *event) {
    // map new window
    XMapWindow(wm_global.display, event->window);

    // add client and focus it
    wm_client_add(event->window);
    wm_client_focus(wm_global.client_list.head_client);

    // set tag_mask of client
    wm_global.client_list.active_client->tag_mask = wm_global.tag_mask;

    // arange windows
    wm_clients_arrange();
}

void wm_on_destroy_notify(XDestroyWindowEvent *event) {
    // search for client
    wm_client_t *client = wm_global.client_list.head_client;
    while (client->window != 0 && client->window != event->window) {
        client = client->next;
    }
    // return if no client found
    if (client->window == 0) {
        return;
    }

    // set new focus
    wm_client_t *next = wm_client_get_next(client);
    wm_client_t *prev = wm_client_get_prev(client);
    wm_client_find_new_focus(next, prev);

    // delete client and rearrange clients
    wm_client_delete(client);
    wm_clients_arrange();

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
        }
    }
}


// user controll
void wm_focus_next() {
    wm_client_t * client = wm_client_get_next(wm_global.client_list.active_client);
    if (client == NULL) {
        return;
    }
    wm_global.client_list.active_client = client;
    wm_client_focus(client);
}

void wm_focus_prev() {
    wm_client_t *client = wm_client_get_prev(wm_global.client_list.active_client);
    if (client == NULL) {
        return;
    }
    wm_global.client_list.active_client = client;
    wm_client_focus(client);
}

void wm_kill_active_client() {
    if (!wm_global.client_list.active_client) {
        return;
    }
    wm_client_send_XEvent(wm_global.client_list.active_client, wm_global.atoms[WM_DELETE_WINDOW]);
}

void wm_spawn(char *name) {
    char buffer[256];
    strcpy(buffer, name);
    strcat(buffer, " &");
    system(buffer);
}

void wm_set_tag_mask_of_focused_client(int tag_mask){

    wm_clients_count(tag_mask);
    wm_global.client_list.active_client->tag_mask = tag_mask;

    if (!(tag_mask & wm_global.tag_mask)) {
        XUnmapWindow(wm_global.display, wm_global.client_list.active_client->window);
        wm_client_find_new_focus(wm_client_get_next(wm_global.client_list.active_client), wm_client_get_prev(wm_global.client_list.active_client));
        wm_clients_arrange();
    }
}


void wm_add_tag_to_tag_mask(int tag) {
    wm_retag(wm_global.tag_mask | tag);
}

void wm_add_tag_to_client(int tag) {
    wm_global.client_list.active_client->tag_mask = wm_global.client_list.active_client->tag_mask | tag;
}

void wm_retag(int tag_mask) {
    if (tag_mask == wm_global.tag_mask) {
        return;
    }

    wm_clients_unmap();
    wm_global.tag_mask = tag_mask;
    if (wm_global.client_list.head_client->tag_mask & tag_mask) {
        wm_global.client_list.active_client = wm_global.client_list.head_client;
    } else {
        wm_global.client_list.active_client = wm_client_get_next(wm_global.client_list.head_client);
    }
    wm_clients_map();
    wm_clients_arrange();
}

void wm_rehead() {
    wm_client_rehead(wm_global.client_list.active_client);
    wm_clients_arrange();
}

void wm_client_up() {
    wm_client_t *prev = wm_client_get_prev(wm_global.client_list.active_client);
    if (prev) {
        wm_client_swap(prev, wm_global.client_list.active_client);
        wm_clients_arrange();
    }
}

void wm_client_down() {
    wm_client_t *next = wm_client_get_next(wm_global.client_list.active_client);
    if (next) {
        wm_client_swap(wm_global.client_list.active_client, next);
        wm_clients_arrange();
    }
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
    if (client1->next->window) {
        client1->next->prev = client1;
    }
}

void wm_client_rehead(wm_client_t *client) {
    if (client == wm_global.client_list.head_client) {
        return;
    }
    client->next->prev = client->prev;
    if (client->prev) {
        client->prev->next = client->next;
    }
    client->prev = NULL;
    client->next = wm_global.client_list.head_client;
    wm_global.client_list.head_client = client;
}

wm_client_t *wm_client_get_next(wm_client_t *client) {
    while (client->window != 0) {
        client = client->next;
        if (client->tag_mask & wm_global.tag_mask) {
            return client;
        }
    }
    return NULL;
}

wm_client_t *wm_client_get_prev(wm_client_t *client) {
    while (client != NULL) {
        client = client->prev;
        if (!client || client->tag_mask & wm_global.tag_mask) {
            return client;
        }
    }
    return NULL;
}

void wm_client_focus(wm_client_t *client) {
    wm_global.client_list.active_client = client;
    XSetInputFocus(wm_global.display, client->window, RevertToPointerRoot, CurrentTime);
}

void wm_client_find_new_focus(wm_client_t *prev, wm_client_t *next) {
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

int wm_clients_count(int tag_mask){
    int result = 0;
    wm_client_t *client = wm_global.client_list.head_client;
    if (client->tag_mask & tag_mask){
        result = 1;
    } else{
        result = 0;
    }
    for (int i=1;i<wm_global.client_list.size;i++){
        client = client->next;
        if (client->tag_mask & tag_mask){
            result++;
        }
    }
    return result;
}

void wm_clients_arrange() {
    int num_of_clients = wm_clients_count(wm_global.tag_mask);

    wm_client_t *client = wm_global.client_list.head_client;
    if (!(client->tag_mask & wm_global.tag_mask)) {
        client = wm_client_get_next(client);
    }

    if (num_of_clients == 0) {

    } else if (num_of_clients == 1) {
        XWindowChanges changes;
        changes.x = 0;
        changes.y = 0;
        changes.width = wm_global.screen_width;
        changes.height = wm_global.screen_height;
        XConfigureWindow(wm_global.display, client->window, 15, &changes);
    } else {
        int num_of_slaves = num_of_clients - 1;

        int slave_height = wm_global.screen_height / num_of_slaves;
        int master_height = wm_global.screen_height;

        int master_width = (int) wm_global.screen_width * MASTER_WIDTH;
        int slave_width = wm_global.screen_width - master_width;

        XWindowChanges changes;

        changes.x = 0;
        changes.y = 0;
        changes.width = master_width;
        changes.height = master_height;

        XConfigureWindow(wm_global.display, client->window, 15, &changes);

        for (int i = 0; i < num_of_slaves; i++) {
            client = wm_client_get_next(client);

            changes.x = master_width;
            changes.y = i*slave_height;
            changes.width = slave_width;
            changes.height = slave_height;

            XConfigureWindow(wm_global.display, client->window, 15, &changes);
        }
    }

    XFlush(wm_global.display);
}

void wm_clients_map() {
    wm_client_t *client = wm_global.client_list.head_client;
    if (client == NULL) {
        return;
    }
    do {
        if (client->tag_mask & wm_global.tag_mask) {
            XMapWindow(wm_global.display, client->window);
        }
    } while ((client = client->next) != NULL);
}

void wm_clients_unmap() {
    wm_client_t *client = wm_global.client_list.head_client;
    if (client == NULL) {
        return;
    }
    do {
        if (client->tag_mask & wm_global.tag_mask) {
            XUnmapWindow(wm_global.display, client->window);
        }
    } while ((client = client->next) != NULL);
}

// basic functions
void wm_run() {
    XEvent event;
    XSync(wm_global.display, False);
    while (!XNextEvent(wm_global.display, &event) && wm_global.running) {
        switch(event.type) {
            case MapRequest:
                wm_on_map_request(&event.xmaprequest);
                break;
            case KeyPress:
                wm_on_key_press(&event.xkey);
                break;
            case CreateNotify:
                break;
            case MappingNotify:
                break;
            case DestroyNotify:
                wm_on_destroy_notify(&event.xdestroywindow);
                break;
            default:
                logsi("unhandled event", event.type);
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
    wm_global.client_list.head_client->window = 0;
    wm_global.client_list.head_client->next = NULL;
    wm_global.client_list.head_client->prev = NULL;
    wm_global.client_list.head_client->tag_mask = 0;
    wm_global.master_width = MASTER_WIDTH;
    wm_global.tag_mask = 1;


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

    // init root window
    wm_global.root_window = RootWindow(wm_global.display, wm_global.screen);

    // set error handler
    XSetErrorHandler(&wm_err_detect_other);

    // tell X-Server to handle the root_window
    XSelectInput(wm_global.display, wm_global.root_window, SubstructureRedirectMask | SubstructureNotifyMask | KeyPressMask);

    // grab all key events while MODKEY is pressed
    XGrabKey(wm_global.display, AnyKey, MODKEY, wm_global.root_window, true, GrabModeAsync, GrabModeAsync);

    // add windows who are actually open
    Window root_return, parent_return, *children_return;
    unsigned int nchildren_return;
    bool client_added = false;
    if (XQueryTree(wm_global.display, wm_global.root_window, &root_return, &parent_return, &children_return, &nchildren_return)) {
        for (int i = 0; i < nchildren_return; i++) {
            fprintf(wm_global.log_fp, "Add client\n");
            fflush(wm_global.log_fp);
            wm_client_add(children_return[i]);
            client_added = true;
        }
        if (client_added) {
            wm_clients_arrange();
            wm_client_focus(wm_global.client_list.head_client);
        }
    }

    // get atoms
    wm_global.atoms[WM_PROTOCOLS] = XInternAtom(wm_global.display, "WM_PROTOCOLS", false);
	wm_global.atoms[WM_DELETE_WINDOW] = XInternAtom(wm_global.display, "WM_DELETE_WINDOW", false);
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
