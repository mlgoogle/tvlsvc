// Copyright (c) 2016 The tvlsvc Authors. All rights reserved.
// chat_interface.h
// Created on: 2016年8月24日.
// Author: Paco.
#ifndef PLUGINS_CHAT_CHAT_INTERFACE_H_
#define PLUGINS_CHAT_CHAT_INTERFACE_H_

#include "public/basic/basictypes.h"
#include "public/config/config.h"
#include "base/thread/base_thread_lock.h"

#include "pub/net/proto_buf.h"
#include "pub/share/data_share_mgr.h"

#include "chat/chat_mysql.h"

namespace chat {

class ChatInterface {
 private:
  ChatInterface();
  ~ChatInterface();
 public:
  static ChatInterface* GetInterface();
  static void FreeInstance();

 public:
  void InitConfig(config::FileConfig* config);

  int32 RecordChatSave();

  int32 AskInvitation(const int32 socket, PacketHead* packet);

  int32 ChatMessage(const int32 socket, PacketHead* packet);

  int32 ChatRecord(const int32 socket, PacketHead* packet);

  void SendPacket(const int socket, PacketHead* packet);

  void SendError(const int socket, PacketHead* packet, int32 err, int16 opcode);

  void SendMsg(const int socket, PacketHead* packet, DicValue* dic,
               int16 opcode);

  void InitShareDataMgr(share::DataShareMgr* data);

 private:
  static ChatInterface* instance_;
  ChatMysql* chat_mysql_;
  share::DataShareMgr* data_share_mgr_;
  std::list<std::string> msg_list_;

  threadrw_t*  lock_;
};

}  // namespace chat



#endif  // PLUGINS_CHAT_CHAT_INTERFACE_H_
