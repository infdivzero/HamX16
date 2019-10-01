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

//Bit layout for dio: 00 000000 00000000 - select and interrupt signals, instruction, and data. However, devices can use this however they are written to

//https://stackoverflow.com/questions/10110905/simple-sound-wave-generator-with-sdl-in-c
//https://stackoverflow.com/questions/20753726/rendering-pixels-from-array-of-rgb-values-in-sdl-1-2

SDL_Window *win;
SDL_Renderer *ren;
SDL_Event ev;

const unsigned int resW = 400, resH = 240;

unsigned int select = 0; //Selects which device has been called. ID assignment is hard coded for now

//Device data
typedef struct { //The gpu will be monochrome until a better version can be written
	//The vram is divided into 2 render buffers and 8kb for other things
	//framebuffer: w * h / 8 (size of display, with each bit representing a pixel)
	//fonts: char count * 8 (each char is 8x8 requiring 8 bytes)
	unsigned char vram[32000]; //total resolution area * framebuffer count. bufferAccess = ((buffer + 1) * (y * resW + x)). 32 kilobytes
	unsigned char regs[16];
	unsigned short display[400 * 240];
	//SDL_Surface *buffer; //Because the display will be monochrome, a buffer texture won't be needed as a pixel array will do
	/*Registers:
	 * -pc
	 * -acc
	 * -tmp
	 * -buf: selects the current render buffer
	 * -col: stores the render color
	 * -typ: constant register which stores the device type ID
	 * -stp: stream pointer, incremented each stream cycle (work backwards or forwards in memory streaming?)
	 */
} gpu;

//Device initialization funtions
void init_gpu() {
	//
}

//Device update funtions
void update_gpu() {
	if(select == 0) {
		//Process instructions
		//When in stream mode, the gpu copies ... to the stm register
		//Instructions are received as a 16 bit register
		//	-mov   arg1, arg2			  - moves register 1 to register 2
		//	-getb0 vram[arg1], arg2 	  - moves an address in the portion of vram used by buffer 0 to arg2
		//	-getb1 vram[arg1], arg2 	  - moves an address in the portion of vram used by buffer 1 to arg2
		//	-getf  vram[arg1], arg2 	  - moves an address in the portion of vram used by the font page to arg2
		//	-setb0 arg1, vram[arg2] 	  - moves arg1 to an address in the portion of vram used by buffer 0
		//	-setb1 arg1, vram[arg2] 	  - moves arg1 to an address in the portion of vram used by buffer 1
		//	-setf  arg1, vram[arg2]		  - moves arg1 to an address in the portion of vram used by the font page
		//	-stm   arg1					  - sets the gpu to data stream mode. The first bit of the register specifies if the stream is to receive data or send it back to the cpu, while the remaining bits select the memory location to start streaming at
		//	-char  arg1, arg2			  - renders the character with ascii code arg1 at vram address arg2 (uses the current buffer)
	}
	//render output
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
