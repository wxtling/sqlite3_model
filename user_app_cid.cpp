#include "stdafx.h"
#include "user_app_cid.h"

UserAppCid::UserAppCid()
{
	const char* fields[][2] = {
		{ "id","integer NOT NULL PRIMARY KEY AUTOINCREMENT" },
		{ "user_guid","integer" }, 
		{ "cid","text" }, 
		{ "online","integer" }, 
		{ "page_location","text" }, 
		{ "online_time","integer" },
		 
		{ "CONSTRAINT","'user_guid' FOREIGN KEY ('user_guid') REFERENCES 'user' ('guid') ON DELETE CASCADE ON UPDATE NO ACTION" },
	};
	 
	setTbName("user_app_cid");

	setFields(fields, _countof(fields)); 
}