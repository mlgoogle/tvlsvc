// Copyright (c) 2016 The tourism Authors. All rights reserved.
// user_mysql.h
// Created on: 2016年8月16日.
// Author: Paco.
#ifndef PLUGINS_USER_USER_MYSQL_H_
#define PLUGINS_USER_USER_MYSQL_H_

#include "public/config/config.h"
#include "public/basic/basictypes.h"

#include "pub/storage/data_engine.h"

#include "pub/net/typedef.h"

namespace user {

class UserMysql {
 public:
  UserMysql(config::FileConfig* config);
  ~UserMysql();

 public:
  int32 UserLoginSelect(std::string phone, std::string pass, int32 type,
                      DicValue* dic);

  int32 NearGuideSelect(double* point, DicValue* dic);
  int32 GuideDetailSelect(int64 uid, DicValue* dic);
  int32 ServiceCitySelect(DicValue* dic);
  int32 RecommendGuideSelect(int64 city, DicValue* dic);

  static void CallUserLoginSelect(void* param, Value* value);
  static void CallNearGuideSelect(void* param, Value* value);
  static void CallGuideDetailSelect(void* param, Value* value);
  static void CallGuideServiceSelect(void* param, Value* value);
  static void CallServiceCitySelect(void* param, Value* value);
  static void CallRecommendGuideSelect(void* param, Value* value);
 private:
  base_logic::DataEngine* mysql_engine_;
};

}  // namespace user



#endif  // PLUGINS_USER_USER_MYSQL_H_
