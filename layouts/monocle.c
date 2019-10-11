void monocle() {
    wm_client_t *client = NULL;
    while (client = wm_client_get_next(client)){
        wm_client_draw(client, 0, 0, wm_global.screen_width, wm_global.screen_height);
    }
}
