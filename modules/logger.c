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
}
