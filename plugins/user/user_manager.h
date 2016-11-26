// Copyright (c) 2016 The tourism Authors. All rights reserved.
// user_manager.h
// Created on: 2016年8月10日.
// Author: Paco.
#ifndef PLUGINS_USER_USER_MANAGER_H_
#define PLUGINS_USER_USER_MANAGER_H_
//用户插件的消息分发管理器，处理消息的解析，分配

#include "basic/basictypes.h"

#include "pub/net/proto_buf.h"


namespace user {

class UserManager {
 private:
  UserManager();
  ~UserManager();

  static UserManager* instance_;
 public:
  static UserManager* GetInstance();
  static void FreeInstance();

  int32 AssignPacket(const int32 socket, PacketHead* packet);
  int32 CheckHeartLoss();
  int32 InitShareGuideData();
  int32 OnSockClose(const int fd);
  int32 OrderStatusCheck();
};

}  // namespace user



#endif  // PLUGINS_USER_USER_MANAGER_H_
