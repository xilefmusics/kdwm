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

void log_monitor_list() {
  printf("MONITOR LIST: ");
  multimon_monitor_t *monitor = multimon_monitor_list.head_monitor;
  while(monitor) {
    printf("((%d, %d, %d, %d), %d,%d)->", monitor->x, monitor->y, monitor->w, monitor->h, monitor->tag_mask, monitor->active_tag_mask);
    monitor = monitor->next;
  }
  printf("NULL\n");
}

void log_pertag() {
  printf("PERTAG LAYOUTS: ");
  for (int i = 0; i < NUM_OF_TAGS; ++i)
    printf("%d ", pertag_layouts[i]);
  printf("\n");
  printf("PERTAG MASTER WIDTHS: ");
  for (int i = 0; i < NUM_OF_TAGS; ++i)
    printf("%d ", pertag_master_widths[i]);
  printf("\n");
  printf("PERTAG WINDOW OF ACTIVE CLIENT: ");
  for (int i = 0; i < NUM_OF_TAGS; ++i)
    printf("%d ", pertag_window_of_active_client[i]);
  printf("\n");
}

void log_state() {
  printf("\n");
  log_client_list();
  log_monitor_list();
  log_pertag();
  fflush(stdout);
}
