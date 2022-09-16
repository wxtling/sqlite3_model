#include "stdafx.h"
#include <algorithm>
#include <string> 
#include <time.h>
#include <iostream>  
#include "base_model.h"
#include "base_sqlite.h"
#include <map> 


#pragma warning(disable:4996)

/*
* 设置表字段
*/
void BaseModel::setFields(const char*keyValue[][2], int len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		auto key = keyValue[i][0];

		std::string value = keyValue[i][1];
		std::string type = "text";

		if (value.find("integer") != -1) {
			type = "int";
		}
		else if (value.find("real") != -1) {
			type = "real";
		}

		if (key == "CONSTRAINT") {
			vesConstraint.push_back(key);
		}
		else {
			vesFields.push_back(key);
			mapFieldsType[key] = type;
		}

		mapFieldsValue[key] = value;
	}

	// 创建时间
	if (is_create_time) {
		vesFields.push_back("create_time");
		mapFieldsValue["create_time"] = "integer";
		mapFieldsType["create_time"] = "int";
	}

	// 更新时间
	if (is_update_time) {
		vesFields.push_back("update_time");
		mapFieldsValue["update_time"] = "integer";
		mapFieldsType["update_time"] = "int";
	}
}

/*
* 设置创建表时其它要执行的sql
*/
void  BaseModel::setSreateOtherSql(const char *keyValue[], int len) {
	for (int i = 0; i < len; i++)
	{
		vecCreateOtherSqls.push_back(keyValue[i]);
	}
}

/*
* 设置表名
*/
std::string BaseModel::setTbName(std::string name) {
	tbName = name;
	return tbName;
}

/*
* 判断表是否存在
*/
bool BaseModel::getTableExist() {
	BaseSqlite ins = BaseSqlite::getInstance();
	std::string table_name = tbName;
	std::string tableSql = "SELECT name FROM sqlite_master WHERE type='table' AND name = ";
	tableSql += +"'" + table_name + "'";
	bool isTableExist = false;
	ins.runSql(tableSql.c_str(), [&isTableExist, table_name](sqlite3_stmt *stmt) {
		const unsigned char *name = sqlite3_column_text(stmt, 0);
		isTableExist = table_name == reinterpret_cast<const char*>(name);
	});
	return isTableExist;
}

/*
* 创建表
*/
void BaseModel::create() {
	BaseSqlite ins = BaseSqlite::getInstance();

	// 判断表是否存在
	bool isTableExist = getTableExist();

	/* 临时表处理 */
	std::string temporaryTables = "";
	if (isTableExist) {
		// 临时表名
		temporaryTables += "_" + tbName + "_old_";
		time_t t = time(0);
		char tmp[32] = { NULL };
		strftime(tmp, sizeof(tmp), "%Y%m%d_%H_%M_%S", localtime(&t));
		temporaryTables += tmp;
		// 临时表 sql
		std::string temporaryTablesSql = "ALTER TABLE ";
		temporaryTablesSql += tbName;
		temporaryTablesSql += " RENAME TO '";
		temporaryTablesSql += temporaryTables;
		temporaryTablesSql += "'";
		// 执行 sql
		ins.runSql(temporaryTablesSql.c_str());
	}

	/* 创建新表 */
	// 创建新表的 sql
	std::string createSql = "CREATE TABLE ";
	createSql += tbName;
	createSql += "(";

	int fieldsLng = vesFields.size();
	int constraintLng = vesConstraint.size();
	int lng = fieldsLng + constraintLng;
	int maxLng = lng - 1;
	int i = 0;
	int constraintI = 0;
	for (i = 0; i < lng; i++)
	{
		std::string field;
		if (i < fieldsLng) {
			field = vesFields[i];
		}
		else {
			field = vesConstraint[constraintI];
			constraintI++;
		}
		auto iter = mapFieldsValue.find(field);
		if (iter != mapFieldsValue.end()) {
			createSql += field + " " + iter->second;
			if (i < maxLng) {
				createSql += ",";
			}
		}
	}

	createSql += ");";

	// 执行创建新表 sql   
	ins.runSql(createSql.c_str());

	// 执行其他(索引、外键等) sql 
	for (int j = 0; j < vecCreateOtherSqls.size(); j++) {
		auto sql = vecCreateOtherSqls[j];
		ins.runSql(sql.c_str());
	}


	/* 导入数据 */
	if (isTableExist) {
		std::vector<std::string> oldFields;
		std::string findTableInfoSql = "PRAGMA table_info('" + temporaryTables + "')";
		ins.runSql(findTableInfoSql.c_str(), [&oldFields](sqlite3_stmt *stmt) {
			const unsigned char *name = sqlite3_column_text(stmt, 1);
			oldFields.push_back(reinterpret_cast<const char*>(name));
		});

		std::string importDataSql = "INSERT INTO ";
		importDataSql += tbName + "(";

		std::string importDataField = "";
		int klng = oldFields.size();
		for (int k = 0; k < klng; k++) {
			auto field = oldFields[k];
			auto iter = mapFieldsValue.find(field);
			if (iter != mapFieldsValue.end())
			{
				importDataField += '"' + field + '"';
				if (k < klng - 1) {
					importDataField += ",";
				}
			}
		}
		importDataSql += importDataField;
		importDataSql += ") SELECT ";
		importDataSql += importDataField;
		importDataSql += " FROM ";
		importDataSql += temporaryTables;

		/* 导入数据 */
		ins.runSql(importDataSql.c_str());

		/* 删除临时表 */
		if (is_del_temporary_tables) {
			std::string delTemporaryTablesSql = "DROP TABLE IF EXISTS " + temporaryTables;
			ins.runSql(delTemporaryTablesSql.c_str());
		}

		/* 更新 sqlite_sequence */
		std::string upSqliteSequenceSql = "UPDATE 'sqlite_sequence' SET seq = 3 WHERE name = '" + tbName + "'";
		ins.runSql(upSqliteSequenceSql.c_str());
	}
}

