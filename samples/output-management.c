/* NOTE: wlr-output-management provides a lot more functionality than simply name / descriptions.
 * If only name/description is needed, we can use `xdg-output` instead, which is more widely supported and simpler to use.
 * */
#include "wlr-output-management-unstable-v1.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <wayland-client-core.h>
#include <wayland-client-protocol.h>
#include <wayland-client.h>

static struct wl_display *display;
static struct wl_registry *registry;
static struct zwlr_output_manager_v1 *output_manager;

static void noop() {}

void output_head_description_handler(void *data,
                              struct zwlr_output_head_v1 *zwlr_output_head_v1,
                              const char *description) {
  printf("Output with description %s\n", description);
}

void output_head_name_handler(void *data,
                              struct zwlr_output_head_v1 *zwlr_output_head_v1,
                              const char *name) {
  printf("Output with name %s\n", name);
}

struct zwlr_output_head_v1_listener output_head_listener = {
    .adaptive_sync = noop,
    .current_mode = noop,
    .description = output_head_description_handler,
    .enabled = noop,
    .finished = noop,
    .make = noop,
    .mode = noop,
    .model = noop,
    .name = output_head_name_handler,
    .physical_size = noop,
    .position = noop,
    .scale = noop,
    .serial_number = noop,
    .transform = noop,
};

static void output_manager_handle_head(
    void *data, struct zwlr_output_manager_v1 *zwlr_output_manager_v1,
    struct zwlr_output_head_v1 *head) {
  zwlr_output_head_v1_add_listener(head, &output_head_listener, NULL);
}
static void output_manager_handle_done(
    void *data, struct zwlr_output_manager_v1 *zwlr_output_manager_v1,
    uint32_t serial) {
  // The output is done transmitting a configuration change
}
static void output_manager_handle_finished(
    void *data, struct zwlr_output_manager_v1 *zwlr_output_manager_v1) {
}
struct zwlr_output_manager_v1_listener output_manager_listener = {
    .done = output_manager_handle_done,
    .head = output_manager_handle_head,
    .finished = output_manager_handle_finished,
};

static void wl_registry_global_add_handler(void *data,
                                           struct wl_registry *wl_registry,
                                           uint32_t name, const char *interface,
                                           uint32_t version) {
  if (strcmp(interface, zwlr_output_manager_v1_interface.name) == 0) {
    output_manager =
        wl_registry_bind(registry, name, &zwlr_output_manager_v1_interface, 2);
    zwlr_output_manager_v1_add_listener(output_manager,
                                        &output_manager_listener, NULL);
  }
}

struct wl_registry_listener registry_listener = {
    .global = wl_registry_global_add_handler,
    .global_remove = noop,
};

int main(void) {
  display = wl_display_connect(NULL);
  assert(display);
  registry = wl_display_get_registry(display);
  assert(registry);
  wl_registry_add_listener(registry, &registry_listener, NULL);
  wl_display_dispatch(display);
  wl_display_roundtrip(display);
  assert(output_manager);

  while (wl_display_dispatch(display) != -1) {
    // wait for exit
  }

  return 0;
}

