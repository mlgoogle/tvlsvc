// Copyright (c) 2016 The tourism Authors. All rights reserved.
// ref_base.h
// Created on: 2016年8月19日.
// Author: Paco.
#ifndef PUB_COMM_REF_BASE_H_
#define PUB_COMM_REF_BASE_H_

#include <stdlib.h>

class RefBase {
 public:
  RefBase():ref_count_(1) {}
  virtual ~RefBase() {};

  void AddRef() {
    __sync_fetch_and_add(&ref_count_,1);
  }
  void DelRef() {
    __sync_fetch_and_sub(&ref_count_, 1);
    if (ref_count_ == 0)
      delete this;
  }
 private:
  int ref_count_;
};

class RefPtr {
 private:
  RefBase* ptr_;
  void Init() {
    if (ptr_ == NULL)
      return;
    ptr_->AddRef();
  }
 public:
  RefPtr(RefBase* p) {
    ptr_ = p;
    Init();
  }
  RefPtr(const RefPtr& rp) {
    ptr_ = rp.ptr_;
    Init();
  }
  RefPtr& operator=(const RefPtr& rp) {
    if (ptr_ == rp.ptr_)
      return *this;
    RefBase* old = ptr_;
    ptr_ = rp.ptr_;
    Init();
    if (old)
      old->DelRef();
    return *this;
  }
  ~RefPtr() {
    if (ptr_)
      ptr_->DelRef();
  }
  RefBase* operator->() const {
    return ptr_;
  }
  RefBase& operator*() const {
    return *ptr_;
  }
};

#endif  // PUB_COMM_REF_BASE_H_
