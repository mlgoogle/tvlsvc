// Copyright (c) 2016 The tvlsvc Authors. All rights reserved.
// chat_proto.h
// Created on: 2016年8月24日.
// Author: Paco.
#ifndef PLUGINS_CHAT_CHAT_PROTO_H_
#define PLUGINS_CHAT_CHAT_PROTO_H_

#include "pub/net/proto_buf.h"

namespace chat {

class Invitation : public PacketHead {
 public:
  Invitation(PacketHead packet);
  int32 Deserialize();

  int64 from_uid() { return from_uid_; }
  int64 to_uid() { return to_uid_; }
  int64 service_id() { return service_id_; }
 private:
  int64 from_uid_;
  int64 to_uid_;
  int64 service_id_;
};

//class InvitationSend : public Invitation {
// public:
//  std::string Serialize();
//
//  void set_from_uid(int64 uid) { from_uid_ = uid; }
//  void set_to_uid(int64 uid) { to_uid_ = uid; }
//  void set_service_id(int64 sid) { service_id_ = sid; }
//};

}  // namespace chat



#endif  // PLUGINS_CHAT_CHAT_PROTO_H_
