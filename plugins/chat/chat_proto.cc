// Copyright (c) 2016 The tvlsvc Authors. All rights reserved.
// chat_proto.cc
// Created on: 2016年8月24日.
// Author: Paco.
#include "chat/chat_proto.h"

#include "glog/logging.h"
#include "logic/logic_comm.h"
#include "base/logic/base_values.h"
#include "public/basic/basic_util.h"
#include "public/basic/md5sum.h"

#include "pub/comm/comm_head.h"

namespace chat {

AskInvitationRecv::AskInvitationRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  from_uid_ = 0;
  to_uid_ = 0;
  service_id_ = 0;
  day_count_ = 1;
}

int32 AskInvitationRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"from_uid_", &from_uid_);
	  if (!r)
	  {
		  LOG_ERROR("AskInvitationRecv::from_uid_ parse error");
	  }
      r = dic->GetBigInteger(L"to_uid_", &to_uid_);
	  if (!r)
	  {
		  LOG_ERROR("AskInvitationRecv::to_uid_ parse error");
	  }
      r = dic->GetBigInteger(L"service_id_", &service_id_);
	  if (!r)
	  {
		  LOG_ERROR("AskInvitationRecv::service_id_ parse error");
	  }
      r = dic->GetBigInteger(L"day_count_", &day_count_);
	  if (!r)
	  {
		  LOG_ERROR("AskInvitationRecv::day_count_ parse error");
	  }
    } else {
	  LOG_ERROR("AskInvitationRecv Deserialize error");
      err = ASK_INVITATION_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

AppointMentGuideRecv::AppointMentGuideRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  from_uid_ = 0;
  to_uid_ = 0;
  service_id_ = 0;
  appointment_id_ = 0;
}

int32 AppointMentGuideRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"from_uid_", &from_uid_);
	  if (!r)
	  {
		  LOG_ERROR("AppointMentGuideRecv::from_uid_ parse error");
	  }
      r = dic->GetBigInteger(L"to_uid_", &to_uid_);
	  if (!r)
	  {
		  LOG_ERROR("AppointMentGuideRecv::to_uid_ parse error");
	  }
      r = dic->GetBigInteger(L"service_id_", &service_id_);
	  if (!r)
	  {
		  LOG_ERROR("AppointMentGuideRecv::service_id_ parse error");
	  }
      r = dic->GetBigInteger(L"appointment_id_", &appointment_id_);
	  if (!r)
	  {
		  LOG_ERROR("AppointMentGuideRecv::appointment_id_ parse error");
	  }
    } else {
	  LOG_ERROR("AppointMentGuideRecv Deserialize error");
      err = REQUEST_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

AnswerInvitationRecv::AnswerInvitationRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  from_uid_ = 0;
  to_uid_ = 0;
  order_id_ = 0;
  order_status_ = 0;
}

int32 AnswerInvitationRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"from_uid_", &from_uid_);
	  if (!r)
	  {
		  LOG_ERROR("AnswerInvitationRecv::from_uid_ parse error");
	  }
      r = dic->GetBigInteger(L"to_uid_", &to_uid_);
	  if (!r)
	  {
		  LOG_ERROR("AnswerInvitationRecv::to_uid_ parse error");
	  }
      r = dic->GetBigInteger(L"order_id_", &order_id_);
	  if (!r)
	  {
		  LOG_ERROR("AnswerInvitationRecv::order_id_ parse error");
	  }
      r = dic->GetBigInteger(L"order_status_", &order_status_);
	  if (!r)
	  {
		  LOG_ERROR("AnswerInvitationRecv::order_status_ parse error");
	  }
    } else {
	  LOG_ERROR("AnswerInvitationRecv Deserialize error");
      err = ANSWER_INVITATION_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

ChatPacket::ChatPacket(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  from_uid_ = -1;
  to_uid_ = -1;
  msg_time_ = 0;
}

int32 ChatPacket::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"from_uid_", &from_uid_);
	  if (!r)
	  {
		  LOG_ERROR("ChatPacket::from_uid_ parse error");
	  }
      r = dic->GetBigInteger(L"to_uid_", &to_uid_);
	  if (!r)
	  {
		  LOG_ERROR("ChatPacket::to_uid_ parse error");
	  }
      r = dic->GetBigInteger(L"msg_time_", &msg_time_);
	  if (!r)
	  {
		  LOG_ERROR("ChatPacket::msg_time_ parse error");
	  }
      r = dic->GetString(L"content_", &content_);
	  if (!r)
	  {
		  LOG_ERROR("ChatPacket::content_ parse error");
	  }
	  r = dic->GetBigInteger(L"msg_type_", &msg_type_);
	  if (!r)
	  {
		  LOG_ERROR("ChatPacket::msg_type_ parse error");
	  }
