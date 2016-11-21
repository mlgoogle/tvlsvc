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

int32 UserMysql::UserDetailSelect(std::string uids, DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_UserDetailSelect('" << uids << "')";
    LOG(INFO)<< "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallUserDetailSelect);
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

int32 UserMysql::ServiceInfoSelect(std::string oid_str, DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    //普通订单
    ss << "call proc_ServiceInfoSelect('" << oid_str << "'," << 0 << ")";
    LOG(INFO)<< "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallServiceInfoSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
    //黑卡订单
    ss.str("");
    ss.clear();
    ss << "call proc_ServiceInfoSelect('" << oid_str << "'," << 1 << ")";
    LOG(INFO)<< "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallBlackServiceInfoSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
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
    LOG(INFO)<< "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallGuideDetailSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
    if (dic->empty()) {
      err = NO_GUIDE;
      break;
    }
    err = GuideServiceSelect(uid, dic);
//    ss.str("");
//    ss.clear();
//    ss << "call proc_GuideServiceSelect(" << uid << ");";
//    LOG(INFO)<< "sql:" << ss.str();
//    r = mysql_engine_->ReadData(ss.str(), dic, CallGuideServiceSelect);
//    if (!r) {
//      err = SQL_EXEC_ERROR;
//      break;
//    }
  } while (0);
  return err;
}

int32 UserMysql::GuideServiceSelect(int64 uid, DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_GuideServiceSelect(" << uid << ");";
    LOG(INFO)<< "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallGuideServiceSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while (0);
  return err;
}

int32 UserMysql::GuideServerUpdateAndSelect(int64 uid, std::list<std::string> sqls,
                                 DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    r = mysql_engine_->WriteDatas(sqls);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
    err = GuideServiceSelect(uid, dic);
  } while (0);
  return err;
}

