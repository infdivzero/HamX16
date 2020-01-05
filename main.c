//Bacon-16: Core

/*ToDo:
 * -implement configuration file loader
 * -implement device plugin dll loader
 * -the configuartion file can specify memory sizes and other emulator settings
 */

#include <Bacon16.h>
#include <Devices.h>
#include <ini.h>
#include <stdio.h>

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
unsigned short *dio;

void initA() {
	printf("%s\n", "initA");
}

void initB() {
	printf("%s\n", "initB");
}

int main(int argc, char *argv[]) {
//	char *a = "ab";
//	char *b = "cde";
//	char *c = NULL;
//	c = calloc(strlen(a) - 1 + strlen(b) - 1, 1);
//	for(unsigned int i = 0; i < strlen(a); i++) c[i] = a[i];
//	for(unsigned int i = 0; i < strlen(b); i++) c[i + strlen(a)] = b[i];
//	printf("%i\n", (int)strlen(a));
//	printf("%i\n", (int)strlen(b));
//	printf("%s\n", c);
//	free(c);

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

	initDevices((unsigned short*)dio, dioCount, cfg);
	ini_free(cfg);

	//Loop
	while(execute) {
		execInstr((unsigned short*)regs, (unsigned short*)dio, (unsigned char*)ram, (unsigned char*)rom, ramSize, romSize, &mem, &execute);
		updateDevices((unsigned short*)dio, (unsigned short*)regs, &execute);
	}

	//Free up memory
	free(ram);
	free(rom);
	free(regs);
	free(dio);

	unloadDevices();

	return 0;
}
