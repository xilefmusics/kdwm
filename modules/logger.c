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
    strcat(path_buffer, "/.kdwm/log.txt");
    FILE *fp = fopen(path_buffer, "a");
    // log
    fprintf(fp, "NEW STATE:\n");
    fprintf(fp, "current_tagmask: %d\n", wm_global.tag_mask);
    // log client list
    wm_client_t *client = wm_global.client_list.head_client;
    do {
        if (client->next && client->prev) {
            fprintf(fp, "\t%d <- %d (%d) -> %d\n", client->prev->window, client->window, client->tag_mask, client->next->window);
        } else if (client->next) {
            fprintf(fp, "\t%d <- %d (%d) -> %d\n", -1, client->window, client->tag_mask, client->next->window);
        } else if (client->prev) {
            fprintf(fp, "\t%d <- %d (%d) -> %d\n", client->prev->window, client->window, client->tag_mask, -1);
        } else {
            fprintf(fp, "\t%d <- %d (%d) -> %d\n", -1, client->window, client->tag_mask, -1);
        }
    } while ((client = client->next));
    // log monitor list
    wm_monitor_t *monitor = wm_global.monitor_list.head_monitor;
    if (monitor) {
        fprintf(fp, "size of monitor list: %d\n", wm_global.monitor_list.size);
        do {
            fprintf(fp, "\t%d <- %d (%d, (x:%d,y:%d,w:%d,h:%d)) -> %d\n", monitor->prev, monitor, monitor->tag_mask, monitor->x, monitor->y, monitor->w, monitor->h, monitor->next);
        } while ((monitor = monitor->next));
    } else {

            fprintf(fp, "monitor list is null\n");
    }
    // close logging
    fclose(fp);
}