int32 UserMysql::RecommendGuideSelect(int64 city, int64 type, DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_RecommendGuideSelect(" << city << "," << type << ");";
    LOG(INFO)<< "sql:" << ss.str();
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

int32 UserMysql::GuidesInfoSelect(std::string uids, DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_GuidesInfoSelect('" << uids << "')";
    LOG(INFO)<< "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallRecommendGuideSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
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
    LOG(INFO)<< "sql:" << ss.str();
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

int32 UserMysql::InvoiceInfoInsert(int64 uid, std::string oids,
                                   std::string title, std::string num,
                                   std::string comp, int64 type,
                                   std::string name, std::string phone,
                                   std::string area, std::string addr,
                                   std::string remarks, DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_OrderStatusSelect('" << oids << "')";
    LOG(INFO)<< "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallOrderStatusSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
    int64 result = 0;
    dic->GetBigInteger(L"result_", &result);
    if (result != 1) {
      err = 0;
      break;
    }
    ss.str("");
    ss.clear();
    ss << "call proc_InvoiceInfoInsert(" << uid << ",'" << oids << "','"
       << title << "','" << num << "','" << comp << "'," << type << ",'" << name
       << "','" << phone << "','" << area << "','" << addr << "','" << remarks
       << "')";
    LOG(INFO)<< "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallInvoiceInfoInsert);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while (0);
  return err;
}

int32 UserMysql::InvoiceRecordSelect(int64 uid, int64 count, int64 iid,
                                     DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_InvoiceRecordSelect(" << uid << "," << count << "," << iid
       << ")";
    LOG(INFO)<< "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallInvoiceRecordSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while (0);
  return err;
}

int32 UserMysql::InvoiceDetailSelect(int64 iid, DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_InvoiceDetailSelect(" << iid << ")";
    LOG(INFO)<< "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallInvoiceDetailSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while (0);
  return err;
}

int32 UserMysql::DeviceTokenUpdate(int64 uid, std::string dt) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_DeviceTokenUpdate(" << uid << ",'" << dt << "')";
    LOG(INFO)<< "sql:" << ss.str();
    r = mysql_engine_->WriteData(ss.str());
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
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
    ss << "call proc_ImproveUserDataUpdate(" << uid << "," << sex << ",'"
       << nickname << "','" << headurl << "','" << addr << "'," << lon << ","
       << lat << ")";
    LOG(INFO)<< "sql:" << ss.str();
    r = mysql_engine_->WriteData(ss.str());
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while (0);
  return err;
}

int32 UserMysql::TripRecordSelect(int64 uid, int64 oid, int64 count,
                                  DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;

    // 邀约
    ss << "call proc_TripRecordSelect(" << uid << "," << oid << "," << count
       << "," << 0 << ")";
    LOG(INFO)<< "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallTripRecordSelect);
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
    LOG(INFO)<< "sql:" << ss.str();
    r = mysql_engine_->WriteData(ss.str());
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while (0);
  return err;
}

int32 UserMysql::ServiceCitySelect(DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_ServiceCitySelect();";
    LOG(INFO)<< "sql:" << ss.str();
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
    ss << "call proc_NearGuideSelect(" << point[0] << "," << point[1] << ","
       << point[2] << "," << point[3] << ");";
    LOG(INFO)<< "sql:" << ss.str();
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
                                 int32 type, int64 time, DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_UserLoginSelect('" << phone << "','" << pass << "',"
       << type << "," << time << ");";
    LOG(INFO)<< "sql:" << ss.str();
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

int32 UserMysql::BlackcardPrivilegeSelect(DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_BlackcardPrivilegeSelect()";
    LOG(INFO)<< "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallBlackcardPrivilegeSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
    if (dic->empty()) {
      err = BLACK_PRIVILEGE_EMPTY;
      break;
    }
  } while (0);
  return err;
}

int32 UserMysql::BlackcardInfoSelect(int64 uid, DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_BlackcardInfoSelect(" << uid << ")";
    LOG(INFO)<< "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallBlackcardInfoSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while (0);
  return err;
}

int32 UserMysql::BlackcardConsumeRecordSelect(int64 uid, DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_BlackcardConsumeRecordSelect(" << uid << ")";
    LOG(INFO)<< "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic,
                                CallBlackcardConsumeRecordSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while (0);
  return err;
}

int32 UserMysql::BlackcardPriceInfoSelect(DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_BlackcardPriceInfoSelect()";
    LOG(INFO)<< "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallBlackcardPriceInfoSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while (0);
  return err;
}

int32 UserMysql::SkillsInfoSelect(DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_SkillsInfoSelect()";
    LOG(INFO)<< "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallSkillsInfoSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while (0);
  return err;
}

int32 UserMysql::ImgTokenSelect(DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_ImgTokenSelect()";
    LOG(INFO)<< "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallImgTokenSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while (0);
  return err;
}

int32 UserMysql::RechargeInfoInsertAndSelect(int64 uid, int64 price,
                                             DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_RechargeInfoInsertAndSelect(" << uid << "," << price
       << ")";
    LOG(INFO)<< "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallRechargeInfoInsertAndSelect);
    if (!r || dic->empty()) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while (0);
  return err;
}

int32 UserMysql::ChangeRechargeStatusAndSelect(int64 uid, int64 rid,
                                               int64 result, DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_ChangeRechargeStatusAndSelect(" << uid << "," << rid << ","
       << result << ")";
    LOG(INFO)<< "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic,
                                CallChangeRechargeStatusAndSelect);
    if (!r || dic->empty()) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while (0);
  return err;
}

int32 UserMysql::IdentityInfoInsertAndSelect(int64 uid, std::string front,
                                             std::string back, DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_IdentityInfoInsertAndSelect(" << uid << ",'" << front
       << "','" << back << "')";
    LOG(INFO)<< "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallIdentityInfoInsertAndSelect);
    if (!r || dic->empty()) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while (0);
  return err;
}

int32 UserMysql::IdentityReviewStatusSelect(int64 uid, DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_IdentityReviewStatusSelect(" << uid << ")";
    LOG(INFO)<< "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallIdentityReviewStatusSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while (0);
  return err;
}

int32 UserMysql::ShareTourismDetailsSelect(DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_ShareTourismDetailsSelect()";
    LOG(INFO)<< "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallShareTourismDetailsSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while (0);
  return err;
}

int32 UserMysql::ShareSkillDetailsSelect(DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_ShareSkillDetailsSelect()";
    LOG(INFO)<< "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallShareSkillDetailsSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while (0);
  return err;
}

int32 UserMysql::ShareSkillEntrySelect(int64 share_id, DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_ShareSkillEntrySelect(" << share_id << ")";
    LOG(INFO)<< "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallShareSkillEntrySelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while (0);
  return err;
}

int32 UserMysql::ShareSkillDiscussSelect(int64 share_id, int64 lastid,
                                         int64 count, DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_ShareSkillDiscussSelect(" << share_id << "," << lastid
       << "," << count << ")";
    LOG(INFO)<< "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallShareSkillDiscussSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while (0);
  return err;
}

int32 UserMysql::EntryShareSkillInsert(int64 share_id, int64 uid,
                                       DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_EntryShareSkillInsert(" << share_id << "," << uid << ")";
    LOG(INFO)<< "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallEntryShareSkillInsert);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while (0);
  return err;
}

int32 UserMysql::ShareTourismTypeSelect(DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_ShareTourismTypeSelect()";
    LOG(INFO)<< "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallShareTourismTypeSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while (0);
  return err;
}

int32 UserMysql::UserCashSelect(int64 uid, DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_UserCashSelect(" << uid << ")";
    LOG(INFO)<< "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallUserCashSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while (0);
  return err;
}

int32 UserMysql::OrderDetailsSelect(int64 oid, int64 type, DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_OrderDetailsSelect(" << oid << "," << type << ")";
    LOG(INFO)<< "sql:" << ss.str();
    dic->SetBigInteger(L"order_type_", type);
    r = mysql_engine_->ReadData(ss.str(), dic, CallOrderDetailsSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while (0);
  return err;
}

int32 UserMysql::NewAppointmentInsert(int64 uid, int64 city, int64 start,
                                      int64 end, std::string skill, int64 other,
                                      std::string name, int64 gender,
                                      std::string phone, DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_NewAppointmentInsert(" << uid << "," << city << ","
       << start << "," << end << ",'" << skill << "'," << other << ",'" << name
       << "'," << gender << ",'" << phone << "')";
    LOG(INFO)<< "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallNewAppointmentInsert);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while (0);
  return err;
}

int32 UserMysql::AppointmentRecordSelect(int64 uid, int64 lastid, int64 count,
                                         DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_AppointmentRecordSelect(" << uid << "," << lastid << ","
       << count << ")";
    LOG(INFO)<< "sql:" << ss.str();
    r = mysql_engine_->ReadData(ss.str(), dic, CallAppointmentRecordSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
    if (dic->empty()) {
      err = APPOINTMENT_RECORD_EMPTY;
    }
  } while (0);
  return err;
}

void UserMysql::CallServiceCitySelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* info = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    ListValue* list = new ListValue();
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
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
    LOG(WARNING)<< "CallGuideServiceSelect count < 0";
  }
}

void UserMysql::CallGuideServiceSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* info = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    ListValue* list = new ListValue();
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      DicValue* dict = new DicValue();
      if (rows[0] != NULL)
        dict->SetBigInteger(L"service_id_", atoll(rows[0]));
      if (rows[1] != NULL)
        dict->SetString(L"service_name_", rows[1]);
      if (rows[2] != NULL)
        dict->SetString(L"service_time_", rows[2]);
      if (rows[3] != NULL)
        dict->SetBigInteger(L"service_price_", atoll(rows[3]));
      if (rows[4] != NULL)
        dict->SetBigInteger(L"service_type_", atoll(rows[4]));
      list->Append(dict);
    }
    info->Set(L"service", list);
  } else {
    LOG(WARNING)<< "CallGuideServiceSelect count < 0";
  }
}

void UserMysql::CallGuideDetailSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
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
    LOG(WARNING)<< "CallUserLoginSelect count < 0";
  }
}

void UserMysql::CallNearGuideSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* info = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    ListValue* list = new ListValue();
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      DicValue* dict = new DicValue();
      if (rows[0] != NULL)
        dict->SetBigInteger(L"uid_", atoll(rows[0]));
      if (rows[1] != NULL)
        dict->SetString(L"phone_num_", rows[1]);
      if (rows[2] != NULL)
        dict->SetString(L"nick_name_", rows[2]);
      if (rows[3] != NULL)
        dict->SetBigInteger(L"praise_lv_", atoll(rows[3]));
      if (rows[4] != NULL)
        dict->SetString(L"head_url_", rows[4]);
      if (rows[5] != NULL)
        dict->SetString(L"address_", rows[5]);
      if (rows[6] != NULL)
        dict->SetReal(L"longitude_", atof(rows[6]));
      if (rows[7] != NULL)
        dict->SetReal(L"latitude_", atof(rows[7]));
      if (rows[8] != NULL)
        dict->SetBigInteger(L"gender_", atoll(rows[8]));
      list->Append(dict);
    }
    info->Set(L"result", list);
  } else {
    LOG(WARNING)<< "CallNearGuideSelect count < 0";
  }
}

