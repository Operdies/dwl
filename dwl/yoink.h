#ifndef YOINK_H
#define YOINK_H

#include "dwl-yoink-unstable-v1-protocol.h"
#include "util.h"
#include <stdio.h>
#include <wayland-server-core.h>
#include <wayland-util.h>

static void dwl_yoink_release(struct wl_client *client,
                                 struct wl_resource *resource);
static void dwl_yoink_yoink_key(struct wl_client *client,
                                  struct wl_resource *resource, uint32_t key,
                                  uint32_t modifiers, int intercept);
static void dwl_yoink_unyoink_key(struct wl_client *client,
                                    struct wl_resource *resource, uint32_t key,
                                    uint32_t modifiers);
static void dwl_yoink_unyoink_all(struct wl_client *client,
                                    struct wl_resource *resource);
void dwl_yoink_manager_bind(struct wl_client *client, void *data,
                               uint32_t version, uint32_t id);
static void dwl_yoink_manager_destroy(struct wl_resource *resource);

static struct zdwl_yoink_manager_v1_interface dwl_yoink_implementation = {
    .release = dwl_yoink_release,
    .yoink_key = dwl_yoink_yoink_key,
    .unyoink_key = dwl_yoink_unyoink_key,
    .unyoink_all = dwl_yoink_unyoink_all,
};

typedef struct {
  struct wl_list link;
  uint32_t key;
  uint32_t modifiers;
  int intercept;
} dwl_yoink_yoink;

typedef struct {
  struct wl_list link;
  struct wl_list yoinkset; /* dwl_yoink_yoink */
  struct wl_resource *resource;
} dwl_yoink_client;

static struct wl_list yoink_clients; /* dwl_yoink_client */

static void unyoink(dwl_yoink_yoink *yoink) {
  wl_list_remove(&yoink->link);
  free(yoink);
}

static void unyoink_all(dwl_yoink_client *client) {
  dwl_yoink_yoink *yoink, *tmp;
  wl_list_for_each_safe(yoink, tmp, &client->yoinkset, link) { unyoink(yoink); }
}

void dwl_yoink_release(struct wl_client *client,
                          struct wl_resource *resource) {
  dwl_yoink_manager_destroy(resource);
}

void dwl_yoink_yoink_key(struct wl_client *client,
                           struct wl_resource *resource, uint32_t key,
                           uint32_t modifiers, int intercept) {

  dwl_yoink_client *c = wl_resource_get_user_data(resource);
  dwl_yoink_yoink *yoink;
  bool order = true;

  if (order) {
    wl_list_for_each(yoink, &c->yoinkset, link) {
      if (key < yoink->key) {
        dwl_yoink_yoink *new = ecalloc(1, sizeof(*new));
        new->key = key;
        new->modifiers = modifiers;
        new->intercept = intercept;
        wl_list_insert(yoink->link.prev, &new->link);
        return;
      } else if (yoink->key == key && yoink->modifiers == modifiers) {
        yoink->intercept = intercept;
        return;
      }
    }
    dwl_yoink_yoink *new = ecalloc(1, sizeof(*new));
    new->key = key;
    new->modifiers = modifiers;
    new->intercept = intercept;
    wl_list_insert(c->yoinkset.prev, &new->link);
    return;
  } else {
    dwl_yoink_yoink *new = ecalloc(1, sizeof(*new));
    new->key = key;
    new->modifiers = modifiers;
    new->intercept = intercept;
    wl_list_insert(c->yoinkset.prev, &new->link);
    return;
  }
}

void dwl_yoink_unyoink_key(struct wl_client *client,
                             struct wl_resource *resource, uint32_t key,
                             uint32_t modifiers) {
  dwl_yoink_client *c = wl_resource_get_user_data(resource);
  dwl_yoink_yoink *yoink, *tmp;
  wl_list_for_each_safe(yoink, tmp, &c->yoinkset, link) {
    if (yoink->key == key && yoink->modifiers == modifiers) {
      unyoink(yoink);
      return;
    }
  }
}

void dwl_yoink_unyoink_all(struct wl_client *client,
                             struct wl_resource *resource) {
  dwl_yoink_client *c = wl_resource_get_user_data(resource);
  unyoink_all(c);
}

void dwl_yoink_manager_bind(struct wl_client *client, void *data,
                               uint32_t version, uint32_t id) {
  dwl_yoink_client *yoink_client;
  struct wl_resource *manager_resource = wl_resource_create(
      client, &zdwl_yoink_manager_v1_interface, version, id);
  if (!manager_resource) {
    wl_client_post_no_memory(client);
    return;
  }
  yoink_client = ecalloc(1, sizeof(*yoink_client));
  wl_resource_set_implementation(manager_resource, &dwl_yoink_implementation,
                                 yoink_client, dwl_yoink_manager_destroy);
  wl_list_init(&yoink_client->yoinkset);

  wl_list_insert(&yoink_clients, &yoink_client->link);
  yoink_client->resource = manager_resource;
}

static void dwl_yoink_manager_destroy(struct wl_resource *resource) {
  dwl_yoink_client *c = wl_resource_get_user_data(resource);
  unyoink_all(c);
  wl_list_remove(&c->link);
}

#endif //  YOINK_H
