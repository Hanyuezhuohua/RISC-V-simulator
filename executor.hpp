#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include "type.hpp"
class executor{
public:
	bool init;
	bool finish = true;
	bool stall;
	bool jumperror;
	int result;
	int immediate;
	int rd = -1;
	int data1, data2;
	TYPE type_brief = UNKNOWN;
	type type_detail = INVALID;
public:
	executor(){
		init = false;
		stall = false;
		jumperror = false;
		finish = true;
		type_brief = UNKNOWN;
		type_detail = INVALID;
	}
	void run(int &pc, int ID_PC, TYPE ID_TYPE, type ID_type, int ID_data1, int ID_data2, int ID_immediate, int ID_rd, bool ID_finish, bool MEM_stall, bool ID_init){
		if(!ID_init){
			init = false;
			return;
		} 
		init = true;
		if(!ID_finish && MEM_stall){
			finish = false;
			stall = true;
			return;
		}
		else if(MEM_stall){
			stall = true;
			finish = true;
			return;
		}
		else if(!ID_finish){
			stall = false;
			finish = false;
			return;
		}
		finish = true;
		stall = false;
		if(ID_type == ADD) result = ID_data1 + ID_data2;
		else if(ID_type == SUB) result = ID_data1 - ID_data2;
		else if(ID_type == SLT) result = ID_data1 < ID_data2;
		else if(ID_type == SLTU){
			unsigned int tmp1 = ID_data1;
			unsigned int tmp2 = ID_data2;
			result = tmp1 < tmp2;
		}
		else if(ID_type == AND) result = ID_data1 & ID_data2;
		else if(ID_type == OR) result = ID_data1 | ID_data2;
		else if(ID_type == XOR) result = ID_data1 ^ ID_data2;
		else if(ID_type == SLL) result = ID_data1 << (ID_data2 & ((1 << 6) - 1));
		else if(ID_type == SRL){
			unsigned int tmp1 = ID_data1;
			unsigned int tmp2 = ID_data2;
			result = tmp1 >> (tmp2 & ((1 << 6) - 1));
		}
		else if(ID_type == SRA) result = ID_data1 >> (ID_data2 & ((1 << 6) - 1));
		else if(ID_type == ADDI) result = ID_data1 + ID_immediate;
		else if(ID_type == SLTI) result = ID_data1 < ID_immediate;
		else if(ID_type == SLTIU){
			unsigned int tmp1 = ID_data1;
			result = tmp1 < ID_immediate;
		}
		else if(ID_type == ANDI) result = ID_data1 & ID_immediate;
		else if(ID_type == ORI) result = ID_data1 | ID_immediate;
		else if(ID_type == XORI) result = ID_data1 ^ ID_immediate;
		else if(ID_type == SLLI) result = ID_data1 << (ID_immediate & ((1 << 6) - 1));
		else if(ID_type == SRLI){
			unsigned int tmp1 = ID_data1;
			result = tmp1 >> (ID_immediate & ((1 << 6) - 1));
		}
		else if(ID_type == SRAI) result = ID_data1 >> (ID_immediate & ((1 << 6) - 1));
		else if(ID_type == JALR){
			result = ID_PC + 4;
			pc = (ID_immediate + ID_data1) & (~1);
			jumperror = true;
		}
		else if(ID_type == LW || ID_type == LH || ID_type == LB || ID_type == LHU || ID_type == LBU){
			immediate = ID_data1 + ID_immediate;
		} 
		else if(ID_type == LUI) result = ID_immediate;
		else if(ID_type == AUIPC) result = ID_immediate + ID_PC;
		else if(ID_type == JAL){
			result = ID_PC + 4;
			pc = ID_PC + ID_immediate;
			jumperror = true;
		}
		else if(ID_TYPE == B_TYPE){
			predictor &tmp = Predictor[(ID_PC >> 2) % 13];
			bool flag = tmp.jump();
			if(ID_type == BEQ){
				if(ID_data1 == ID_data2){
					if(!flag){
						jumperror = true;
						pc = ID_PC + ID_immediate;
					}
					else pc = ID_PC + ID_immediate + 4;
				}
				else if(flag) jumperror = true;
				tmp.modify(ID_data1 == ID_data2);
			}
			else if(ID_type == BNE){
				if(ID_data1 != ID_data2){
					if(!flag){
						jumperror = true;
						pc = ID_PC + ID_immediate;
					}
					else pc = ID_PC + ID_immediate + 4;
				}
				else if(flag) jumperror = true;
				tmp.modify(ID_data1 != ID_data2);
			}
			else if(ID_type == BLT){
				if(ID_data1 < ID_data2){
					if(!flag){
						jumperror = true;
						pc = ID_PC + ID_immediate;
					}
					else pc = ID_PC + ID_immediate + 4;
				}
				else{
					if(flag) jumperror = true;
				} 
				tmp.modify(ID_data1 < ID_data2);
			}
			else if(ID_type == BLTU){
				unsigned int tmp1 = ID_data1;
				unsigned int tmp2 = ID_data2;
				if(tmp1 < tmp2){
					if(!flag){
						jumperror = true;
						pc = ID_PC + ID_immediate;
					}
					else pc = ID_PC + ID_immediate + 4;
				}
				else if(flag) jumperror = true;
				tmp.modify(tmp1 < tmp2);
			}
			else if(ID_type == BGE){
				if(ID_data1 >= ID_data2){
					if(!flag){
						jumperror = true;
						pc = ID_PC + ID_immediate;
					}
					else pc = ID_PC + ID_immediate + 4;
				}
				else if(flag) jumperror = true;
				tmp.modify(ID_data1 >= ID_data2);
			}
			else if(ID_type == BGEU){
				unsigned int tmp1 = ID_data1;
				unsigned int tmp2 = ID_data2;
				if(tmp1 >= tmp2){
					if(!flag){
						jumperror = true;
						pc = ID_PC + ID_immediate;
					}
					else pc = ID_PC + ID_immediate + 4;
				}
				else if(flag) jumperror = true;
				tmp.modify(tmp1 >= tmp2);
			}
		}
		else if(ID_TYPE == S_TYPE) immediate = ID_immediate + ID_data1;
		type_brief = ID_TYPE;
		type_detail = ID_type;
		rd = ID_rd;
		data1 = ID_data1;
		data2 = ID_data2;
	}
};

#endif
