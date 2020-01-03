#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>

std::string ver = "v0.0.0(a)"; //v#rewrites.#majorImplementations.#patches(alpha/beta/release)

int main(int argc, char *argv[]) {
	std::cout << "Bacon-16 Disassembler " + ver << std::endl;

	std::vector<unsigned char> machineCode;

	std::string inFilePath, outFilePath, output;
	std::unordered_map<unsigned char, std::string> regs = {
			{0x0, "pc"},
			{0x1, "acc"},
			{0x2, "tmp"},
			{0x3, "flg"},
			{0x4, "int"},
			{0x5, "one"},
			{0x6, "tim"},
			{0x7, "arg1"},
			{0x8, "arg2"},
			{0x9, "ret"},
			{0xA, "retd"},
			{0xB, "g1"},
			{0xC, "g2"},
			{0xD, "g3"},
			{0xE, "g4"},
			{0xF, "g5"}
	};

	bool disassemble = true;

	//Get arguments
	if(argc >= 3) { //add support for default output
		inFilePath = argv[1];
		outFilePath = argv[2];
	} else {
		std::cout << "Invalid command line arguments provided" << std::endl;
		disassemble = false;
	}

	//Load binary
	if(disassemble) {
		std::ifstream iFile(inFilePath, std::ios::binary);
		if(iFile) {
			std::stringstream buf;
			std::string raw;

			buf << iFile.rdbuf();
			raw = buf.str();

			for(unsigned int i = 0; i < raw.size(); i++) machineCode.push_back(raw[i]);
		} else disassemble = false;

		iFile.close();
	}

	//the main disassembly code could be a reused version of the instruction decoder code used in the emulator
	for(unsigned int i = 0; i < machineCode.size() && disassemble; i += 2) {
		unsigned char mode, opcode, arg1, arg2, lByte, rByte;
		unsigned short imm;

		//Get isa components
		lByte = machineCode[i];
		rByte = machineCode[i + 1];

		mode = lByte >> 6;
		opcode = (lByte & 0b00111110) >> 1;
		arg1 = rByte >> 4;
		arg2 = rByte & 0b00001111;
		imm = ((lByte & 0b00000001) << 8) | rByte;

		//output += std::to_string(i) + ": ";

		switch(opcode) { //might simplify this later
			case 0x00: { //nop
				output += "nop";
				if(arg1 + arg2 > 0) output += " " + (std::string)regs[arg1] + ", " + regs[arg2];
				break;
			}
			case 0x01: { //clr
				output += "clr " + ((mode >> 1)? std::to_string(imm) : regs[arg1]);
				break;
			}
			case 0x02: { //rst
				output += "rst";
				break;
			}
			case 0x03: { //hlt
				output += "hlt";
				break;
			}
			case 0x04: { //mem
				output += "mem " + ((mode >> 1)? std::to_string(imm) : regs[arg1]);
				break;
			}
			case 0x05: { //mov/m
				output += (mode & 0b01? "movm " : "mov ") + regs[arg1] + ", " + regs[arg2];
				break;
			}
			case 0x06: { //ldd
				output += "ldd " + regs[arg1] + ", " + regs[arg2];
				break;
			}
			case 0x07: { //svd
				output += "svd " + regs[arg1] + ", " + regs[arg2];
				break;
			}
			case 0x08: { //dat
				output += "dat " + regs[arg1] + "; ";
				i += 2;
				std::stringstream tmp;
				tmp << std::hex << std::setw(4) << std::setfill('0') << ((machineCode[i] << 8) | machineCode[i + 1]);
				output += "0x" + tmp.str();
				break;
			}
			case 0x09: { //c/add
				output += ((mode & 0b01)? "cadd " : "add ") + ((mode >> 1)? std::to_string(imm) : regs[arg1] + ", " + regs[arg2]);
				break;
			}
			case 0x0A: { //c/sub
				output += ((mode & 0b01)? "csub " : "sub ") + ((mode >> 1)? std::to_string(imm) : regs[arg1] + ", " + regs[arg2]);
				break;
			}
			case 0x0B: { //c/mul
				output += ((mode & 0b01)? "cmul " : "mul ") + ((mode >> 1)? std::to_string(imm) : regs[arg1] + ", " + regs[arg2]);
				break;
			}
			case 0x0C: { //c/div
				output += ((mode & 0b01)? "cdiv " : "div ") + ((mode >> 1)? std::to_string(imm) : regs[arg1] + ", " + regs[arg2]);
				break;
			}
			case 0x0D: { //shl
				output += "shl " + ((mode >> 1)? std::to_string(imm) : regs[arg1] + ", " + regs[arg2]);
				break;
			}
			case 0x0E: { //shr
				output += "shl " + ((mode >> 1)? std::to_string(imm) : regs[arg1] + ", " + regs[arg2]);
				break;
			}
			case 0x0F: { //and
				output += "and " + ((mode >> 1)? std::to_string(imm) : regs[arg1] + ", " + regs[arg2]);
				break;
			}
			case 0x10: { //or
				output += "or " + ((mode >> 1)? std::to_string(imm) : regs[arg1] + ", " + regs[arg2]);
				break;
			}
			case 0x11: { //not
				output += "not " + ((mode >> 1)? std::to_string(imm) : regs[arg1]);
				break;
			}
			case 0x12: { //xor
				output += "xor " + ((mode >> 1)? std::to_string(imm) : regs[arg1] + ", " + regs[arg2]);
				break;
			}
			case 0x13: { //cmp
				output += "cmp " + ((mode >> 1)? std::to_string(imm) : regs[arg1] + ", " + regs[arg2]);
				break;
			}
			case 0x14: { //jmp
				output += "jmp " + ((mode >> 1)? std::to_string(imm) : regs[arg1]);
				break;
			}
			case 0x15: { //jal
				output += (mode & 0b01? "jnal " : "jal ") + ((mode >> 1)? std::to_string(imm) : regs[arg1]);
				break;
			}
			case 0x16: { //jeq
				output += (mode & 0b01? "jneq " : "jeq ") + ((mode >> 1)? std::to_string(imm) : regs[arg1]);
				break;
			}
			case 0x17: { //jze
				output += (mode & 0b01? "jnze " : "jze ") + ((mode >> 1)? std::to_string(imm) : regs[arg1]);
				break;
			}
			case 0x18: { //jof
				output += (mode & 0b01? "jnof " : "jof ") + ((mode >> 1)? std::to_string(imm) : regs[arg1]);
				break;
			}
			case 0x19: { //juf
				output += (mode & 0b01? "jnuf " : "juf ") + ((mode >> 1)? std::to_string(imm) : regs[arg1]);
				break;
			}
			case 0x1A: { //jng
				output += (mode & 0b01? "jnng " : "jng ") + ((mode >> 1)? std::to_string(imm) : regs[arg1]);
				break;
			}
			case 0x1B: { //jin
				output += (mode & 0b01? "jnin " : "jin ") + ((mode >> 1)? std::to_string(imm) : regs[arg1]);
				break;
			}
			case 0x1C: { //sdr
				output += "sdr " + regs[arg1] + ", " + regs[arg2];
				break;
			}
			case 0x1D: { //gdr
				output += "gdr " + regs[arg1] + ", " + regs[arg2];
				break;
			}
			case 0x1E: { //deb
				output += "deb " + regs[arg1];
				break;
			}
			default: {
				std::stringstream tmp;
				tmp << std::hex << ((machineCode[i] << 8) | machineCode[i + 1]);
				output += "0x" + tmp.str();
			}
		}

		output += "\n";
	}

	std::ofstream oFile(outFilePath);
	oFile << output;
	oFile.close();

	std::cout << (disassemble? "Contents output to " + outFilePath : "Invalid command line arguments") << std::endl;

	return 0;
}
