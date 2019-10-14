void logs(char *s) {
    // init logging
    char path_buffer[256];
    char *homedir = getenv("HOME");
    strcpy(path_buffer, homedir);
    strcat(path_buffer, "/.kdwm/log.txt");
    FILE *fp = fopen(path_buffer, "a");
    // log
    fprintf(fp, "%s\n", s);
    // close logging
    fclose(fp);
}

void logsi(char *s, int i) {
    // init logging
    char path_buffer[256];
    char *homedir = getenv("HOME");
    strcpy(path_buffer, homedir);
    strcat(path_buffer, "/.kdwm/log.txt");
    FILE *fp = fopen(path_buffer, "a");
    // log
    fprintf(fp, "%s: %d\n", s, i);
    // close logging
    fclose(fp);
}

void log_state() {
    // init logging
    char path_buffer[256];
    char *homedir = getenv("HOME");
    strcpy(path_buffer, homedir);
    strcat(path_buffer, "/.kdwm/state.txt");
    FILE *fp = fopen(path_buffer, "a");
    // log
    fprintf(fp, "state:\n");
    wm_client_t *client = wm_global.client_list.head_client;
    while (client->next != NULL) {
        fprintf(fp, "client window-ID %d, tag-mask (in hex) %x; ", client->window, client->tag_mask);
        client = client->next;
    }
    if (client!=NULL) {
        fprintf(fp, "client window-ID %d, tag-mask (in hex) %x; \n", client->window, client->tag_mask);
    }
    // close logging
    fclose(fp);
}
