// Copyright (c) 2016 The tvlsvc Authors. All rights reserved.
// chat_mysql.cc
// Created on: 2016年8月24日.
// Author: Paco.
#include "chat/chat_mysql.h"

#include <mysql/mysql.h>
#include <sys/time.h>

#include "pub/storage/data_engine.h"
#include "pub/comm/comm_head.h"
#include "pub/util/util.h"
#include "logic/logic_comm.h"
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
	int64 time, int64 is_push, int64 type) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_ChatRecordInsertV1(" << from << "," << to << ",'" << msg
		<< "'," << time << "," << is_push << "," << type << ");";
    LOG_MSG2("sql:%s\n",ss.str().c_str());
    r = mysql_engine_->WriteData(ss.str());
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while (0);
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
  } while (0);
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
     LOG_MSG2("sql: %s", ss.str().c_str());
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
     LOG_MSG2("sql: %s", ss.str().c_str());
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
     LOG_MSG2("sql: %s", ss.str().c_str());
    r = mysql_engine_->ReadData(ss.str(), &dic, CallDeviceTokenSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
    if (dic.empty()) {
      err = NO_USER_DEVICE_TOKEN;
      break;
    }
    dic.GetString(L"device_token_", token);
  } while (0);
  return err;
}

//todo
int32 ChatMysql::EvaluateInfoSelect(int64 oid, DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_EvaluateInfoSelect(" << oid << ")";
     LOG_MSG2("sql: %s", ss.str().c_str());
    r = mysql_engine_->ReadData(ss.str(), dic, CallEvaluateInfoSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
    if (dic->empty()) {
      err = NO_EVALUTE_INFO;
      break;
    }
  } while (0);
  return err;
}

int32 ChatMysql::PullPushMsgSelect(int64 uid, DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_PullPushMsgSelectV1(" << uid << ")";
     LOG_MSG2("sql: %s", ss.str().c_str());
    r = mysql_engine_->ReadData(ss.str(), dic, CallPullPushMsgSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while (0);
  return err;
}

int32 ChatMysql::OrderCreateInsert(int64 from_uid, int64 to_uid, int64 service_prince, std::string wx_id, DicValue* dic)
{
	int32 err = 0;
	bool r = false;
	do {
		std::stringstream ss;
		ss << "call proc_OrderCreateInsertNew(" << from_uid << "," << to_uid << "," << service_prince << ",'" <<  wx_id << "')";
		LOG_MSG2("sql: %s", ss.str().c_str());
		r = mysql_engine_->WriteData(ss.str());
		if (!r) {
			err = SQL_EXEC_ERROR;
			break;
		}
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
     LOG_MSG2("sql: %s", ss.str().c_str());
    r = mysql_engine_->WriteData(ss.str());
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while (0);
  return err;
}

int32 ChatMysql::UpDateTripCommission(int64 orderId) {
	int32 err = 0;
	bool r = false;
	do {
		std::stringstream ss;
		ss << "call proc_UserInvitationInfo(" << orderId <<")";
		LOG_MSG2("sql:%s\n",ss.str().c_str());
		DicValue dic;
		r = mysql_engine_->ReadData(ss.str(), &dic, CallGetUserInvitationInfo);
		if (!r) {
			err = SQL_EXEC_ERROR;
			break;
		}		
		if (!dic.empty())
		{
			std::string strUserRegTime;
			int nInvitationUserTime;
			int64 nUserId = 0;
			
			dic.GetString(L"startime", &strUserRegTime);
			dic.GetInteger(L"date", &nInvitationUserTime);
			dic.GetBigInteger(L"userId", &nUserId);

			time_t timep;
			struct tm *p;
			time(&timep);
			timep = timep - nInvitationUserTime * 24 * 60 * 60;
			p = localtime(&timep);
			int nInvitationTime = (p->tm_year + 1900) * 10000 + (p->tm_mon + 1) * 100 + p->tm_mday;
			//LOG(INFO) << "Invitation time  is  " << nInvitationTime << '\n';	
			LOG_MSG2("Invitation time  is %d \n", nInvitationTime);

			tm tmStartTime;
			time(&timep);
			strptime(strUserRegTime.c_str(), "%Y-%m-%d %H:%M:%S", &tmStartTime); //将字符串转换为tm时间 
			int nRegTime = (tmStartTime.tm_year + 1900) * 10000 + (tmStartTime.tm_mon + 1) * 100 + tmStartTime.tm_mday;
			//LOG(INFO) << "Reg time  is  " << nRegTime << '\n';
			LOG_MSG2("Reg time  is %d \n", nRegTime);

			if (nRegTime >= nInvitationTime)
			{				
				if (nUserId != 0)
				{
					std::stringstream sSql;
					sSql << "call proc_UpdateCommission(" << orderId << "," << nUserId << ")";
					//LOG(INFO) << sSql.str() << '\n';
					LOG_MSG2("Reg time  is %s \n", sSql.str().c_str());
					r = mysql_engine_->WriteData(sSql.str());

					if (!r) {
						err = SQL_EXEC_ERROR;
						break;
					}
				}				
			}
		}

	} while (0);
	return err;
}

int32 ChatMysql::UpDateGtPushComm(int64 from_uid, int64 to_uid, int64 msg_type, int64 msg_time, std::string servants_id, int64 appointment_id, std::string content){
	int32 err = 0;
	bool r = false;
	do 
	{
		std::stringstream ss;
		ss << "call proc_OrderMsgInsert(" << from_uid << "," << to_uid << "," << msg_type << "," << msg_time << ",'" << servants_id << "'," << appointment_id << ",'" << content << "')";
		LOG_MSG2("sql %s \n", ss.str().c_str());
		r = mysql_engine_->WriteData(ss.str());
		if (!r) {
			err = SQL_EXEC_ERROR;
			break;
		}
	} while (0);
	return err;
}

int32 ChatMysql::GuideOrderStatusUpdate(int64 oid, int64 o_status) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_GuideOrderStatusUpdate(" << oid << "," << o_status << ")";
     LOG_MSG2("sql: %s", ss.str().c_str());
    DicValue dic;
    r = mysql_engine_->ReadData(ss.str(), &dic, CallGuideOrderStatusUpdate);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
    int64 result_code = 0;
    dic.GetBigInteger(L"result_", &result_code);
    if (result_code == -1)
      err = ORDER_STATUS_ERR;
  } while (0);
  return err;
}

