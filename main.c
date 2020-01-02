//Bacon-16: Core

/*ToDo:
 * -implement configuration file loader
 * -implement device plugin dll loader
 * -the configuartion file can specify memory sizes and other emulator settings
 */

#include <Bacon16.h>
#include <Devices.h>

#include <stdio.h>
#include <pthread.h>

int execute = 1;
int mem = 0;
unsigned short *flgReg;

//keep the memory sizes as multiples of 2 else the emulator will crash if pc + 1 > memSize. The last two bytes of each memory array contains a hard coded jump instruction to pc = 0 to prevent access violations
unsigned int ramSize = 2048;
unsigned int romSize = 1024;
unsigned int regCount = 16;
unsigned int dioCount = 16;

unsigned char *ram;
unsigned char *rom;
unsigned short *regs;
unsigned short *dio[16];

int main(int argc, char *argv[]) {
	initDevices((unsigned short*)dio);

	//Load config

	//Allocate memory
	ram = calloc(ramSize, 1);
	rom = calloc(romSize, 1);
	regs = calloc(regCount, 1);

	//Load rom
	FILE *data = fopen("rom.bin", "rb");
	fseek(data, 0, SEEK_END);
	unsigned int dataSize = ftell(data);
	fseek(data, 0, SEEK_SET);
	fread(rom, dataSize, 1, data);
	fclose(data);

	//Loop
	while(execute) {
		execInstr((unsigned short*)regs, (unsigned short*)dio, (unsigned char*)ram, (unsigned char*)rom, ramSize, romSize, &mem, &execute);
		updateDevices((unsigned short*)dio, (unsigned short*)regs, &execute);
	}
	
	free(rom);
	end();

	return 0;
}
