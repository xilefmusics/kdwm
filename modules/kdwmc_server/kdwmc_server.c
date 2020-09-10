void kdwmc_server_start() {
  kdwmc_server_running = true;
  pthread_create(&kdwmc_server_thread_id, NULL, kdwmc_server_thread, NULL);
}

void *kdwmc_server_thread(void *vargp) {
  struct sockaddr_in si_me, si_other;
  int s, len, slen = sizeof(si_other);

  if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    return NULL;

  si_me.sin_family = AF_INET; // IPv4
  si_me.sin_addr.s_addr = htonl(INADDR_ANY); // Ip-Adresse
  si_me.sin_port = htons(KDWMC_SERVER_PORT); // Port

  if(bind(s, (struct sockaddr*)&si_me, sizeof(si_me)) == -1) {
    close(s);
    return NULL;
  }

  while (kdwmc_server_running) {
    if ((len = recvfrom(s, kdwmc_server_buf, KDWMC_SERVER_BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
      continue;

    len = kdwmc_server_handler(len, (struct sockaddr*) &si_other);

    if (sendto(s, kdwmc_server_buf, len, 0, (struct sockaddr*) &si_other, slen) == -1)
      continue;
  }
  close(s);
  return NULL;
}

int kdwmc_server_handler(int len, struct sockaddr *client) {
  char *ptr = kdwmc_server_buf, *space;

  if (!strcmp(ptr, "subscribe")) {
    kdwmc_server_subscriber = *client;
    kdwmc_server_has_subscriber = true;
    sprintf(kdwmc_server_buf, "0");
    return strlen(kdwmc_server_buf) + 1;
  }

  if (!(space = strchr(ptr, ' '))) {
    sprintf(kdwmc_server_buf, "ERROR: unsupported operation");
    return strlen(kdwmc_server_buf) + 1;
  }
  *space = '\0';

  if (!strcmp(ptr, "get")) {
    ptr = space+1;
    if (!strcmp(ptr, "tag_mask")) {
      sprintf(kdwmc_server_buf, "%d", wm_global.tag_mask);
    } else if (!strcmp(ptr, "dimensions")) {
      sprintf(kdwmc_server_buf, "%d %d %d %d", wm_global.x, wm_global.y, wm_global.w, wm_global.h);
    } else if (!strcmp(ptr, "master_width")) {
      sprintf(kdwmc_server_buf, "%d", wm_global.master_width);
    } else if (!strcmp(ptr, "border_width")) {
      sprintf(kdwmc_server_buf, "%d", wm_global.border_width);
    } else if (!strcmp(ptr, "current_layout")) {
      sprintf(kdwmc_server_buf, "%d", wm_global.current_layout);
    } else if (!strcmp(ptr, "num_of_tags")) {
      sprintf(kdwmc_server_buf, "%d", NUM_OF_TAGS);
    } else if (!strcmp(ptr, "client_list")) {
      wm_client_t *client = wm_global.client_list.head_client;
      sprintf(kdwmc_server_buf, "(%d, %d)", client->window, client->tag_mask);
      while ((client = client->next) != NULL) {
        sprintf(kdwmc_server_buf+strlen(kdwmc_server_buf), "->(%d, %d)", client->window, client->tag_mask);
      }
    } else if (!strcmp(ptr, "active_client")) {
      if (wm_global.client_list.active_client) {
        sprintf(kdwmc_server_buf, "%d", wm_global.client_list.active_client->window);
      } else {
        sprintf(kdwmc_server_buf, "%d", -1);
      }
    } else {
      sprintf(kdwmc_server_buf, "ERROR: unsupported operation");
    }
  }
  return strlen(kdwmc_server_buf) + 1;
}

void kdwmc_server_subscribe() {
  if (!kdwmc_server_has_subscriber)
    return;
  int s;
  if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    return;
  char buffer[16];
  sprintf(buffer, "%d %d", wm_global.tag_mask, wm_global.current_layout);
  sendto(s, buffer, strlen(buffer)+1, 0, &kdwmc_server_subscriber, sizeof(kdwmc_server_subscriber));
}

void kdwmc_server_stop() {
  kdwmc_server_running = false;
}