//      base::BasicUtil::UrlDecode(content_, content_);
     // LOG(INFO)<< "from:" << from_uid_ << " to:" << to_uid_ << " content:"
	//	  << content_ << "msg_type" << msg_type_;
	  LOG_MSG2("from:%d to: %d content; %s msg_type:%d\n", from_uid_, to_uid_, content_.c_str(), msg_type_);
    } else {
      //LOG(ERROR) << "ChatPacket Deserialize error";
		LOG_ERROR("ChatPacket Deserialize error");
      err = CHAT_PACKET_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

ChatRecordRecv::ChatRecordRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  from_uid_ = 0;
  to_uid_ = 0;
  count_ = 20;
  last_chat_id_ = 0;
}

int32 ChatRecordRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"from_uid_", &from_uid_);
	  if (!r)
	  {
		  LOG_ERROR("ChatRecordRecv::from_uid_ parse error");
	  }
      r = dic->GetBigInteger(L"to_uid_", &to_uid_);
	  if (!r)
	  {
		  LOG_ERROR("ChatRecordRecv::to_uid_ parse error");
	  }
      r = dic->GetBigInteger(L"count_", &count_);
	  if (!r)
	  {
		  LOG_ERROR("ChatRecordRecv::count_ parse error");
	  }
      r = dic->GetBigInteger(L"last_chat_id_", &last_chat_id_);
	  if (!r)
	  {
		  LOG_ERROR("ChatRecordRecv::last_chat_id_ parse error");
	  }
    } else {
      //LOG(ERROR)<< "ChatRecordRecv Deserialize error";
	  LOG_ERROR("ChatRecordRecv Deserialize error");
      err = CHAT_RECORD_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

PushMsgReadRecv::PushMsgReadRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  uid_ = 0;
  count_ = 0;
}

int32 PushMsgReadRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
	  if (!r)
	  {
		  LOG_ERROR("PushMsgReadRecv::uid_ parse error");
	  }
      r = dic->GetBigInteger(L"count_", &count_);
	  if (!r)
	  {
		  LOG_ERROR("PushMsgReadRecv::count_ parse error");
	  }
    } else {
      //LOG(ERROR)<< "PushMsgReadRecv Deserialize error";
	  LOG_ERROR("PushMsgReadRecv Deserialize error");
      err = MSG_READ_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

EvaluateTripRecv::EvaluateTripRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  from_uid_ = 0;
  to_uid_ = 0;
  order_id_ = 0;
  service_score_ = 0;
  user_score_ = 0;
}

int32 EvaluateTripRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"from_uid_", &from_uid_);
	  if (!r)
	  {
		  LOG_ERROR("EvaluateTripRecv::from_uid_ parse error");
	  }
      r = dic->GetBigInteger(L"to_uid_", &to_uid_);
	  if (!r)
	  {
		  LOG_ERROR("EvaluateTripRecv::to_uid_ parse error");
	  }
      r = dic->GetBigInteger(L"order_id_", &order_id_);
	  if (!r)
	  {
		  LOG_ERROR("EvaluateTripRecv::order_id_ parse error");
	  }
      r = dic->GetBigInteger(L"service_score_", &service_score_);
	  if (!r)
	  {
		  LOG_ERROR("EvaluateTripRecv::service_score_ parse error");
	  }
      r = dic->GetBigInteger(L"user_score_", &user_score_);
	  if (!r)
	  {
		  LOG_ERROR("EvaluateTripRecv::user_score_ parse error");
	  }
      r = dic->GetString(L"remarks_", &remarks_);
	  if (!r)
	  {
		  LOG_ERROR("EvaluateTripRecv::remarks_ parse error");
	  }
    } else {
      //LOG(ERROR)<< "EvaluateTripRecv Deserialize error";
	  LOG_ERROR("EvaluateTripRecv Deserialize error");
      err = EVALUATE_TRIP_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

EvaluateInfoRecv::EvaluateInfoRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  order_id_ = 0;
}

