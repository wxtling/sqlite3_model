#pragma once
#include <string>  
#include "../../sqlite/sqlite3.h"
#include <functional>

class BaseSqlite {

private:
	BaseSqlite() 
	{ 
	} 

	sqlite3 *db;
	int rc;

	bool isOpen = false;
	
	void close();

public:
	static BaseSqlite &getInstance()
	{
		static BaseSqlite ins;
		return ins;
	}

	bool isOpenLog = true;

	using SqlCallback = std::function<void(sqlite3_stmt *stmt)>;

	void open(const std::string &file);
	int runSql(const char * sql, const SqlCallback &callback = nullptr);
};