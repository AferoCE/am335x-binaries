/*
 * aflib_hub.h
 *
 * hub version of aflib
 *
 * Copyright (C) 2018 Afero, Inc. All rights reserved.
 */

#include <stdint.h>
#include <stdbool.h>
#include <event2/event.h>
#include <af_log.h>

#include "aflib_mcu.h"

af_lib_error_t af_lib_set_event_base(struct event_base *ev);

/*
 * af_lib_set_debug_level
 *
 * sets the debug level of the AfLib library, which specifies how much
 * debugging information is logged to syslog. Set to LOG_INFO to disable
 * debug logging. The debug levels range from LOG_DEBUG1 to LOG_DEBUG4
 * with LOG_DEBUG4 being the most verbose.
 *
 * @param level                     - log verboseness level
 *
 */
void af_lib_set_debug_level(int level);

/*
 * af_lib_destroy
 *
 * Gracefully shuts down the AfLib library
 *
 * @param af_lib                    - library to close
 *
 */

void af_lib_destroy(af_lib_t *af_lib);


/*********************** deprecated compatibility API ****************************/

typedef af_lib_error_t af_status_t;

/* API will not accept attribute values larger than this */
#define MAX_ATTRIBUTE_SIZE                  255

/*
 * a remote client is requesting that an attribute be changed.
 * return `true` to accept the change, or `false` to reject it.
 */
typedef bool (*aflib_set_handler_t)(const uint16_t attr_id, const uint16_t value_len, const uint8_t *value);

/*
 * notification of an attribute's current value, either because it has
 * changed internally, or because you asked for the current value with
 * `aflib_get_attribute`.
 */
typedef void (*aflib_notify_handler_t)(const uint16_t attr_id, const uint16_t value_len, const uint8_t *value);

/*
 * start aflib and register callbacks.
 */
af_status_t aflib_init(struct event_base *ev, aflib_set_handler_t set_handler, aflib_notify_handler_t notify_handler);

/*
 * request the current value of an attribute. the result is sent via the
 * aflib_notify_handler_t callback.
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
 * closes the AfLib library
 */
void aflib_shutdown(void);

#define aflib_set_debug_level af_lib_set_debug_level
