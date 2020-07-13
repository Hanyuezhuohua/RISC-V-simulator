#ifndef WRITE_BACKER_HPP
#define WRITE_BACKER_HPP

#include "type.hpp"

class write_backer{
public:
	bool stall;
public:
	write_backer(){
		stall = false;
	}
	void run(bool MEM_finish, bool MEM_init, bool MEM_stall, TYPE MEM_type, int MEM_rd, int MEM_result, int *visit, int *x){
		if(!MEM_init) return;
		if(MEM_stall && !MEM_finish){
			stall = true;
			return;
		}
		else if(!MEM_finish && !MEM_stall){
			return;
		}
		else{
			stall = false;
			if(MEM_type == R_TYPE || MEM_type == I_TYPE || MEM_type == U_TYPE || MEM_type == J_TYPE){
				visit[MEM_rd]--;
				if(MEM_rd) x[MEM_rd] = MEM_result;
			}
		} 
	}
};
#endif
