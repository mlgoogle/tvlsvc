// Copyright (c) 2016 The tvlsvc Authors. All rights reserved.
// chat_manager.cc
// Created on: 2016年8月24日.
// Author: Paco.

#include "chat/chat_manager.h"

#include "glog/logging.h"

#include "chat/chat_opcode.h"
#include "chat/chat_interface.h"

namespace chat {

ChatManager* ChatManager::instance_ = NULL;

ChatManager* ChatManager::GetInstance() {
  if (instance_ == NULL)
    instance_ = new ChatManager();
  return instance_;
}

void ChatManager::FreeInstance() {
  if (instance_ == NULL) {
    delete instance_;
    instance_ = NULL;
  }
}

ChatManager::ChatManager() {

}

ChatManager::~ChatManager() {

}

int32 ChatManager::AssignPacket(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  ChatInterface* interface = ChatInterface::GetInterface();
  LOG(INFO) << "AssignPacket opcode" << (int)packet->operate_code();
  switch (packet->operate_code()) {
    case ASK_INVITATION_REQ: {
      interface->AskInvitation(socket, packet);
      break;
    }
    case CHAT_MESSAGE_REQ: {
      interface->ChatMessage(socket, packet);
      break;
    }
    case CHAT_RECORD_REQ: {
      interface->ChatRecord(socket, packet);
      break;
    }
  }
  return err;
}

int32 ChatManager::RecordChatSave() {
  return ChatInterface::GetInterface()->RecordChatSave();
}

}  // namespace chat

