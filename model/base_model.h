#pragma once
#include <vector>
#include "stdafx.h" 
#include <map>  
#include "../nlohmann/json.hpp"
#include <memory>
#include <sqlite3.h> 
#include <algorithm>

class BaseModel:public std::enable_shared_from_this<BaseModel> {

public:  
	// 创建表
	void create();
	 
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
	bool save(const nlohmann::json &data);

	// 查询
	nlohmann::json get(int id = 0);

	// 删除
	bool del(int id = 0);

	// 全部删除
	bool deleteAll();  
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
	std::string setTbName(std::string name); // 设置表名称

	// 主键名
	std::string pk = "id";
	 
	// 判断表是否存在
	bool getTableExist();

	// 
	std::string getWhereSql();
	std::string getSelectSql();

	// 清除缓存sql  sql  sqlLimit  sqlOrder  vecSqlWhere
	void clearCacheSql();
private:
	std::string global_sql = "";
	std::string global_sqlLimit = "";
	std::string global_sqlOrder = ""; 

	std::vector<std::string> global_sqlSelect;
	std::vector<std::string> global_vecSqlWhere;

	nlohmann::json getDealWith(sqlite3_stmt *stmt, std::vector<std::string> fields);
};	