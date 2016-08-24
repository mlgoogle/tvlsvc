// Copyright (c) 2016 The tvlsvc Authors. All rights reserved.
// chat_manager.h
// Created on: 2016年8月24日.
// Author: Paco.
#ifndef PLUGINS_CHAT_CHAT_MANAGER_H_
#define PLUGINS_CHAT_CHAT_MANAGER_H_

#include "basic/basictypes.h"

#include "pub/net/proto_buf.h"

namespace chat {

class ChatManager {
 private:
  ChatManager();
  ~ChatManager();

  static ChatManager* instance_;
 public:
  static ChatManager* GetInstance();
  static void FreeInstance();

  int32 AssignPacket(const int32 socket, PacketHead* packet);
};

}  // namespace chat



#endif  // PLUGINS_CHAT_CHAT_MANAGER_H_
