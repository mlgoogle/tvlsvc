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
  user_mysql_ = new UserMysql(config);
//  data_share_mgr_ = share::DataShareMgr::GetInstance();
}

void UserInterface::InitShareDataMgr(share::DataShareMgr* data) {
  data_share_mgr_ = data;
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
  Heartbeat rev(*packet);
  do {
    err = rev.Deserialize();
    if (err < 0)
      break;
    data_share_mgr_->UserHeart(rev.uid());
  } while(0);
  return err;
}

int32 UserInterface::ObtainUserInfo(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    UserDetailRecv rev(*packet);
    err = rev.Deserialize();
    if (err < 0)
      break;
    DicValue dict;
    UserInfo* u = data_share_mgr_->GetUser(rev.uid());
    if (u != NULL) {
      dict.SetBigInteger(L"uid_", u->uid());
      dict.SetString(L"phone_num_", u->phone_num());
      dict.SetString(L"nickname_", u->nickname());
      dict.SetBigInteger(L"credit_lv_", u->credit_lv());
      dict.SetBigInteger(L"praise_lv_", u->praise_lv());
      dict.SetBigInteger(L"cash_lv_", u->cash_lv());
      dict.SetString(L"head_url_", u->head_url());
      dict.SetString(L"address_", u->usual_addr());
      dict.SetReal(L"longitude_", u->usual_lon());
      dict.SetReal(L"latitude_", u->usual_lat());
      SendMsg(socket, packet, &dict, USER_INFO_RLY);
      break;
    }
    err = user_mysql_->UserDetailSelect(rev.uid(), &dict);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dict, USER_INFO_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, USER_INFO_RLY);
  }
  return err;
}


int32 UserInterface::GuideDetail(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    //获取导游详
    GuideDetailRecv rev(*packet);
    err = rev.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->GuideDetailSelect(rev.uid(), &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, GUIDE_DETAIL_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, GUIDE_DETAIL_RLY);
  }
  return err;
}

int32 UserInterface::ChangePasswd(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    ChangePasswdRecv rev(*packet);
    err = rev.Deserialize();
    if (err < 0)
      break;
    UserInfo* p = data_share_mgr_->GetUser(rev.uid());
    if (p == NULL || !p->is_login()) {
      err = USER_NOT_IN_CACHE;
      break;
    }
    LOG(INFO) << "oldpwd:" << rev.old_passwd();
    LOG(INFO) << "pwd" << p->passwd();
    if (p->passwd() != rev.old_passwd()) {
      err = CHANGE_OLD_PWD_ERR;
      break;
    } else {
      err = user_mysql_->ChangePwdUpdate(rev.uid(), rev.new_passwd());
      if (err < 0)
        break;
      p->set_passwd(rev.new_passwd());
      SendMsg(socket, packet, NULL, CHANGE_PASSWD_RLY);
    }
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, CHANGE_PASSWD_RLY);
  }
  return err;
}

int32 UserInterface::ServiceCity(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    DicValue dic;
    err = user_mysql_->ServiceCitySelect(&dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, SERVICE_CITY_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, SERVICE_CITY_RLY);
  }
  return err;
}

int32 UserInterface::RecommendGuide(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  RecommendGuideRecv rev(*packet);
  do {
    err = rev.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->RecommendGuideSelect(rev.city_code(), &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, GUIDE_RECOMMEND_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, GUIDE_RECOMMEND_RLY);
  }
  return err;
}

int32 UserInterface::NearbyGuide(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  ObtainGuideRecv rev(*packet);
  do {
    err = rev.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    double point[4];
    util::BonderOfCoordinate(rev.longitude(), rev.latitude(),
                             rev.distance(), point);
    err = user_mysql_->NearGuideSelect(point, &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, NEARBY_GUIDE_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, NEARBY_GUIDE_RLY);
  }
  return err;
}

int32 UserInterface::UserLogin(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  LoginRecv rev(*packet);
  do {
    err = rev.Deserialize();
    if (err < 0)
      break;
    if (UserIsLogin(rev.phone_num())) {
      // todo
    }
    DicValue dic;
    err = AuthorUser(rev.phone_num(), rev.passwd(), rev.user_type(), &dic);
    LOG(INFO) << "UserLogin err:" << err;
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, USER_LOGIN_RLY);
    AddUser(socket, &dic, rev.user_type(), rev.passwd());
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

void UserInterface::AddUser(int32 fd, DicValue* v, int64 type,
                            std::string pwd) {
  UserInfo* user = NULL;
  //游客
  if (type == 1)
    user = new Visitor();
  else
    user = new Guide();
  user->Serialization(v);
  user->set_user_type(type);
  user->set_is_login(true);
  user->set_socket_fd(fd);
  user->set_passwd(pwd);
  LOG(INFO) << "adduser login :" << user->uid();
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
  p_err.set_type(ERROR_TYPE);
  p_err.set_error(err);
  p_err.Serialize();
  p_err.AdapterLen();
  p_err.set_operate_code(opcode);
  SendPacket(socket, &p_err);
}

void UserInterface::SendMsg(const int socket, PacketHead* packet, DicValue* dic,
             int16 opcode) {
  PacketHead send;
  send.set_head(packet->head());
  send.Serialize(dic);
  send.AdapterLen();
  send.set_operate_code(opcode);
  SendPacket(socket, &send);
}

}  // namespace user

