// Copyright (c) 2016 The user Authors. All rights reserved.
// user_logic.cc
// Created on: 2016年8月8日
// Author: Paco

#include "user/user_logic.h"

#include <assert.h>
#include <typeinfo>

#include "glog/logging.h"
#include "core/common.h"
#include "public/basic/basictypes.h"
#include "public/config/config.h"
#include "base/logic/base_values.h"

#include "user/user_interface.h"
#include "user/user_opcode.h"
#include "pub/comm/comm_head.h"

#define DEFAULT_CONFIG_PATH "./plugins/user/user_config.xml"

#define CONNECT_CKECK 100
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
//  std::string str = "[{\"passwd_\":\"a12345\"}]";
//  base_logic::ValueSerializer* serializer_ =
//      base_logic::ValueSerializer::Create(0, &str);
//  std::string s;
//  int32 err = 0;
//  Value* v = serializer_->Deserialize(&err, &s);
//  if (typeid(*v) == typeid(ListValue)) {
//      LOG(INFO) << "listvalue:" ;
//  }
//  if (typeid(*v) == typeid(DicValue)) {
//      LOG(INFO) << "dicvalue:" ;
//  }
//  if (v != NULL) {
//      std::string out;
//     v->GetString(L"passwd_", &out);
//     LOG(INFO) << "json_str:" << out;
//  }
//  DicValue* body_dic_ = new DicValue();
//  body_dic_->SetBigInteger(L"uid_", 1);
//  body_dic_->SetBigInteger(L"level_", 2);
//  body_dic_->SetReal(L"balance_", 2.5);
//  body_dic_->SetString(L"user_name_", "abc");
//  std::string str;
//  base_logic::ValueSerializer* serializer_ =
//        base_logic::ValueSerializer::Create(0);
//  std::string out;
//  serializer_->Serialize(*body_dic_, &out);
//  LOG(INFO) << "json_str:" << out;
  return true;
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
  LOG(INFO) << "OnUserMessage:len-" << len;
  PacketHead packet_head(reinterpret_cast<char*>((void*)msg));
  if (packet_head.type() == USER_TYPE) {
    err = user_manager_->AssignPacket(socket, &packet_head);
    return true;
  }
  return false;
}

bool Userlogic::OnUserClose(struct server *srv, const int socket) {
  LOG(INFO) << "socket has be closed:" << socket;
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
  return true;
}



bool Userlogic::OnTimeout(struct server *srv, char *id, int opcode, int time) {
  switch (opcode) {
    case CONNECT_CKECK: {
//      user_manager_->CheckHeartLoss();
      break;
    }
  }
  return true;
}

}  // namespace bigv

