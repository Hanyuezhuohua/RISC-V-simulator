#ifndef INSTRUCTION_FETCHER_HPP
#define INSTRUCTION_FETCHER_HPP

#include <iostream>
#include "type.hpp"
class instruction_fetcher{
public:
	int PC;
	int code;
	bool init;
public:
    instruction_fetcher(){
    	code = 0;
    	init = false;
	}
	void run(char *memory, int &pc, bool ID_stall, bool &jump, int address1, int address2){
		if(ID_stall){return;}
		else if(jump){
			if(Predictor[(address1 >> 2) % 13].jump()){
				memcpy(&code, memory + address2, 4);
				PC = address2;
				jump = false;
				return;
			}
			else{jump = false;}
		}
		memcpy(&code, memory + pc, 4);
		PC = pc;
		pc += 4;
		init = true;
	} 
};

#endif
