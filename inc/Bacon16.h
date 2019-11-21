//Bacon-16: Instruction set
#ifndef BACON16_H
#define BACON16_H

#include <stdio.h>
#include <time.h>

void execInstr(unsigned short *regs, unsigned short *dio, unsigned char *ram, unsigned char *rom, unsigned int ramSize, unsigned int romSize, int *mem, int *execute) {
	static unsigned char lmilli = 0;
	static unsigned char milli = 0;
	unsigned char mode, opcode, arg1, arg2, lByte, rByte;
	unsigned short imm;
	int jumped = 0;

	//Reset constants
	regs[5] = 1;

	lmilli = milli;
	milli = clock();
	if(milli > lmilli) regs[6]++;

	ram[ramSize - 2] = (0b10 << 6) | 20 << 1;
	ram[ramSize - 1] = 0;
	rom[romSize - 2] = (0b10 << 6) | 20 << 1;
	rom[romSize - 1] = 0;

	//Get isa components
	if(regs[0] < (*mem? (ramSize - 1) : (romSize - 1))) {
		lByte = *mem? ram[regs[0]] : rom[regs[0]];
		rByte = *mem? ram[regs[0] + 1] : rom[regs[0] + 1];
	} else {
		lByte = 0;
		rByte = 0;
	}

	mode = lByte >> 6;
	opcode = (lByte & 0b00111110) >> 1;
	arg1 = rByte >> 4;
	arg2 = rByte & 0b00001111;
	imm = ((lByte & 0b00000001) << 8) | rByte;

	//Execute opcode
	switch(opcode) {
		case 0x01: { //clr
			regs[arg1] = 0;
			break;
		}
		case 0x02: { //rst
			regs[0] = 0;
			break;
		}
		case 0x03: { //hlt
			*execute = 0;
			break;
		}
		case 0x04: { //mem
			*mem = !(*mem);
			regs[0] = 0;
			break;
		}
		case 0x05: { //mov/m
			if(mode == 0) {
				regs[arg2] 	  = regs[arg1];
			} else if(regs[arg2] < ramSize - 1) {
				ram[regs[arg2]] = rom[regs[arg1]];
				ram[regs[arg2] + 1] = rom[regs[arg1] + 1];
			}
			break;
		}
		case 0x06: { //ldd
			regs[arg2] = *mem? ((regs[arg1] >= ramSize - 1)? 0 : (ram[regs[arg1]] << 8) | ram[regs[arg1] + 1]) : ((regs[arg1] >= romSize - 1)? 0 : (rom[regs[arg1]] << 8) | rom[regs[arg1] + 1]);
			break;
		}
		case 0x07: { //svd
			if(mem && regs[arg2] < ramSize - 1) {
				ram[regs[arg2]] = regs[arg1] >> 8;
				ram[regs[arg2] + 1] = regs[arg1] & 0xFF;
			}
			break;
		}
		case 0x08: { //dat
			regs[0] += 2;
			regs[arg1] = *mem? ((ram[regs[0]] << 8) | ram[regs[0] + 1]) : ((rom[regs[0]] << 8) | rom[regs[0] + 1]);
			break;
		}
		case 0x09: { //c/add
			regs[1] = (mode >> 1)? (regs[2] + imm) : (regs[arg1] + regs[arg2]);
			if(mode & 0b01) regs[1] = ~regs[1] + 1;
			break;
		}
		case 0x0A: { //c/sub
			regs[1] = (mode >> 1)? (regs[2] - imm) : (regs[arg1] - regs[arg2]);
			if(mode & 0b01) regs[1] = ~regs[1] + 1;
			break;
		}
		case 0x0B: { //c/mul
			regs[1] = (mode >> 1)? (regs[2] * imm) : (regs[arg1] * regs[arg2]);
			if(mode & 0b01) regs[1] = ~regs[1] + 1;
			break;
		}
		case 0x0C: { //c/div
			if(((mode >> 1)? imm : regs[arg2]) != 0) {
				regs[1] = (mode >> 1)? (regs[2] / imm) : (regs[arg1] / regs[arg2]);
			} else {
				regs[1] = 0;
			}
			if(mode & 0b01) regs[1] = ~regs[1] + 1;
			break;
		}
		case 0x0D: { //shl
			regs[1] = (mode >> 1)? (regs[2] << imm) : (regs[arg1] << regs[arg2]);
			regs[3] |= 0b0001000;
			break;
		}
		case 0x0E: { //shr
			regs[1] = (mode >> 1)? (regs[2] >> imm) : (regs[arg1] >> regs[arg2]);
			regs[3] |= 0b0010000;
			break;
		}
		case 0x0F: { //and
			regs[1] = (mode >> 1)? (regs[2] & imm) : (regs[arg1] & regs[arg2]);
			break;
		}
		case 0x10: { //or
			regs[1] = (mode >> 1)? (regs[2] | imm) : (regs[arg1] | regs[arg2]);
			break;
		}
		case 0x11: { //not
			regs[1] = (mode >> 1)? ~imm : ~regs[arg1];
			break;
		}
		case 0x12: { //xor
			regs[1] = (mode >> 1)? (regs[2] ^ imm) : (regs[arg1] ^ regs[arg2]);
			break;
		}
		case 0x13: { //cmp
			regs[3] = 0;
			if(mode >> 1) {
				if(regs[2] >  imm) regs[3] |= 0b0000001;
				if(regs[2] == imm) regs[3] |= 0b0000010;
				if(regs[2] == 0)   regs[3] |= 0b0000100;
				if(regs[2] >> 15)  regs[3] |= 0b0100000;
			} else {
				if(regs[arg1] >  regs[arg2]) regs[3] |= 0b0000001;
				if(regs[arg1] == regs[arg2]) regs[3] |= 0b0000010;
				if(regs[arg1] == 0)			 regs[3] |= 0b0000100;
				if(regs[arg1] >> 15)		 regs[3] |= 0b0100000;
			}
			break;
		}
		case 0x14: { //jmp
			regs[0] = (mode >> 1)? imm : regs[arg1];
			jumped = 1;
			break;
		}
		case 0x15: { //jal
			if((mode & 0b01)? !(regs[3] & 0b0000001) : (regs[3] & 0b0000001)) {
				regs[0] = (mode >> 1)? imm : regs[arg1];
				jumped = 1;
			}
			break;
		}
		case 0x16: { //jeq
			if((mode & 0b01)? !(regs[3] & 0b0000010) : (regs[3] & 0b0000010)) {
				regs[0] = (mode >> 1)? imm : regs[arg1];
				jumped = 1;
			}
			break;
		}
		case 0x17: { //jze
			if((mode & 0b01)? !(regs[3] & 0b0000100) : (regs[3] & 0b0000100)) {
				regs[0] = (mode >> 1)? imm : regs[arg1];
				jumped = 1;
			}
			break;
		}
		case 0x18: { //jof
			if((mode & 0b01)? !(regs[3] & 0b0001000) : (regs[3] & 0b0001000)) {
				regs[0] = (mode >> 1)? imm : regs[arg1];
				jumped = 1;
			}
			break;
		}
		case 0x19: { //juf
			if((mode & 0b01)? !(regs[3] & 0b0010000) : (regs[3] & 0b0010000)) {
				regs[0] = (mode >> 1)? imm : regs[arg1];
				jumped = 1;
			}
			break;
		}
		case 0x1A: { //jng
			if((mode & 0b01)? !(regs[3] & 0b0100000) : (regs[3] & 0b0100000)) {
				regs[0] = (mode >> 1)? imm : regs[arg1];
				jumped = 1;
			}
			break;
		}
		case 0x1B: { //jin
			if((mode & 0b01)? !(regs[3] & 0b1000000) : (regs[3] & 0b1000000)) {
				regs[0] = (mode >> 1)? imm : regs[arg1];
				jumped = 1;
			}
			break;
		}
		case 0x1C: { //sdr
			dio[regs[arg2]] = regs[arg1];
			break;
		}
		case 0x1D: { //gdr
			regs[arg2] = dio[regs[arg1]];
			break;
		}
		case 0x1E: { //deb
			printf("%i\n", regs[arg1]);
			//printf("%i %i\n", ram[2], ram[3]);
			fflush(stdout);
			break;
		}
	}

	if((regs[0] <= (*mem? (ramSize - 2) : (romSize - 2))) && !jumped) regs[0] += 2;
}

#endif
