void autostart() {
  for (int i = 0; i < LENGTH(autostart_commands); i++)
    system(autostart_commands[i]);
}
