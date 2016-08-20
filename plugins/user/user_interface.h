// Copyright (c) 2016 The tourism Authors. All rights reserved.
// user_interface.h
// Created on: 2016年8月10日.
// Author: Paco.
#ifndef PLUGINS_USER_USER_INTERFACE_H_
#define PLUGINS_USER_USER_INTERFACE_H_

#include "public/basic/basictypes.h"
#include "public/config/config.h"
#include "pub/net/proto_buf.h"

#include "user/user_mysql.h"
namespace user {

class UserInterface {
 private:
  UserInterface();
  ~UserInterface();

 public:
  static UserInterface* GetInstance();
  static void FreeInstance();

 public:
  void InitConfig(config::FileConfig* config);

  int32 UserLogin(const int32 socket, PacketHead* packet);

  int32 NearbyGuide(const int32 socket, PacketHead* packet);

  int32 AuthorUser(std::string phone, std::string passwd, int32 type,
                   DicValue* v);

  //判断用户是否登陆
  bool UserIsLogin(std::string u);

  void SendPacket(const int socket, PacketHead* packet);

 private:
  static UserInterface* instance_;
  UserMysql* user_mysql_;
};

}  // namespace user



#endif  // PLUGINS_USER_USER_INTERFACE_H_
