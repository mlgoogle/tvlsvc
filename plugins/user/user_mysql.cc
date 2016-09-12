// Copyright (c) 2016 The tourism Authors. All rights reserved.
// user_mysql.cc
// Created on: 2016年8月16日.
// Author: Paco.

#include "user/user_mysql.h"

#include <mysql/mysql.h>

#include "pub/storage/data_engine.h"
#include "pub/comm/comm_head.h"

#include "glog/logging.h"

namespace user {

UserMysql::UserMysql(config::FileConfig* config) {
  mysql_engine_ = base_logic::DataEngine::Create(MYSQL_TYPE);
  mysql_engine_->InitParam(config->mysql_db_list_);
}

UserMysql::~UserMysql() {
  if (mysql_engine_) {
    delete mysql_engine_;
  }
  mysql_engine_ = NULL;
}

int32 UserMysql::UserDetailSelect(int64 uid, DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_UserDetailSelect(" << uid << ");";
    LOG(INFO) << "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallUserLoginSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
    if (dic->empty()) {
      err = NO_GUIDE;
      break;
    }
  } while (0);
  return err;
}

int32 UserMysql::GuideDetailSelect(int64 uid, DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_GuideDetailSelect(" << uid << ");";
    LOG(INFO) << "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallGuideDetailSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
    if (dic->empty()) {
      err = NO_GUIDE;
      break;
    }
    ss.str("");
    ss.clear();
    ss << "call proc_GuideServiceSelect(" << uid << ");";
    LOG(INFO) << "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallGuideServiceSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while (0);
  return err;
}

int32 UserMysql::RecommendGuideSelect(int64 city, DicValue* dic) {
  int32 err = 0;
  bool r =  false;
  do {
    std::stringstream ss;
    ss << "call proc_RecommendGuideSelect(" << city << ");";
    LOG(INFO) << "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallRecommendGuideSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
    if (dic->empty()) {
      err = NO_RECOMMEND_GUIDE;
      break;
    }
  } while (0);
  return err;
}

int32 UserMysql::RegisterInsertAndSelect(std::string phone, std::string pass,
                              int64 type, DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_RegisterInsertAndSelect('" << phone << "','" << pass
        << "'," << type << ")";
    LOG(INFO) << "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallRegisterInsertAndSelect);
    //注册一定有结果返回
    if (!r || dic->empty()) {
      err = SQL_EXEC_ERROR;
      break;
    }
//    dic->GetBigInteger(L"result")
  } while (0);
  return err;
}

int32 UserMysql::ImproveUserUpdate(int64 uid, int64 sex, std::string nickname,
                                   std::string headurl, std::string addr,
                                   double lon, double lat) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_ImproveUserDataUpdate(" << uid << "," << sex  << ",'"
        << nickname << "','" << headurl << "','" << addr << "'," << lon
        << "," << lat << ")";
    LOG(INFO) << "sql:" << ss.str();
    r = mysql_engine_->WriteData(ss.str());
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while (0);
  return err;
}

int32 UserMysql::ChangePwdUpdate(int64 uid, std::string pwd) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_ChangePwdUpdate(" << uid << ",'" << pwd << "');";
    LOG(INFO) << "sql:" << ss.str();
    r = mysql_engine_->WriteData(ss.str());
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while(0);
  return err;
}

int32 UserMysql::ServiceCitySelect(DicValue* dic) {
  int32 err = 0;
  bool r =  false;
  do {
    std::stringstream ss;
    ss << "call proc_ServiceCitySelect();";
    LOG(INFO) << "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallServiceCitySelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
    if (dic->empty()) {
      err = NO_GUIDE_NEARBY;
      break;
    }
  } while (0);
  return err;
}

int32 UserMysql::NearGuideSelect(double* point, DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_NearGuideSelect(" << point[0] << "," << point[1] <<
        "," << point[2] << "," << point[3] << ");";
    LOG(INFO) << "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallNearGuideSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
    if (dic->empty()) {
      err = NO_GUIDE_NEARBY;
      break;
    }
  } while (0);
  return err;
}

int32 UserMysql::UserLoginSelect(std::string phone, std::string pass,
                                 int32 type, DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_UserInfoAddrSelect('" << phone << "','" << pass <<
        "'," << type << ");";
    LOG(INFO) << "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallUserLoginSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
    if (dic->empty()) {
      err = PHONE_OR_PASSWD_ERR;
      break;
    }
  } while (0);
  return err;
}

void UserMysql::CallServiceCitySelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*)(param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* info = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    ListValue* list = new ListValue();
    while (rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)) {
      DicValue* dict = new DicValue();
      if (rows[0] != NULL)
        dict->SetBigInteger(L"province_code_", atoi(rows[0]));
      if (rows[1] != NULL)
        dict->SetBigInteger(L"city_code_", atoi(rows[1]));
      if (rows[2] != NULL)
        dict->SetString(L"province_name_", rows[2]);
      if (rows[3] != NULL)
        dict->SetString(L"city_name_", rows[3]);
      list->Append(dict);
    }
    info->Set(L"service_city_", list);
  } else {
    LOG(WARNING) << "CallGuideServiceSelect count < 0";
  }
}

void UserMysql::CallGuideServiceSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*)(param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* info = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    ListValue* list = new ListValue();
    while (rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)) {
      DicValue* dict = new DicValue();
      if (rows[0] != NULL)
        dict->SetBigInteger(L"service_id_", atoll(rows[0]));
      if (rows[1] != NULL)
        dict->SetString(L"service_name_", rows[1]);
      if (rows[2] != NULL)
        dict->SetString(L"service_time_", rows[2]);
      if (rows[3] != NULL)
        dict->SetBigInteger(L"service_price_", atoll(rows[3]));
      list->Append(dict);
    }
    info->Set(L"service", list);
  } else {
    LOG(WARNING) << "CallGuideServiceSelect count < 0";
  }
}

