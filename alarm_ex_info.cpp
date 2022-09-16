#include "stdafx.h"
#include "alarm_ex_info.h"

AlarmExInfo::AlarmExInfo()
{
	const char* fields[][2] = {
		{ "id","integer NOT NULL PRIMARY KEY AUTOINCREMENT" },
		{ "user_guid","integer" }, 
		{ "type","integer" },
		{ "content","text" },
		{ "news","integer" },
		{ "time","integer" },
		{ "end_time","integer" }, 

		{ "CONSTRAINT","'user_guid' FOREIGN KEY ('user_guid') REFERENCES 'user' ('guid') ON DELETE CASCADE ON UPDATE NO ACTION" }, 
	};
	  
	setTbName("alarm_ex_info");

	setFields(fields, _countof(fields)); 
}