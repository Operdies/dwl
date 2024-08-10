#include "blur-protocol.h"
#include <wayland-server-core.h>

static void blur_unset(struct wl_client *client, struct wl_resource *resource, struct wl_resource *surface);
static void blur_create(struct wl_client *client, struct wl_resource *resource, uint32_t id, struct wl_resource *surface);

static struct org_kde_kwin_blur_manager_interface blur_manager_implementation = { .unset = blur_unset, .create = blur_create };

static void blur_commit(struct wl_client *client, struct wl_resource *resource);
static void blur_set_region(struct wl_client *client, struct wl_resource *resource, struct wl_resource *region);
static void blur_release(struct wl_client *client, struct wl_resource *resource);

static struct org_kde_kwin_blur_interface blur_implementation = { .commit = blur_commit, .set_region = blur_set_region, .release = blur_release};


void blur_unset(struct wl_client *client, struct wl_resource *resource, struct wl_resource *surface) {
	wlr_log(WLR_INFO, "%s", __PRETTY_FUNCTION__);
}
void blur_create(struct wl_client *client, struct wl_resource *resource, uint32_t id, struct wl_resource *surface){
	wlr_log(WLR_INFO, "%s", __PRETTY_FUNCTION__);
}

void blur_commit(struct wl_client *client, struct wl_resource *resource){
	wlr_log(WLR_INFO, "%s", __PRETTY_FUNCTION__);
}
void blur_set_region(struct wl_client *client, struct wl_resource *resource, struct wl_resource *region){
	wlr_log(WLR_INFO, "%s", __PRETTY_FUNCTION__);
}
void blur_release(struct wl_client *client, struct wl_resource *resource){
	wlr_log(WLR_INFO, "%s", __PRETTY_FUNCTION__);
}

void blur_manager_destroy(struct wl_resource *resource) {
	wlr_log(WLR_INFO, "%s", __PRETTY_FUNCTION__);
	/* No state to destroy */
}
void blur_manager_bind(struct wl_client *client, void *data, uint32_t version, uint32_t id) {
	wlr_log(WLR_INFO, "%s", __PRETTY_FUNCTION__);
	struct wl_resource *manager_resource = wl_resource_create(client, &org_kde_kwin_blur_manager_interface, version, id);
	if (!manager_resource) {
		wl_client_post_no_memory(client);
		return;
	}
	wl_resource_set_implementation(manager_resource, &blur_manager_implementation, NULL, blur_manager_destroy);
}
