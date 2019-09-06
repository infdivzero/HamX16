//Bacon-16: Device manager
#ifndef DEVICES_H
#define DEVICES_H

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
 * 	-similar to the harddisk, but a binary file can be dynamically loaded from a folder when the user inputs a specific key combo
 * dio hub
 * 	-is responsible for "managing" the other devices and contains a device interrupt queue which lists the device IDs which sent an interrupt signal
 */

//Bit layout for dio: 00 000000 00000000 - select and interrupt signals, instruction, and data. However, devices can use this hwoever they are written to

void initDevices(unsigned short *dio) {
	//Some dll loading code, or maybe lua instead
}

void updateDevices(unsigned short *dio, unsigned short *flgReg) {
	//
}

#endif
