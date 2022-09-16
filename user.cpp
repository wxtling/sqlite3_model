#include "stdafx.h"
#include "user.h"
 
User::User()
{ 
	const char* fields[][2] = {
		{ "id","integer NOT NULL PRIMARY KEY AUTOINCREMENT" },
		{ "guid","integer NOT NULL" },
		{ "account","text NOT NULL" },
		{ "arm","integer" },
		{ "white_fill_light","integer" },
		{ "abnormal","integer" },
		{ "ip","text" },
		{ "move_model","text" },
		{ "move_vendor","text" }
	};

	const char* createOtherSql[] = {
		"DROP INDEX IF EXISTS 'user_id';" ,
		"DROP INDEX IF EXISTS 'user_guid';" ,

		"CREATE UNIQUE INDEX user_id ON user (id ASC);" ,
		"CREATE UNIQUE INDEX user_guid ON user (guid ASC);" ,
	};
	 
	setTbName("user");

	setFields(fields, _countof(fields));
	setSreateOtherSql(createOtherSql, _countof(createOtherSql));
}