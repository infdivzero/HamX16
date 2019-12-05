//Bacon-16: Core
#include <Bacon16.h>
#include <Devices.h>

#include <stdio.h>
#include <pthread.h>


int execute = 1;
int mem = 0;

//keep the memory sizes as multiples of 2 else the emulator will crash if pc + 1 > memSize. The last two bytes of each memory array contains a hard coded jump instruction to pc = 0
unsigned int ramSize = 2048;
unsigned int romSize = 1024;
unsigned int regCount = 16;
unsigned int dioCount = 16;

unsigned char ram[2048];
unsigned char rom[1024];
unsigned short regs[16];
unsigned short dio[16];

pthread_t cpuThread;//, deviceThread;

void *cpuUpdateWrapper() {
	execInstr((unsigned short*)regs, (unsigned short*)dio, (unsigned char*)ram, (unsigned char*)rom, ramSize, romSize, &mem, &execute);
	return NULL;
}

int main(int argc, char *argv[]) {
	//Initialize
	initDevices((unsigned short*)dio);

	//These are temporary?
	for(unsigned int i = 0; i < romSize; i++) rom[i] = 0;
	for(unsigned int i = 0; i < ramSize; i++) ram[i] = 0;
	for(unsigned int i = 0; i < regCount; i++) regs[i] = 0;

	//Load rom
	FILE *data = fopen("rom.bin", "rb");
	fseek(data, 0, SEEK_END);
	unsigned int dataSize = ftell(data);
	fseek(data, 0, SEEK_SET);
	fread(rom, dataSize, 1, data);
	fclose(data);

	//pthread_create(&cpuThread, NULL, &cpuUpdateWrapper, NULL);
	//char buffer[8192];

	//setvbuf(stdout, buffer, _IOFBF, sizeof(buffer));
	//Loop
	while(execute) {
		execInstr((unsigned short*)regs, (unsigned short*)dio, (unsigned char*)ram, (unsigned char*)rom, ramSize, romSize, &mem, &execute);
		updateDevices((unsigned short*)dio, &regs[3], &execute);
		//pthread_join(cpuThread, NULL);
	}
	
	//pthread_exit(NULL);
	end();

	return 0;
}
