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
                      int64 time, DicValue* dic);

  int32 NearGuideSelect(double* point, DicValue* dic);
  int32 GuideDetailSelect(int64 uid, DicValue* dic);
  int32 UserDetailSelect(std::string uids, DicValue* dic);
  int32 ServiceCitySelect(DicValue* dic);
  int32 RecommendGuideSelect(int64 city, int64 type, DicValue* dic);
  int32 RegisterInsertAndSelect(std::string phone, std::string pass,
                                int64 type, DicValue* dic);
  int32 ChangePwdUpdate(int64 uid, std::string pwd);
  int32 ImproveUserUpdate(int64 uid, int64 sex, std::string nickname,
                          std::string headurl, std::string addr, double lon,
                          double lat);

  int32 TripRecordSelect(int64 uid, int64 oid, int64 count, DicValue* dic);
  int32 ServiceInfoSelect(std::string sid_str, DicValue* dic);
  int32 InvoiceInfoInsert(int64 oid, std::string title, std::string num,
                          std::string comp, int64 type, std::string name,
                          std::string phone, std::string area, std::string addr,
                          std::string remarks, DicValue* dic);

  int32 InvoiceRecordSelect(int64 uid, DicValue* dic);

  int32 DeviceTokenUpdate(int64 uid, std::string dt);
  int32 BlackcardPrivilegeSelect(DicValue* dic);
  int32 BlackcardInfoSelect(int64 uid, DicValue* dic);


  //@brief 查询用户uid黑卡消费记录
  //@author paco
  //@param[in] uid 用户id
  //@param[out] dic 查询结果集
  //@return 返回mysql查询状态 正常返回0，异常返回负数 ，详情见pub/comm/comm_head.h
  //@warning
  int32 BlackcardConsumeRecordSelect(int64 uid, DicValue* dic);

  static void CallUserLoginSelect(void* param, Value* value);
  static void CallUserDetailSelect(void* param, Value* value);
  static void CallNearGuideSelect(void* param, Value* value);
  static void CallGuideDetailSelect(void* param, Value* value);
  static void CallGuideServiceSelect(void* param, Value* value);
  static void CallServiceCitySelect(void* param, Value* value);
  static void CallRecommendGuideSelect(void* param, Value* value);
  static void CallRegisterInsertAndSelect(void* param, Value* value);
  static void CallTripRecordSelect(void* param, Value* value);
  static void CallServiceInfoSelect(void* param, Value* value);
  static void CallInvoiceInfoInsert(void* param, Value* value);
  static void CallInvoiceRecordSelect(void* param, Value* value);
  static void CallBlackcardPrivilegeSelect(void* param, Value* value);
  static void CallBlackcardInfoSelect(void* param, Value* value);
  static void CallBlackcardConsumeRecordSelect(void* param, Value* value);
 private:
  base_logic::DataEngine* mysql_engine_;
};

}  // namespace user



#endif  // PLUGINS_USER_USER_MYSQL_H_