void UserMysql::CallRecommendGuideSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* info = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    ListValue* list = new ListValue();
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
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
    LOG(WARNING)<< "CallRecommendGuideSelect count < 0";
  }
}

void UserMysql::CallUserLoginSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
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
      if (rows[12] != NULL)
        dict->SetBigInteger(L"has_recharged_", atoll(rows[12]));
    }
  } else {
    LOG(WARNING)<< "CallUserLoginSelect count < 0";
  }
}

void UserMysql::CallUserDetailSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* info = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    ListValue* list = new ListValue();
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
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
        dict->SetBigInteger(L"register_status_", atoll(rows[10]));
      if (rows[11] != NULL)
        dict->SetBigInteger(L"gender_", atoll(rows[11]));
      list->Append(dict);
    }
    info->Set(L"userinfo_list", list);
  } else {
    LOG(WARNING)<< "CallUserDetailSelect count < 0";
  }
}

void UserMysql::CallRegisterInsertAndSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      if (rows[0] != NULL) {
        dict->SetBigInteger(L"result", atoll(rows[0]));
        //用户已注册过
        if (atoi(rows[0]) == 0)
          break;
      }
      if (rows[1] != NULL)
        dict->SetBigInteger(L"uid_", atoll(rows[1]));
    }
  } else {
    LOG(WARNING)<< "CallRegisterInsertAndSelect count < 0";
  }
}

