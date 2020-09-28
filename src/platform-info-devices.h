/*
 * Copyright (C) 2016, 2018, 2020 "IoT.bzh"
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef PLATFORM_INFO_DEVICE_H
#define PLATFORM_INFO_DEVICE_H

#include <afb/afb-binding.h>
#include "json-c/json.h"
#include <pthread.h>
#include <libudev.h>
#include <unistd.h>

#define PINFO_OK               (0)
#define PINFO_ERR              (-1)

typedef struct {
    json_object* info;
    int client_count;
}pinfo_api_ctx_t;

typedef struct {
	struct udev *udev_ctx;
    struct udev_monitor *umon_hndl;
    json_object *filter;
    json_object *mask;
    pinfo_api_ctx_t *api_ctx;
    void(*umon_cb)(struct pinfo_client_ctx_t* ctx, json_object* jdevice);
    pthread_t th;
    void* event_device_changed;
}pinfo_client_ctx_t;


int                 pinfo_device_monitor(afb_req_t req);
json_object*        pinfo_device_scan(json_object *jfilter, json_object* jmask);

static void*        pinfo_device_client_new(void* req_ctx);
static void         pinfo_device_client_free(void* client_ctx);
static void*        pinfo_device_monitor_loop(pinfo_client_ctx_t* ctx);
static void         pinfo_device_monitor_detect(pinfo_client_ctx_t* ctx, json_object* jdevice);
static json_object* pinfo_device_udevice_to_jdevice(struct udev_device* udevice, json_object* jmask);
static void         pinfo_device_jdev_destructor(json_object* jdevice, void* udevice);
int                 pinfo_device_filter_monitoring(pinfo_client_ctx_t* ctx);
static void         pinfo_device_filter_scan(struct udev_enumerate* udev_enum, json_object* jfilter);

static json_object* pinfo_device_udevice_to_jlist(
                                    struct udev_device* udevice,
                                    struct udev_list_entry*(*udevice_elist)(struct udev_device*),
                                    const char*(*elist_val_get)(struct udev_list_entry*,const char*),
                                    const unsigned int jflags);
static json_object* pinfo_device_udevice_to_jlist_mask(
                                    struct udev_device* udevice,
                                    const char*(*udevice_val_getter)(struct udev_device*,const char*),
                                    json_object* jkeys,
                                    unsigned jcpy_flags);
#endif