// Copyright (c) 2016 The tourism Authors. All rights reserved.
// user.cc
// Created on: 2016年8月19日.
// Author: Paco.

#include "pub/comm/user.h"

User::User() {
  data_ = new UserData();
}

User::User(const User& user) : data_(user.data_) {
  if (data_ != NULL)
    data_->AddRef();
}

User::~User() {

}

User& User::operator=(const User& user) {
  if (data_ == user.data_)
    return *this;
  if (data_ != NULL)
    data_->DelRef();
  if (user.data_ != NULL)
    user.data_->AddRef();
  data_ = user.data_;
  return *this;
}

Guide::Guide() {
  data_ = new GuideData();
}
