// Copyright (c) 2016 The tvlsvc Authors. All rights reserved.
// chat_proto.cc
// Created on: 2016年8月24日.
// Author: Paco.
#include "chat/chat_proto.h"

#include "glog/logging.h"
#include "base/logic/base_values.h"
#include "public/basic/basic_util.h"

#include "pub/comm/comm_head.h"

namespace chat {

AskInvitationRecv::AskInvitationRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  from_uid_ = 0;
  to_uid_ = 0;
  service_id_ = 0;
}

int32 AskInvitationRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer =
     base_logic::ValueSerializer::Create(base_logic::IMPL_JSON, &body_str_);
  std::string err_str;
  DicValue* dic = (DicValue*)serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"from_uid_", &from_uid_);
      LOG_IF(ERROR, !r) << "AskInvitationRecv::from_uid_ parse error";
      r = dic->GetBigInteger(L"to_uid_", &to_uid_);
      LOG_IF(ERROR, !r) << "AskInvitationRecv::to_uid_ parse error";
      r = dic->GetBigInteger(L"service_id_", &service_id_);
      LOG_IF(ERROR, !r) << "AskInvitationRecv::service_id_ parse error";
    } else {
      LOG(ERROR) << "AskInvitationRecv Deserialize error";
      err = ASK_INVITATION_JSON_ERR;
    break;
    }
  } while (0);
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
  base_logic::ValueSerializer* serializer =
     base_logic::ValueSerializer::Create(base_logic::IMPL_JSON, &body_str_);
  std::string err_str;
  DicValue* dic = (DicValue*)serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"from_uid_", &from_uid_);
      LOG_IF(ERROR, !r) << "AnswerInvitationRecv::from_uid_ parse error";
      r = dic->GetBigInteger(L"to_uid_", &to_uid_);
      LOG_IF(ERROR, !r) << "AnswerInvitationRecv::to_uid_ parse error";
      r = dic->GetBigInteger(L"order_id_", &order_id_);
      LOG_IF(ERROR, !r) << "AnswerInvitationRecv::order_id_ parse error";
      r = dic->GetBigInteger(L"order_status_", &order_status_);
      LOG_IF(ERROR, !r) << "AnswerInvitationRecv::order_status_ parse error";
    } else {
      LOG(ERROR) << "AnswerInvitationRecv Deserialize error";
      err = ANSWER_INVITATION_JSON_ERR;
    break;
    }
  } while (0);
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
  base_logic::ValueSerializer* serializer =
     base_logic::ValueSerializer::Create(base_logic::IMPL_JSON, &body_str_);
  std::string err_str;
  DicValue* dic = (DicValue*)serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"from_uid_", &from_uid_);
      LOG_IF(ERROR, !r) << "ChatPacket::from_uid_ parse error";
      r = dic->GetBigInteger(L"to_uid_", &to_uid_);
      LOG_IF(ERROR, !r) << "ChatPacket::to_uid_ parse error";
      r = dic->GetBigInteger(L"msg_time_", &msg_time_);
      LOG_IF(ERROR, !r) << "ChatPacket::msg_time_ parse error";
      r = dic->GetString(L"content_", &content_);
      LOG_IF(ERROR, !r) << "ChatPacket::content_ parse error";
      base::BasicUtil::UrlDecode(content_, content_);
      LOG(INFO) << "from:" << from_uid_ << " to:" << to_uid_ << " content:"
          << content_;
    } else {
      LOG(ERROR) << "ChatPacket Deserialize error";
      err = CHAT_PACKET_JSON_ERR;
    break;
    }
  } while (0);
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
  base_logic::ValueSerializer* serializer =
     base_logic::ValueSerializer::Create(base_logic::IMPL_JSON, &body_str_);
  std::string err_str;
  DicValue* dic = (DicValue*)serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"from_uid_", &from_uid_);
      LOG_IF(ERROR, !r) << "ChatRecordRecv::from_uid_ parse error";
      r = dic->GetBigInteger(L"to_uid_", &to_uid_);
      LOG_IF(ERROR, !r) << "ChatRecordRecv::to_uid_ parse error";
      r = dic->GetBigInteger(L"count_", &count_);
      LOG_IF(ERROR, !r) << "ChatRecordRecv::count_ parse error";
      r = dic->GetBigInteger(L"last_chat_id_", &last_chat_id_);
      LOG_IF(ERROR, !r) << "ChatRecordRecv::last_chat_id_ parse error";
    } else {
      LOG(ERROR) << "ChatRecordRecv Deserialize error";
      err = CHAT_RECORD_JSON_ERR;
    break;
    }
  } while (0);
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
  base_logic::ValueSerializer* serializer =
     base_logic::ValueSerializer::Create(base_logic::IMPL_JSON, &body_str_);
  std::string err_str;
  DicValue* dic = (DicValue*)serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "PushMsgReadRecv::uid_ parse error";
      r = dic->GetBigInteger(L"count_", &count_);
      LOG_IF(ERROR, !r) << "PushMsgReadRecv::count_ parse error";
    } else {
      LOG(ERROR) << "PushMsgReadRecv Deserialize error";
      err = MSG_READ_JSON_ERR;
    break;
    }
  } while (0);
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
  base_logic::ValueSerializer* serializer =
     base_logic::ValueSerializer::Create(base_logic::IMPL_JSON, &body_str_);
  std::string err_str;
  DicValue* dic = (DicValue*)serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"from_uid_", &from_uid_);
      LOG_IF(ERROR, !r) << "EvaluateTripRecv::from_uid_ parse error";
      r = dic->GetBigInteger(L"to_uid_", &to_uid_);
      LOG_IF(ERROR, !r) << "EvaluateTripRecv::to_uid_ parse error";
      r = dic->GetBigInteger(L"order_id_", &order_id_);
      LOG_IF(ERROR, !r) << "EvaluateTripRecv::order_id_ parse error";
      r = dic->GetBigInteger(L"service_score_", &service_score_);
      LOG_IF(ERROR, !r) << "EvaluateTripRecv::service_score_ parse error";
      r = dic->GetBigInteger(L"user_score_", &user_score_);
      LOG_IF(ERROR, !r) << "EvaluateTripRecv::user_score_ parse error";
      r = dic->GetString(L"remarks_", &remarks_);
      LOG_IF(ERROR, !r) << "EvaluateTripRecv::remarks_ parse error";
    } else {
      LOG(ERROR) << "EvaluateTripRecv Deserialize error";
      err = EVALUATE_TRIP_JSON_ERR;
    break;
    }
  } while (0);
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
  base_logic::ValueSerializer* serializer =
     base_logic::ValueSerializer::Create(base_logic::IMPL_JSON, &body_str_);
  std::string err_str;
  DicValue* dic = (DicValue*)serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"order_id_", &order_id_);
      LOG_IF(ERROR, !r) << "EvaluateInfoRecv::order_id_ parse error";
    } else {
      LOG(ERROR) << "EvaluateInfoRecv Deserialize error";
      err = EVALUATE_INFO_JSON_ERR;
    break;
    }
  } while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                               serializer);
  return err;
}


}  // namespace chat
