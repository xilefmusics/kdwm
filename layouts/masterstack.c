void masterstack(wm_global_t * global) {
    int num_of_clients = wm_clients_count(global->tag_mask);

    wm_client_t *client = global->client_list.head_client;
    if (!(client->tag_mask & global->tag_mask)) {
        client = wm_client_get_next(client);
    }

    if (num_of_clients == 0) {

    } else if (num_of_clients == 1) {
        XWindowChanges changes;
        changes.x = 0;
        changes.y = 0;
        changes.width = global->screen_width;
        changes.height = global->screen_height;
        XConfigureWindow(global->display, client->window, 15, &changes);
    } else {
        int num_of_slaves = num_of_clients - 1;

        int slave_height = global->screen_height / num_of_slaves;
        int master_height = global->screen_height;

        int master_width = (int) global->screen_width * global->master_width;
        int slave_width = global->screen_width - master_width;

        XWindowChanges changes;

        changes.x = 0;
        changes.y = 0;
        changes.width = master_width;
        changes.height = master_height;

        XConfigureWindow(global->display, client->window, 15, &changes);

        for (int i = 0; i < num_of_slaves; i++) {
            client = wm_client_get_next(client);

            changes.x = master_width;
            changes.y = i*slave_height;
            changes.width = slave_width;
            changes.height = slave_height;

            XConfigureWindow(global->display, client->window, 15, &changes);
        }
    }

    XFlush(global->display);
}
