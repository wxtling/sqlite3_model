#include "stdafx.h"
#include "user.h"
#include "util/json.hpp"

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
		{ "move_vendor","text" },
		{ "alarm_ex_info_news","text"},  
		{ "alarm_ex_info_news_add","integer" }
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


void User::setAlarmExInfoNews(int type, bool empty) {
	isReserveWhereSql = true;

	auto userInfo = first();

	auto alarm_ex_info_news_add = userInfo.value("alarm_ex_info_news_add", 0);
	if (alarm_ex_info_news_add == type) {
		return;
	}

	// …Ë÷√Œ¥∂¡
	unordered_map<int, int> AlarmExInfoNews;

	auto alarm_ex_info_news = userInfo.value("alarm_ex_info_news", "");
	if (alarm_ex_info_news == "") {
		for (auto iter = mapAlarmExInfoType.begin(); iter != mapAlarmExInfoType.end(); ++iter) {
			AlarmExInfoNews[iter->first] = 0;
		}
	}
	else {
		nlohmann::json j = nlohmann::json::parse(alarm_ex_info_news);
		auto  players = j.get<unordered_map<int, int>>();
		for (auto iter = players.begin(); iter != players.end(); ++iter) {
			AlarmExInfoNews[iter->first] = iter->second;
		}
	}

	if (empty == true) {
		AlarmExInfoNews[type] = 0; 
	}
	else {
		AlarmExInfoNews[type] = AlarmExInfoNews[type]++; 
	}
	  
	isReserveWhereSql = false;
	nlohmann::json data; 
	nlohmann::json j_umap(AlarmExInfoNews);
	data["alarm_ex_info_news"] = j_umap.dump(); 
	save(data);
	return;
}

