// Copyright (c) 2016 The chat Authors. All rights reserved.
// chat_logic.cc
// Created on: 2016年8月24日
// Author: Paco

#include "chat/chat_logic.h"

#include <assert.h>

#include "core/common.h"
#include "glog/logging.h"
#include "gtpush/IGtPush.h"
#include "public/config/config.h"
#include "public/basic/native_library.h"

#include "pub/net/proto_buf.h"
#include "pub/comm/comm_head.h"

#include "chat/chat_interface.h"
#include "chat/chat_opcode.h"

#define DEFAULT_CONFIG_PATH "./plugins/chat/chat_config.xml"
static char *host ="http://sdk.open.api.getui.net/apiex.htm";
static char *appKey = "yEIPB4YFxw64Ag9yJpaXT9";
static char *masterSecret = "bMsRgf7RrA6jBG4sNbv0F6";
namespace chat {
Chatlogic* Chatlogic::instance_ = NULL;


Chatlogic::Chatlogic() {
  if (!Init())
    assert(0);
}

Chatlogic::~Chatlogic() {
}

bool Chatlogic::Init() {
  bool r = false;
  Result res = pushInit(host, appKey, masterSecret, "编码");
   if(res!=SUCCESS){
     LOG(ERROR) << "DataShareMgr pushInit err";
   }
  chat_manager_ = ChatManager::GetInstance();
  config::FileConfig* config = config::FileConfig::GetFileConfig();
  std::string path = DEFAULT_CONFIG_PATH;
  if (config == NULL) {
    LOG(ERROR) << "Chatlogic config init error";
    return false;
  }
  r = config->LoadConfig(path);
  if (!r) {
    LOG(ERROR) << "chat config load error";
    return false;
  }
  LOG(INFO) << "chat plugin init 111";
  ChatInterface::GetInterface()->InitConfig(config);
  LOG(INFO) << "chat plugin init over";
  InitShareData();
  return true;
}

bool Chatlogic::InitShareData() {
  basic::libhandle  handle = NULL;
  handle = basic::load_native_library("./data.so");
  if (handle==NULL){
    LOG(ERROR) << "Can't load path data.so\n";
  }
  LOG(INFO) << "load data.so success";
  share::DataShareMgr* (*pengine) (void);
  pengine = (share::DataShareMgr *(*)(void))basic::get_function_pointer(handle, "GetDataShareMgr");
  if(pengine == NULL){
    LOG(ERROR) << "Can't find GetDataShareMgr\n";
    return false;
  }

  share::DataShareMgr* data_engine_ = (*pengine)();
  ChatInterface::GetInterface()->InitShareDataMgr(data_engine_);
  return false;
}

Chatlogic* Chatlogic::GetInstance() {
  if (instance_ == NULL)
    instance_ = new Chatlogic();
  return instance_;
}

void Chatlogic::FreeInstance() {
  delete instance_;
  instance_ = NULL;
}

bool Chatlogic::OnChatConnect(struct server *srv, const int socket) {
  return true;
}

bool Chatlogic::OnChatMessage(struct server *srv, const int socket,
                              const void *msg, const int len) {
  int32 err = 0;
  LOG(INFO) << "OnChatMessage:len-" << len;

  char* msg_c = new char[len + 1];
  memset(msg_c, 0, len+1);
  memcpy(msg_c, msg, len);
  PacketHead packet_head(msg_c);
  delete[] msg_c;
  msg_c = NULL;

  if (packet_head.type() == CHAT_TYPE) {
    err = chat_manager_->AssignPacket(socket, &packet_head);
    return true;
  }
  return false;
}

bool Chatlogic::OnChatClose(struct server *srv, const int socket) {
  LOG(INFO) << "OnChatClose:socket-" << socket;
  chat_manager_->OnSockClose(socket);
  LOG(INFO) << "OnSockClose:socket-" << socket;
  return true;
}

bool Chatlogic::OnBroadcastConnect(struct server *srv, const int socket,
                                   const void *msg, const int len) {
  return true;
}

bool Chatlogic::OnBroadcastMessage(struct server *srv, const int socket,
                                   const void *msg, const int len) {
  return true;
}



bool Chatlogic::OnBroadcastClose(struct server *srv, const int socket) {
  return true;
}

bool Chatlogic::OnInitTimer(struct server *srv) {
  srv->add_time_task(srv, "chat", RECORD_CHAT_MSG, 10, -1);
  return true;
}



bool Chatlogic::OnTimeout(struct server *srv, char *id, int opcode, int time) {
  switch (opcode) {
    case RECORD_CHAT_MSG: {
      chat_manager_->RecordChatSave();
      break;
    }
  }
  return true;
}

}  // namespace bigv

