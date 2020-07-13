#ifndef TYPE_HPP
#define TYPE_HPP

#include "predictor.hpp"
#include "utility.hpp"
#include <iostream>
#include <bitset>
enum TYPE{R_TYPE, I_TYPE, S_TYPE, B_TYPE, U_TYPE, J_TYPE, UNKNOWN};
enum type{
	LUI, AUIPC,
	JAL, 
	BEQ, BNE, BLT, BGE, BLTU, BGEU,
	LB, LH, LW, LBU, LHU, ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI, JALR,
	SB, SH, SW, 
	ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND,
	INVALID
};

enum Regname{
	zero = 0, ra, sp, gp, tp, t0, t1, t2, s0, s1, a0, a1, a2, a3, a4, a5, a6, a7, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, t3, t4, t5, t6
};

Regname regname[32] = {zero, ra, sp, gp, tp, t0, t1, t2, s0, s1, a0, a1, a2, a3, a4, a5, a6, a7, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, t3, t4, t5, t6};

unsigned int get_opcode(unsigned int x){
	return x << 25 >> 25;
}

unsigned int get_funct3(unsigned int x){
	return x << 17 >> 29;
}

unsigned int get_funct7(unsigned int x){
	return x >> 25;
}

TYPE get_TYPE(unsigned int x){
	unsigned int opcode = get_opcode(x);
//	std::cout << std::bitset<7>(opcode) << std::endl;
	if(opcode == 0b0110011) return R_TYPE;
	else if(opcode == 0b0010011 || opcode == 0b0000011 || opcode == 0b1100111) return I_TYPE;
	else if(opcode == 0b0100011) return S_TYPE;
	else if(opcode == 0b1100011) return B_TYPE;
	else if(opcode == 0b0110111 || opcode == 0b0010111) return U_TYPE;
	else if(opcode == 0b1101111) return J_TYPE;
}

type get_type(unsigned int x, TYPE Type){
	if(Type == R_TYPE){
		unsigned int funct3 = get_funct3(x);
		if(funct3 == 0b000){
			unsigned int funct7 = get_funct7(x);
			if(funct7 == 0b0000000) return ADD;
			else if(funct7 == 0b0100000) return SUB;
		}
		else if(funct3 == 0b001) return SLL;
		else if(funct3 == 0b010) return SLT;
		else if(funct3 == 0b011) return SLTU;
		else if(funct3 == 0b100) return XOR;
		else if(funct3 == 0b101){
			unsigned int funct7 = get_funct7(x);
			if(funct7 == 0b0000000) return SRL;
			else if(funct7 == 0b0100000) return SRA;
		}
		else if(funct3 == 0b110) return OR;
		else if(funct3 == 0b111) return AND;
	}
	else if(Type == I_TYPE){
		unsigned int opcode = get_opcode(x);
		if(opcode == 0b0010011){
			unsigned int funct3 = get_funct3(x);
			if(funct3 == 0b000) return ADDI;
			else if(funct3 == 0b001) return SLLI;
			else if(funct3 == 0b010) return SLTI;
			else if(funct3 == 0b011) return SLTIU;
			else if(funct3 == 0b100) return XORI;
			else if(funct3 == 0b101){
				unsigned int funct7 = get_funct7(x);
				if(funct7 == 0b0000000) return SRLI;
				else if(funct7 == 0b0100000) return SRAI;
			}
			else if(funct3 == 0b110) return ORI;
			else if(funct3 == 0b111) return ANDI; 
		}
		else if(opcode == 0b0000011){
			unsigned int funct3 = get_funct3(x);
			if(funct3 == 0b000) return LB;
			else if(funct3 == 0b001) return LH;
			else if(funct3 == 0b010) return LW;
			else if(funct3 == 0b100) return LBU;
			else if(funct3 == 0b101) return LHU;
		}
		else if(opcode == 0b1100111) return JALR;
	}
	else if(Type == S_TYPE){
		unsigned int funct3 = get_funct3(x);
		if(funct3 == 0b000) return SB;
		else if(funct3 == 0b001) return SH;
		else if(funct3 == 0b010) return SW;
	}
	else if(Type == B_TYPE){
		unsigned int funct3 = get_funct3(x);
		if(funct3 == 0b000) return BEQ;
		else if(funct3 == 0b001) return BNE;
		else if(funct3 == 0b100) return BLT;
		else if(funct3 == 0b101) return BGE;
		else if(funct3 == 0b110) return BLTU;
		else if(funct3 == 0b111) return BGEU;
	}
	else if(Type == U_TYPE){
		unsigned int opcode = get_opcode(x);
		if(opcode == 0b0110111) return LUI;
		else if(opcode == 0b0010111) return AUIPC;
	}
	else if(Type == J_TYPE) return JAL;
}

unsigned int get_rs1(unsigned int x){return x << 12 >> 27;}
unsigned int get_rs2(unsigned int x){return x << 7 >> 27;}
unsigned int get_rd(unsigned int x){return x << 20 >> 27;}
int get_imm(unsigned int x, TYPE Type){
	if(Type == R_TYPE){return 0;}
	else if(Type == I_TYPE){return int(x >> 20) << 20 >> 20;}
	else if(Type == S_TYPE){
		return int(x >> 25 << 5 | x << 20 >> 27) << 20 >> 20;
	}
	else if(Type == B_TYPE){
		return int(x >> 31 << 12 | x << 1 >> 26 << 5 | x << 20 >> 28 << 1 | x << 24 >> 31 << 11) << 19 >> 19; 
	}
	else if(Type == U_TYPE){
		return int(x >> 12) << 12;
	}
	else if(Type == J_TYPE){
		return int(x >> 31 << 20 | x << 1 >> 22 << 1 | x << 11 >> 31 << 11 | x << 12 >> 24 << 12) << 11 >> 11;
	}
}

predictor Predictor[13];
#endif
