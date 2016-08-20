// Copyright (c) 2016 The tourism Authors. All rights reserved.
// data_share_mgr.cc
// Created on: 2016年8月16日.
// Author: Paco.

#include "pub/share/data_share_mgr.h"


namespace share {
static DataShareMgr* DataShareMgr::instance_ = NULL;

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

void DataShareMgr::AddGuide(Guide* guide) {
  base_logic::WLockGd lk(lock_);
  guide_map_[guide->uid()] = guide;
  user_map_[guide->uid()] = guide;
}

void DataShareMgr::AddVisitor(Visitor* visitor) {
  base_logic::WLockGd lk(lock_);
  visitor_map_[visitor->uid()] = visitor;
  user_map_[visitor->uid()] = visitor;
}

void DataShareMgr::DelGuide(int64 uid) {
  base_logic::WLockGd lk(lock_);
  GuideMap::iterator it1 = guide_map_.find(uid);
  if (it1 != guide_map_.end()) {
    Guide* p = it1->second;
    guide_map_.erase(it1);
    if (p != NULL) {
      delete p;
      p = NULL;
    }
  }
  UserMap::iterator it2 = user_map_.find(uid);
  if (it2 != user_map_.end()) {
    UserMap* p = it2->second;
    user_map_.erase(it2);
    if (p != NULL) {
      delete p;
      p = NULL;
    }
  }
}

void DataShareMgr::DelVisitor(int64 uid) {
  base_logic::WLockGd lk(lock_);
  VisitorMap::iterator it1 = visitor_map_.find(uid);
  if (it1 != visitor_map_.end()) {
    Visitor* p = it1->second;
    visitor_map_.erase(it1);
    if (p != NULL) {
      delete p;
      p = NULL;
    }
  }
  UserMap::iterator it2 = user_map_.find(uid);
  if (it2 != user_map_.end()) {
    UserMap* p = it2->second;
    user_map_.erase(it2);
    if (p != NULL) {
      delete p;
      p = NULL;
    }
  }
}

}  // namespace share
