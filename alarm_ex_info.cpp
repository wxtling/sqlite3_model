#include "stdafx.h"
#include "alarm_ex_info.h"

AlarmExInfo::AlarmExInfo()
{
	const char* fields[][2] = {
		{ "notify_id","integer NOT NULL PRIMARY KEY AUTOINCREMENT" },
		{ "id","text NOT NULL" },
		{ "user_guid","integer" }, 
		{ "type","integer" }, 
		{ "time","integer" },
		{ "end_time","integer" }, 

		{ "CONSTRAINT","'user_guid' FOREIGN KEY ('user_guid') REFERENCES 'user' ('guid') ON DELETE CASCADE ON UPDATE NO ACTION" }, 
	};
	  

	setTbName("alarm_ex_info");
	setPk("notify_id");
	 
	setFields(fields, _countof(fields)); 
}