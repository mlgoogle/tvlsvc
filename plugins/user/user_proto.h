// Copyright (c) 2016 The tourism Authors. All rights reserved.
// user_proto.h
// Created on: 2016年8月12日.
// Author: Paco.
#ifndef PLUGINS_USER_USER_PROTO_H_
#define PLUGINS_USER_USER_PROTO_H_

#include "pub/net/proto_buf.h"
namespace user {

//recv
class LoginRecv:public PacketHead {
 public:
  LoginRecv(PacketHead packet);
  int32 Deserialize();

  inline std::string phone_num() { return phone_num_; }
  inline std::string passwd() { return passwd_; }
  inline int64 user_type() { return user_type_; }
 private:
  std::string phone_num_;
  std::string passwd_;
  int64 user_type_;
};

class ObtainGuideRecv:public PacketHead {
 public:
  ObtainGuideRecv(PacketHead packet);
  int32 Deserialize();

  inline double longitude() { return longitude_; }
  inline double latitude() { return latitude_; }
  inline double distance() { return distance_; }
 private:
  double longitude_;
  double latitude_;
  double distance_;
};

//send



}  // namespace user



#endif  // PLUGINS_USER_USER_PROTO_H_
