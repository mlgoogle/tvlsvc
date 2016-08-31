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

  inline int64 from_uid() { return from_uid_; }
  inline int64 to_uid() { return to_uid_; }
  inline int64 service_id() { return service_id_; }
 private:
  int64 from_uid_;
  int64 to_uid_;
  int64 service_id_;
};

class ChatPacket : public PacketHead {
 public:
  ChatPacket(PacketHead packet);
  int32 Deserialize();

  inline int64 from_uid() { return from_uid_; }
  inline int64 to_uid() { return to_uid_; }
  inline int64 msg_time() { return msg_time_; }
  inline std::string content() { return content_; }
 private:
  int64 from_uid_;
  int64 to_uid_;
  int64 msg_time_;
  std::string content_;
};

class ChatRecordRecv : public PacketHead {
 public:
  ChatRecordRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 from_uid() { return from_uid_; }
  inline int64 to_uid() { return to_uid_; }
  inline int64 count() { return count_; }
  inline int64 last_chat_id() { return last_chat_id_; }
 private:
  int64 from_uid_;
  int64 to_uid_;
  int64 count_;
  int64 last_chat_id_;
};

}  // namespace chat



#endif  // PLUGINS_CHAT_CHAT_PROTO_H_
