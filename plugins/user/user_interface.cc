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
}

UserInterface::~UserInterface() {

}

void UserInterface::InitConfig(config::FileConfig* config) {
  LOG(INFO) << "UserInterface::InitConfig:";
  user_mysql_ = new UserMysql(config);
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
    PacketHead send;
    send.set_head(packet->head());
    send.Serialize(&dic);
    send.AdapterLen();
    send.set_operate_code(NEARBY_WAITER_RLY);
    SendPacket(socket, &send);
  } while (0);
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
  } while (0);
  if (err < 0) {
    PacketErr p_err;
    p_err.set_head(packet->head());
    p_err.set_error(err);
    p_err.Serialize();
    p_err.AdapterLen();
    p_err.set_operate_code(USER_LOGIN_RLY);
    SendPacket(socket, &p_err);
  }
  LOG(INFO) << "UserLogin finish err:" << err;
  return err;
}

bool UserInterface::UserIsLogin(std::string u) {
  return false;
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

}  // namespace user