void UserMysql::CallGuideDetailSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*)(param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)) {
      if (rows[0] != NULL)
        dict->SetBigInteger(L"uid_", atoll(rows[0]));
      if (rows[1] != NULL)
        dict->SetBigInteger(L"is_certification_", atoi(rows[1]));
      if (rows[2] != NULL)
        dict->SetBigInteger(L"business_lv_", atoi(rows[2]));
      if (rows[3] != NULL)
        dict->SetString(L"business_tag_", rows[3]);
      if (rows[4] != NULL)
        dict->SetString(L"traval_tag_", rows[4]);
      if (rows[5] != NULL)
        dict->SetString(L"heag_bg_url_", rows[5]);
      if (rows[6] != NULL)
        dict->SetString(L"photo_list_", rows[6]);
    }
  } else {
    LOG(WARNING) << "CallUserLoginSelect count < 0";
  }
}

void UserMysql::CallNearGuideSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*)(param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* info = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    ListValue* list = new ListValue();
    while (rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)) {
      DicValue* dict = new DicValue();
      if (rows[0] != NULL)
        dict->SetBigInteger(L"uid_", atoll(rows[0]));
      if (rows[1] != NULL)
        dict->SetString(L"phone_num_", rows[1]);
      if (rows[2] != NULL)
        dict->SetString(L"nick_name_", rows[2]);
      if (rows[3] != NULL)
        dict->SetBigInteger(L"level_", atoll(rows[3]));
      if (rows[4] != NULL)
        dict->SetString(L"head_url_", rows[4]);
      if (rows[5] != NULL)
        dict->SetString(L"address_", rows[5]);
      if (rows[6] != NULL)
        dict->SetReal(L"longitude_", atof(rows[6]));
      if (rows[7] != NULL)
        dict->SetReal(L"latitude_", atof(rows[7]));
      list->Append(dict);
    }
    info->Set(L"result", list);
  } else {
    LOG(WARNING) << "CallUserLoginSelect count < 0";
  }
}

void UserMysql::CallRecommendGuideSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*)(param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* info = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    ListValue* list = new ListValue();
    while (rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)) {
      DicValue* dict = new DicValue();
      if (rows[0] != NULL)
        dict->SetBigInteger(L"uid_", atoll(rows[0]));
      if (rows[1] != NULL)
        dict->SetString(L"phone_num_", rows[1]);
      if (rows[2] != NULL)
        dict->SetString(L"nickname_", rows[2]);
      if (rows[3] != NULL)
        dict->SetBigInteger(L"credit_lv_", atoll(rows[3]));
      if (rows[4] != NULL)
        dict->SetBigInteger(L"praise_lv_", atoll(rows[4]));
      if (rows[5] != NULL)
        dict->SetBigInteger(L"cash_lv_", atoll(rows[5]));
      if (rows[6] != NULL)
        dict->SetString(L"head_url_", rows[6]);
      if (rows[7] != NULL)
        dict->SetString(L"address_", rows[7]);
      if (rows[8] != NULL)
        dict->SetReal(L"longitude_", atof(rows[8]));
      if (rows[9] != NULL)
        dict->SetReal(L"latitude_", atof(rows[9]));
      if (rows[10] != NULL)
        dict->SetBigInteger(L"is_certification_", atoi(rows[10]));
      if (rows[11] != NULL)
        dict->SetString(L"business_tag_", rows[11]);
      if (rows[12] != NULL)
        dict->SetString(L"traval_tag_", rows[12]);
      if (rows[13] != NULL)
        dict->SetString(L"heag_bg_url_", rows[13]);
      list->Append(dict);
    }
    info->Set(L"recommend_guide", list);
  } else {
    LOG(WARNING) << "CallUserLoginSelect count < 0";
  }
}

void UserMysql::CallUserLoginSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*)(param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)) {
      if (rows[0] != NULL)
        dict->SetBigInteger(L"uid_", atoll(rows[0]));
      if (rows[1] != NULL)
        dict->SetString(L"phone_num_", rows[1]);
      if (rows[2] != NULL)
        dict->SetString(L"nickname_", rows[2]);
      if (rows[3] != NULL)
        dict->SetBigInteger(L"credit_lv_", atoll(rows[3]));
      if (rows[4] != NULL)
        dict->SetBigInteger(L"praise_lv_", atoll(rows[4]));
      if (rows[5] != NULL)
        dict->SetBigInteger(L"cash_lv_", atoll(rows[5]));
      if (rows[6] != NULL)
        dict->SetString(L"head_url_", rows[6]);
      if (rows[7] != NULL)
        dict->SetString(L"address_", rows[7]);
      if (rows[8] != NULL)
        dict->SetReal(L"longitude_", atof(rows[8]));
      if (rows[9] != NULL)
        dict->SetReal(L"latitude_", atof(rows[9]));
      if (rows[10] != NULL)
        dict->SetBigInteger(L"register_status_", atoll(rows[10]));
      if (rows[11] != NULL)
        dict->SetBigInteger(L"gender_", atoll(rows[11]));
    }
  } else {
    LOG(WARNING) << "CallUserLoginSelect count < 0";
  }
}

void UserMysql::CallRegisterInsertAndSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*)(param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)) {
      if (rows[0] != NULL) {
        dict->SetBigInteger(L"result", atoll(rows[0]));
          //用户已注册过
        if(atoi(rows[0]) == 0)
           break;
      }
      if (rows[1] != NULL)
        dict->SetBigInteger(L"uid_", atoll(rows[1]));
    }
  } else {
    LOG(WARNING) << "CallRegisterInsertAndSelect count < 0";
  }
}


}  // namespace user

