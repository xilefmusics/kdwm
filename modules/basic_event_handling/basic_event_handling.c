void basic_event_handling_on_map_request(XEvent *event) {
  wm_client_manage(event->xmaprequest.window);
}

void basic_event_handling_on_configure_request(XEvent *event) {
  XWindowChanges changes;
  changes.x = event->xconfigure.x;
  changes.y = event->xconfigure.y;
  changes.width = event->xconfigure.width;
  changes.height = event->xconfigure.height;
  XConfigureWindow(wm_global.display, event->xconfigure.window, 15, &changes);
}

void basic_event_handling_on_unmap_notify(XEvent *event) {
  if (event->xunmap.send_event)
    wm_client_unmanage(event->xunmap.window);
}

void basic_event_handling_on_destroy_notify(XEvent *event) {
  wm_client_unmanage(event->xdestroywindow.window);
}

void basic_event_handling_init() {
  wm_global.event_handler[MapRequest] = basic_event_handling_on_map_request;
  wm_global.event_handler[ConfigureRequest] = basic_event_handling_on_configure_request;
  wm_global.event_handler[UnmapNotify] = basic_event_handling_on_unmap_notify;
  wm_global.event_handler[DestroyNotify] = basic_event_handling_on_destroy_notify;
}
