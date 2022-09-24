#pragma once
#include <vector>
#include "stdafx.h" 
#include <map>  
#include "util/json.hpp"
#include <memory>
#include "../../sqlite/sqlite3.h"

#include <algorithm>

class BaseModel:public std::enable_shared_from_this<BaseModel> {

public:  
	// 创建表
	// isForceAdd 是否强制创建表
	void create(bool isForceAdd = false);
	 
	// 条件
	std::shared_ptr<BaseModel> where(const std::string &field, const std::string &value, const std::string &formula = "=");
	std::shared_ptr<BaseModel> whereOr(const std::string &field, const std::string &value, const std::string &formula = "=");
	std::shared_ptr<BaseModel> whereSql(const std::string &sql);

	// 排序 
	// isDesc 是否降序排列
	std::shared_ptr<BaseModel> order(const std::string &field,bool isDesc = true);

	// 分页
	std::shared_ptr<BaseModel> limit(int page = 1 ,int pageSize = 10);

	// 设置 SELECT
	std::shared_ptr<BaseModel> select(const std::string &field);

	// 保存数据 新增/修改 sqlite3_prepare_v2 的结果
	int save(const nlohmann::json &data, bool isUpdata = true);

	// 查询
	nlohmann::json get(int id = 0); 
	nlohmann::json first(int id = 0); // 第一条
	std::string value(std::string field, const std::string &defaultValue = ""); // 

	
	// 删除
	bool del(int id);

	// 全部删除
	bool deleteAll();  

	std::string getJsonValue(const nlohmann::json &data, const std::string &field, const std::string &defaultValue = "");

protected:
	void setFields(const char*keyValue[][2], int len);
	void setSreateOtherSql(const char*keyValue[], int len);

	std::vector<std::string> vesFields;
	std::vector<std::string> vesConstraint;
	std::unordered_map<std::string, std::string> mapFieldsValue;
	std::unordered_map<std::string, std::string> mapFieldsType;

	std::vector<std::string> vecCreateOtherSqls; 

	bool is_create_time = true; // 创建时间 字段为 create_time
	bool is_update_time = true; // 更新时间 字段为 update_time
	bool is_del_temporary_tables = true; // 删除临时表

	std::string tbName; // 表名称  
	std::string setTbName(const std::string &name); // 设置表名称

	// 主键名
	std::string pk = "id";
	std::string setPk(const std::string &id); // 设置主键名

	// 判断表是否存在
	bool getTableExist();

	// 
	std::string getWhereSql();
	std::string getSelectSql();

	// 清除缓存sql  sql  sqlLimit  sqlOrder  vecSqlWhere
	void clearCacheSql();

	// 保留判断条件
	bool isReserveWhereSql = false;

private:
	std::string global_sql = "";
	std::string global_sqlLimit = "";
	std::string global_sqlOrder = ""; 

	std::vector<std::string> global_sqlSelect;
	std::vector<std::string> global_vecSqlWhere;

	nlohmann::json getDealWith(sqlite3_stmt *stmt, std::vector<std::string> fields);

};	