void UserMysql::CallTripRecordSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* info = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    ListValue* list = new ListValue();
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      DicValue* dict = new DicValue();
      if (rows[0] != NULL)
        dict->SetBigInteger(L"order_id_", atoll(rows[0]));
      if (rows[1] != NULL)
        dict->SetBigInteger(L"start_", atoll(rows[1]));
      if (rows[2] != NULL)
        dict->SetBigInteger(L"end_", atoll(rows[2]));
      if (rows[3] != NULL)
        dict->SetBigInteger(L"status_", atoll(rows[3]));
      if (rows[4] != NULL)
        dict->SetBigInteger(L"from_uid_", atoll(rows[4]));
      if (rows[5] != NULL)
        dict->SetBigInteger(L"to_uid_", atoll(rows[5]));
      if (rows[6] != NULL)
        dict->SetBigInteger(L"service_id_", atoll(rows[6]));
      if (rows[7] != NULL)
        dict->SetString(L"from_head_", rows[7]);
      if (rows[8] != NULL)
        dict->SetString(L"from_name_", rows[8]);
      if (rows[9] != NULL)
        dict->SetString(L"to_head_", rows[9]);
      if (rows[10] != NULL)
        dict->SetString(L"to_name_", rows[10]);
      if (rows[11] != NULL)
        dict->SetString(L"service_name_", rows[11]);
      if (rows[12] != NULL)
        dict->SetBigInteger(L"order_price_", atoll(rows[12]));
      if (rows[13] != NULL)
        dict->SetString(L"service_time_", rows[13]);
      if (rows[14] != NULL)
        dict->SetBigInteger(L"service_type_", atoll(rows[14]));
      if (rows[15] != NULL)
        dict->SetString(L"order_addr", rows[15]);
      if (rows[16] != NULL)
        dict->SetBigInteger(L"days_", atoll(rows[16]));
      list->Append(dict);
    }
    info->Set(L"trip_list", list);
  } else {
    LOG(WARNING)<< "CallTripRecordSelect count < 0";
  }
}

void UserMysql::CallServiceInfoSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* info = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    ListValue* list = new ListValue();
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      DicValue* dict = new DicValue();
      if (rows[0] != NULL)
        dict->SetBigInteger(L"service_id_", atoll(rows[0]));
      if (rows[1] != NULL)
        dict->SetBigInteger(L"uid_", atoll(rows[1]));
      if (rows[2] != NULL)
        dict->SetString(L"nick_name_", rows[2]);
      if (rows[3] != NULL)
        dict->SetString(L"service_name_", rows[3]);
      if (rows[4] != NULL)
        dict->SetString(L"service_time_", rows[4]);
      if (rows[5] != NULL)
        dict->SetBigInteger(L"service_price_", atoll(rows[5]));
      if (rows[6] != NULL)
        dict->SetBigInteger(L"service_type_", atoll(rows[6]));
      if (rows[7] != NULL)
        dict->SetBigInteger(L"order_time_", atoll(rows[7]));
      list->Append(dict);
    }
    info->Set(L"service_list", list);
  } else {
    LOG(WARNING)<< "CallServiceInfoSelect count < 0";
  }
}

void UserMysql::CallBlackServiceInfoSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* info = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    ListValue* list = new ListValue();
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      DicValue* dict = new DicValue();
      if (rows[0] != NULL)
        dict->SetBigInteger(L"order_id_", atoll(rows[0]));
      if (rows[1] != NULL)
        dict->SetString(L"privilege_pic_", rows[1]);
      if (rows[2] != NULL)
        dict->SetString(L"privilege_name_", rows[2]);
      if (rows[3] != NULL)
        dict->SetBigInteger(L"privilege_price_", atoll(rows[3]));
      if (rows[4] != NULL)
        dict->SetBigInteger(L"privilege_lv_", atoll(rows[4]));
      if (rows[5] != NULL)
        dict->SetBigInteger(L"order_time_", atoll(rows[5]));
      list->Append(dict);
    }
    info->Set(L"black_list", list);
  } else {
    LOG(WARNING)<< "CallBlackServiceInfoSelect count < 0";
  }
}

void UserMysql::CallInvoiceInfoInsert(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      if (rows[0] != NULL) {
        dict->SetBigInteger(L"result_", atoll(rows[0]));
      }
    }
  } else {
    LOG(WARNING)<<"CallInvoiceInfoInsert count < 0";
  }
}

