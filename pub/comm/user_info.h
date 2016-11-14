// Copyright (c) 2016 The tourism Authors. All rights reserved.
// user.h
// Created on: 2016年8月19日.
// Author: Paco.
#ifndef PUB_COMM_USER_INFO_H_
#define PUB_COMM_USER_INFO_H_

#include <string>
#include <list>

#include "public/basic/basictypes.h"
#include "base/logic/base_values.h"

#include "pub/comm/ref_base.h"

typedef base_logic::DictionaryValue DicValue;

class UserData : public RefBase {
 public:
  UserData():uid_(-1), usual_lon_(0.0), usual_lat_(0.0), credit_lv_(0),
    praise_lv_(0), cash_lv_(0), user_type_(-1), socket_fd_(-1), heart_loss_(0),
    gender_(0), is_login_(false) {
  }
  virtual ~UserData() {}
 public:
  int64 uid_;
  std::string phone_num_;
  std::string nickname_;
  std::string passwd_;
  std::string head_url_;
  std::string usual_addr_;
  double usual_lon_;
  double usual_lat_;
  //信用度
  int64 credit_lv_;
  int64 praise_lv_;
  int64 cash_lv_;

  int32 user_type_;
  int32 socket_fd_;
  int32 heart_loss_;
  int64 gender_;
  bool is_login_;
};

class GuideData : public UserData {
 public:
  GuideData() {
    UserData();
    business_lv_ = 0;
  }
 public:
  int32 business_lv_;
  std::string travel_label_;
  std::string business_label_;
  std::list<std::string> photo_list_;
};

//客服
class CoordinatorData : public UserData {
 public:
  CoordinatorData() {
    UserData();
    customers_num_ = 0;
  }
 public:
  int32 customers_num_;
};

class UserInfo {
 public:
  UserInfo();
  virtual ~UserInfo();
  UserInfo(const UserInfo& user);
  UserInfo& operator=(const UserInfo& user);

  void Serialization(DicValue* dic);
  inline int64 uid() const { return data_->uid_; }
  inline std::string phone_num() const { return data_->phone_num_; }
  inline std::string nickname() const { return data_->nickname_; }
  inline std::string passwd() const { return data_->passwd_; }
  inline std::string head_url() const { return data_->head_url_; }
  inline std::string usual_addr() const { return data_->usual_addr_; }
  inline double usual_lon() const { return data_->usual_lon_; }
  inline double usual_lat() const { return data_->usual_lat_; }
  inline int32 credit_lv() const { return data_->credit_lv_; }
  inline int32 praise_lv() const { return data_->praise_lv_; }
  inline int32 cash_lv() const { return data_->cash_lv_; }
  inline int32 user_type() const { return data_->user_type_; }
  inline bool is_login() const { return data_->is_login_; }
  inline int32 socket_fd() const { return data_->socket_fd_; }
  inline int32 heart_loss() const { return data_->heart_loss_; }
  inline int64 gender() const { return data_->gender_; }

  inline void set_uid(int64 id) { data_->uid_ = id; }
  inline void set_phone_num(std::string mob) { data_->phone_num_ = mob; }
  inline void set_nickname(std::string nm) { data_->nickname_ = nm; }
  inline void set_passwd(std::string pwd) { data_->passwd_ = pwd; }
  inline void set_head_url(std::string h) { data_->head_url_ = h; }
  inline void set_usual_addr(std::string addr) { data_->usual_addr_ = addr; }
  inline void set_usual_lon(double lon) { data_->usual_lon_ = lon; }
  inline void set_usual_lat(double lat) { data_->usual_lat_ = lat; }
  inline void set_credit_lv(int32 credit) { data_->credit_lv_ = credit; }
  inline void set_praise_lv(int32 praise) { data_->praise_lv_ = praise; }
  inline void set_cash_lv(int32 cash) { data_->cash_lv_ = cash; }
  inline void set_user_type(int32 type) { data_->user_type_ = type; }
  inline void set_is_login(bool login) { data_->is_login_ = login; }
  inline void set_socket_fd(int32 fd) { data_->socket_fd_ = fd; }
  inline void set_heart_loss(int32 loss) { data_->heart_loss_ = loss; }
  inline void set_gender(int64 sex) { data_->gender_ = sex; }
 public:
  UserData* data_;
};

class Guide : public UserInfo {
 public:
  Guide();

  inline int32 bussiness_lv() {
    return ((GuideData*)data_)->business_lv_;
  }
  inline std::string travel_label() const {
    return ((GuideData*)data_)->travel_label_;
  }
  inline std::string business_label() const {
    return ((GuideData*)data_)->business_label_;
  }
  inline std::list<std::string> photo_list() const {
    return ((GuideData*)data_)->photo_list_;
  }
  inline void set_business_lv(int32 lv) {
    ((GuideData*)data_)->business_lv_ = lv;
  }
  inline void set_travel_label(std::string lab) {
    ((GuideData*)data_)->travel_label_ = lab;
  }
  inline void set_business_label(std::string lab) {
    ((GuideData*)data_)->business_label_ = lab;
  }
  inline void AddPhone(std::string url) {
    ((GuideData*)data_)->photo_list_.push_back(url);
  }
};

class Visitor : public UserInfo {

};

class Coordinator : public UserInfo {
 public:
  Coordinator();
  inline int32 customers_num() {
    return  ((CoordinatorData*)data_)->customers_num_;
  }
  inline void add_customers() {
    ((CoordinatorData*)data_)->customers_num_++;
  }
  inline void del_customers() {
    ((CoordinatorData*)data_)->customers_num_ --;
    if (((CoordinatorData*)data_)->customers_num_ < 0)
      ((CoordinatorData*)data_)->customers_num_ = 0;
  }
};


#endif  // PUB_COMM_USER_INFO_H_
