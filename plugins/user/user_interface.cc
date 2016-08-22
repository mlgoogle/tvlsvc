// Copyright (c) 2016 The tourism Authors. All rights reserved.
// user_interface.cc
// Created on: 2016年8月10日.
// Author: Paco.
#include "user/user_interface.h"

#include <stdio.h>
#include <time.h>
#include <sys/socket.h>

#include "glog/logging.h"

#include "user/user_proto.h"
#include "user/user_opcode.h"
#include "pub/util/util.h"

namespace user {
UserInterface* UserInterface::instance_;

UserInterface* UserInterface::GetInstance() {
  if (instance_ == NULL)
    instance_ = new UserInterface();
  return instance_;
}

void UserInterface::FreeInstance() {
  if (instance_ == NULL) {
    delete instance_;
    instance_ = NULL;
  }
}

UserInterface::UserInterface() {
  user_mysql_ = NULL;
  data_share_mgr_ = NULL;
}

UserInterface::~UserInterface() {

}

void UserInterface::InitConfig(config::FileConfig* config) {
  LOG(INFO) << "UserInterface::InitConfig:";
  user_mysql_ = new UserMysql(config);
  data_share_mgr_ = share::DataShareMgr::GetInstance();
}

int32 UserInterface::CheckHeartLoss() {
  int32 err = 0;
  do {
    data_share_mgr_->CheckHeartLoss();
  } while (0);

  return err;
}

int32 UserInterface::HeartPacket(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  Heartbeat recv(*packet);
  do {
    err = recv.Deserialize();
    if (err < 0)
      break;
    data_share_mgr_->UserHeart(recv.uid());
  } while(0);
  return err;
}

int32 UserInterface::GuideDetail(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    //获取导游详
    GuideDetailRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->GuideDetailSelect(recv.uid(), &dic);
    if (err < 0)
      break;
    PacketHead send;
    send.set_head(packet->head());
    send.Serialize(&dic);
    send.AdapterLen();
    send.set_operate_code(GUIDE_DETAIL_RLY);
    SendPacket(socket, &send);
  } while (0);
  return err;
}

int32 UserInterface::RecommendGuide(const int32 socket, PacketHead* packet) {
  int32 err = 0;

  return err;
}

int32 UserInterface::NearbyGuide(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  ObtainGuideRecv recv(*packet);
  do {
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    double point[4];
    util::BonderOfCoordinate(recv.longitude(), recv.latitude(),
                             recv.distance(), point);
    err = user_mysql_->NearGuideSelect(point, &dic);
    if (err < 0)
      break;
    PacketHead send;
    send.set_head(packet->head());
    send.Serialize(&dic);
    send.AdapterLen();
    send.set_operate_code(NEARBY_GUIDE_RLY);
    SendPacket(socket, &send);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, NEARBY_GUIDE_RLY);
  }
  return err;
}

int32 UserInterface::UserLogin(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  LoginRecv recv(*packet);
  do {
    err = recv.Deserialize();
    if (err < 0)
      break;
    if (UserIsLogin(recv.phone_num())) {
      // todo
    }
    DicValue dic;
    err = AuthorUser(recv.phone_num(), recv.passwd(), recv.user_type(), &dic);
    LOG(INFO) << "UserLogin err:" << err;
    if (err < 0)
      break;
    PacketHead send;
    send.set_head(packet->head());
    send.Serialize(&dic);
    send.AdapterLen();
    send.set_operate_code(USER_LOGIN_RLY);
    SendPacket(socket, &send);
    AddUser(socket, &dic, recv.user_type());
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, USER_LOGIN_RLY);
  }
  LOG(INFO) << "UserLogin finish err:" << err;
  return err;
}

bool UserInterface::UserIsLogin(std::string u) {
  return false;
}

void UserInterface::AddUser(int32 fd, DicValue* v, int64 type) {
  UserInfo* user;
  //游客
  if (type == 1)
    user = new Visitor();
  else
    user = new Guide();
  user->Serialization(v);
  user->set_user_type(type);
  user->set_is_login(true);
  user->set_socket_fd(fd);
  data_share_mgr_->AddUser(user);
}

int32 UserInterface::AuthorUser(std::string phone, std::string passwd,
                                int32 type, DicValue* v) {
  int32 err = 0;
  do {
    err = user_mysql_->UserLoginSelect(phone, passwd, type, v);
    if (err < 0)
      break;
  } while (0);
  return err;
}

void UserInterface::SendPacket(const int socket, PacketHead* packet) {
  char* s = new char[packet->packet_length()];
  LOG(INFO) << "packet body:" << packet->body_str();
  memset(s, 0, packet->packet_length());
  memcpy(s, &packet->head(), HEAD_LENGTH);
  memcpy(s+HEAD_LENGTH, packet->body_str().c_str(), packet->body_str().length());
  int total = util::SendFull(socket, s, packet->packet_length());
  delete[] s;
  s = NULL;
  LOG_IF(ERROR, total != packet->packet_length()) << "send packet wrong";
}

void UserInterface::SendError(const int socket, PacketHead* packet, int32 err,
                              int16 opcode) {
  PacketErr p_err;
  p_err.set_head(packet->head());
  p_err.set_error(err);
  p_err.Serialize();
  p_err.AdapterLen();
  p_err.set_operate_code(opcode);
  SendPacket(socket, &p_err);
}

}  // namespace user