int32 ChatMysql::SpentCashUpdate(int64 uid, int64 oid, std::string pwd,
                                 DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_SpentCashUpdate(" << uid << "," << oid << ",'" << pwd
       << "')";
     LOG_MSG2("sql: %s", ss.str().c_str());
    r = mysql_engine_->ReadData(ss.str(), dic, CallSpentCashUpdate);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while (0);
  return err;
}

int32 ChatMysql::CancelOrderPayUpdate(int64 oid, int64 otype, DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_CancelOrderPayUpdate(" << oid << "," << otype << ")";
     LOG_MSG2("sql: %s", ss.str().c_str());
    r = mysql_engine_->ReadData(ss.str(), dic, CallCancelOrderPayUpdate);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
  } while (0);
  return err;
}

int32 ChatMysql::NewOrderInsertAndSelect(int64 from, int64 to, int64 sid,
                                         int64 day, DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    //   std::string time_format = util::GetStrftime("%y%m%d%H%M%S", time(NULL));
    std::stringstream ss;
    //0-邀约订单 1-预约订单
    ss << "call proc_NewOrderInsertAndSelect(" << from << "," << to << ","
       << sid << "," << day << "," << 0 << ")";
     LOG_MSG2("sql: %s", ss.str().c_str());
    r = mysql_engine_->ReadData(ss.str(), dic, CallNewOrderInsertAndSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
    if (dic->empty()) {
      //这里不应该运行
      err = NEW_ORDER_ERR;
      break;
    }
  } while (0);
  return err;
}

int32 ChatMysql::NewAppointmentOrderInsertAndSelect(int64 from, int64 to,
                                                    int64 sid, int64 aid,
                                                    DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    //   std::string time_format = util::GetStrftime("%y%m%d%H%M%S", time(NULL));
    std::stringstream ss;
    //0-邀约订单 1-预约订单
    ss << "call proc_NewAppointmentOrderInsertAndSelect(" << from << "," << to
       << "," << sid << "," << aid << ")";
     LOG_MSG2("sql: %s", ss.str().c_str());
    r = mysql_engine_->ReadData(ss.str(), dic, CallNewOrderInsertAndSelect);
    if (!r) {
      err = SQL_EXEC_ERROR;
      break;
    }
    if (dic->empty()) {
      //这里不应该运行
      err = NEW_ORDER_ERR;
      break;
    }
  } while (0);
  return err;
}

void ChatMysql::CallNewOrderInsertAndSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      if (rows[0] != NULL)
        dict->SetBigInteger(L"order_id_", atoll(rows[0]));
      if (rows[1] != NULL)
        dict->SetBigInteger(L"status_", atoi(rows[1]));
      if (rows[3] != NULL)
        dict->SetBigInteger(L"is_asked_", atoll(rows[3]));
    }
  } else {
    //LOG(WARNING)<< "Call NewOrderInsertAndSelect count < 0";
	  LOG_WARN("Call NewOrderInsertAndSelect count < 0");
  }
}

void ChatMysql::CallChatRecordQuery(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  DicValue* info = reinterpret_cast<DicValue*>(value);
  int32 num = engine->RecordCount();
  if (num > 0) {
    ListValue* list = new ListValue();
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
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
    info->Set(L"chat_list_", list);
  } else {
    //LOG(WARNING)<< "Call ChatRecordQuery count < 0";
	  LOG_WARN("Call ChatRecordQuery count < 0");
  }
}

