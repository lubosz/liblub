#include <wayland-client.h>
#include <wayland-egl.h>

#include "eglutint.h"

struct display {
   struct wl_display *display;
   struct wl_compositor *compositor;
   struct wl_shell *shell;
   uint32_t mask;
};

struct window {
   struct wl_surface *surface;
   struct wl_shell_surface *shell_surface;
   struct wl_callback *callback;
};

static struct display display = {0, };
static struct window window = {0, };

static void
display_handle_global(struct wl_display *display, uint32_t id,
		      const char *interface, uint32_t version, void *data)
{
   struct display *d = data;

   if (strcmp(interface, "wl_compositor") == 0) {
      d->compositor =
         wl_display_bind(display, id, &wl_compositor_interface);
   } else if (strcmp(interface, "wl_shell") == 0) {
      d->shell = wl_display_bind(display, id, &wl_shell_interface);
   }
}

static int
event_mask_update(uint32_t mask, void *data)
{
   struct display *d = data;

   d->mask = mask;

   return 0;
}

void
_eglutNativeInitDisplay(void)
{
   _eglut->native_dpy =  display.display = wl_display_connect(NULL);

   if (!_eglut->native_dpy)
      _eglutFatal("failed to initialize native display");

   wl_display_add_global_listener(_eglut->native_dpy,
         display_handle_global, &display);

   wl_display_get_fd(_eglut->native_dpy, event_mask_update, &display);
   wl_display_iterate(_eglut->native_dpy, WL_DISPLAY_READABLE);

   _eglut->surface_type = EGL_WINDOW_BIT;
}

void
_eglutNativeFiniDisplay(void)
{
   wl_display_flush(_eglut->native_dpy);
   wl_display_disconnect(_eglut->native_dpy);
}

void
_eglutNativeInitWindow(struct eglut_window *win, const char *title,
                       int x, int y, int w, int h)
{
   struct wl_egl_window *native;

   window.surface = wl_compositor_create_surface(display.compositor);
   window.shell_surface = wl_shell_get_shell_surface(display.shell,
         window.surface);
   native = wl_egl_window_create(window.surface, w, h);

   wl_shell_surface_set_toplevel(window.shell_surface);

   win->native.u.window = native;
   win->native.width = w;
   win->native.height = h;
}

void
_eglutNativeFiniWindow(struct eglut_window *win)
{
   wl_egl_window_destroy(win->native.u.window);

   wl_shell_surface_destroy(window.shell_surface);
   wl_surface_destroy(window.surface);

   if (window.callback)
      wl_callback_destroy(window.callback);
}

static void
draw(void *data, struct wl_callback *callback, uint32_t time);

static const struct wl_callback_listener frame_listener = {
   draw
};

static void
draw(void *data, struct wl_callback *callback, uint32_t time)
{	
   struct window *window = (struct window *)data;
   struct eglut_window *win = _eglut->current;

   if (win->display_cb)
      win->display_cb();
   eglSwapBuffers(_eglut->dpy, win->surface);

   if (callback)
      wl_callback_destroy(callback);

   window->callback = wl_surface_frame(window->surface);
   wl_callback_add_listener(window->callback, &frame_listener, window);
}

void
_eglutNativeEventLoop(void)
{
   draw(&window, NULL, 0);

   while (1) {
      wl_display_iterate(display.display, display.mask);

      if (_eglut->idle_cb)
         _eglut->idle_cb();
   }
}
