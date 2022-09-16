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
	// ������
	void create();
	 
	// ����
	std::shared_ptr<BaseModel> where(const std::string &field, const std::string &value, const std::string &formula = "=");
	std::shared_ptr<BaseModel> whereOr(const std::string &field, const std::string &value, const std::string &formula = "=");
	std::shared_ptr<BaseModel> whereSql(const std::string &sql);

	// ���� 
	// isDesc �Ƿ�������
	std::shared_ptr<BaseModel> order(const std::string &field,bool isDesc = true);

	// ��ҳ
	std::shared_ptr<BaseModel> limit(int page = 1 ,int pageSize = 10);

	// ���� SELECT
	std::shared_ptr<BaseModel> select(const std::string &field);

	// �������� ����/�޸� sqlite3_prepare_v2 �Ľ��
	bool save(const nlohmann::json &data);

	// ��ѯ
	nlohmann::json get(int id = 0);

	// ɾ��
	bool del(int id = 0);

	// ȫ��ɾ��
	bool deleteAll();  
protected:
	void setFields(const char*keyValue[][2], int len);
	void setSreateOtherSql(const char*keyValue[], int len);

	std::vector<std::string> vesFields;
	std::vector<std::string> vesConstraint;
	std::unordered_map<std::string, std::string> mapFieldsValue;
	std::unordered_map<std::string, std::string> mapFieldsType;

	std::vector<std::string> vecCreateOtherSqls; 

	bool is_create_time = true; // ����ʱ�� �ֶ�Ϊ create_time
	bool is_update_time = true; // ����ʱ�� �ֶ�Ϊ update_time
	bool is_del_temporary_tables = true; // ɾ����ʱ��

	std::string tbName; // ������  
	std::string setTbName(std::string name); // ���ñ�����

	// ������
	std::string pk = "id";
	 
	// �жϱ��Ƿ����
	bool getTableExist();

	// 
	std::string getWhereSql();
	std::string getSelectSql();

	// �������sql  sql  sqlLimit  sqlOrder  vecSqlWhere
	void clearCacheSql();
private:
	std::string global_sql = "";
	std::string global_sqlLimit = "";
	std::string global_sqlOrder = ""; 

	std::vector<std::string> global_sqlSelect;
	std::vector<std::string> global_vecSqlWhere;

	nlohmann::json getDealWith(sqlite3_stmt *stmt, std::vector<std::string> fields);
};	