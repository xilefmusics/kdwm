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

    len = kdwmc_server_handler(len);

    if (sendto(s, kdwmc_server_buf, len, 0, (struct sockaddr*) &si_other, slen) == -1)
      continue;
  }
  close(s);
  return NULL;
}

int kdwmc_server_handler(int len) {
  strcpy(kdwmc_server_buf, "Hello from kdwmc.");
  return 18;
}

void kdwmc_server_stop() {
  kdwmc_server_running = false;
  pthread_join(kdwmc_server_thread_id, NULL);
}
