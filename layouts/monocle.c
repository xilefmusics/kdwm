void monocle(wm_global_t *global) {
    wm_client_t *client = global->client_list.head_client;
    if (!(client->tag_mask & global->tag_mask)) {
        client = wm_client_get_next(client);
    }
    do {
        XWindowChanges changes;
        changes.x = 0;
        changes.y = 0;
        changes.width = global->screen_width;
        changes.height = global->screen_height;
        XConfigureWindow(global->display, client->window, 15, &changes);
    } while (client = wm_client_get_next(client));
}
