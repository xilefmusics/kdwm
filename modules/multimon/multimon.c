multimon_monitor_t *multimon_get_monitor(int tag_mask) {
    multimon_monitor_t *monitor = multimon_monitor_list.head_monitor;
    while(monitor && !(monitor->tag_mask & tag_mask)) {
        monitor = monitor->next;
    }
    return monitor ? monitor : multimon_monitor_list.head_monitor;
}

void multimon_update() {
    if (XineramaIsActive(wm_global.display)) {
        // load new monitor information
        int number;
        XineramaScreenInfo *scr_info = XineramaQueryScreens(wm_global.display, &number);
        // search for removed monitors and update of not removed
        multimon_monitor_t *monitor = multimon_monitor_list.head_monitor;
        multimon_monitor_t *next_monitor;
        bool found_monitor;
        while(monitor) {
            found_monitor = false;
            for (int i = 0; i < number; i++) {
                if (scr_info[i].x_org == monitor->x && scr_info[i].y_org == monitor->y) {
                    found_monitor = true;
                    // update monitor
                    monitor->w = scr_info[i].width;
                    monitor->h = scr_info[i].height;
                }
            }
            next_monitor = monitor->next;
            if (!found_monitor) {
                // delete monitor
                if (monitor->next) {
                    monitor->next->prev = monitor->prev;
                }
                if (monitor->prev) {
                    monitor->prev->next = monitor->next;
                }
                multimon_monitor_list.size--;
                multimon_monitor_list.head_monitor->tag_mask = multimon_monitor_list.head_monitor->tag_mask | monitor->tag_mask;
                if (monitor == multimon_monitor_list.head_monitor) {
                    multimon_monitor_list.head_monitor = monitor->next;
                }
                free(monitor);
            }
            monitor = next_monitor;
        }
        // search for new monitors
        for (int i = 0; i < number; i++) {
            monitor = multimon_monitor_list.head_monitor;
            if (!monitor) {
                    // add monitor
                    next_monitor = malloc(sizeof(multimon_monitor_t));
                    next_monitor->x = scr_info[i].x_org;
                    next_monitor->y = scr_info[i].y_org;
                    next_monitor->w = scr_info[i].width;
                    next_monitor->h = scr_info[i].height;
                    next_monitor->tag_mask = -1;
                    next_monitor->active_tag_mask = 0;
                    next_monitor->next = NULL;
                    next_monitor->prev = NULL;
                    multimon_monitor_list.size = 1;
                    multimon_monitor_list.head_monitor = next_monitor;
            }
            while (monitor) {
                if (monitor->x == scr_info[i].x_org && monitor->y == scr_info[i].y_org) {
                    break;
                }
                if (monitor->next == NULL) {
                    // add monitor
                    next_monitor = malloc(sizeof(multimon_monitor_t));
                    next_monitor->x = scr_info[i].x_org;
                    next_monitor->y = scr_info[i].y_org;
                    next_monitor->w = scr_info[i].width;
                    next_monitor->h = scr_info[i].height;
                    next_monitor->tag_mask = 0;
                    next_monitor->active_tag_mask = 0;
                    next_monitor->next = NULL;
                    next_monitor->prev = monitor;
                    monitor->next = next_monitor;
                    multimon_monitor_list.size++;
                    break;
                }
                monitor = monitor->next;
            }
        }
        // free new monitor information
        XFree(scr_info);
    }
}

void multimon_init() {
    multimon_update();
    multimon_monitor_t *monitor = multimon_get_monitor(wm_global.tag_mask);
    monitor->active_tag_mask = wm_global.tag_mask;
    wm_global.x = monitor->x;
    wm_global.y = monitor->y;
    wm_global.w = monitor->w;
    wm_global.h = monitor->h;
}

void multimon_retag(int tag_mask) {
  multimon_update();
  multimon_monitor_t *monitor_old = multimon_get_monitor(wm_global.tag_mask);
  multimon_monitor_t *monitor_new = multimon_get_monitor(tag_mask);
  if (monitor_old != monitor_new) {
    wm_global.tag_mask = 0;
  }
  wm_global.x = monitor_new->x;
  wm_global.y = monitor_new->y;
  wm_global.w = monitor_new->w;
  wm_global.h = monitor_new->h;
  wm_retag(tag_mask);
}

void multimon_move_tag_mask_to_monitor(multimon_monitor_t *old, multimon_monitor_t *new) {
      wm_clients_unmap(new->active_tag_mask);
      old->tag_mask = old->tag_mask ^ wm_global.tag_mask;
      old->active_tag_mask = 0;
      new->tag_mask = new->tag_mask ^ wm_global.tag_mask;
      new->active_tag_mask = wm_global.tag_mask;
      wm_global.x = new->x;
      wm_global.y = new->y;
      wm_global.w = new->w;
      wm_global.h = new->h;
      wm_clients_arrange();
}

void multimon_move_tag_mask_to_next_monitor() {
    multimon_update();
    multimon_monitor_t *monitor = multimon_get_monitor(wm_global.tag_mask);
    if (monitor, monitor->next)
      multimon_move_tag_mask_to_monitor(monitor, monitor->next);
}

void multimon_move_tag_mask_to_prev_monitor() {
    multimon_update();
    multimon_monitor_t *monitor = multimon_get_monitor(wm_global.tag_mask);
    if (monitor, monitor->prev)
      multimon_move_tag_mask_to_monitor(monitor, monitor->prev);
}