void UserMysql::CallOrderStatusSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      if (rows[0] != NULL) {
        if (atoll(rows[0]) != 4) {
          if (atoll(rows[0]) == 6) {  //已记录开票
            dict->SetBigInteger(L"result_", -2);
            LOG(ERROR)<< "CallOrderStatusSelect result -2:订单已开票";
          } else {
            dict->SetBigInteger(L"result_",-3);
            LOG(ERROR) << "CallOrderStatusSelect result -3:订单未支付";
          }
          break;
        } else {  //订单状态均为4
          dict->SetBigInteger(L"result_", 1);
        }
      }
    }
  } else {
    dict->SetBigInteger(L"result_", 0);
    LOG(ERROR) << "CallOrderStatusSelect result 0:订单号不存在";
  }
}

void UserMysql::CallInvoiceRecordSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* info = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    ListValue* list = new ListValue();
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      DicValue* dict = new DicValue();
      if (rows[0] != NULL)
        dict->SetBigInteger(L"invoice_id_", atoll(rows[0]));
      if (rows[1] != NULL)
        dict->SetString(L"oid_str_", rows[1]);
      if (rows[2] != NULL)
        dict->SetBigInteger(L"invoice_status_", atoll(rows[2]));
      if (rows[3] != NULL)
        dict->SetBigInteger(L"invoice_time_", atoll(rows[3]));
      if (rows[4] != NULL)
        dict->SetBigInteger(L"invoice_price_", atoll(rows[4]));
      list->Append(dict);
    }
    info->Set(L"invoice_list", list);
  } else {
    LOG(WARNING)<<"CallInvoiceRecordSelect count < 0";
  }
}

void UserMysql::CallInvoiceDetailSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      if (rows[0] != NULL)
        dict->SetBigInteger(L"invoice_id_", atoll(rows[0]));
      if (rows[1] != NULL)
        dict->SetString(L"oid_str_", rows[1]);
      if (rows[2] != NULL)
        dict->SetString(L"title_", rows[2]);
      if (rows[3] != NULL)
        dict->SetString(L"user_name_", rows[3]);
      if (rows[4] != NULL)
        dict->SetString(L"user_mobile_", rows[4]);
      if (rows[5] != NULL)
        dict->SetString(L"area_", rows[5]);
      if (rows[6] != NULL)
        dict->SetString(L"addr_detail_", rows[6]);
      if (rows[7] != NULL)
        dict->SetBigInteger(L"invoice_type_", atoll(rows[7]));
      if (rows[8] != NULL)
        dict->SetBigInteger(L"total_price_", atoll(rows[8]));
      if (rows[9] != NULL)
        dict->SetBigInteger(L"invoice_time_", atoll(rows[9]));
      if (rows[10] != NULL)
        dict->SetBigInteger(L"order_num_", atoll(rows[10]));
      if (rows[11] != NULL)
        dict->SetBigInteger(L"first_time_", atoll(rows[11]));
      if (rows[12] != NULL)
        dict->SetBigInteger(L"final_time_", atoll(rows[12]));
    }
  } else {
    LOG(WARNING)<<"CallInvoiceDetailSelect count < 0";
  }
}

void UserMysql::CallBlackcardPrivilegeSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* info = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    ListValue* list = new ListValue();
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      DicValue* dict = new DicValue();
      if (rows[0] != NULL)
        dict->SetBigInteger(L"privilege_id_", atoll(rows[0]));
      if (rows[1] != NULL)
        dict->SetString(L"privilege_name_", rows[1]);
      if (rows[2] != NULL)
        dict->SetString(L"privilege_pic_yes_", rows[2]);
      if (rows[3] != NULL)
        dict->SetBigInteger(L"privilege_lv_", atoll(rows[3]));
      if (rows[4] != NULL)
        dict->SetString(L"privilege_bg_", rows[4]);
      if (rows[5] != NULL)
        dict->SetString(L"privilege_summary_", rows[5]);
      if (rows[6] != NULL)
        dict->SetString(L"privilege_details_", rows[6]);
      if (rows[7] != NULL)
        dict->SetString(L"privilege_pic_no_", rows[7]);
      list->Append(dict);
    }
    info->Set(L"privilege_list", list);
  } else {
    LOG(WARNING)<< "CallBlackcardPrivilegeSelect count < 0";
  }
}

void UserMysql::CallBlackcardInfoSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      if (rows[0] != NULL) {
        dict->SetBigInteger(L"result", atoll(rows[0]));
        //非黑卡用户
        if (atoi(rows[0]) == 0)
          break;
      }
      if (rows[1] != NULL)
        dict->SetBigInteger(L"start_time_", atoll(rows[1]));
      if (rows[2] != NULL)
        dict->SetBigInteger(L"end_time_", atoll(rows[2]));
      if (rows[3] != NULL)
        dict->SetBigInteger(L"blackcard_lv_", atoll(rows[3]));
      if (rows[4] != NULL)
        dict->SetBigInteger(L"blackcard_id_", atoll(rows[4]));
      if (rows[5] != NULL)
        dict->SetString(L"name_", rows[5]);

    }
  } else {
    LOG(WARNING)<<"CallBlackcardInfoSelect count < 0";
  }
}

