//Bacon-16: Core
#include <Bacon16.h>
#include <Devices.h>

#include <stdio.h>

int main() {
	//Vars - keep the memory sizes as multiples of 2 else the emulator will crash if pc + 1 > memSize. The last two bytes of each memory array contains a hard coded jump instruction to pc = 0
	unsigned int ramSize = 2048;
	unsigned int romSize = 512;
	unsigned int regCount = 16;
	unsigned int dioCount = 32;

	unsigned char ram[ramSize];
	unsigned char rom[romSize];
	unsigned short regs[regCount];
	unsigned short dio[dioCount];

	int execute = 1;
	int mem = 0;

	//Initialize
	initDevices((unsigned short*)dio);

	for(unsigned int i = 0; i < romSize; i++) {
		rom[i] = 0;
	}

	for(unsigned int i = 0; i < ramSize; i++) {
		ram[i] = 0;
	}

	for(unsigned int i = 0; i < regCount; i++) {
		regs[i] = 0;
	}

	//Load rom
	FILE *data = fopen("rom.bin", "rb");
	fseek(data, 0, SEEK_END);
	unsigned int dataSize = ftell(data);
	fseek(data, 0, SEEK_SET);
	fread(rom, dataSize, 1, data);

	rom[2] = 4 << 1;
	//rom[2] = 3 << 1;

	//Loop
	while(execute) {
		regs[0] += 2;
		printf("%i\n", regs[0]);
		execInstr((unsigned short*)regs, (unsigned short*)dio, (unsigned char*)ram, (unsigned char*)rom, ramSize, romSize, &mem, &execute);
		updateDevices((unsigned short*)dio, &regs[3]);
		if(regs[0] > mem? (ramSize - 1) : (romSize - 1)) regs[0] = mem? (ramSize - 1): (romSize - 1); //Fix and add safety measure
	}

	return 0;
}
