void masterstack(int x, int y, int w, int h) {
    int num_of_clients = wm_clients_count();
    wm_client_t *client = wm_client_get_next(NULL);
    if (num_of_clients == 0) {
        return;
    } else if (num_of_clients == 1) {
        wm_client_draw(client, x, y, w, h, false);
    } else {
        int num_of_slaves = num_of_clients - 1;
        int slave_height = h / num_of_slaves;
        int master_height = h;
        int master_width = (int) (((float)w) * ((float)wm_global.master_width)/100);
        int slave_width = w - master_width;
        wm_client_draw(client, x, y, master_width, master_height, true);
        for (int i = 0; i < num_of_slaves; i++) {
            client = wm_client_get_next(client);
            wm_client_draw(client, master_width+x, i*slave_height+y, slave_width, slave_height, true);
        }
    }
    XFlush(wm_global.display);
}
