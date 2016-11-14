// Copyright (c) 2016 The user Authors. All rights reserved.
// user_logic.cc
// Created on: 2016年8月8日
// Author: Paco

#include "user/user_logic.h"

#include <assert.h>
#include <typeinfo>
#include <stdio.h>
#include <string.h>

#include "glog/logging.h"
#include "gtpush/IGtPush.h"
#include "core/common.h"
#include "public/basic/basictypes.h"
#include "public/config/config.h"
#include "public/basic/native_library.h"
#include "base/logic/base_values.h"

#include "user/user_interface.h"
#include "user/user_opcode.h"
#include "pub/comm/comm_head.h"


#define DEFAULT_CONFIG_PATH "./plugins/user/user_config.xml"
static char *host ="http://sdk.open.api.getui.net/apiex.htm";
static char *appKey = "yEIPB4YFxw64Ag9yJpaXT9";
static char *masterSecret = "bMsRgf7RrA6jBG4sNbv0F6";

namespace user {
Userlogic* Userlogic::instance_ = NULL;

Userlogic::Userlogic() {
  if (!Init())
    assert(0);
}

Userlogic::~Userlogic() {
}

bool Userlogic::Init() {
  bool r = false;
  Result res = pushInit(host, appKey, masterSecret, "编码");
   if(res!=SUCCESS){
     LOG(ERROR) << "DataShareMgr pushInit err";
   }
  user_manager_ = UserManager::GetInstance();
  config::FileConfig* config = config::FileConfig::GetFileConfig();
  std::string path = DEFAULT_CONFIG_PATH;
  if (config == NULL) {
    LOG(ERROR) << "Userlogic config init error";
    return false;
  }
  r = config->LoadConfig(path);
  if (!r) {
    LOG(ERROR) << "user config load error";
    return false;
  }
  LOG(INFO) << "user plugin init 111";
  UserInterface::GetInstance()->InitConfig(config);
  LOG(INFO) << "user plugin init over";
  InitShareData();
  return true;
}

bool Userlogic::InitShareData() {
  basic::libhandle  handle = NULL;
  handle = basic::load_native_library("./data.so");
  if (handle==NULL){
    LOG(ERROR) << "Can't load path data.so\n";
  }
  LOG(INFO) << "load data.so success";
  share::DataShareMgr* (*pengine) (void);
  pengine = (share::DataShareMgr *(*)(void))basic::get_function_pointer(handle, "GetDataShareMgr");
  if(pengine==NULL){
    LOG(ERROR) << "Can't find GetDataShareMgr\n";
    return false;
  }
  share::DataShareMgr* data_engine_ = (*pengine)();
  UserInterface::GetInstance()->InitShareDataMgr(data_engine_);
  return false;
}

Userlogic* Userlogic::GetInstance() {
  if (instance_ == NULL)
    instance_ = new Userlogic();
  return instance_;
}

void Userlogic::FreeInstance() {
  delete instance_;
  instance_ = NULL;
}

bool Userlogic::OnUserConnect(struct server *srv, const int socket) {
  LOG(INFO) << "socket has be connected:" << socket;
  return true;
}

bool Userlogic::OnUserMessage(struct server *srv, const int socket,
                              const void *msg, const int len) {
  bool r = false;
  int32 err = 0;
  char* msg_c = new char[len + 1];
  memset(msg_c, 0, len+1);
  memcpy(msg_c, msg, len);
  LOG(INFO) << "OnUserMessage:len-" << len;
  PacketHead packet_head(msg_c);
  delete[] msg_c;
  msg_c = NULL;
  if (packet_head.type() == USER_TYPE) {
    err = user_manager_->AssignPacket(socket, &packet_head);
    return true;
  }
  return false;
}

bool Userlogic::OnUserClose(struct server *srv, const int socket) {
  LOG(INFO) << "socket has be closed:" << socket;
  user_manager_->OnSockClose(socket);
  LOG(INFO) << "OnSockClose:" << socket;
  return true;
}

bool Userlogic::OnBroadcastConnect(struct server *srv, const int socket,
                                   const void *msg, const int len) {
  return true;
}

bool Userlogic::OnBroadcastMessage(struct server *srv, const int socket,
                                   const void *msg, const int len
                                   ) {
  return true;
}



bool Userlogic::OnBroadcastClose(struct server *srv, const int socket) {
  return true;
}

bool Userlogic::OnInitTimer(struct server *srv) {
  srv->add_time_task(srv, "user", CONNECT_CKECK, 10, -1);
  srv->add_time_task(srv, "user", SHARE_DATA_INIT, 1, 1);
  srv->add_time_task(srv, "user", SHARE_DATA_INIT_TEN, 10*60, 1);
  return true;
}



bool Userlogic::OnTimeout(struct server *srv, char *id, int opcode, int time) {
  switch (opcode) {
    case CONNECT_CKECK: {
//      user_manager_->CheckHeartLoss();
      break;
    }
    case SHARE_DATA_INIT: {
      user_manager_->InitShareGuideData();
      break;
    }
    case SHARE_DATA_INIT_TEN: {
      user_manager_->InitShareGuideData();
      break;
    }
  }
  return true;
}

}  // namespace bigv