// 条件
std::shared_ptr<BaseModel> BaseModel::where(const std::string &field, const std::string &value, const std::string &formula) {
	if (global_vecSqlWhere.size() > 1) {
		global_vecSqlWhere.push_back(" AND ");
	}
	whereSql(field + " " + formula + " '" + value + "'");
	return shared_from_this();
}

std::shared_ptr<BaseModel> BaseModel::whereOr(const std::string &field, const std::string &value, const std::string &formula) {
	if (global_vecSqlWhere.size() > 1) {
		global_vecSqlWhere.push_back(" OR ");
	}
	whereSql(field + " " + formula + " '" + value + "'");
	return shared_from_this();
}

std::shared_ptr<BaseModel> BaseModel::whereSql(const std::string &sql) {
	if (global_vecSqlWhere.size() < 1) {
		global_vecSqlWhere.push_back(" WHERE ");
	}
	global_vecSqlWhere.push_back(" " + sql + " ");
	return shared_from_this();
}

// 排序  
std::shared_ptr<BaseModel> BaseModel::order(const std::string &field, bool isDesc) {
	if (global_sqlOrder == "") {
		global_sqlOrder += " ORDER BY ";
	}
	else {
		global_sqlOrder += ",";
	}
	global_sqlOrder += field + " ";
	global_sqlOrder += isDesc == true ? "DESC" : "ASC";
	return shared_from_this();
}

// 设置 SELECT 
std::shared_ptr<BaseModel> BaseModel::select(const std::string &field) {
	global_sqlSelect.push_back(field);
	return shared_from_this();
}

// 分页
std::shared_ptr<BaseModel> BaseModel::limit(int page, int pageSize) {
	global_sqlLimit = " LIMIT ";
	global_sqlLimit += std::to_string(pageSize);
	global_sqlLimit += " OFFSET ";
	global_sqlLimit += " (";
	global_sqlLimit += std::to_string(page - 1);
	global_sqlLimit += " * ";
	global_sqlLimit += std::to_string(pageSize);
	global_sqlLimit += ") ";
	return shared_from_this();
}

// 获取条件的 sql
std::string BaseModel::getWhereSql() {
	std::string sql = "";
	for (int i = 0; i < global_vecSqlWhere.size(); i++) {
		sql += global_vecSqlWhere[i];
	}
	return sql;
}

// 获取SELECT的sql
std::string BaseModel::getSelectSql() {
	std::string sql = "";
	int size = global_sqlSelect.size();
	for (int i = 0; i < size; i++) {
		sql += global_sqlSelect[i];
		if (i < size - 1) {
			sql += ",";
		}
	}
	return size == 0 ? "*" : sql;
}