void ChatMysql::CallDeviceTokenSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      if (rows[0] != NULL)
        dict->SetString(L"device_token_", rows[0]);
    }
  } else {
    //LOG(WARNING)<< "Call DeviceTokenSelect count < 0";
	  LOG_WARN("Call DeviceTokenSelect count < 0");
  }
}

void ChatMysql::CallUserNickSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      if (rows[0] != NULL)
        dict->SetString(L"nickname_", rows[0]);
    }
  } else {
    //LOG(WARNING)<< "Call UserNickSelect count < 0";
	  LOG_WARN("Call UserNickSelect count < 0");
  }
}

void ChatMysql::CallEvaluateInfoSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      if (rows[0] != NULL)
        dict->SetBigInteger(L"order_id_", atoll(rows[0]));
      if (rows[1] != NULL)
        dict->SetBigInteger(L"service_score_", atoll(rows[1]));
      if (rows[2] != NULL)
        dict->SetBigInteger(L"user_score_", atoll(rows[2]));
      if (rows[3] != NULL)
        dict->SetString(L"remarks_", rows[3]);
    }
  } else {
    //LOG(WARNING)<<"Call CallEvaluateInfoSelect count < 0";
	  LOG_WARN("Call UserNickSelect count < 0");
  }
}

void ChatMysql::CallSpentCashUpdate(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      if (rows[0] != NULL)
        dict->SetBigInteger(L"order_id_", atoll(rows[0]));
      if (rows[1] != NULL)
        dict->SetBigInteger(L"result_", atoll(rows[1]));
      if (rows[2] != NULL)
        dict->SetBigInteger(L"uid_", atoll(rows[2]));
      if (rows[3] != NULL)
        dict->SetBigInteger(L"user_cash_", atoll(rows[3]));
      if (rows[4] != NULL)
        dict->SetBigInteger(L"order_status_", atoll(rows[4]));
    }
  } else {
    //LOG(WARNING)<<"Call CallSpentCashUpdate count < 0";
	  LOG_WARN("Call CallSpentCashUpdate count < 0");
  }
}

void ChatMysql::CallCancelOrderPayUpdate(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      if (rows[0] != NULL)
        dict->SetBigInteger(L"order_id_", atoll(rows[0]));
      if (rows[1] != NULL)
        dict->SetBigInteger(L"order_status_", atoll(rows[1]));
      if (rows[2] != NULL)
        dict->SetBigInteger(L"order_type_", atoll(rows[2]));
    }
  } else {
    //LOG(WARNING)<<"Call CallCancelOrderPayUpdate count < 0";
	  LOG_WARN("Call CallCancelOrderPayUpdate count < 0");
  }
}

void ChatMysql::CallGuideOrderStatusUpdate(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      if (rows[0] != NULL)
        dict->SetBigInteger(L"result_", atoll(rows[0]));
    }
  } else {
    //LOG(WARNING)<<"Call CallGuideOrderStatusUpdate count < 0";
	  LOG_WARN("Call CallGuideOrderStatusUpdate count < 0");
  }
}

void ChatMysql::CallGetUserInvitationInfo(void* param, Value* value) {
	base_storage::DBStorageEngine* engine =
		(base_storage::DBStorageEngine*) (param);
	MYSQL_ROW rows;
	int32 num = engine->RecordCount();
	DicValue* dict = reinterpret_cast<DicValue*>(value);
	if (num > 0) {
		while (rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)) {
			if (rows[0] != NULL)
			{
				dict->SetString(L"startime", rows[0]);
			}
			if (rows[1] != NULL)
			{
				dict->SetInteger(L"date", atoi(rows[1]));
			}
			else
			{
				dict->SetInteger(L"date", 0);
			}
			if (rows[2] != NULL)
			{
				dict->SetBigInteger(L"userId", atoll(rows[2]));
			}
			else
			{
				dict->SetBigInteger(L"userId", 0);
			}
		}
	}
	else {
		//LOG(WARNING) << "Call CallGuideOrderStatusUpdate count < 0";
		LOG_WARN("Call CallGuideOrderStatusUpdate count < 0");
	}
}

void ChatMysql::CallPullPushMsgSelect(void* param, Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* info = reinterpret_cast<DicValue*>(value);
  ListValue* list = new ListValue();
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
      DicValue* dict = new DicValue();
      if (rows[0] != NULL)
        dict->SetBigInteger(L"from_uid_", atoll(rows[0]));
      if (rows[1] != NULL)
        dict->SetBigInteger(L"to_uid_", atoll(rows[1]));
      if (rows[2] != NULL)
        dict->SetBigInteger(L"msg_time_", atoll(rows[2]));
      if (rows[3] != NULL)
        dict->SetString(L"content_", (rows[3]));
	  if (rows[4] != NULL)
		  dict->SetBigInteger(L"msg_type_", atoll(rows[4]));
      list->Append(dict);
    }
  } else {
    //LOG(WARNING)<<"Call CallPullPushMsgSelect count < 0";
	  LOG_WARN("Call CallPullPushMsgSelect count < 0");
  }
  info->Set(L"msg_list_", list);
}

}
  // namespace chat
