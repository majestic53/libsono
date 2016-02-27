/**
 * libsono
 * Copyright (C) 2016 David Jolly
 * ----------------------
 *
 * libsono is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libsono is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/lib/sono.h"

#define DEVICE_DISCOVERY_TIMEOUT 1 // sec
#define SERVICE_ACTION_TIMEOUT 2 // sec
#define SERVICE_ACTION_PROPERTIES_LED_OFF "Off"
#define SERVICE_ACTION_PROPERTIES_LED_ON "On"
#define SERVICE_ACTION_PROPERTIES_LED_STATE_CURRENT "CurrentLEDState"
#define SERVICE_ACTION_PROPERTIES_LED_STATE_DESIRE "DesiredLEDState"
#define SERVICE_ACTION_PROPERTIES_LED_STATE_GET "GetLEDState"
#define SERVICE_ACTION_PROPERTIES_LED_STATE_SET "SetLEDState"
#define SERVICE_DEVICE_PROPERTIES "DeviceProperties"
#define SERVICE_DISCOVERY_TIMEOUT 2 // sec

void 
event_handler(
	/*__in*/ uint32_t id,
	/*__in*/ const char *svc,
	/*__in*/ const char *act,
	/*__in*/ const char *data
	)
{
	uint32_t length;

	length = strlen(data);
	fprintf(stdout, "{0x%x} -- SVC. %s, ACT. %s, DATA[%lu]", id, svc, act, length);

	if(length) {
		fprintf(stdout, "\n%s", data);
	}

	fprintf(stdout, "\n");
} 

