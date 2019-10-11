void masterstack(wm_global_t * global) {
    int num_of_clients = wm_clients_count();

    wm_client_t *client = wm_client_get_next(NULL);

    if (num_of_clients == 0) {
        return;
    } else if (num_of_clients == 1) {
        wm_client_draw(client, 0, 0, global->screen_width, global->screen_height);
    } else {
        int num_of_slaves = num_of_clients - 1;

        int slave_height = global->screen_height / num_of_slaves;
        int master_height = global->screen_height;

        int master_width = (int) (((float)global->screen_width) * ((float)global->master_width)/100);
        int slave_width = global->screen_width - master_width;

        wm_client_draw(client, 0, 0, master_width, master_height);

        for (int i = 0; i < num_of_slaves; i++) {
            client = wm_client_get_next(client);
            wm_client_draw(client, master_width, i*slave_height, slave_width, slave_height);
        }
    }

    XFlush(global->display);
}