int32 EvaluateInfoRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"order_id_", &order_id_);
	  if (!r)
	  {
		  LOG_ERROR("EvaluateInfoRecv::order_id_ parse error");
	  }
    } else {
		LOG_ERROR("EvaluateInfoRecv Deserialize error");
      err = EVALUATE_INFO_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

SpentCashRecv::SpentCashRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  uid_ = 0;
  order_id_ = 0;
}

int32 SpentCashRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"order_id_", &order_id_);
	  if (!r)
	  {
		  LOG_ERROR("SpentCashRecv::order_id_ parse error");
	  }
      r = dic->GetBigInteger(L"uid_", &uid_);
	  if (!r)
	  {
		  LOG_ERROR("SpentCashRecv::uid_ parse error");
	  }
      r = dic->GetString(L"passwd_", &passwd_);
      if (r) {
        base::MD5Sum md5(passwd_);
        passwd_ = md5.GetHash();
      }
	  if (!r)
	  {
		  LOG_ERROR("SpentCashRecv::passwd_ parse error");
	  }
    } else {
      //LOG(ERROR)<< "SpentCashRecv Deserialize error";
		LOG_ERROR("SpentCashRecv Deserialize error");
      err = REQUEST_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

CancelOrderRecv::CancelOrderRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  order_id_ = 0;
  order_type_ = 0;
}

int32 CancelOrderRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"order_id_", &order_id_);
	  if (!r)
	  {
		  LOG_ERROR("CancelOrderRecv::order_id_ parse error");
	  }
      r = dic->GetBigInteger(L"order_type_", &order_type_);
	  if (!r)
	  {
		  LOG_ERROR("CancelOrderRecv::order_type_ parse error");
	  }
    } else {
      //LOG(ERROR)<< "CancelOrderRecv Deserialize error";
	  LOG_ERROR("CancelOrderRecv Deserialize error");
      err = REQUEST_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

GtPushCommRecv::GtPushCommRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  from_uid_ = 0;
  to_uid_ = 0;
  msg_type_ = 0;
}

int32 GtPushCommRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"from_uid_", &from_uid_);
	  if (!r)
	  {
		  LOG_ERROR("GtPushCommRecv::from_uid_ parse error");
	  }
      r = dic->GetBigInteger(L"to_uid_", &to_uid_);
	  if (!r)
	  {
		  LOG_ERROR("GtPushCommRecv::to_uid_ parse error");
	  }
      r = dic->GetBigInteger(L"msg_type_", &msg_type_);
	  if (!r)
	  {
		  LOG_ERROR("GtPushCommRecv::msg_type_ parse error");
	  }
      r = dic->GetString(L"content_", &content_);
	  if (!r)
	  {
		  LOG_ERROR("GtPushCommRecv::content_ parse error");
	  }
      DicValue* msg_dic = NULL;
      r = dic->GetDictionary(L"msg_body_", &msg_dic);
	  if (!r)
	  {
		  LOG_ERROR("GtPushCommRecv::msg_body_ parse error");
	  }
      if (r) {
        base_logic::ValueSerializer* seri = base_logic::ValueSerializer::Create(
            base_logic::IMPL_JSON);
        seri->Serialize(*msg_dic, &msg_body_);
        base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                      seri);
      }
    } else {
      //LOG(ERROR)<< "GtPushCommRecv Deserialize error";
	  LOG_ERROR("GtPushCommRecv Deserialize error");
      err = REQUEST_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

PulledPushmsgRecv::PulledPushmsgRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  uid_ = 0;
}

int32 PulledPushmsgRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
	  if (!r)
	  {
		  LOG_ERROR("PulledPushmsgRecv::uid_ parse error");
	  }
    } else {
	  LOG_ERROR("GtPushCommRecv Deserialize error");
      err = REQUEST_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

}  // namespace chat
