#include "stdafx.h" 
#include "base_sqlite.h"
#include <sqlite3.h> 


int BaseSqlite::runSql(const char *sql, const SqlCallback &callback) {
	sqlite3_stmt *stmt = NULL;
	int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

	if (isOpenLog) {
		std::string msg = "执行sql:\n";
		msg += sql;
		msg += "\n";
		fprintf(stdout, msg.c_str());
	}
	 
	if (result == SQLITE_OK) {
		if (isOpenLog) {
			fprintf(stdout, "结果：OK \n\n");
		}
		// 执行该语句
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			if (callback != nullptr) {
				callback(stmt);
			}
		}
	}
	else {
		if (isOpenLog) {
			std::string msg = "结果：error : " + result;
			msg += "\n\n";
			fprintf(stdout, msg.c_str());
		}
	} 
	sqlite3_finalize(stmt);
	return result;
}

void BaseSqlite::open() {
	if (isOpen) {
		return;
	}
	rc = sqlite3_open_v2("app_user_info.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_SHAREDCACHE, NULL);
	if (rc == SQLITE_OK) {
		isOpen = true;
		fprintf(stdout, "打开数据库连接成功\n");
	}
	else {
		fprintf(stdout, "打开数据库连接失败\n");
		exit(0);
	}
}


void BaseSqlite::close() {
	if (!isOpen) {
		return;
	}
	sqlite3_close_v2(db);
	isOpen = false;
}