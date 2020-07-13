#ifndef PREDICTOR_HPP
#define PREDICTOR_HPP

#include <algorithm>

class predictor{
public:
	char bht1, bht2;
	int jump_info[4];
public:
	predictor(){
		bht1 = bht2 = false;
		for(int i = 0; i < 4; ++i) jump_info[i] = 0;
	}
	~predictor() = default;
	void modify(bool flag){
		if(flag)jump_info[bht1 << 1 + bht2] = std::min(jump_info[bht1 << 1 + bht2] + 1, 3);
		else jump_info[bht1 << 1 + bht2] = std::max(jump_info[bht1 << 1 + bht2] - 1, 0);
		bht1 = bht2;
		bht2 = flag;
	}
	bool jump(){
		if(jump_info[bht1 << 1 + bht2] > 1) return true;
		else return false;
	}
};

#endif