void UserMysql::CallBlackcardConsumeRecordSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* info = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    ListValue* list = new ListValue();
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      DicValue* dict = new DicValue();
      if (rows[0] != NULL)
        dict->SetBigInteger(L"order_id_", atoll(rows[0]));
      if (rows[1] != NULL)
        dict->SetString(L"privilege_pic_", rows[1]);
      if (rows[2] != NULL)
        dict->SetString(L"privilege_name_", rows[2]);
      if (rows[3] != NULL)
        dict->SetBigInteger(L"privilege_price_", atoll(rows[3]));
      if (rows[4] != NULL)
        dict->SetBigInteger(L"privilege_lv_", atoll(rows[4]));
      if (rows[5] != NULL)
        dict->SetBigInteger(L"order_time_", atoll(rows[5]));
      if (rows[6] != NULL)
        dict->SetBigInteger(L"order_status_", atoll(rows[6]));
      list->Append(dict);
    }
    info->Set(L"blackcard_consume_record", list);
  } else {
    LOG(WARNING)<<"CallBlackcardConsumeRecordSelect count < 0";
  }
}

void UserMysql::CallSkillsInfoSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* info = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    ListValue* list = new ListValue();
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      DicValue* dict = new DicValue();
      if (rows[0] != NULL)
        dict->SetBigInteger(L"skill_id_", atoll(rows[0]));
      if (rows[1] != NULL)
        dict->SetString(L"skill_name_", rows[1]);
      if (rows[2] != NULL)
        dict->SetBigInteger(L"skill_type", atoll(rows[2]));
      list->Append(dict);
    }
    info->Set(L"skills_list", list);
  } else {
    LOG(WARNING)<<"CallSkillsInfoSelect count < 0";
  }
}

void UserMysql::CallImgTokenSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      if (rows[0] != NULL) {
        dict->SetString(L"img_token_", rows[0]);
      }
      if (rows[1] != NULL) {
        dict->SetBigInteger(L"valid_time_", atoll(rows[1]));
      }
    }
  } else {
    LOG(WARNING)<<"CallImgTokenSelect count < 0";
  }
}

void UserMysql::CallRechargeInfoInsertAndSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      if (rows[0] != NULL) {
        dict->SetString(L"recharge_id_", rows[0]);
      }
    }
  } else {
    LOG(WARNING)<<"CallRechargeInfoInsertAndSelect count < 0";
  }
}

void UserMysql::CallChangeRechargeStatusAndSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      if (rows[0] != NULL) {
        dict->SetBigInteger(L"return_code_", atoll(rows[0]));
      }
      if (rows[1] != NULL) {
        dict->SetBigInteger(L"user_cash_", atoll(rows[1]));
      }
    }
  } else {
    LOG(WARNING)<<"CallChangeRechargeStatusAndSelect count < 0";
  }
}

void UserMysql::CallIdentityInfoInsertAndSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      if (rows[0] != NULL) {
        dict->SetBigInteger(L"review_status_", atoll(rows[0]));
      }
    }
  } else {
    LOG(WARNING)<<"CallIdentityInfoInsertAndSelect count < 0";
  }
}

void UserMysql::CallIdentityReviewStatusSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      if (rows[0] != NULL) {
        dict->SetBigInteger(L"review_status_", atoll(rows[0]));
      } else {
        //没有查到用户
        dict->SetBigInteger(L"review_status_", -1);
      }
      if (rows[1] != NULL) {
        dict->SetString(L"failed_reason_", rows[1]);
      } else {
        dict->SetString(L"failed_reason_", "");
      }
    }
  } else {
    LOG(WARNING)<<"CallIdentityReviewStatusSelect count < 0";
    dict->SetBigInteger(L"review_status_", -1);
    dict->SetString(L"failed_reason_", "");
  }
}

void UserMysql::CallShareTourismDetailsSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* info = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    ListValue* list = new ListValue();
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      DicValue* dict = new DicValue();
      if (rows[0] != NULL)
        dict->SetBigInteger(L"share_id_", atoll(rows[0]));
      if (rows[1] != NULL)
        dict->SetBigInteger(L"share_type_", atoll(rows[1]));
      if (rows[2] != NULL)
        dict->SetString(L"share_theme_", rows[2]);
      if (rows[3] != NULL)
        dict->SetString(L"per_cash_", rows[3]);
      if (rows[4] != NULL)
        dict->SetString(L"addr_region_", rows[4]);
      if (rows[5] != NULL)
        dict->SetString(L"addr_detail_", rows[5]);
      if (rows[6] != NULL)
        dict->SetString(L"summary_", rows[6]);
      if (rows[7] != NULL)
        dict->SetString(L"telephone_", rows[7]);
      if (rows[8] != NULL)
        dict->SetString(L"detail_pic_", rows[8]);
      if (rows[9] != NULL)
        dict->SetString(L"brief_pic_", rows[9]);
      if (rows[10] != NULL)
        dict->SetBigInteger(L"is_recommend_", atoll(rows[10]));
      list->Append(dict);
    }
    info->Set(L"data_list", list);
  } else {
    LOG(WARNING)<<"CallShareTourismDetailsSelect count < 0";
  }
}

