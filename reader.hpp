#ifndef READER_HPP
#define READER_HPP

#include <iostream>
#include <cstring>
#include <cstdio>
#include <fstream>
class Reader{
public:
	Reader() = default;
	~Reader() = default;
	void read(char *mem){
		int address;
	    std::string data;
	    while(1){
		    std::cin >> data;
		    if(std::cin.eof()){
	    		return;
			}
		    if(data[0] == '@'){
		    	sscanf(data.c_str() + 1, "%x", &address);
			}
			else{
				int sum;
				sscanf(data.c_str(), "%x", &sum);
				for(int i = 1; i <= 3; ++i){
					int tmp_data;
					std::cin >> std::hex >> tmp_data;
					sum += (tmp_data << (i * 8));
				}
				memcpy(mem + address, &sum, sizeof(sum));
				address += 4;
			} 
	    }	
	}
};

#endif
