#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <event2/event.h>
#include <af_log.h>

/*
 * aflib API for:
 *   - receiving attribute "set" requests from the service
 *   - sending attribute updates to the service
 */

/* API will not accept attribute values larger than this */
#define MAX_ATTRIBUTE_SIZE                  255

typedef enum {
    AF_SUCCESS = 0,
    // errors that don't apply to edge:
    // AF_ERROR_NO_SUCH_ATTRIBUTE = -1,        // unknown attribute id
    // AF_ERROR_BUSY
    // AF_ERROR_INVALID_COMMAND
    // AF_ERROR_QUEUE_OVERFLOW
    // AF_ERROR_QUEUE_UNDERFLOW
    AF_ERROR_INVALID_PARAM = -6,            // bad input parameter
    AF_ERROR_UNAVAILABLE = -7,              // hubby is not available right now
} af_status_t;

/*
 * a remote client is requesting that an attribute be changed.
 * return `true` to accept the change, or `false` to reject it.
 * (to process changes asynchronously, set `aflib_...`.)
 */
typedef bool (*aflib_set_handler_t)(const uint16_t attr_id, const uint16_t value_len, const uint8_t *value);

/*
 * notification of an attribute's current value, either because it has
 * changed internally, or because you asked for the current value with
 * `aflib_get_attribute`.
 */
typedef void (*aflib_notify_handler_t)(const uint16_t attr_id, const uint16_t value_len, const uint8_t *value);

/*
 * service connection status has changed.
 */
typedef void (*aflib_connect_handler_t)(bool connected);

/*
 * start aflib and register callbacks.
 */
af_status_t aflib_init(struct event_base *ev, aflib_set_handler_t set_handler, aflib_notify_handler_t notify_handler);

/*
 * request the current value of an attribute. the result is sent via the
 * `aflib_notify_handler_t` callback.
 */
af_status_t aflib_get_attribute(const uint16_t attr_id);

/*
 * request an attribute to be set.
 */
af_status_t aflib_set_attribute_bytes(const uint16_t attr_id, const uint16_t value_len, const uint8_t *value);

/*
 * variants of setting an attribute, for convenience.
 */
af_status_t aflib_set_attribute_bool(const uint16_t attr_id, const bool value);
af_status_t aflib_set_attribute_i8(const uint16_t attr_id, const int8_t value);
af_status_t aflib_set_attribute_i16(const uint16_t attr_id, const int16_t value);
af_status_t aflib_set_attribute_i32(const uint16_t attr_id, const int32_t value);
af_status_t aflib_set_attribute_i64(const uint16_t attr_id, const int64_t value);
af_status_t aflib_set_attribute_str(const uint16_t attr_id, const uint16_t value_len, const char *value);

/*
 * if you want to get notified when the hub's connection to the service goes
 * up/down, register this handler.
 */
void aflib_set_connect_handler(aflib_connect_handler_t handler);

/*
 * if set to true, ignore the return code from an `aflib_set_handler_t`.
 * instead, you must call `aflib_confirm_attr` to confim or reject a client
 * "set" request.
 */
void aflib_handle_set_async(bool async);
void aflib_confirm_attr(uint16_t attr_id, bool accepted);

/*
 * for debugging: set to one of LOG_DEBUG1, ... LOG_DEBUG4, or LOG_DEBUG_OFF (the default)
 */
void aflib_set_debug_level(int level);
