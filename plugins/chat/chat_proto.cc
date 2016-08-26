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

Invitation::Invitation(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  from_uid_ = -1;
  to_uid_ = -1;
  service_id_ = -1;
}

int32 Invitation::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer =
     base_logic::ValueSerializer::Create(base_logic::IMPL_JSON, &body_str_);
  std::string err_str;
  DicValue* dic = (DicValue*)serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"from_uid_", &from_uid_);
      LOG_IF(ERROR, !r) << "Invitation::from_uid_ parse error";
      r = dic->GetBigInteger(L"to_uid_", &to_uid_);
      LOG_IF(ERROR, !r) << "Invitation::to_uid_ parse error";
      r = dic->GetBigInteger(L"service_id_", &service_id_);
      LOG_IF(ERROR, !r) << "Invitation::service_id_ parse error";
    } else {
      LOG(ERROR) << "Invitation Deserialize error";
      err = Invitation_PACKET_JSON_ERR;
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

}  // namespace chat
