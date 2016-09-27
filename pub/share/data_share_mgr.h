// Copyright (c) 2016 The tourism Authors. All rights reserved.
// data_share_mgr.h
// Created on: 2016年8月16日.
// Author: Paco.
#ifndef PUB_SHARE_DATA_SHARE_MGR_H_
#define PUB_SHARE_DATA_SHARE_MGR_H_

#include <stdio.h>
#include <map>

#include "base/thread/base_thread_lock.h"
#include "public/basic/basictypes.h"

#include "pub/comm/user_info.h"
#include "pub/net/typedef.h"

namespace share {
__attribute__((visibility("default")))
class DataShareMgr {
 public:
  static DataShareMgr* GetInstance();
  void AddUser(UserInfo* user);
  void DelUser(int64 uid);
  void UserHeart(int64 uid);
  UserInfo* GetUser(int64 uid);

  int32 AddDeviceToken(int64 uid, std::string token); //0-新增 1-更新 -1-未修改
  int32 AddUnReadCount(int64 uid); //返回未读条数
  void DelUnReadCount(int64 uid, int32 count); //count -1 全读
  std::string GetDeviceToken(int64 uid);

  void AddNick(int64 uid, std::string nick);
  std::string GetNick(int64 uid);

  void CheckHeartLoss();
  void UserOffline(int fd);

 private:
  DataShareMgr();
  ~DataShareMgr();

  void DelGuide(int64 uid);
  void DelVisitor(int64 uid);

 private:
  static DataShareMgr* instance_;
  threadrw_t*  lock_;

  UserMap user_map_;
  GuideMap guide_map_;
  VisitorMap visitor_map_;
  DeviceTokenMap dt_map_;
  UnReadMap unread_map_;
  NickMap nick_map_;
};
}  // namespace share

extern "C" share::DataShareMgr *GetDataShareMgr(void);



#endif  // PUB_SHARE_DATA_SHARE_MGR_H_
