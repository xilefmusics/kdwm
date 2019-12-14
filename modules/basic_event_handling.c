void wm_on_map_request(XEvent *e) {
    XMapRequestEvent event = e->xmaprequest;
    wm_client_manage(event.window);
}

void wm_on_configure_request(XEvent *e) {
    XConfigureEvent event = e->xconfigure;
    XWindowChanges changes;
    changes.x = event.x;
    changes.y = event.y;
    changes.width = event.width;
    changes.height = event.height;
    XConfigureWindow(wm_global.display, event.window, 15, &changes);
}

void wm_on_unmap_notify(XEvent *e) {
    XUnmapEvent event = e->xunmap;
    if (!event.send_event) {
        return;
    }
    wm_client_unmanage(event.window);
}

void wm_on_destroy_notify(XEvent *e) {
    XDestroyWindowEvent event = e->xdestroywindow;
    wm_client_unmanage(event.window);
}

void wm_on_key_press(XEvent *e) {
    XKeyEvent event = e->xkey;
    int keysym = XKeycodeToKeysym(wm_global.display, event.keycode, 0);
    for (int i = 0; i < LENGTH(wm_keybindings); ++i) {
        if (wm_keybindings[i].keysym == keysym && wm_keybindings[i].mod == event.state) {
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
