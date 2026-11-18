/*
 * Keep the "windows" layer in lockstep with the active BLE profile:
 * profile 1 (Windows host) -> layer on, any other profile -> layer off.
 *
 * zmk_ble_active_profile_changed fires for every path that changes the
 * active profile - &bt BT_SEL, BT_NXT/BT_PRV, and the reconnect after a
 * reboot (the profile index itself is restored from flash by ZMK). That
 * makes this listener the single source of truth: no keymap macros needed,
 * no state to persist, nothing runs while idle.
 *
 * Layer indices must match config/shared.keymap (windows layer = index 1).
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zmk/event_manager.h>
#include <zmk/events/ble_active_profile_changed.h>
#include <zmk/keymap.h>

LOG_MODULE_REGISTER(profile_layer_sync, CONFIG_ZMK_LOG_LEVEL);

#define WINDOWS_PROFILE_INDEX 1
#define WINDOWS_LAYER_INDEX 1

static int profile_layer_sync_listener(const zmk_event_t *eh) {
    const struct zmk_ble_active_profile_changed *ev = as_zmk_ble_active_profile_changed(eh);
    if (ev == NULL) {
        return ZMK_EV_EVENT_BUBBLE;
    }

    zmk_keymap_layer_id_t layer = zmk_keymap_layer_index_to_id(WINDOWS_LAYER_INDEX);
    if (layer == ZMK_KEYMAP_LAYER_ID_INVAL) {
        LOG_WRN("windows layer index %d not found in keymap", WINDOWS_LAYER_INDEX);
        return ZMK_EV_EVENT_BUBBLE;
    }

    if (ev->index == WINDOWS_PROFILE_INDEX) {
        zmk_keymap_layer_activate(layer, false);
    } else {
        zmk_keymap_layer_deactivate(layer, false);
    }

    LOG_INF("profile %d -> windows layer %s", ev->index,
            ev->index == WINDOWS_PROFILE_INDEX ? "on" : "off");

    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(profile_layer_sync, profile_layer_sync_listener);
ZMK_SUBSCRIPTION(profile_layer_sync, zmk_ble_active_profile_changed);
