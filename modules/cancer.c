#include <X11/Xatom.h>
void cancer() {
	Atom netwmname = XInternAtom(wm_global.display, "_NET_WM_NAME", false);
	Atom utf8_string = XInternAtom(wm_global.display, "UTF8_STRING", false);
	Atom netwmcheck = XInternAtom(wm_global.display, "_NET_SUPPORTING_WM_CHECK", false);
	XChangeProperty(wm_global.display, wm_global.root_window, netwmcheck, XA_WINDOW, 32, PropModeReplace, (unsigned char *)&wm_global.root_window, 1);
    XChangeProperty(wm_global.display, wm_global.root_window, netwmname, utf8_string, 8, PropModeReplace, "LG3D", 4);
	XSync(wm_global.display, false);
}
