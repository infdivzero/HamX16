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

const int AMPLITUDE = 28000;
const int SAMPLE_RATE = 44100;

void audio_callback(void *user_data, Uint8 *raw_buffer, int bytes) {
    Sint16 *buffer = (Sint16*)raw_buffer;
    int length = bytes / 2; // 2 bytes per sample for AUDIO_S16SYS
    int &sample_nr = ((int*)user_data);

    for(int i = 0; i < length; i++, sample_nr++)
    {
        double time = (double)sample_nr / (double)SAMPLE_RATE;
        buffer[i] = (Sint16)(AMPLITUDE * sin(2.0f * M_PI * 441.0f * time)); // render 441 HZ sine wave
    }
}

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
	SDL_RenderPresent(ren);
}

void end() {
	SDL_DestroyWindow(win);
	SDL_DestroyRenderer(ren);
	SDL_Quit();
}

#endif
