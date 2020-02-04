//Bacon-16: Device manager
#ifndef DEVICES_H
#define DEVICES_H

//ToDo: implement external lib loader for Linux

#include <ini.h>
#include <windows.h>
#include <math.h>

typedef void (*init_t)(ini_t*, unsigned int);
typedef void (*update_t)(unsigned short*, unsigned char*, int*);
typedef void (*unload_t)();

init_t *inits;
update_t *updates;
unload_t *unloads;
HINSTANCE *devices;

unsigned int deviceCount = 0;
unsigned int initCount = 0;
unsigned int updateCount = 0;
unsigned int unloadCount = 0;
unsigned int nextDIO = 0; //perhaps replace this small system and the current HINSTANCE handling with a struct containing HINSTANCE and its assigned dio port?

//DIO functions
void initDevices(unsigned short *dio, unsigned char **deviceCalls, unsigned int dioCount, ini_t *cfg) {
	devices = calloc(0, sizeof(HINSTANCE));
	inits = calloc(1, sizeof(init_t));
	updates = calloc(1, sizeof(update_t));
	unloads = calloc(1, sizeof(unload_t));

	for(unsigned int i = 0; i < dioCount; i++) {
		char *module;
		char device[5];
		sprintf(device, "%i", i);
		module = (char*)ini_get(cfg, "devices", device);
		if(module) {
			deviceCount++;
			devices = realloc(devices, deviceCount);
			devices[deviceCount - 1] = (HINSTANCE)LoadLibrary(module);

			if(devices[deviceCount - 1]) {
				initCount++;
				inits = realloc(inits, initCount);
				inits[initCount - 1] = (init_t)GetProcAddress(devices[deviceCount - 1], "init");
				updateCount++;
				updates = realloc(updates, updateCount);
				updates[updateCount - 1] = (update_t)GetProcAddress(devices[deviceCount - 1], "update");
				unloadCount++;
				unloads = realloc(unloads, unloadCount);
				unloads[unloadCount - 1] = (unload_t)GetProcAddress(devices[deviceCount - 1], "unload");
			}
		}
	}

	for(unsigned int i = 0; i < initCount && inits[i]; i++) {
		if(nextDIO + 1 < dioCount) inits[i](cfg, nextDIO);
		nextDIO++;
	}

	*deviceCalls = calloc(1, 1);
}

void updateDevices(unsigned short *dio, unsigned char *deviceCalls, unsigned short *flgReg, unsigned short *intReg, int *execute) {
	//implement a means to signal a device that its dio register has been modified by the CPU

	for(unsigned int i = 0; i < updateCount && updates[i]; i++) updates[i](dio, deviceCalls, execute);

	//Clear interrupt bits
	*flgReg &= 0b1111111101111111;
	*intReg  = 0;
}

void unloadDevices(unsigned char *deviceCalls) {
	for(unsigned int i = 0; i < unloadCount && unloads[i]; i++) unloads[i]();
	for(unsigned int i = 0; i < deviceCount; i++) FreeLibrary(devices[i]);
	free(devices);
	free(inits);
	free(updates);
	free(deviceCalls);
}

#endif
