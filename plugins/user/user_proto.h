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

class Heartbeat:public PacketHead {
 public:
  Heartbeat(PacketHead packet);
  int32 Deserialize();

  inline int64 uid() { return uid_; }
 private:
  int64 uid_;
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

class RecommendGuideRecv:public PacketHead {
 public:
  RecommendGuideRecv(PacketHead packet);
  int32 Deserialize();
  inline int64 city_code() { return city_code_; }
 private:
  int64 city_code_;
};

class ChangePasswdRecv:public PacketHead {
 public:
  ChangePasswdRecv(PacketHead packet);
  int32 Deserialize();
  inline int64 uid() { return uid_; }
  inline std::string old_passwd() { return old_passwd_; }
  inline std::string new_passwd() { return new_passwd_; }
 private:
  int64 uid_;
  std::string old_passwd_;
  std::string new_passwd_;
};

class ObtainVerifyCodeRecv : public PacketHead {
 public:
  ObtainVerifyCodeRecv(PacketHead packet);
  int32 Deserialize();
  inline int64 verify_type() { return verify_type_; }
  inline std::string phone_num() { return phone_num_; }
 private:
  int64 verify_type_;
  std::string phone_num_;
};

class RegisterAccountRecv : public PacketHead {
 public:
  RegisterAccountRecv(PacketHead packet);
  int32 Deserialize();
  inline int64 timestamp() { return timestamp_; }
  inline int64 verify_code() { return verify_code_; }
  inline std::string phone_num() { return phone_num_; }
  inline std::string passwd() { return passwd_; }
  inline std::string token() { return token_; }
  inline int64 user_type() { return user_type_; }
 private:
  int64 timestamp_;
  int64 verify_code_;
  int64 user_type_;
  std::string phone_num_;
  std::string passwd_;
  std::string token_;
};

class ImproveDataRecv : public PacketHead {
 public:
  ImproveDataRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 uid() { return uid_; }
  inline int64 gender() { return gender_; }
  inline std::string nickname() { return nickname_; }
  inline std::string head_url() { return head_url_; }
  inline std::string address() { return address_; }
  inline double longitude() { return longitude_; }
  inline double latitude() { return latitude_; }

 private:
  int64 uid_;
  int64 gender_;
  std::string nickname_;
  std::string head_url_;
  std::string address_;
  double longitude_;
  double latitude_;
};

typedef Heartbeat GuideDetailRecv;
typedef Heartbeat UserDetailRecv;
//send



}  // namespace user



#endif  // PLUGINS_USER_USER_PROTO_H_
