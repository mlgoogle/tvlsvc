// Copyright (c) 2016 The tvlsvc Authors. All rights reserved.
// chat_interface.cc
// Created on: 2016年8月24日.
// Author: Paco.


#include "chat/chat_interface.h"

#include <stdio.h>
#include <time.h>
#include <sys/socket.h>

#include "glog/logging.h"

#include "chat/chat_proto.h"
#include "chat/chat_opcode.h"
#include "pub/util/util.h"

namespace chat {
ChatInterface* ChatInterface::instance_ = NULL;

ChatInterface* ChatInterface::GetInterface() {
  if (instance_ == NULL)
    instance_ = new ChatInterface();
  return instance_;
}

void ChatInterface::FreeInstance() {
  if (instance_ == NULL) {
    delete instance_;
    instance_ = NULL;
  }
}

ChatInterface::ChatInterface() {
  data_share_mgr_ = NULL;
  chat_mysql_ = NULL;
}

ChatInterface::~ChatInterface() {

}

void ChatInterface::InitConfig(config::FileConfig* config) {
  chat_mysql_ = new ChatMysql(config);
  data_share_mgr_ = share::DataShareMgr::GetInstance();
}

int32 ChatInterface::AskInvitation(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    Invitation  rev(*packet);
    err = rev.Deserialize();
    if (err < 0)
      break;
    UserInfo* u = data_share_mgr_->GetUser(rev.to_uid());
    if (u == NULL || !u->is_login()) {
      //todo 不在线处理
      LOG(INFO) << "to user is not login";
      break;
    } else {
      SendPacket(u->socket_fd(), packet);
    }

  } while (0);
  return err;
}

void ChatInterface::SendPacket(const int socket, PacketHead* packet) {


  char* s = new char[packet->packet_length()];
  LOG(INFO) << "packet body:" << packet->body_str();
  memset(s, 0, packet->packet_length());
  memcpy(s, &packet->head(), HEAD_LENGTH);
  memcpy(s+HEAD_LENGTH, packet->body_str().c_str(), packet->body_str().length());
  int total = util::SendFull(socket, s, packet->packet_length());
  delete[] s;
  s = NULL;
  LOG_IF(ERROR, total != packet->packet_length()) << "send packet wrong";
}

void ChatInterface::SendError(const int socket, PacketHead* packet, int32 err,
                              int16 opcode) {
  PacketErr p_err;
  p_err.set_head(packet->head());
  p_err.set_error(err);
  p_err.Serialize();
  p_err.AdapterLen();
  p_err.set_operate_code(opcode);
  SendPacket(socket, &p_err);
}

void ChatInterface::SendMsg(const int socket, PacketHead* packet, DicValue* dic,
             int16 opcode) {
  PacketHead send;
  send.set_head(packet->head());
  send.Serialize(dic);
  send.AdapterLen();
  send.set_operate_code(opcode);
  SendPacket(socket, &send);
}


}  // namespace chat
