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

static struct wl_display *display;
static struct wl_registry *registry;

typedef struct {
  uint32_t modifiers;
  uint32_t key;
  char *action;
} keybind;

keybind keybinds[] = {
  { WLR_MODIFIER_ALT | WLR_MODIFIER_SHIFT, XKB_KEY_Return, "thunar" }
};

static void
dwl_yoink_keyevent(void *data,
                      struct zdwl_yoink_manager_v1 *m,
                      uint32_t key, uint32_t event, uint32_t modifiers) {
  char *evt = event == ZDWL_YOINK_MANAGER_V1_EVENT_TYPE_PRESSED ? "pressed" : "released";

  if (event == ZDWL_YOINK_MANAGER_V1_EVENT_TYPE_PRESSED) {
    for (int i = 0; i < (int)(sizeof(keybinds) / sizeof(keybinds[0])); i++) {
      keybind kb = keybinds[i];
      if (kb.action && kb.key == key && kb.modifiers == modifiers) {
        if (fork() == 0) {
          system(kb.action);
        }
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
    
    for (int i = 0; i < (int)(sizeof(keybinds) / sizeof(keybinds[0])); i++) {
      keybind kb = keybinds[i];
      zdwl_yoink_manager_v1_yoink_key(yoink_manager, kb.key, kb.modifiers, 1);
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
