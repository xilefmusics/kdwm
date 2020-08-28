void monocle(int x, int y, int w, int h) {
    wm_client_t *client = NULL;
    while (client = wm_client_get_next(client)){
        wm_client_draw(client, x, y, w, h, false);
    }
}
