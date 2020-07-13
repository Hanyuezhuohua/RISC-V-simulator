#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "reader.hpp"
#include "type.hpp"
#include "instruction_fetcher.hpp"
#include "instruction_decoder.hpp"
#include "executor.hpp"
#include "memory_accesser.hpp"
#include "write_backer.hpp"

const int mem_size = 4194304;
class simulator{
private:
	int x[32];
	int pc;
	char *memory;
	int visit[32];
	instruction_fetcher IF;
	instruction_decoder ID;
	executor EXE;
	memory_accesser MEM;
	write_backer WB;
public:
	simulator(){
		memory = new char[mem_size];
		for(int i = 0; i < mem_size; ++i){ memory[i] = 0;}
		for(int i = 0; i < 32; ++i){x[i] = 0;}
		for(int i = 0; i < 32; ++i){visit[i] = 0;}
		pc = 0;
	}
	void scan(){
		Reader reader;
		reader.read(memory);
	}
	int cycle = 0;
	void run(){
		while(!(IF.code ==  0x0ff00513 && ID.type_brief != B_TYPE && ID.type_detail != JAL && ID.type_detail != JALR)){
			++cycle;
			WB.run(MEM.finish, MEM.init, MEM.stall, MEM.type_brief, MEM.rd, MEM.result, visit, x);
			MEM.run(memory, EXE.init, EXE.finish, WB.stall, EXE.rd, EXE.result, EXE.immediate, EXE.type_brief, EXE.type_detail, EXE.rd, EXE.data1, EXE.data2);
			EXE.run(pc, ID.pc, ID.type_brief, ID.type_detail, ID.data1, ID.data2, ID.immediate, ID.rd, ID.finish, MEM.stall, ID.init);
			if(IF.code == 0x0ff00513 && !EXE.jumperror) break;
			if(IF.code == 0x0ff00513)IF.init = false;
			ID.run(EXE.stall, EXE.jumperror, IF.code, IF.PC, IF.init, EXE.finish, MEM.finish, EXE.type_brief, EXE.type_detail, EXE.result, MEM.result, EXE.rd, MEM.rd, visit, x);
			IF.run(memory, pc, ID.stall, ID.jump, ID.jump_from, ID.jump_to);

		}
		std::cout << std::dec << (((unsigned int)x[10]) & 255u);

	}
	~simulator(){delete [] memory;} 
};

#endif
