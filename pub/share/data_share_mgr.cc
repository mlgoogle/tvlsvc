// Copyright (c) 2016 The tourism Authors. All rights reserved.
// data_share_mgr.cc
// Created on: 2016年8月16日.
// Author: Paco.

#include "pub/share/data_share_mgr.h"

#include "glog/logging.h"

#include "base/logic/logic_comm.h"


namespace share {
DataShareMgr* DataShareMgr::instance_ = NULL;

DataShareMgr::DataShareMgr() {
  InitThreadrw(&lock_);
}

DataShareMgr::~DataShareMgr() {
  DeinitThreadrw(lock_);
}

DataShareMgr* DataShareMgr::GetInstance() {
  if (instance_ == NULL) {
    instance_ = new DataShareMgr();
  }
  return instance_;
}

void DataShareMgr::AddUser(UserInfo* user) {
  base_logic::WLockGd lk(lock_);
  user_map_[user->uid()] = user;
  if (user->user_type() == 1)
    visitor_map_[user->uid()] = reinterpret_cast<Visitor*>(user);
  else
    guide_map_[user->uid()] = reinterpret_cast<Guide*>(user);
}

void DataShareMgr::DelUser(int64 uid) {
  UserInfo* p_user = NULL;
  {
    base_logic::WLockGd lk(lock_);
    UserMap::iterator it1 = user_map_.find(uid);
    if (it1 != user_map_.end()) {
      p_user = it1->second;
      user_map_.erase(it1);
    }
  }
  DelGuide(uid);
  DelVisitor(uid);
  // 清理完3个map 最后释放
  if (p_user != NULL) {
    delete p_user;
    p_user = NULL;
  }
}

void DataShareMgr::CheckHeartLoss() {
  base_logic::RLockGd lk(lock_);
  UserMap::iterator it1 = user_map_.begin();
  for (; it1 != user_map_.end(); ) {
    UserInfo* p = it1->second;
    if (p == NULL) {
      ++it1;
      LOG(ERROR) << "CheckHeartLoss UserInfo NULL";
      continue;
    }
    p->set_heart_loss(p->heart_loss() + 1);
    if (p->heart_loss() < 3) {
      ++it1;
      continue;
    } else {
      LOG(WARNING) << "user lost connect uid:" << p->uid();
      user_map_.erase(it1++);
      LOG(INFO) << "uid:" << p->uid();
      DelGuide(p->uid());
      DelVisitor(p->uid());
      delete p;
      p = NULL;
    }
  }
}

void DataShareMgr::UserHeart(int64 uid) {
  base_logic::WLockGd lk(lock_);
  UserMap::iterator it1 = user_map_.find(uid);
  if (it1 != user_map_.end()) {
    UserInfo* p = it1->second;
    if (p != NULL) {
      p->set_heart_loss(0);
    }
  }
}


void DataShareMgr::DelGuide(int64 uid) {
  base_logic::WLockGd lk(lock_);
  GuideMap::iterator it1 = guide_map_.find(uid);
  if (it1 != guide_map_.end()) {
    Guide* p = it1->second;
    guide_map_.erase(it1);
  }
}

void DataShareMgr::DelVisitor(int64 uid) {
  base_logic::WLockGd lk(lock_);
  VisitorMap::iterator it1 = visitor_map_.find(uid);
  if (it1 != visitor_map_.end()) {
    Visitor* p = it1->second;
    visitor_map_.erase(it1);
  }
}

}  // namespace share
