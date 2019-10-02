void monocle(wm_global_t *global) {
    wm_client_t *client = NULL;
    while (client = wm_client_get_next(client)){
        XWindowChanges changes;
        changes.x = 0;
        changes.y = 0;
        changes.width = global->screen_width;
        changes.height = global->screen_height;
        XConfigureWindow(global->display, client->window, 15, &changes);
    }
}
