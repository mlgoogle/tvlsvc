// Copyright (c) 2016 The tvlsvc Authors. All rights reserved.
// chat_interface.cc
// Created on: 2016年8月24日.
// Author: Paco.


#include "chat/chat_interface.h"

#include <stdio.h>
#include <time.h>
#include <sys/socket.h>

#include "glog/logging.h"
#include "base/logic/logic_comm.h"

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
  InitThreadrw(&lock_);
}

ChatInterface::~ChatInterface() {
  DeinitThreadrw(lock_);
}

void ChatInterface::InitConfig(config::FileConfig* config) {
  chat_mysql_ = new ChatMysql(config);
//  data_share_mgr_ = share::DataShareMgr::GetInstance();
}

void ChatInterface::InitShareDataMgr(share::DataShareMgr* data) {
  data_share_mgr_ = data;
}

int32 ChatInterface::RecordChatSave() {
  int32 err = 0;
  std::list<std::string> t;
  if (!msg_list_.empty()) {
    base_logic::RLockGd lk(lock_);
    t.splice(t.begin(), msg_list_);
  }
  err = chat_mysql_->ChatRecordInsert(t);
  if (err < 0) {
    base_logic::RLockGd lk(lock_);
    msg_list_.splice(msg_list_.begin(), t);
  }
  return err;
}

int32 ChatInterface::AskInvitation(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    Invitation  rev(*packet);
    err = rev.Deserialize();
    if (err < 0)
      break;
    UserInfo* u = data_share_mgr_->GetUser(rev.to_uid());
    //发起邀请，记录订单信息
    DicValue dic;
    err = chat_mysql_->NewOrderInsertAndSelect(rev.from_uid(), rev.to_uid(),
                                            rev.service_id(), &dic);
    if (err < 0)
      break;
    // 回复发起者
    SendMsg(socket, packet, &dic, ASK_INVITATION_RLY);
    if (u == NULL || !u->is_login()) {
      //todo 不在线处理
      LOG(INFO) << "invitate to user is not login";
      break;
    } else {
 //     rev.set_operate_code(ASK_INVITATION_RLY);
      // 回复被邀请者
      SendMsg(u->socket_fd(), packet, &dic, ASK_INVITATION_RLY);
    }

  } while (0);
  if (err < 0)
    SendError(socket, packet, err, ASK_INVITATION_RLY);
  return err;
}

int32 ChatInterface::ChatMessage(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    ChatPacket rev(*packet);
    err = rev.Deserialize();
    if (err < 0)
      break;
    UserInfo* u = data_share_mgr_->GetUser(rev.to_uid());
    if (u != NULL) {
       LOG(INFO) << "chat to user is not null uid:" << u->uid() << "is login:" <<
           u->is_login() ;
    }
    if (u == NULL || !u->is_login()) {
      //todo 不在线处理

      // to_id 为-1 表示意见反馈
      if (rev.to_uid() == -1) {
        LOG(INFO) << "chat to user -1";
        err = chat_mysql_->ChatRecordInsert(rev.from_uid(), rev.to_uid(),
                                            rev.content(), rev.msg_time());
      }
      break;
    } else {
      rev.set_operate_code(CHAT_MESSAGE_RLY);
      SendPacket(u->socket_fd(), &rev);
      std::stringstream ss;
      ss << "call proc_ChatRecordInsert(" << rev.from_uid() << ","
          << rev.to_uid() << ",'"
          << rev.content() << "',"
          << rev.msg_time() << ")";
      msg_list_.push_back(ss.str());
      if (msg_list_.size() > 10) {
        LOG(INFO) << "msg_list > 10";
        std::list<std::string> new_list_;
        {
          base_logic::RLockGd lk(lock_);
          new_list_.splice(new_list_.begin(), msg_list_);
        }
        LOG(INFO) << "new_list size:" << new_list_.size();
        err = chat_mysql_->ChatRecordInsert(new_list_);
        if (err < 0) {
          {
            LOG(INFO) << "new_list insert mysql err:";
            base_logic::RLockGd lk(lock_);
            msg_list_.splice(msg_list_.begin(), new_list_);
          }
        }

      }

    }
  } while (0);
  return err;
}

int32 ChatInterface::ChatRecord(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    ChatRecordRecv rev(*packet);
    err = rev.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = chat_mysql_->ChatRecordQuery(rev.from_uid(), rev.to_uid(),
                                       rev.count(), rev.last_chat_id(), &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, CHAT_RECORD_RLY);
  } while (0);
  if (err < 0)
    SendError(socket, packet, err, CHAT_RECORD_RLY);
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
