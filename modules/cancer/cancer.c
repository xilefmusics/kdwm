void cancer() {
  XChangeProperty(wm_global.display, wm_global.root_window, wm_global.atoms[_NET_WM_NAME], wm_global.atoms[UTF8_STRING], 8, PropModeReplace, "LG3D_aka_kdwm", 13);
  XSync(wm_global.display, false);
}
