#define _POSIX_C_SOURCE 200809L
#include <unistd.h>

#include <uvc_camera.h>

#include "flutter-pi.h"
#include "platformchannel.h"
#include "pluginregistry.h"
#include "util/logging.h"

#define UVC_CAMERA_PLUGIN_METHOD_CHANNEL "uvc_camera_flutterpi"

#define UVC_CAMERA_PLUGIN_DEBUG 1
#define LOG_UVC_CAMERA_DEBUG(fmt, ...)         \
    do {                                   \
        if (UVC_CAMERA_PLUGIN_DEBUG)           \
            LOG_DEBUG(fmt, ##__VA_ARGS__); \
    } while (0)

struct uvc_camera_plugin {
    bool uvc_camera_initialized;
};

static void on_method_call(void *userdata, const FlutterPlatformMessage *message) {
    const FlutterPlatformMessageResponseHandle *responsehandle;
    const struct raw_std_value *envelope, *method, *arg;
    struct uvc_camera_plugin *plugin;

    ASSERT_NOT_NULL(userdata);
    ASSERT_NOT_NULL(message);
    plugin = userdata;
    responsehandle = message->response_handle;
    envelope = (const struct raw_std_value *) (message->message);
    if (!raw_std_method_call_check(envelope, message->message_size)) {
        platch_respond_error_std(responsehandle, "malformed-message", "", &STDNULL);
        return;
    }

    method = raw_std_method_call_get_method(envelope);
    arg = raw_std_method_call_get_arg(envelope);

    // if (raw_std_string_equals(method, "initNativeSdk")) {
    //     on_init_native_sdk(plugin, arg, responsehandle);
    // } else if (raw_std_string_equals(method, "captureEnvelope")) {
    //     on_capture_envelope(plugin, arg, responsehandle);
    // } else if (raw_std_string_equals(method, "loadImageList")) {
    //     on_load_image_list(plugin, arg, responsehandle);
    // } else if (raw_std_string_equals(method, "closeNativeSdk")) {
    //     on_close_native_sdk(plugin, arg, responsehandle);
    // } else if (raw_std_string_equals(method, "fetchNativeAppStart")) {
    //     on_fetch_native_app_start(plugin, arg, responsehandle);
    // } else if (raw_std_string_equals(method, "beginNativeFrames")) {
    //     on_begin_native_frames(plugin, arg, responsehandle);
    // } else if (raw_std_string_equals(method, "endNativeFrames")) {
    //     on_end_native_frames(plugin, arg, responsehandle);
    // } else if (raw_std_string_equals(method, "setUser")) {
    //     on_set_user(plugin, arg, responsehandle);
    // } else if (raw_std_string_equals(method, "addBreadcrumb")) {
    //     on_add_breadcrumb(plugin, arg, responsehandle);
    // } else if (raw_std_string_equals(method, "clearBreadcrumbs")) {
    //     on_clear_breadcrumbs(plugin, arg, responsehandle);
    // } else if (raw_std_string_equals(method, "setContexts")) {
    //     on_set_contexts(plugin, arg, responsehandle);
    // } else if (raw_std_string_equals(method, "removeContexts")) {
    //     on_remove_contexts(plugin, arg, responsehandle);
    // } else if (raw_std_string_equals(method, "setExtra")) {
    //     on_set_extra(plugin, arg, responsehandle);
    // } else if (raw_std_string_equals(method, "removeExtra")) {
    //     on_remove_extra(plugin, arg, responsehandle);
    // } else if (raw_std_string_equals(method, "setTag")) {
    //     on_set_tag(plugin, arg, responsehandle);
    // } else if (raw_std_string_equals(method, "removeTag")) {
    //     on_remove_tag(plugin, arg, responsehandle);
    // } else if (raw_std_string_equals(method, "discardProfiler")) {
    //     on_discard_profiler(plugin, arg, responsehandle);
    // } else if (raw_std_string_equals(method, "collectProfile")) {
    //     on_collect_profile(plugin, arg, responsehandle);
    // } else {
    //     platch_respond_error_std(responsehandle, "unknown-method", "", &STDNULL);
    // }
}

enum plugin_init_result uvc_camera_plugin_deinit(struct flutterpi *flutterpi, void **userdata_out) {
    struct uvc_camera_plugin *plugin;
    int ok;

    plugin = malloc(sizeof *plugin);
    if (plugin == NULL) {
        return PLUGIN_INIT_RESULT_ERROR;
    }

    ok = plugin_registry_set_receiver_v2_locked(
        flutterpi_get_plugin_registry(flutterpi),
        UVC_CAMERA_PLUGIN_METHOD_CHANNEL,
        on_method_call,
        plugin
    );
    if (ok != 0) {
        free(plugin);
        return PLUGIN_INIT_RESULT_ERROR;
    }

    *userdata_out = plugin;

    return PLUGIN_INIT_RESULT_INITIALIZED;
}

void uvc_camera_plugin_init(struct flutterpi *flutterpi, void *userdata) {
    struct uvc_camera_plugin *plugin;

    ASSERT_NOT_NULL(userdata);
    plugin = userdata;

    if (plugin->uvc_camera_initialized) {
    }

    plugin_registry_remove_receiver_v2_locked(flutterpi_get_plugin_registry(flutterpi), UVC_CAMERA_PLUGIN_METHOD_CHANNEL);
    free(plugin);
}

FLUTTERPI_PLUGIN("uvc_camera", uvc_camera_plugin_init, uvc_camera_plugin_deinit, NULL);
