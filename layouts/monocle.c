void monocle(multimon_monitor_t *monitor) {
    wm_client_t *client = NULL;
    while (client = wm_client_get_next(client)){
        wm_client_draw(client, monitor->x, monitor->y, monitor->w, monitor->h, false);
    }
}
