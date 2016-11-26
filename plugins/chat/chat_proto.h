// Copyright (c) 2016 The tvlsvc Authors. All rights reserved.
// chat_proto.h
// Created on: 2016年8月24日.
// Author: Paco.
#ifndef PLUGINS_CHAT_CHAT_PROTO_H_
#define PLUGINS_CHAT_CHAT_PROTO_H_

#include "pub/net/proto_buf.h"

namespace chat {

class AskInvitationRecv : public PacketHead {
 public:
  AskInvitationRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 from_uid() {
    return from_uid_;
  }
  inline int64 to_uid() {
    return to_uid_;
  }
  inline int64 service_id() {
    return service_id_;
  }
  inline int64 day_count() {
    return day_count_;
  }
 private:
  int64 from_uid_;
  int64 to_uid_;
  int64 service_id_;
  int64 day_count_;
};

class AppointMentGuideRecv : public PacketHead {
 public:
  AppointMentGuideRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 from_uid() {
    return from_uid_;
  }
  inline int64 to_uid() {
    return to_uid_;
  }
  inline int64 service_id() {
    return service_id_;
  }
  inline int64 appointment_id() {
    return appointment_id_;
  }
 private:
  int64 from_uid_;
  int64 to_uid_;
  int64 service_id_;
  int64 appointment_id_;
};

class AnswerInvitationRecv : public PacketHead {
 public:
  AnswerInvitationRecv(PacketHead packet);

  int32 Deserialize();

  inline int64 from_uid() {
    return from_uid_;
  }
  inline int64 to_uid() {
    return to_uid_;
  }
  inline int64 order_id() {
    return order_id_;
  }
  inline int64 order_status() {
    return order_status_;
  }
 private:
  int64 from_uid_;
  int64 to_uid_;
  int64 order_id_;
  int64 order_status_;   // 0-发起邀请1-被导游拒绝.2-接收邀请。3-等待支付。4-支付完成，5-取消支付
};

class ChatPacket : public PacketHead {
 public:
  ChatPacket(PacketHead packet);
  int32 Deserialize();

  inline int64 from_uid() {
    return from_uid_;
  }
  inline int64 to_uid() {
    return to_uid_;
  }
  inline int64 msg_time() {
    return msg_time_;
  }
  inline std::string content() {
    return content_;
  }
 private:
  int64 from_uid_;
  int64 to_uid_;
  int64 msg_time_;
  std::string content_;
};

class ChatRecordRecv : public PacketHead {
 public:
  ChatRecordRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 from_uid() {
    return from_uid_;
  }
  inline int64 to_uid() {
    return to_uid_;
  }
  inline int64 count() {
    return count_;
  }
  inline int64 last_chat_id() {
    return last_chat_id_;
  }
 private:
  int64 from_uid_;
  int64 to_uid_;
  int64 count_;
  int64 last_chat_id_;
};

class PushMsgReadRecv : public PacketHead {
 public:
  PushMsgReadRecv(PacketHead packet);
  int32 Deserialize();
  inline int64 uid() {
    return uid_;
  }
  inline int64 count() {
    return count_;
  }
 private:
  int64 uid_;
  int64 count_;
};

class EvaluateTripRecv : public PacketHead {
 public:
  EvaluateTripRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 from_uid() {
    return from_uid_;
  }
  inline int64 to_uid() {
    return to_uid_;
  }
  inline int64 order_id() {
    return order_id_;
  }
  inline int64 service_score() {
    return service_score_;
  }
  inline int64 user_score() {
    return user_score_;
  }
  inline std::string remarks() {
    return remarks_;
  }
 private:
  int64 from_uid_;  //游客id
  int64 to_uid_;  //导游id
  int64 order_id_;  //订单id
  int64 service_score_;   //服务评价
  int64 user_score_;  //服务者评分
  std::string remarks_;  //评论
};

class EvaluateInfoRecv : public PacketHead {
 public:
  EvaluateInfoRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 order_id() {
    return order_id_;
  }
 private:
  int64 order_id_;
};

class SpentCashRecv : public PacketHead {
 public:
  SpentCashRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 uid() {
    return uid_;
  }
  inline int64 order_id() {
    return order_id_;
  }
  std::string passwd() {
    return passwd_;
  }
 private:
  int64 uid_;
  int64 order_id_;
  std::string passwd_;
};

class CancelOrderRecv : public PacketHead {
 public:
  CancelOrderRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 order_type() {
    return order_type_;
  }
  inline int64 order_id() {
    return order_id_;
  }
 private:
  int64 order_type_;
  int64 order_id_;
};

class GtPushCommRecv : public PacketHead {
 public:
  GtPushCommRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 from_uid() {
    return from_uid_;
  }
  inline int64 to_uid() {
    return to_uid_;
  }
  inline int64 msg_type() {
    return msg_type_;
  }
  inline std::string msg_body() {
    return msg_body_;
  }
  inline std::string content() {
    return content_;
  }
 private:
  int64 from_uid_;
  int64 to_uid_;
  int64 msg_type_;  //需要推送消息对应协议操作码
  std::string msg_body_;
  std::string content_;
};

}  // namespace chat

#endif  // PLUGINS_CHAT_CHAT_PROTO_H_
