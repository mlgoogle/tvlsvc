// Copyright (c) 2016 The user Authors. All rights reserved.
// user_logic.h
// Created on: 2016年8月8日
// Author: Paco

#ifndef PLUGINS_USER_USER_LOGIC_H_
#define PLUGINS_USER_USER_LOGIC_H_

#include "core/common.h"

#include "user/user_manager.h"
namespace user {

class Userlogic {
 public:
  Userlogic();
  virtual ~Userlogic();

 private:
  static Userlogic  *instance_;

 public:
  static Userlogic *GetInstance();
  static void FreeInstance();

 public:
  bool OnUserConnect(struct server *srv, const int socket);
  bool OnUserMessage(struct server *srv, const int socket,
                     const void *msg, const int len);
  bool OnUserClose(struct server *srv, const int socket);
  bool OnBroadcastConnect(struct server *srv, const int socket,
                          const void *data, const int len);
  bool OnBroadcastMessage(struct server *srv, const int socket,
                          const void *msg, const int len);
  bool OnBroadcastClose(struct server *srv, const int socket);
  bool OnInitTimer(struct server *srv);
  bool OnTimeout(struct server *srv, char* id, int opcode, int time);

 private:
  bool Init();
  UserManager* user_manager_; //负责user插件消息管理
};

}  // namespace user

#endif  // PLUGINS_USER_USER_LOGIC_H_

