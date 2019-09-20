//Bacon-16: Device manager
#ifndef DEVICES_H
#define DEVICES_H

#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

/*Devices to emulate:
 * buzzer
 * 	-simple audio output, generates single tone beeps n amount of times when it receives a call state
 * gpu
 * 	-implemented in sdl2
 * 	-rendering is provided by direct pixel manipulation or providing an ascii value to display a char. Cursor position can be sent to the gpu as well
 * keyboard
 * 	-simply stores physical input in a char register and sets the interrupt flag high
 * harddisk
 * 	-should be as close to a real hard drive as possible, else can be an emulated ssd. Data is stored in a raw binary file using the emulator's os's file system
 * external drive (like a floppy drive or cd rom)
 * 	-similar to the harddisk, but a binary file can be dynamically loaded from a folder when the user inputs a specific key combo which mounts and unmounts the file
 * dio hub
 * 	-is responsible for "managing" the other devices and contains a device interrupt queue which lists the device IDs which sent an interrupt signal. Functions like a north bridge/south bridge
 */

//Bit layout for dio: 00 000000 00000000 - select and interrupt signals, instruction, and data. However, devices can use this hwoever they are written to

//https://stackoverflow.com/questions/10110905/simple-sound-wave-generator-with-sdl-in-c

SDL_Window *win;
SDL_Renderer *ren;
SDL_Event ev;

unsigned int resW = 800, resH = 600, resA = resW * resH;

//Device data
typedef struct {
	unsigned char vram[4 * resA];
	unsigned char vrom[512];
} gpuData;

//Device initialization funtions
void init_gpu() {
	//load video BIOS
	//gpu uses instruction set similar to cpu
}

//Device update funtions
void update_gpu() {
	//Process instructions
	//Output graphics
	for(unsigned int y = 0; y < resH; y++) {
		for(unsigned int x = 0; x < resW; x++) {
			//get rgba from vram, y * width + x
			//draw pixel
		}
	}
}

//DIO functions
void initDevices(unsigned short *dio) {
	//Some dll loading code, or maybe lua instead. Will be hard coded for now
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	win = SDL_CreateWindow("HamX16", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
}

void updateDevices(unsigned short *dio, unsigned short *flgReg, int *execute) {
	SDL_PollEvent(&ev);
	switch(ev.type) {
		case SDL_QUIT: {
			*execute = 0;
			break;
		}
	}

	SDL_RenderClear(ren);
	
	update_gpu();
	
	SDL_RenderPresent(ren);
}

void end() {
	SDL_DestroyWindow(win);
	SDL_DestroyRenderer(ren);
	SDL_Quit();
}

#endif