void UserMysql::CallShareSkillDetailsSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* info = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    ListValue* list = new ListValue();
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      DicValue* dict = new DicValue();
      if (rows[0] != NULL)
        dict->SetBigInteger(L"share_id_", atoll(rows[0]));
      if (rows[1] != NULL)
        dict->SetString(L"share_theme_", rows[1]);
      if (rows[2] != NULL)
        dict->SetString(L"share_user_", rows[2]);
      if (rows[3] != NULL)
        dict->SetString(L"user_label_", rows[3]);
      if (rows[4] != NULL)
        dict->SetBigInteger(L"share_start_", atoll(rows[4]));
      if (rows[5] != NULL)
        dict->SetBigInteger(L"share_end_", atoll(rows[5]));
      if (rows[6] != NULL)
        dict->SetBigInteger(L"share_status_", atoll(rows[6]));
      if (rows[7] != NULL)
        dict->SetString(L"detail_pic_", rows[7]);
      if (rows[8] != NULL)
        dict->SetString(L"brief_pic_", rows[8]);
      if (rows[9] != NULL)
        dict->SetString(L"banner_pic_", rows[9]);
      if (rows[10] != NULL)
        dict->SetBigInteger(L"is_banner_", atoll(rows[10]));
      if (rows[11] != NULL)
        dict->SetString(L"share_head_", rows[11]);
      if (rows[12] != NULL)
        dict->SetString(L"summary_", rows[12]);
      if (rows[13] != NULL)
        dict->SetBigInteger(L"entry_num_", atoll(rows[13]));
      list->Append(dict);
    }
    info->Set(L"data_list", list);
  } else {
    LOG(WARNING)<<"CallShareSkillDetailsSelect count < 0";
  }
}

void UserMysql::CallShareTourismTypeSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* info = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    ListValue* list = new ListValue();
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      DicValue* dict = new DicValue();
      if (rows[0] != NULL)
        dict->SetBigInteger(L"type_id_", atoll(rows[0]));
      if (rows[1] != NULL)
        dict->SetString(L"type_title_", rows[1]);
      if (rows[2] != NULL)
        dict->SetString(L"type_pic_", rows[2]);
      list->Append(dict);
    }
    info->Set(L"data_list", list);
  } else {
    LOG(WARNING)<<"CallShareTourismTypeSelect count < 0";
  }
}

void UserMysql::CallShareSkillEntrySelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* info = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    ListValue* list = new ListValue();
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      DicValue* dict = new DicValue();
      if (rows[0] != NULL)
        dict->SetBigInteger(L"uid_", atoll(rows[0]));
      if (rows[1] != NULL)
        dict->SetBigInteger(L"head_url_", atoll(rows[1]));
      list->Append(dict);
    }
    info->Set(L"user_list", list);
  } else {
    LOG(WARNING)<<"CallShareSkillEntrySelect count < 0";
  }
}

void UserMysql::CallShareSkillDiscussSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* info = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    ListValue* list = new ListValue();
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      DicValue* dict = new DicValue();
      if (rows[0] != NULL)
        dict->SetBigInteger(L"discuss_id_", atoll(rows[0]));
      if (rows[1] != NULL)
        dict->SetBigInteger(L"uid_", atoll(rows[1]));
      if (rows[2] != NULL)
        dict->SetString(L"content_", rows[2]);
      if (rows[3] != NULL)
        dict->SetBigInteger(L"discuss_time_", atoll(rows[3]));
      if (rows[4] != NULL)
        dict->SetString(L"head_url_", rows[4]);
      list->Append(dict);
    }
    info->Set(L"data_list", list);
  } else {
    LOG(WARNING)<<"CallShareSkillDiscussSelect count < 0";
  }
}

void UserMysql::CallEntryShareSkillInsert(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      if (rows[0] != NULL) {
        dict->SetBigInteger(L"result_", atoll(rows[0]));
      }
    }
  } else {
    LOG(WARNING)<<"CallEntryShareSkillInsert count < 0";
  }
}

void UserMysql::CallUserCashSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      if (rows[0] != NULL) {
        dict->SetBigInteger(L"user_cash_", atoll(rows[0]));
      }
    }
  } else {
    LOG(WARNING)<<"CallUserCashSelect count < 0";
    dict->SetBigInteger(L"user_cash_", 0);
  }
}

