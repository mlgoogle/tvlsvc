// Copyright (c) 2016 The tourism Authors. All rights reserved.
// user.cc
// Created on: 2016年8月19日.
// Author: Paco.

#include "pub/comm/user_info.h"
#include "glog/logging.h"

UserInfo::UserInfo() {
  data_ = new UserData();
}

UserInfo::UserInfo(const UserInfo& user) : data_(user.data_) {
  if (data_ != NULL)
    data_->AddRef();
}

UserInfo::~UserInfo() {
  LOG(INFO) << "delete ~UserInfo()";
  if (data_ != NULL) {
    data_->DelRef();
  }
}

UserInfo& UserInfo::operator=(const UserInfo& user) {
  if (data_ == user.data_)
    return *this;
  if (data_ != NULL)
    data_->DelRef();
  if (user.data_ != NULL)
    user.data_->AddRef();
  data_ = user.data_;
  return *this;
}

void UserInfo::Serialization(DicValue* dic) {
  dic->GetBigInteger(L"uid_", &data_->uid_);
  dic->GetString(L"phone_num_", &data_->phone_num_);
  dic->GetString(L"nickname_", &data_->nickname_);
  dic->GetBigInteger(L"credit_lv_", &data_->credit_lv_);
  dic->GetBigInteger(L"praise_lv_", &data_->praise_lv_);
  dic->GetBigInteger(L"cash_lv_", &data_->cash_lv_);
  dic->GetString(L"head_url_", &data_->head_url_);
  dic->GetString(L"address_", &data_->usual_addr_);
  dic->GetReal(L"longitude_", &data_->usual_lon_);
  dic->GetReal(L"latitude_", &data_->usual_lat_);
  dic->GetBigInteger(L"gender_",&data_->gender_);
}

Guide::Guide() {
  data_ = new GuideData();
}

Coordinator::Coordinator() {
  data_ = new CoordinatorData();
}
