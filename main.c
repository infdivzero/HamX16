//Bacon-16: Core

/*ToDo:
 * -implement CPU and device timing
 * -this will allow the devices to "run slower" than the CPU and will allow for configurable clock speeds
 * -certain functions such as the millisecond timer may need to be updated separately for better accuracy
 */

#include <Bacon16.h>
#include <Devices.h>
#include <ini.h>
#include <time.h>
#include <stdio.h>

char *ver = "v1.12.68(a)";

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
unsigned int *dio;

unsigned char lmilli = 0;
unsigned char milli = 0;

int main(int argc, char *argv[]) {
	//Load config
	ini_t *cfg = ini_load("cfg.ini");
	ini_sget(cfg, "emulator", "ramSize", "%i", &ramSize);
	ini_sget(cfg, "emulator", "romSize", "%i", &romSize);
	ini_sget(cfg, "emulator", "deviceBuffers", "%i", &dioCount);

	//Allocate memory
	ram  = calloc(ramSize, 1);
	rom  = calloc(romSize, 1);
	regs = calloc(regCount, 2);
	dio  = calloc(dioCount, 2);

	//Load rom
	FILE *data = fopen("rom.bin", "rb");
	fseek(data, 0, SEEK_END);
	unsigned int dataSize = ftell(data);
	fseek(data, 0, SEEK_SET);
	fread(rom, dataSize, 1, data);
	fclose(data);

	initDevices(dio, dioCount, cfg);
	ini_free(cfg);

	//Loop
	while(execute) {
		lmilli = milli;
		milli = clock();
		if(milli - lmilli > 0) regs[6] += milli - lmilli; //tim register

		execInstr(regs, dio, ram, rom, ramSize, romSize, &mem, &execute);
		updateDevices(dio, &regs[3], &regs[4], &execute);
	}

	//Free up memory
	free(ram);
	free(rom);
	free(regs);
	free(dio);

	unloadDevices();

	return 0;
}
