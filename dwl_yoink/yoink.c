#include "../dwl/util.c"
#include "dwl-yoink-unstable-v1-client-protocol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wayland-client-protocol.h>
#include <wayland-client.h>
#include <wayland-util.h>
#include <wlr/types/wlr_keyboard.h>
#include <xkbcommon/xkbcommon-keysyms.h>
#include "../dwl/dwl.h"

/* TODO: Create Yeet protocol
 * The use case of this is basically hot key remapping.
 * investigate if this is needed or wltype does everything already.
 */

/* TODO: Implement yoink rejection on both client / server side.
 * A yoink should be rejected if it is already in use by the server,
 * and potentially by other yoinkers. `intercept` should reserve a yoink
 * since the server will not propagate yoinks in this case, so there is
 * (seemingly) no need for an `exclusive` flag in the protocol.
 */

/* TODO: implement dwl_ipc protocol
 * This is needed to migrate the rest of the binds that are still implemented on
 * the dwl side. This will likely also require some additional IPC functions for
 * things like manipulating the stack. There is no need to migrate things like
 * CHVT since I really don't want to risk breaking the compositor itself.
 */

/* TODO: Add way to reload binds
 * The simplest way to do this is probably just:
 * 0. User rebuilds yoink application
 * 1. cleanup
 * 2. exec(self)
 */

/* If you want to use the windows key for MODKEY, use WLR_MODIFIER_LOGO */
#ifndef MODKEY
#define MODKEY WLR_MODIFIER_LOGO
#endif

static struct wl_display *display;
static struct wl_registry *registry;

static const char *menucmd[] = { "rofi", "-show", "drun", NULL };
static const char *whichkeycmd[] = { "wlr-which-key", NULL };
static const char *powermenucmd[] = { "sh", "-c", "wlr-which-key ~/.config/wlr-which-key/power.yaml", NULL };
static const char *clipboardcmd[] = { "sh", "-c", "clipman pick -t wofi --err-on-no-selection && wtype -M ctrl -M shift v", NULL };
static const char *screenshotcmd[] = { "sh", "-c", "wlr-which-key ~/.config/wlr-which-key/screenshot.yaml", NULL };
static const char *hoardmacro[] = { "sh", "-c", "soffice ~/repos/ffxiv/Hoard\\ Farm.ods 'macro:///Standard.Module1.IncrementHoard()'", NULL };

static const Key keys[] = {
  /* Note that Shift changes certain key codes: c -> C, 2 -> at, etc. */
  /* modifier                  key                 function        argument */
  { MODKEY,                    XKB_KEY_p,          spawn,          {.v = menucmd} },
  { MODKEY,                    XKB_KEY_r,          spawn,          {.v = whichkeycmd } },
  { MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_E,          spawn,          {.v = powermenucmd } },
  { MODKEY,                    XKB_KEY_v,          spawn,          {.v = clipboardcmd} },
  { MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_S,          spawn,          {.v = screenshotcmd} },
  { MODKEY,                    XKB_KEY_grave,      spawn,          {.v = hoardmacro } },
};


void
spawn(const Arg *arg)
{
	if (fork() == 0) {
		dup2(STDERR_FILENO, STDOUT_FILENO);
		setsid();
		execvp(((char **)arg->v)[0], (char **)arg->v);
		die("dwl: execvp %s failed:", ((char **)arg->v)[0]);
	}
}



static void
dwl_yoink_keyevent(void *data,
                      struct zdwl_yoink_manager_v1 *m,
                      uint32_t key, uint32_t event, uint32_t modifiers) {
  if (event == ZDWL_YOINK_MANAGER_V1_EVENT_TYPE_PRESSED) {
    for (int i = 0; i < (int)(sizeof(keys) / sizeof(keys[0])); i++) {
	  const Key *k = &keys[i];
	  if (k && k->func && k->mod == modifiers && k->keysym == key){
		k->func(&k->arg);
      }
    }
  }
}

static const struct zdwl_yoink_manager_v1_listener keylogger = {
    .keyevent = dwl_yoink_keyevent,
};

static void global_add(void *data, struct wl_registry *wl_registry,
                       uint32_t name, const char *interface, uint32_t version) {
  if (strcmp(interface, zdwl_yoink_manager_v1_interface.name) == 0) {
    struct zdwl_yoink_manager_v1 *yoink_manager = wl_registry_bind(
        wl_registry, name, &zdwl_yoink_manager_v1_interface, 1);
    zdwl_yoink_manager_v1_add_listener(yoink_manager, &keylogger, NULL);
    
    for (int i = 0; i < (int)(sizeof(keys) / sizeof(keys[0])); i++) {
      const Key *kb = &keys[i];
      zdwl_yoink_manager_v1_yoink_key(yoink_manager, kb->keysym, kb->mod, 1);
    }
  }
}

static void global_remove(void *data, struct wl_registry *registry, uint32_t name) { }

static const struct wl_registry_listener registry_listener = {
    .global = global_add,
    .global_remove = global_remove,
};

int main(void) {
  display = wl_display_connect(NULL);
  if (!display)
    die("bad display");
  registry = wl_display_get_registry(display);
  if (!registry)
    die("bad registry");

  wl_registry_add_listener(registry, &registry_listener, NULL);

  wl_display_dispatch(display);
  wl_display_roundtrip(display);

  // dispatch to keep yoinking keys
  while (wl_display_dispatch(display) != -1) {
  }

  return 0;
}
