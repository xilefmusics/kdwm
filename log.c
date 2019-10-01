void logs(char *s) {
    fprintf(wm_global.log_fp, "%s\n", s);
    fflush(wm_global.log_fp);
}

void logsi(char *s, int i) {
    fprintf(wm_global.log_fp, "%s: %d\n", s, i);
    fflush(wm_global.log_fp);
}

void log_state() {
}
