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
    case ANSWER_INVITATION_REQ: {
      interface->AnswerInvitation(socket, packet);
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
    case CHAT_READ_REQ: {
      interface->PushMsgRead(socket, packet);
      break;
    }
    case EVALUATE_TRIP_REQ: {
      interface->EvaluateTrip(socket, packet);
      break;
    }
    case ORDER_EVALUATE_INFO_REQ: {
      interface->EvaluateInfo(socket, packet);
      break;
    }
    case FREE_COORDINATOR_REQ: {
      interface->FreeCoordinator(socket, packet);
      break;
    }
    case SPENT_CASH_REQ: {
      interface->SpentCash(socket, packet);
      break;
    }
  }
  return err;
}

int32 ChatManager::RecordChatSave() {
  return ChatInterface::GetInterface()->RecordChatSave();
}

int32 ChatManager::OnSockClose(const int fd) {
  return ChatInterface::GetInterface()->CloseSocket(fd);
}

}  // namespace chat

