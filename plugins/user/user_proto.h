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
  inline int64 recommend_type() { return recommend_type_; }
 private:
  int64 city_code_;
  int64 recommend_type_; //1-推荐 2-预约
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

class UserDetailRecv : public PacketHead {
public:
 UserDetailRecv(PacketHead packet);
 int32 Deserialize();

 inline std::string uid_str() { return uid_str_; }
private:
 std::string uid_str_; // "1,2,3,4"
};

class ObtainTripRecv : public PacketHead {
 public:
  ObtainTripRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 uid() { return uid_; }
  inline int64 order_id() { return order_id_; }
  inline int64 count() { return count_; }
 private:
  int64 uid_;
  int64 order_id_; //上次请求的最小orderid
  int64 count_; // 请求条数
};

class ServiceDetailsRecv : public PacketHead {
 public:
  ServiceDetailsRecv(PacketHead packet);
  int32 Deserialize();

  inline std::string sid_str() { return sid_str_; }
 private:
  std::string sid_str_; // "1,2,3,4"
};

class DrawBillRecv : public PacketHead {
 public:
  DrawBillRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 order_id() { return order_id_; }
  inline std::string title() { return title_; }
  inline std::string taxpayer_num() { return taxpayer_num_; }
  inline std::string company_addr() { return company_addr_; }
  inline int64 invoice_type() { return invoice_type_; }
  inline std::string user_name() { return user_name_; }
  inline std::string user_mobile() { return user_mobile_; }
  inline std::string area() { return area_; }
  inline std::string addr_detail() { return addr_detail_; }
  inline std::string remarks() { return remarks_; }
 private:
  int64 order_id_; // 订单号
  std::string title_; //发票抬头
  std::string taxpayer_num_; //纳税号
  std::string company_addr_; // 注册地址
  int64 invoice_type_; //发票类型
  std::string user_name_; //联系人
  std::string user_mobile_; //联系电话
  std::string area_; //邮寄区域
  std::string addr_detail_; //详细地址
  std::string remarks_; //备注
};

class BillRecordRecv : public PacketHead {
 public:
  BillRecordRecv(PacketHead packet);
  int32 Deserialize();
  inline int64 uid() { return uid_; }
 private:
  int64 uid_;
};

class DeviceTokenRecv : public PacketHead {
 public:
  DeviceTokenRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 uid() { return uid_; }
  inline std::string device_token() { return device_token_; }
 private:
  int64 uid_;
  std::string device_token_;
};

class BlackcardInfoRecv : public PacketHead {
 public:
  BlackcardInfoRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 uid() { return uid_; }
 private:
  int64 uid_;
};

class BlackcardConsumRecordRecv : public PacketHead {
 public:
  BlackcardConsumRecordRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 uid() { return uid_; }
 private:
  int64 uid_;
};

class NewAppointmentRecv : public PacketHead {
 public:
  NewAppointmentRecv(PacketHead packet);
  int32 Deserialize();
  inline int64 uid() { return uid_; }
  inline int64 city_code() { return city_code_; }
  inline int64 start_time() { return start_time_; }
  inline int64 end_time() { return end_time_; }
  inline std::string skills() { return skills_; }
  inline int64 is_other() { return is_other_; }
  inline std::string other_name() { return skills_; }
  int64 other_gender() { return other_gender_; }
  inline std::string ohter_phone() { return other_phone_; }
 private:
  int64 uid_;
  int64 city_code_;
  int64 start_time_;
  int64 end_time_;
  std::string skills_;
  int64 is_other_;
  std::string other_name_;
  int64 other_gender_;
  std::string other_phone_;
};

typedef Heartbeat GuideDetailRecv;
//send

}  // namespace user



#endif  // PLUGINS_USER_USER_PROTO_H_
