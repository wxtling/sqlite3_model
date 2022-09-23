#pragma once
#include <windows.h>
#include <string>
#include <algorithm>

#include "model/base_model.h"

  
class User :public BaseModel {
	 
public:
	User(); 

	std::unordered_map<int, std::string> mapAlarmExInfoType{ { 1,"����" },{ 2,"ȼ��" },{ 3,"�쳣����" } ,{ 4,"�ⲿ�쳣" } };

	void setAlarmExInfoNews(int type, bool empty);
};