void UserMysql::CallNewAppointmentInsert(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      if (rows[0] != NULL) {
        dict->SetBigInteger(L"appointment_id_", atoll(rows[0]));
      }
    }
  } else {
    LOG(WARNING)<<"CallNewAppointmentInsert count < 0";
  }
}

void UserMysql::CallBlackcardPriceInfoSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* info = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    ListValue* list = new ListValue();
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      DicValue* dict = new DicValue();
      if (rows[0] != NULL)
        dict->SetBigInteger(L"blackcard_lv_", atoll(rows[0]));
      if (rows[1] != NULL)
        dict->SetBigInteger(L"blackcard_price_", atoll(rows[1]));
      list->Append(dict);
    }
    info->Set(L"data_list", list);
  } else {
    LOG(WARNING)<<"CallBlackcardPriceInfoSelect count < 0";
  }
}

void UserMysql::CallOrderDetailsSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  int64 type = 0;
  dict->GetBigInteger(L"order_type_", & type);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      if (rows[0] != NULL)
        dict->SetBigInteger(L"order_id_", atoll(rows[0]));
      if (rows[1] != NULL)
        dict->SetBigInteger(L"user_id_", atoll(rows[1]));
      if (rows[2] != NULL)
        dict->SetString(L"nickname_", (rows[2]));
      if (rows[3] != NULL)
        dict->SetString(L"user_head_", (rows[3]));
      if (rows[4] != NULL)
        dict->SetString(L"user_addr_", (rows[4]));
      if (rows[5] != NULL)
        dict->SetString(L"service_name_", (rows[5]));
      if (rows[6] != NULL)
        dict->SetBigInteger(L"service_score_", atoll(rows[6]));
      if (rows[7] != NULL)
        dict->SetBigInteger(L"user_score_", atoll(rows[7]));
      if (rows[8] != NULL)
        dict->SetString(L"remarks_", (rows[8]));
      if (rows[9] != NULL)
        dict->SetBigInteger(L"start_", atoll(rows[9]));
      if (rows[10] != NULL)
        dict->SetBigInteger(L"end_", atoll(rows[10]));
      if (rows[11] != NULL) {
        if (type == 1)
          dict->SetString(L"skills_", rows[11]);
        else
          dict->SetBigInteger(L"days_", atoll(rows[11]));
      }
    }
  } else {
    LOG(WARNING)<<"CallNewAppointmentInsert count < 0";
  }
}

void UserMysql::CallAppointmentRecordSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* info = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    ListValue* list = new ListValue();
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      DicValue* dict = new DicValue();
      if (rows[0] != NULL)
        dict->SetBigInteger(L"appointment_id_", atoll(rows[0]));
      if (rows[1] != NULL)
        dict->SetBigInteger(L"user_id_", atoll(rows[1]));
      if (rows[2] != NULL)
        dict->SetBigInteger(L"city_code_", atoll(rows[2]));
      if (rows[3] != NULL)
        dict->SetBigInteger(L"start_time_", atoll(rows[3]));
      if (rows[4] != NULL)
        dict->SetBigInteger(L"end_time_", atoll(rows[4]));
      if (rows[5] != NULL)
        dict->SetBigInteger(L"is_other_", atoll(rows[5]));
      if (rows[6] != NULL)
        dict->SetString(L"other_name_", rows[6]);
      if (rows[7] != NULL)
        dict->SetBigInteger(L"other_gender_", atoll(rows[7]));
      if (rows[8] != NULL)
        dict->SetString(L"other_phone_", rows[8]);
      if (rows[9] != NULL)
        dict->SetBigInteger(L"status_", atoll(rows[9]));
      if (rows[10] != NULL)
        dict->SetBigInteger(L"service_id_", atoll(rows[10]));
      if (rows[11] != NULL)
        dict->SetString(L"service_name_", rows[11]);
      if (rows[12] != NULL)
        dict->SetBigInteger(L"order_price_", atoll(rows[12]));
      if (rows[13] != NULL)
        dict->SetString(L"service_time_", rows[13]);
      if (rows[14] != NULL)
        dict->SetBigInteger(L"service_type_", atoll(rows[14]));
      if (rows[15] != NULL)
        dict->SetBigInteger(L"to_user_", atoll(rows[15]));
      if (rows[16] != NULL)
        dict->SetString(L"to_name_", rows[16]);
      if (rows[17] != NULL)
        dict->SetString(L"to_head_", rows[17]);
      if (rows[18] != NULL)
        dict->SetBigInteger(L"order_id_", atoll(rows[18]));
      list->Append(dict);
    }
    info->Set(L"data_list", list);
  } else {
    LOG(WARNING)<<"CallAppointmentRecordSelect count < 0";
  }
}

}
  // namespace user