int 
main(
	/*__in*/ int argc,
	/*__in*/ const char **argv
	)
{
	sono_dev *dev_lst = NULL;
	sono_dev_arg in[1], out[1];
	sono_err_t result = SONO_ERR_NONE;
	uint8_t dev_count = 0, dev_iter = 0, in_cnt, out_cnt, out_iter;

	fprintf(stdout, "sonoctl %s\n", sono_ver());

	result = sono_init(event_handler);
	if(!SONO_SUCCESS(result)) {
		fprintf(stderr, "sono_init failed, status: 0x%x\n", result);
		goto exit;
	}

	result = sono_dev_disc(&dev_count, DEVICE_DISCOVERY_TIMEOUT, SERVICE_DISCOVERY_TIMEOUT);
	if(!SONO_SUCCESS(result)) {
		fprintf(stderr, "sono_dev_disc failed, status: 0x%x\n", result);
		goto exit_uninit;
	}

	fprintf(stdout, "Found %lu device(s).\n---\n", (uint32_t) dev_count);

	dev_lst = (sono_dev *) malloc(sizeof(sono_dev) * dev_count);
	if(!dev_lst) {
		fprintf(stderr, "Failed to allocate device list!\n");
		result = SONO_ERR_FAILED;
		goto exit_uninit;
	}

	result = sono_dev_list(dev_lst, &dev_count);
	if(!SONO_SUCCESS(result)) {
		fprintf(stderr, "sono_dev_list failed, status: 0x%x\n", result);
		goto exit_uninit;
	}

	fprintf(stdout, "Listing %lu device(s).\n---\n", (uint32_t) dev_count);

	for(; dev_iter < dev_count; ++dev_iter) {
		fprintf(stdout, "\n--- [%lu] {0x%x} %s:%u", (uint32_t) dev_iter, dev_lst[dev_iter].id,
			dev_lst[dev_iter].addr, dev_lst[dev_iter].port);
	}

	// LED OFF
	fprintf(stdout, "\n\n--- [ACT] %s\n", SERVICE_ACTION_PROPERTIES_LED_STATE_SET);
	in_cnt = 1;
	memset(&in[0], 0, sizeof(sono_dev_arg));
	memcpy(in[0].name, SERVICE_ACTION_PROPERTIES_LED_STATE_DESIRE, 
		strlen(SERVICE_ACTION_PROPERTIES_LED_STATE_DESIRE));
	memcpy(in[0].val, SERVICE_ACTION_PROPERTIES_LED_OFF, 
		strlen(SERVICE_ACTION_PROPERTIES_LED_OFF));
	out_cnt = 0;

	result = sono_dev_act(&dev_lst[0], SERVICE_DEVICE_PROPERTIES, SERVICE_ACTION_PROPERTIES_LED_STATE_SET,
		in, in_cnt, NULL, &out_cnt, SERVICE_ACTION_TIMEOUT);
	if(!SONO_SUCCESS(result)) {
		fprintf(stderr, "sono_dev_act failed, status: 0x%x\n", result);
		goto exit_uninit;
	}

	fprintf(stdout, "--- [ACT] %s", SERVICE_ACTION_PROPERTIES_LED_STATE_GET);
	out_cnt = 1;
	memset(&out[0], 0, sizeof(sono_dev_arg));

	result = sono_dev_act(&dev_lst[0], SERVICE_DEVICE_PROPERTIES, SERVICE_ACTION_PROPERTIES_LED_STATE_GET,
		NULL, 0, out, &out_cnt, SERVICE_ACTION_TIMEOUT);
	if(!SONO_SUCCESS(result)) {
		fprintf(stderr, "sono_dev_act failed, status: 0x%x\n", result);
		goto exit_uninit;
	}

	for(out_iter = 0; out_iter < out_cnt; ++out_iter) {
		fprintf(stdout, "\n---- [OUT] %s=%s", out[out_iter].name, out[out_iter].val);
	}

	getchar();

	// LED ON
	fprintf(stdout, "--- [ACT] %s\n", SERVICE_ACTION_PROPERTIES_LED_STATE_SET);
	in_cnt = 1;
	memset(&in[0], 0, sizeof(sono_dev_arg));
	memcpy(in[0].name, SERVICE_ACTION_PROPERTIES_LED_STATE_DESIRE, 
		strlen(SERVICE_ACTION_PROPERTIES_LED_STATE_DESIRE));
	memcpy(in[0].val, SERVICE_ACTION_PROPERTIES_LED_ON, 
		strlen(SERVICE_ACTION_PROPERTIES_LED_ON));
	out_cnt = 0;

	result = sono_dev_act(&dev_lst[0], SERVICE_DEVICE_PROPERTIES, SERVICE_ACTION_PROPERTIES_LED_STATE_SET,
		in, in_cnt, NULL, &out_cnt, SERVICE_ACTION_TIMEOUT);
	if(!SONO_SUCCESS(result)) {
		fprintf(stderr, "sono_dev_act failed, status: 0x%x\n", result);
		goto exit_uninit;
	}

	fprintf(stdout, "--- [ACT] %s", SERVICE_ACTION_PROPERTIES_LED_STATE_GET);
	out_cnt = 1;
	memset(&out[0], 0, sizeof(sono_dev_arg));

	result = sono_dev_act(&dev_lst[0], SERVICE_DEVICE_PROPERTIES, SERVICE_ACTION_PROPERTIES_LED_STATE_GET,
		NULL, 0, out, &out_cnt, SERVICE_ACTION_TIMEOUT);
	if(!SONO_SUCCESS(result)) {
		fprintf(stderr, "sono_dev_act failed, status: 0x%x\n", result);
		goto exit_uninit;
	}

	for(out_iter = 0; out_iter < out_cnt; ++out_iter) {
		fprintf(stdout, "\n---- [OUT] %s=%s", out[out_iter].name, out[out_iter].val);
	}

	fprintf(stdout, "\n");

exit_uninit:

	if(!SONO_SUCCESS(result)) {
		fprintf(stderr, "Error: %s\n", sono_err());
	}

	result = sono_uninit();
	if(!SONO_SUCCESS(result)) {
		fprintf(stderr, "sono_uninit failed, status: 0x%x\n", result);
		goto exit;
	}

	if(dev_lst) {
		free(dev_lst);
		dev_lst = NULL;
	}

exit:
	return result;
}
