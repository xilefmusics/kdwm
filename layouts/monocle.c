void monocle(wm_global_t *global) {
    wm_client_t *client = NULL;
    while (client = wm_client_get_next(client)){
        wm_client_draw(client, 0, 0, global->screen_width, global->screen_height);
    }
}
