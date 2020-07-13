#ifndef INSTRUCTION_DECODER_HPP
#define INSTRUCTION_DECODER_HPP

#include "type.hpp"
class instruction_decoder{
public:
	bool init;
	bool finish = true;
	bool stall;
	bool jump;
	TYPE type_brief = UNKNOWN;
	type type_detail = INVALID;
	int rs1 = -1, rs2 = -1, rd = -1;
	int pc = -1;
	int immediate;
	int data1, data2; // data hazard
	int jump_from, jump_to; // control hazard
public:
	instruction_decoder(){
		init = false;
		stall = false;
		jump = false;
		type_brief = UNKNOWN;
		type_detail = INVALID;
		rs1 = rs2 = rd = -1;
		pc = -1;
	}
	sjtu::pair<bool, int> data_fetch(int rs, bool EXE_finish, bool MEM_finish, TYPE EXE_TYPE, type EXE_type, int EXE_result, int MEM_result, int EXE_RD, int MEM_RD, int *visit, int *x){
		if(!visit[rs]){ return sjtu::pair<bool, int>(true, x[rs]);}
		else if(EXE_RD == rs && EXE_finish){
			if(EXE_TYPE != I_TYPE || (EXE_TYPE == I_TYPE && (EXE_type > 13 || EXE_type < 9))){
				int result = 0;
				if(rs != 0) result = EXE_result; 
				return sjtu::pair<bool, int>(true, result);
			}
			else return sjtu::pair<bool, int>(false, 0);
		}
		else if(MEM_RD == rs && MEM_finish){
			int result = 0;
			if(rs != 0) result = MEM_result; 
			return sjtu::pair<bool, int>(true, result);
		}
		else return sjtu::pair<bool , int>(false, 0);
	}
	void run(bool EXE_stall, bool &EXE_jumperror, unsigned int IF_code, int IF_pc, bool IF_init, bool EXE_finish, bool MEM_finish, TYPE EXE_TYPE, type EXE_type, int EXE_result, int MEM_result, int EXE_RD, int MEM_RD, int *visit, int *x){
		if(!IF_init){
			init = false;
			if(EXE_jumperror){
				finish = false;
				EXE_jumperror = false;
			}
			if(!EXE_stall){
				stall = false;
			}
			if(EXE_stall){
				stall = true;
			}
		    return;
		}
		if(EXE_stall && EXE_jumperror){
			stall = true;
			EXE_jumperror = false;
			finish = false;
			return;
		}
		if(EXE_stall){
			stall = true;
			return;
		}
		if(EXE_jumperror){
			stall = false;
			EXE_jumperror = false;
			finish = false;
			return;
		}
		init = true;
		stall = false;
		type_brief = get_TYPE(IF_code);
		type_detail = get_type(IF_code, type_brief);
		if(type_brief == R_TYPE){ 
			rs1 = get_rs1(IF_code);
			rs2 = get_rs2(IF_code);
			rd = get_rd(IF_code);
			sjtu::pair<bool, int> tmp1 = data_fetch(rs1,EXE_finish, MEM_finish, EXE_TYPE, EXE_type, EXE_result, MEM_result, EXE_RD, MEM_RD, visit, x);
			sjtu::pair<bool, int> tmp2 = data_fetch(rs2,EXE_finish, MEM_finish, EXE_TYPE, EXE_type, EXE_result, MEM_result, EXE_RD, MEM_RD, visit, x);
			if(tmp1.first && tmp2.first){
				data1 = tmp1.second;
				data2 = tmp2.second;
				visit[rd]++;
			}
			else{
				finish = false;
				stall = true;
				return;
			}
		}
		else if(type_brief == I_TYPE){
			rs1 = get_rs1(IF_code);
			rd = get_rd(IF_code);
			immediate = get_imm(IF_code, type_brief);
			sjtu::pair<bool, int> tmp1 = data_fetch(rs1,EXE_finish, MEM_finish, EXE_TYPE, EXE_type, EXE_result, MEM_result, EXE_RD, MEM_RD, visit, x);
			if(tmp1.first){
				data1 = tmp1.second;
				visit[rd]++;
			}
			else{
				finish = false;
				stall = true;
				return;
			}
		}
		else if(type_brief == S_TYPE){
			rs1 = get_rs1(IF_code);
			rs2 = get_rs2(IF_code);
			immediate = get_imm(IF_code, type_brief);
			sjtu::pair<bool, int> tmp1 = data_fetch(rs1,EXE_finish, MEM_finish, EXE_TYPE, EXE_type, EXE_result, MEM_result, EXE_RD, MEM_RD, visit, x);
			sjtu::pair<bool, int> tmp2 = data_fetch(rs2,EXE_finish, MEM_finish, EXE_TYPE, EXE_type, EXE_result, MEM_result, EXE_RD, MEM_RD, visit, x);
			if(tmp1.first && tmp2.first){
				data1 = tmp1.second;
				data2 = tmp2.second;
			}
			else{
				finish = false;
				stall = true;
				return;
			}
		}
		else if(type_brief == B_TYPE){
			rs1 = get_rs1(IF_code);
			rs2 = get_rs2(IF_code);
			immediate = get_imm(IF_code, type_brief);
			sjtu::pair<bool, int> tmp1 = data_fetch(rs1,EXE_finish, MEM_finish, EXE_TYPE, EXE_type, EXE_result, MEM_result, EXE_RD, MEM_RD, visit, x);
			sjtu::pair<bool, int> tmp2 = data_fetch(rs2,EXE_finish, MEM_finish, EXE_TYPE, EXE_type, EXE_result, MEM_result, EXE_RD, MEM_RD, visit, x);
			if(tmp1.first && tmp2.first){
				data1 = tmp1.second;
				data2 = tmp2.second;
				jump_from = IF_pc;
				jump_to = IF_pc + immediate;
				jump = true;
			}
			else{
				finish = false;
				stall = true;
				return;
			}
		}
		else if(type_brief == U_TYPE){
			rd = get_rd(IF_code);
			++visit[rd];
			immediate = get_imm(IF_code, type_brief);
		}
		else if(type_brief == J_TYPE){
			rd = get_rd(IF_code);
			++visit[rd];
			immediate = get_imm(IF_code, type_brief);
		}
		stall = false;
		finish = true;
		pc = IF_pc;
	}
};

#endif
