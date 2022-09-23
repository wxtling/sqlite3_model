#include "stdafx.h"
#include "user_log.h"

UserLog::UserLog()
{
	const char* fields[][2] = {
		{ "id","integer NOT NULL PRIMARY KEY AUTOINCREMENT" },
		{ "user_guid","integer" },
		{ "title","text" },
		{ "content","text" },
		{ "time","integer" },

		{ "CONSTRAINT","'user_guid' FOREIGN KEY ('user_guid') REFERENCES 'user' ('guid') ON DELETE CASCADE ON UPDATE NO ACTION" },
	};

	setTbName("user_log");

	setFields(fields, _countof(fields));
}