# SQLite - C++ 操作模型

## 创建表

```c++
#include "sqlite3_model/user.h"  

auto userModel = std::make_shared<User>(); 

// 创建表
userModel->create();
 

```

## 查询

```c++ 

auto userInfo = userModel->where("arm", "0")->get();
//  SELECT * FROM user  WHERE  arm = '0'

auto userInfo = userModel->where("arm", "0")->whereOr("arm", "2")->get();
//  SELECT * FROM user  WHERE  arm = '0'  OR  arm = '2'

auto userInfo = userModel->where("arm", "0")->whereOr("arm", "2")->where("white_fill_light", "0")->get();
//  SELECT * FROM user  WHERE  arm = '0'  OR  arm = '2'  AND  white_fill_light = '0'

auto userInfo = userModel->whereSql("arm = '0' AND white_fill_light = '0'")->get();
// SELECT * FROM user  WHERE  arm = '0' AND white_fill_light = '0'


// 分页  
// page = 1 第一页
// pageSize = 10 每页多少条 
auto userInfo = userModel->limit(1,10)->get();
// SELECT * FROM user   LIMIT 10 OFFSET  (0 * 10)

// 排序
// isDesc 是否降序排列 默认: true 
auto userInfo = userModel->order("id")->order("arm",false)->get();
// SELECT * FROM user  ORDER BY id DESC,arm ASC



```