// 查询结果处理 
nlohmann::json BaseModel::getDealWith(sqlite3_stmt *stmt, std::vector<std::string> fields) {
	nlohmann::json item;
	for (int i = 0; i < fields.size(); i++) {
		auto field = fields[i];
		auto iter = mapFieldsType.find(field);

		std::string type = "text";
		if (iter != mapFieldsType.end())
		{
			type = iter->second;
		}

		if (type == "int") {
			item[field] = sqlite3_column_int(stmt, i);
		}
		else if (type == "real") {
			item[field] = sqlite3_column_double(stmt, i);
		}
		else {
			const unsigned char *text = sqlite3_column_text(stmt, i);
			if (text != nullptr) {
				item[field] = reinterpret_cast<const char*>(text);
			}
			else {
				item[field] = "";
			}
		}
	}
	return item;
}

// 保存数据
bool BaseModel::save(const nlohmann::json &data) {
	BaseSqlite ins = BaseSqlite::getInstance();

	auto id = data.find(pk);
	if (id != data.end()) {
		where(pk, std::to_string(data[pk].get<int>()));
	}

	bool isUp = global_vecSqlWhere.size() > 0;

	time_t t = time(NULL);

	std::string fields = "";
	std::string values = t + "";

	// 是否更新
	if (isUp) {
		global_sql = " UPDATE " + tbName + " ";
		fields += "update_time";
	}
	else {
		global_sql = " INSERT or IGNORE INTO " + tbName + " ";
		fields += "create_time,update_time";
		values += "," + t; 
	}
	 
	for (auto& item : data.items()) {
		auto field = item.key();
		auto iter = mapFieldsValue.find(field);
		if (iter != mapFieldsValue.end())
		{
			auto key = item.key();

			std::string val = item.value().dump();
			val.erase(std::remove(val.begin(), val.end(), '"'), val.end());

			if (values != "") {
				values += ",";
			}
			if (isUp) {
				values += key + " = ";
			}
			else {
				if (fields != "") {
					fields += ",";
				}
				fields += key;
			}
			values += "'";
			values += val;
			values += "'";
		}
	}

	if (isUp) {
		global_sql += " SET ";
		global_sql += values;
		global_sql += getWhereSql();
	}
	else {
		global_sql += "(" + fields + ") ";
		global_sql += " VALUES ";
		global_sql += "(" + values + ");";
	}


	int result = ins.runSql(global_sql.c_str());
	clearCacheSql();
	if (result == SQLITE_OK) {
		return true;
	}
	return  false;
}

// 查询
nlohmann::json BaseModel::get(int id) {
	BaseSqlite ins = BaseSqlite::getInstance();
	nlohmann::json result;

	if (id != 0) {
		where(pk, std::to_string(id));
	}

	global_sql = " SELECT " + getSelectSql() + " FROM " + tbName + " ";
	global_sql += getWhereSql();
	global_sql += global_sqlOrder + " ";
	global_sql += global_sqlLimit;

	ins.runSql(global_sql.c_str(), [&result, this](sqlite3_stmt *stmt) {
		if (global_sqlSelect.size() > 0) {
			result.push_back(getDealWith(stmt, global_sqlSelect));
		}
		else {
			result.push_back(getDealWith(stmt, vesFields));
		}
	});

	clearCacheSql();
	return result;
}

// 清除缓存的sql
void BaseModel::clearCacheSql() {
	global_sql = "";
	global_sqlLimit = "";
	global_sqlOrder = "";

	global_sqlSelect.clear();
	global_vecSqlWhere.clear();
}

// 删除
bool BaseModel::del(int id) {
	BaseSqlite ins = BaseSqlite::getInstance();
	if (id != 0) {
		where(pk, std::to_string(id));
	}
	// 防止误删除所有记录
	if (global_vecSqlWhere.size() < 1) {
		return  false;
	}
	global_sql = " DELETE FROM " + tbName + " ";
	global_sql += getWhereSql();
	int result = ins.runSql(global_sql.c_str());
	clearCacheSql();
	if (result == SQLITE_OK) {
		return true;
	}
	return  false;
}

// 删除所有记录
bool BaseModel::deleteAll() {
	BaseSqlite ins = BaseSqlite::getInstance();
	global_sql = " DELETE FROM " + tbName + " ";
	int result = ins.runSql(global_sql.c_str());
	clearCacheSql();
	if (result == SQLITE_OK) {
		return true;
	}
	return  false;
}