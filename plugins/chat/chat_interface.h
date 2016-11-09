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
#include "chat/chat_proto.h"

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

  int32 AnswerInvitation(const int32 socket, PacketHead* packet);

  int32 ChatMessage(const int32 socket, PacketHead* packet);

  int32 ChatRecord(const int32 socket, PacketHead* packet);

  int32 PushMsgRead(const int32 socket, PacketHead* packet);
//评论
  int32 EvaluateTrip(const int32 socket, PacketHead* packet);
//回去评论信息
  int32 EvaluateInfo(const int32 socket, PacketHead* packet);

  //请求空闲客服
  int32 FreeCoordinator(const int32 socket, PacketHead* packet);
//余额支付 邀约-黑卡
  int32 SpentCash(const int32 socket, PacketHead* packet);

  int32 PushChatMsg(ChatPacket rev);

  int32 PushAskMsg(AskInvitationRecv rev);

  int32 PushGtMsg(int64 from, int64 to, std::string body, std::string lockey,
                  int64 type);

  void SendPacket(const int socket, PacketHead* packet);

  void SendError(const int socket, PacketHead* packet, int32 err, int16 opcode);

  void SendMsg(const int socket, PacketHead* packet, DicValue* dic,
               int16 opcode);

  void InitShareDataMgr(share::DataShareMgr* data);

  std::string SpliceGtPushBody(std::string json, int64 type);

  int32 CloseSocket(const int fd);

 private:
  static ChatInterface* instance_;
  ChatMysql* chat_mysql_;
  share::DataShareMgr* data_share_mgr_;
  std::list<std::string> msg_list_;

  threadrw_t*  lock_;
};

}  // namespace chat



#endif  // PLUGINS_CHAT_CHAT_INTERFACE_H_
