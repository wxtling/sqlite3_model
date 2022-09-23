#pragma once
#include <windows.h>
#include <string>
#include <algorithm>

#include "model/base_model.h"

  
class User :public BaseModel {
	 
public:
	User(); 

	std::unordered_map<int, std::string> mapAlarmExInfoType{ { 1,"烟雾" },{ 2,"燃气" },{ 3,"异常闯入" } ,{ 4,"外部异常" } };

	void setAlarmExInfoNews(int type, bool empty);
};