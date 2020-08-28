void basic_key_handling_on_key_press(XEvent *event) {
  int keysym = XKeycodeToKeysym(wm_global.display, event->xkey.keycode, 0);
  for (int i = 0; i < LENGTH(basic_key_handling_keybindings); ++i) {
    if (basic_key_handling_keybindings[i].keysym == keysym && basic_key_handling_keybindings[i].mod == event->xkey.state) {
      switch (basic_key_handling_keybindings[i].arg_type) {
        case NONE:
          basic_key_handling_keybindings[i].func();
          break;
        case STRING:
          basic_key_handling_keybindings[i].func(basic_key_handling_keybindings[i].arg);
          break;
        case INTEGER:
          basic_key_handling_keybindings[i].func(atoi(basic_key_handling_keybindings[i].arg));
          break;
      }
      return;
    }
  }
}

void basic_key_handling_keys_grab() {
  KeyCode keycode;
  XUngrabButton(wm_global.display, AnyKey, AnyModifier, wm_global.root_window);
  for (int i = 0; i < LENGTH(basic_key_handling_keybindings); i++) {
    if (keycode = XKeysymToKeycode(wm_global.display, basic_key_handling_keybindings[i].keysym)) {
      XGrabKey(wm_global.display, keycode,  basic_key_handling_keybindings[i].mod, wm_global.root_window, true, GrabModeAsync, GrabModeAsync);
    }
  }
}

void basic_key_handling_init() {
  wm_global.event_handler[KeyPress] = basic_key_handling_on_key_press;
  basic_key_handling_keys_grab();
}
