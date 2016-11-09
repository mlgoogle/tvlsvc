// Copyright (c) 2016 The tvlsvc Authors. All rights reserved.
// chat_mysql.h
// Created on: 2016年8月24日.
// Author: Paco.
#ifndef PLUGINS_CHAT_CHAT_MYSQL_H_
#define PLUGINS_CHAT_CHAT_MYSQL_H_

#include "public/config/config.h"
#include "public/basic/basictypes.h"

#include "pub/storage/data_engine.h"

#include "pub/net/typedef.h"

namespace chat {

class ChatMysql {
 public:
  ChatMysql(config::FileConfig* config);
  ~ChatMysql();

  int32 ChatRecordInsert(int64 from, int64 to, std::string msg, int64 time);
  int32 ChatRecordInsert(std::list<std::string> sqls);

  int32 ChatRecordQuery(int64 from, int64 to, int64 count, int64 id,
                        DicValue* dic);
  //生成订单信息同时返回
  int32 NewOrderInsertAndSelect(int64 from, int64 to, int64 sid,
                                   DicValue* dic);

  int32 EvaluateTripInsert(int64 oid, int64 s_score, int64 u_score,
                           std::string remarks, int64 from, int64 to);

  int32 EvaluateInfoSelect(int64 oid, DicValue* dic);

  int32 DeviceTokenSelect(int64 uid, std::string* token);

  int32 UserNickSelect(int64 uid, DicValue* dic);

  int32 OrderStatusUpdate(int64 oid, int64 o_status);

  static void CallNewOrderInsertAndSelect(void* param, Value* value);
  static void CallChatRecordQuery(void* param, Value* value);
  static void CallDeviceTokenSelect(void* param, Value* value);
  static void CallUserNickSelect(void* param, Value* value);
  static void CallEvaluateInfoSelect(void* param, Value* value);

 private:
  base_logic::DataEngine* mysql_engine_;
};

}  // namespace chat



#endif  // PLUGINS_CHAT_CHAT_MYSQL_H_
