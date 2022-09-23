#include "stdafx.h" 
#include "base_sqlite.h"
#include "../../sqlite/sqlite3.h"


int BaseSqlite::runSql(const char *sql, const SqlCallback &callback) {
	sqlite3_stmt *stmt = NULL;
	int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

	if (isOpenLog) {
		std::string msg ="ִ�� SQL��\n";
		msg += sql;
		LOG_INFO(_ModuleTask,"%s", msg.c_str());
	}

	if (result == SQLITE_OK) {
		if (isOpenLog) { 
			LOG_INFO(_ModuleTask, "�����OK \n\n");
		}
		// ִ�и����
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			if (callback != nullptr) {
				callback(stmt);
			}
		}
	}
	else {
		LOG_INFO(_ModuleTask, "�����error : [%d] ", result); 
	}
	sqlite3_finalize(stmt);
	return result;
}

void BaseSqlite::open(const std::string &file) {
	if (isOpen) {
		return;
	}

	rc = sqlite3_open_v2(file.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_SHAREDCACHE, NULL);
	if (rc == SQLITE_OK) {
		isOpen = true; 
		LOG_INFO(_ModuleTask, "�����ݿ����ӳɹ�\n");
	}
	else {
		LOG_INFO(_ModuleTask, "�����ݿ�����ʧ��\n"); 
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