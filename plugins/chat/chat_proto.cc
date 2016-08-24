// Copyright (c) 2016 The tvlsvc Authors. All rights reserved.
// chat_proto.cc
// Created on: 2016年8月24日.
// Author: Paco.
#include "chat/chat_proto.h"

#include "glog/logging.h"
#include "base/logic/base_values.h"

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
      LOG_IF(ERROR, !r) << "InvitationRecv::from_uid_ parse error";
      r = dic->GetBigInteger(L"to_uid_", &to_uid_);
      LOG_IF(ERROR, !r) << "InvitationRecv::to_uid_ parse error";
      r = dic->GetBigInteger(L"service_id_", &service_id_);
      LOG_IF(ERROR, !r) << "InvitationRecv::service_id_ parse error";
    } else {
      LOG(ERROR) << "InvitationRecv Deserialize error";
      err = Invitation_PACKET_JSON_ERR;
    break;
    }
  } while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                               serializer);
  return err;
}

//std::string InvitationSend::Serialize() {
//  DicValue* dic = new DicValue();
//  dic->SetBigInteger(L"from_uid_", from_uid_);
//  dic->SetBigInteger(L"to_uid_", to_uid_);
//  dic->SetBigInteger(L"service_id_", service_id_);
//  do{
//    if (dic == NULL || dic->empty())
//      break;
//    base_logic::ValueSerializer* serializer =
//          base_logic::ValueSerializer::Create(base_logic::IMPL_JSON);
//    serializer->Serialize(*dic, &body_str_);
//    base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
//                                                   serializer);
//  } while (0);
//  return body_str_;
//}

}  // namespace chat
