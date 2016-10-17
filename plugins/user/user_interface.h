// Copyright (c) 2016 The tourism Authors. All rights reserved.
// user_interface.h
// Created on: 2016年8月10日.
// Author: Paco.
#ifndef PLUGINS_USER_USER_INTERFACE_H_
#define PLUGINS_USER_USER_INTERFACE_H_

#include "public/basic/basictypes.h"
#include "public/config/config.h"
#include "pub/net/proto_buf.h"
#include "pub/share/data_share_mgr.h"

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

  int32 HeartPacket(const int32 socket, PacketHead* packet);

  int32 GuideDetail(const int32 socket, PacketHead* packet);

  int32 ObtainUserInfo(const int32 socket, PacketHead* packet);

  int32 RecommendGuide(const int32 socket, PacketHead* packet);

  int32 ServiceCity(const int32 socket, PacketHead* packet);

  int32 ChangePasswd(const int32 socket, PacketHead* packet);

  int32 AlipayServer(const int32 socket, PacketHead* packet);

  int32 AlipayClient(const int32 socket, PacketHead* packet);

  int32 ObtainVerifyCode(const int32 socket, PacketHead* packet);

  int32 RegisterAccount(const int32 socket, PacketHead* packet);

  int32 ImproveUserData(const int32 socket, PacketHead* packet);

  int32 ObtainTripRecord(const int32 socket, PacketHead* packet);

  int32 ObtainServiceInfo(const int32 socket, PacketHead* packet);

  int32 DrawBillTrip(const int32 socket, PacketHead* packet);

  int32 BillingRecord(const int32 socket, PacketHead* packet);

  int32 DeviceToken(const int32 socket, PacketHead* packet);

  int32 AuthorUser(std::string phone, std::string passwd, int32 type,
                   DicValue* v);

  int32 CheckHeartLoss();
  int32 CloseSocket(const int fd);

  void AddUser(int32 fd, DicValue* v, int64 type, std::string pwd);

  //判断用户是否登陆
  bool UserIsLogin(std::string u);

  void SendPacket(const int socket, PacketHead* packet);

  void SendError(const int socket, PacketHead* packet, int32 err, int16 opcode);

  void SendMsg(const int socket, PacketHead* packet, DicValue* dic,
               int16 opcode);

  void InitShareDataMgr(share::DataShareMgr* data);
 private:
  static UserInterface* instance_;
  UserMysql* user_mysql_;
  share::DataShareMgr* data_share_mgr_;
};

}  // namespace user



#endif  // PLUGINS_USER_USER_INTERFACE_H_
