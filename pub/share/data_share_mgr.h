// Copyright (c) 2016 The tourism Authors. All rights reserved.
// data_share_mgr.h
// Created on: 2016年8月16日.
// Author: Paco.
#ifndef PUB_SHARE_DATA_SHARE_MGR_H_
#define PUB_SHARE_DATA_SHARE_MGR_H_

#include <stdio.h>
#include <map>

#include "base/thread/base_thread_lock.h"

namespace share {

class DataShareMgr {
 public:
  DataShareMgr* GetInstance();
  void AddGuide(Guide* guide);
  void AddVisitor(Visitor* visitor);
  void DelGuide(int64 uid);
  void DelVisitor(int64 uid);
 private:
  DataShareMgr();
  ~DataShareMgr();

 private:
  static DataShareMgr* instance_;
  threadrw_t*  lock_;

  UserMap user_map_;
  GuideMap guide_map_;
  VisitorMap visitor_map_;
};

}  // namespace share



#endif  // PUB_SHARE_DATA_SHARE_MGR_H_
