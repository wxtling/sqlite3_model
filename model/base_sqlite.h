#pragma once
#include <string>  
#include <sqlite3.h> 
#include <functional>

class BaseSqlite {

private:
	BaseSqlite() {
		open();
	} 

	sqlite3 *db;
	int rc;

	bool isOpen = false;
	
	void close();
	void open();
public:
	static BaseSqlite &getInstance()
	{
		static BaseSqlite ins;
		return ins;
	}

	bool isOpenLog = true;

	using SqlCallback = std::function<void(sqlite3_stmt *stmt)>;

	int runSql(const char * sql, const SqlCallback &callback = nullptr);
};