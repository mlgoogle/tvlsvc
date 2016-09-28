// Copyright (c) 2016 The tvlsvc Authors. All rights reserved.
// chat_mysql.cc
// Created on: 2016年8月24日.
// Author: Paco.
#include "chat/chat_mysql.h"

#include <mysql/mysql.h>

#include "pub/storage/data_engine.h"
#include "pub/comm/comm_head.h"
#include "pub/util/util.h"

#include "glog/logging.h"

namespace chat {
ChatMysql::ChatMysql(config::FileConfig* config) {
  mysql_engine_ = base_logic::DataEngine::Create(MYSQL_TYPE);
  mysql_engine_->InitParam(config->mysql_db_list_);
}

ChatMysql::~ChatMysql() {
  if (mysql_engine_) {
    delete mysql_engine_;
  }
  mysql_engine_ = NULL;
}

int32 ChatMysql::ChatRecordInsert(int64 from, int64 to, std::string msg,
                                  int64 time) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_ChatRecordInsert(" << from << "," << to << ",'" << msg <<
        "'," << time << ");";
    LOG(INFO) << "sql:" << ss.str();
    r = mysql_engine_->WriteData(ss.str());
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while(0);
  return err;
}

int32 ChatMysql::ChatRecordInsert(std::list<std::string> sqls) {
  int32 err = 0;
  bool r = false;
  do {
    r = mysql_engine_->WriteDatas(sqls);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while(0);
  return err;
}

int32 ChatMysql::ChatRecordQuery(int64 from, int64 to, int64 count, int64 id,
                      DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_ChatRecordQuery(" << from << "," << to << "," << count
        << "," << id << ")";
    LOG(INFO) << "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallChatRecordQuery);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
    if (dic->empty()) {
      err = NO_CHAT_RECORD;
      break;
    }
  } while (0);
  return err;
}

int32 ChatMysql::UserNickSelect(int64 uid, DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_UserNickSelect(" << uid << ")";
    LOG(INFO) << "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallUserNickSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
    if (dic->empty()) {
      err = NO_USER;
      break;
    }
  } while (0);
  return err;
}

int32 ChatMysql::DeviceTokenSelect(int64 uid, std::string* token) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_DeviceTokenSelect(" << uid << ")";
    DicValue dic;
    LOG(INFO) << "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), &dic, CallDeviceTokenSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
    if (dic.empty()) {
      err = NO_USER_DEVICE_TOKEN;
      break;
    }
    dic.GetString(L"device_token", token);
  } while (0);
  return err;
}

int32 ChatMysql::EvaluateTripInsert(int64 oid, int64 s_score, int64 u_score,
                         std::string remarks, int64 from, int64 to) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_EvaluateTripInsert(" << oid << "," << s_score << ","
        << u_score << ",'" << remarks << "'," << from << "," << to << ")";
    LOG(INFO) << "sql:" << ss.str();
    r = mysql_engine_->WriteData(ss.str());
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while (0);
  return err;
}

int32 ChatMysql::OrderStatusUpdate(int64 oid, int64 o_status) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_OrderStatusUpdate(" << oid << "," << o_status << ")";
    LOG(INFO) << "sql:" << ss.str();
    r = mysql_engine_->WriteData(ss.str());
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while (0);
  return err;
}

int32 ChatMysql::NewOrderInsertAndSelect(int64 from, int64 to, int64 sid,
                                 DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
 //   std::string time_format = util::GetStrftime("%y%m%d%H%M%S", time(NULL));
    std::stringstream ss;
    ss << "call proc_NewOrderInsertAndSelect(" << from << "," << to
        << "," << sid << "," << time(NULL) << ")";
    LOG(INFO) << "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallNewOrderInsertAndSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
    if (dic->empty()) {
      err = NEW_ORDER_ERR;
      break;
    }
  } while (0);
  return err;
}

void ChatMysql::CallNewOrderInsertAndSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*)(param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)) {
      if (rows[0] != NULL)
        dict->SetBigInteger(L"order_id_", atoll(rows[0]));
      if (rows[1] != NULL)
        dict->SetBigInteger(L"status_", atoi(rows[1]));
      if (rows[2] != NULL)
        dict->SetString(L"service_name_", rows[2]);
      if (rows[3] != NULL)
        dict->SetString(L"service_time_", rows[3]);
      if (rows[4] != NULL)
        dict->SetBigInteger(L"service_price_", atoll(rows[4]));
      if (rows[5] != NULL)
        dict->SetBigInteger(L"from_uid_", atoll(rows[5]));
      if (rows[6] != NULL)
        dict->SetString(L"from_name_", rows[6]);
      if (rows[7] != NULL)
        dict->SetString(L"from_head_", rows[7]);
      if (rows[8] != NULL)
        dict->SetBigInteger(L"to_uid_", atoll(rows[8]));
      if (rows[9] != NULL)
        dict->SetString(L"to_name_", rows[9]);
      if (rows[10] != NULL)
        dict->SetString(L"to_head_", rows[10]);
      if (rows[11] != NULL)
        dict->SetBigInteger(L"service_id_", atoll(rows[11]));
      if (rows[12] != NULL)
        dict->SetBigInteger(L"start_", atoll(rows[12]));
      if (rows[13] != NULL)
        dict->SetBigInteger(L"end_", atoll(rows[13]));
      if (rows[14] != NULL)
        dict->SetBigInteger(L"service_type_", atoll(rows[14]));
    }
  } else {
    LOG(WARNING) << "Call NewOrderInsertAndSelect count < 0";
  }
}

void ChatMysql::CallChatRecordQuery(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*)(param);
  MYSQL_ROW rows;
  DicValue* info = reinterpret_cast<DicValue*>(value);
  int32 num = engine->RecordCount();
  if (num > 0) {
    ListValue* list = new ListValue();
    while (rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)) {
      DicValue* dict = new DicValue();
      if (rows[0] != NULL)
        dict->SetBigInteger(L"chat_id_", atoll(rows[0]));
      if (rows[1] != NULL)
        dict->SetBigInteger(L"from_uid_", atoll(rows[1]));
      if (rows[2] != NULL)
        dict->SetBigInteger(L"to_uid_", atoll(rows[2]));
      if (rows[3] != NULL)
        dict->SetString(L"content_", rows[3]);
      if (rows[4] != NULL)
        dict->SetBigInteger(L"msg_time_", atoll(rows[4]));
      list->Append(dict);
    }
    info->Set(L"chat_list", list);
  } else {
    LOG(WARNING) << "Call ChatRecordQuery count < 0";
  }
}

void ChatMysql::CallDeviceTokenSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*)(param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)) {
      if (rows[0] != NULL)
        dict->SetString(L"device_token", rows[0]);
    }
  } else {
    LOG(WARNING) << "Call DeviceTokenSelect count < 0";
  }
}

void ChatMysql::CallUserNickSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*)(param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)) {
      if (rows[0] != NULL)
        dict->SetString(L"nickname", rows[0]);
    }
  } else {
    LOG(WARNING) << "Call UserNickSelect count < 0";
  }
}


}  // namespace chat
