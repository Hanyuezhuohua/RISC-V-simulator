#ifndef MEMORY_ACCESSER_HPP
#define MEMORY_ACCESSER_HPP

#include "type.hpp"

class memory_accesser{
public:
	bool init;
	bool finish = true;
	bool stall;
	int rd = -1;
	int result;
	TYPE type_brief = UNKNOWN;
	type type_detail = INVALID;
public:
	memory_accesser(){
		init = false;
		stall = false;
		type_brief = UNKNOWN;
		type_detail = INVALID;
	}
	void run(char *memory, bool EXE_init, bool EXE_finish, bool WB_stall, int EXE_RD, int EXE_result, int EXE_immediate, TYPE EXE_TYPE, type EXE_type, int EXE_rd, int EXE_data1, int EXE_data2){
		if(!EXE_init){
			init = false;
		    return;
		}
		init = true;
		static int time = 0;
		if(time != 0){
			time++;
			time %= 3;
			if(time == 0){
				finish = true;
			    stall = false;
			}
			return;
		}
		else{
		    if(!EXE_finish && WB_stall){
			    finish = false;
			    stall = true;
			    return;
		    }
		    else if(WB_stall){
			    stall = true;
			    finish = true;
			    return;
		    }
		    else if(!EXE_finish){
			    stall = false;
			    finish = false;
			    return;
		    }
		    result = EXE_result;
			rd = EXE_rd;
			type_brief = EXE_TYPE;
			type_detail = EXE_type;
		    if(EXE_type == LW){
		    	memcpy(&result, memory + EXE_immediate, 4);
		    	time++;
		    	time %= 3;
		    	finish = false;
			    stall = true;
			}
			else if(EXE_type == LH){
				short tmp;
				memcpy(&tmp, memory + EXE_immediate, 2);
				result = tmp;
				time++;
				time %= 3;
				finish = false;
			    stall = true;
			}
			else if(EXE_type == LHU){
				unsigned short tmp;
				memcpy(&tmp, memory + EXE_immediate, 2);
				result = tmp;
				time++;
				time %= 3;
				finish = false;
			    stall = true;
			}
			else if(EXE_type == LB){
				char tmp;
				memcpy(&tmp, memory + EXE_immediate, 1);
				result = tmp;
				time++;
				time %= 3;
				finish = false;
			    stall = true;
			}
			else if(EXE_type == LBU){
				unsigned char tmp;
				memcpy(&tmp, memory + EXE_immediate, 1);
				result = tmp;
				time++;
				time %= 3;
				finish = false;
			    stall = true;
			}
			else if(EXE_type == SW){
				memcpy(memory + EXE_immediate, &EXE_data2, 4);
				time++;
				time %= 3;
				finish = false;
			    stall = true;
			} 
			else if(EXE_type == SH){
				short tmp = EXE_data2;
				memcpy(memory + EXE_immediate, &tmp, 2);
				time++;
				time %= 3;
				finish = false;
			    stall = true;
			}
			else if(EXE_type == SB){
				char tmp = EXE_data2;
				memcpy(memory + EXE_immediate, &tmp, 1);
				time++;
				time %= 3;
				finish = false;
			    stall = true;
			}
			else{
				finish = true;
				stall = false;
			}
		}
	}
};

#endif
