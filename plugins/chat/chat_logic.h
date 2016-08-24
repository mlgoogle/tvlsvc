// Copyright (c) 2016 The chat Authors. All rights reserved.
// chat_logic.h
// Created on: 2016年8月24日
// Author: Paco

#ifndef PLUGINS_CHAT_CHAT_LOGIC_H_
#define PLUGINS_CHAT_CHAT_LOGIC_H_


#include "core/common.h"

#include "chat/chat_manager.h"

namespace chat {

class Chatlogic {
 public:
  Chatlogic();
  virtual ~Chatlogic();

 private:
  static Chatlogic  *instance_;

 public:
  static Chatlogic *GetInstance();
  static void FreeInstance();

 public:
  bool OnChatConnect(struct server *srv, const int socket);
  bool OnChatMessage(struct server *srv, const int socket,
                     const void *msg, const int len);
  bool OnChatClose(struct server *srv, const int socket);
  bool OnBroadcastConnect(struct server *srv, const int socket,
                          const void *data, const int len);
  bool OnBroadcastMessage(struct server *srv, const int socket,
                          const void *msg, const int len);
  bool OnBroadcastClose(struct server *srv, const int socket);
  bool OnInitTimer(struct server *srv);
  bool OnTimeout(struct server *srv, char* id, int opcode, int time);

 private:
  bool Init();

  ChatManager* chat_manager_;
};

}  // namespace chat

#endif  // PLUGINS_CHAT_CHAT_LOGIC_H_

