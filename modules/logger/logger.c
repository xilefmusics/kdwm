void log_client_list() {
    printf("CLIENT LIST: ");
    wm_client_t *client = wm_global.client_list.head_client;
    while (client) {
        printf("(%d, %d)->", client->window, client->tag_mask);
        client = client->next;
    }
    printf("NULL\n");
    if (wm_global.client_list.active_client) {
        printf("ACTIVE CLIENT: %d\n", wm_global.client_list.active_client->window);
    } else {
        printf("ACTIVE CLIENT: NULL\n");
    }
}

void log_state() {
    printf("\n");
    log_client_list();
}
