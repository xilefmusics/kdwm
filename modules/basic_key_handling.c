void basic_key_handling_on_key_press(XEvent *event) {
    int keysym = XKeycodeToKeysym(wm_global.display, event->xkey.keycode, 0);
    for (int i = 0; i < LENGTH(wm_keybindings); ++i) {
        if (wm_keybindings[i].keysym == keysym && wm_keybindings[i].mod == event->xkey.state) {
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

void basic_key_handling_init() {
    wm_global.event_handler[KeyPress] = basic_key_handling_on_key_press;
}
