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
#include "public/basic/basic_util.h"

#include "chat/chat_proto.h"
#include "chat/chat_opcode.h"
#include "pub/util/util.h"
#include "pub/comm/comm_head.h"

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
//  util::PushApnChatMsg("26e19d9dea4ad267c32ab06d0a0a9c65b9131607f54cc97d09216b092db24ef6",
//                                  4,
//                                  "QQ:哈哈", "多手持");
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

int32 ChatInterface::AnswerInvitation(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    AnswerInvitationRecv rev(*packet);
    LOG(INFO) << "AnswerInvitation";
    err = rev.Deserialize();
    if (err < 0)
      break;
    //更新订单状态
    LOG(INFO) << "before GuideOrderStatusUpdate";
    err = chat_mysql_->GuideOrderStatusUpdate(rev.order_id(), rev.order_status());
    LOG(INFO) << "after GuideOrderStatusUpdate";
    if (err < 0)
      break;
    rev.set_operate_code(ANSWER_INVITATION_RLY);
    //回复导游（受邀人）
    SendPacket(socket, &rev);
    //回复游客（发起人）
    UserInfo* u = data_share_mgr_->GetUser(rev.from_uid());
    if (u == NULL || !u->is_login()) {
      std::string content;
      if (rev.order_status() == 3)
        content = " 愿与你同游";
      else if (rev.order_status() == 1)
        content = " 残忍拒绝";
      else if (rev.order_status() == 6)
        content = " 服务开始了";
      else if (rev.order_status() == 7)
        content = " 服务已完成";
      PushGtMsg(rev.to_uid(), rev.from_uid(), rev.body_str(), content,
      ANSWER_INVITATION_RLY);
      break;
    } else {
      SendPacket(u->socket_fd(), &rev);
    }
  } while (0);
  if (err < 0)
    SendError(socket, packet, err, ANSWER_INVITATION_RLY);
  return err;
}

int32 ChatInterface::AskInvitation(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    AskInvitationRecv rev(*packet);
    err = rev.Deserialize();
    if (err < 0)
      break;
    UserInfo* u = data_share_mgr_->GetUser(rev.to_uid());
    //发起邀请，记录订单信息
    DicValue dic;
    err = chat_mysql_->NewOrderInsertAndSelect(rev.from_uid(), rev.to_uid(),
                                               rev.service_id(),
                                               rev.day_count(), &dic);
    if (err < 0)
      break;
    int64 result = 0;
    // result 0-成功邀约  result=1已经邀约过
    dic.GetBigInteger(L"is_asked_", &result);
    // 回复发起者
    SendMsg(socket, packet, &dic, ASK_INVITATION_RLY);
    if (result == 1)
      break;
    // 通知被邀约者
    /*/========================================
    // todo 测试用 默认服务者同意
    int64 oid;
    int64 oid_status;
    dic.GetBigInteger(L"order_id_", &oid);
    LOG(INFO)<< "change order status oid:" << oid;
    chat_mysql_->OrderStatusUpdate(oid, 3);
    ============================*/
    if (u == NULL || !u->is_login()) {
      LOG(INFO)<< "invitate to user is not login";
      // 回复被邀请者
      PushGtMsg(rev.from_uid(), rev.to_uid(), rev.body_str(), " 邀你同游",
          ASK_INVITATION_RLY);
      LOG(INFO) << "PushGtMsg";
      break;
    } else {
      //     rev.set_operate_code(ASK_INVITATION_RLY);
      // 回复被邀请者
      SendMsg(u->socket_fd(), packet, &dic, ASK_INVITATION_RLY);
    }
    //
  }while (0);
  if (err < 0)
    SendError(socket, packet, err, ASK_INVITATION_RLY);
  return err;
}

int32 ChatInterface::AppointMentGuide(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    AppointMentGuideRecv rev(*packet);
    err = rev.Deserialize();
    if (err < 0)
      break;
    UserInfo* u = data_share_mgr_->GetUser(rev.to_uid());
    //发起邀请，记录订单信息
    DicValue dic;
    err = chat_mysql_->NewAppointmentOrderInsertAndSelect(rev.from_uid(),
                                                          rev.to_uid(),
                                                          rev.service_id(),
                                                          rev.appointment_id(),
                                                          &dic);
    if (err < 0)
      break;
    int64 result = 0;
    // result 0-成功预约  result=1已经预约过
    dic.GetBigInteger(L"is_asked_", &result);
    // 回复发起者
    SendMsg(socket, packet, &dic, APPOINTMENT_GUIDE_RLY);
    if (result == 1)
      break;
    // 通知被邀约者
    /*/========================================
    // todo 测试用 默认服务者同意
    int64 oid;
    int64 oid_status;
    dic.GetBigInteger(L"order_id_", &oid);
    LOG(INFO)<< "change order status oid:" << oid;
    chat_mysql_->OrderStatusUpdate(oid, 3);
    ============================*/
    if (u == NULL || !u->is_login()) {
      LOG(INFO)<< "appointment to user is not login";
      // 回复被邀请者
      PushGtMsg(rev.from_uid(), rev.to_uid(), rev.body_str(), " 邀你同游",
          APPOINTMENT_GUIDE_RLY);
      LOG(INFO) << "PushGtMsg";
      break;
    } else {
      //     rev.set_operate_code(ASK_INVITATION_RLY);
      // 回复被邀请者
      SendMsg(u->socket_fd(), packet, &dic, APPOINTMENT_GUIDE_RLY);
    }
    //
  }while (0);
  if (err < 0)
    SendError(socket, packet, err, APPOINTMENT_GUIDE_RLY);
  return err;
}

int32 ChatInterface::ChatMessage(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  ChatPacket rev(*packet);
  do {
    err = rev.Deserialize();
    if (err < 0)
      break;
    UserInfo* u = data_share_mgr_->GetUser(rev.to_uid());
    if (u != NULL) {
      LOG(INFO)<< "chat to user is not null uid:" << u->uid() << "is login:" <<
      u->is_login();
    }
    if (u == NULL || !u->is_login()) {
      // to_id 为-1 表示意见反馈
      if (rev.to_uid() == -1) {
        LOG(INFO)<< "chat to user -1";
        err = chat_mysql_->ChatRecordInsert(rev.from_uid(), rev.to_uid(),
            rev.content(), rev.msg_time(), 0);
        break;
      }
      //     PushChatMsg(rev);
      std::string temp;
      std::string temp_content = rev.content();
      base::BasicUtil::UrlDecode(temp_content, temp);
      PushGtMsg(rev.from_uid(), rev.to_uid(), rev.body_str(), temp,
          CHAT_MESSAGE_RLY);
      err = chat_mysql_->ChatRecordInsert(rev.from_uid(), rev.to_uid(),
          rev.content(), rev.msg_time(), 1);
      break;
    } else {
      rev.set_operate_code(CHAT_MESSAGE_RLY);
      SendPacket(u->socket_fd(), &rev);
      //保存及时消息
      std::stringstream ss;
      ss << "call proc_ChatRecordInsert(" << rev.from_uid() << "," << rev.to_uid()
         << ",'" << rev.content() << "'," << rev.msg_time() << "," << 0 << ")";
      msg_list_.push_back(ss.str());
      if (msg_list_.size() > 10) {
        LOG(INFO)<< "msg_list > 10";
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
  }while (0);
  if (err < 0)
    SendError(socket, packet, err, CHAT_MESSAGE_RLY);
  else
    SendMsg(socket, packet, NULL, CHAT_MESSAGE_RLY);
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

int32 ChatInterface::PushMsgRead(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    PushMsgReadRecv rev(*packet);
    err = rev.Deserialize();
    if (err < 0)
      break;
    data_share_mgr_->DelUnReadCount(rev.uid(), rev.count());
    SendMsg(socket, packet, NULL, CHAT_READ_RLY);
  } while (0);
  if (err < 0)
    SendError(socket, packet, err, CHAT_READ_RLY);
  return err;
}

int32 ChatInterface::FreeCoordinator(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    UserInfo* user = NULL;
    user = data_share_mgr_->GetFreeCoordinator();
    if (user == NULL) {
      err = NO_ONLINE_COORDINATOR;
      break;
    }
    DicValue dict;
    dict.SetBigInteger(L"uid_", user->uid());
    dict.SetString(L"phone_num_", user->phone_num());
    dict.SetString(L"nickname_", user->nickname());
    dict.SetString(L"head_url_", user->head_url());
    dict.SetBigInteger(L"gender_", user->gender());
    SendMsg(socket, packet, &dict, FREE_COORDINATOR_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, FREE_COORDINATOR_RLY);
  }
  return err;
}

int32 ChatInterface::SpentCash(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    SpentCashRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = chat_mysql_->SpentCashUpdate(recv.uid(), recv.order_id(),
                                       recv.passwd(), &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, SPENT_CASH_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, SPENT_CASH_RLY);
  }
  return err;
}

int32 ChatInterface::CancelOrder(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    CancelOrderRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = chat_mysql_->CancelOrderPayUpdate(recv.order_id(), recv.order_type(),
                                         &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, SPENT_CASH_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, CANCEL_ORDER_RLY);
  }
  return err;
}

int32 ChatInterface::GtPushComm(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    GtPushCommRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    //推送消息
    PushGtMsg(recv.from_uid(), recv.to_uid(), recv.body_str(), recv.content(),
              recv.msg_type());
    SendMsg(socket, packet, NULL, COMM_GEPUSH_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, COMM_GEPUSH_RLY);
  }
  return err;
}

int32 ChatInterface::UnreadPushmsgRecord(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    PulledPushmsgRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = chat_mysql_->PullPushMsgSelect(recv.uid(), &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, UNREAD_PUSH_MSG_RECORD_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, UNREAD_PUSH_MSG_RECORD_RLY);
  }
  return err;
}


int32 ChatInterface::EvaluateTrip(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    EvaluateTripRecv rev(*packet);
    err = rev.Deserialize();
    if (err < 0)
      break;
    err = chat_mysql_->EvaluateTripInsert(rev.order_id(), rev.service_score(),
                                          rev.user_score(), rev.remarks(),
                                          rev.from_uid(), rev.to_uid());
    if (err < 0)
      break;
    //通知评价者
    SendMsg(socket, packet, NULL, EVALUATE_TRIP_RLY);
    //todo 通知被评价者
    {

    }

	//提成给邀请者
	err = chat_mysql_->UpDateTripCommission(rev.order_id());

  } while (0);
  if (err < 0)
    SendError(socket, packet, err, EVALUATE_TRIP_RLY);
  return err;
}

int32 ChatInterface::EvaluateInfo(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    EvaluateInfoRecv rev(*packet);
    err = rev.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = chat_mysql_->EvaluateInfoSelect(rev.order_id(), &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, ORDER_EVALUATE_INFO_RLY);

  } while (0);
  if (err < 0)
    SendError(socket, packet, err, ORDER_EVALUATE_INFO_RLY);
  return err;
}

int32 ChatInterface::PushGtMsg(int64 from, int64 to, std::string category,
                               std::string content, int64 type) {
  int32 err = 0;
  LOG(INFO)<< "ChatInterface::PushAskMsg";
  do {
    std::string token = data_share_mgr_->GetDeviceToken(to);
    if (token == "") {
      err = chat_mysql_->DeviceTokenSelect(to, &token);
      if (err < 0)
        break;
    }
    if (token.length() < 5) {
      err = DEVICE_TOKEN_ERR;
      break;
    }

    std::string nick = data_share_mgr_->GetNick(from);
    if (nick == "") {
      DicValue dic;
      err = chat_mysql_->UserNickSelect(from, &dic);
      if (err < 0)
        break;
      nick = dic.GetString(L"nickname_", &nick);
      data_share_mgr_->AddNick(from, nick);
    }
    LOG(INFO)<< "token::" << token;
    LOG(INFO)<< "content::" << category;
    util::PushApnChatMsg((char*) token.c_str(),
                         data_share_mgr_->AddUnReadCount(to),
                         (char*) nick.c_str(), (char*) content.c_str(),
                         (char*) SpliceGtPushBody(category, type).c_str());
  } while (0);
  return err;
}

int32 ChatInterface::PushAskMsg(AskInvitationRecv rev) {
  int32 err = 0;
  LOG(INFO)<< "ChatInterface::PushAskMsg";
  do {
    std::string token = data_share_mgr_->GetDeviceToken(rev.to_uid());
    if (token == "") {
      err = chat_mysql_->DeviceTokenSelect(rev.to_uid(), &token);
      if (err < 0)
        break;
    }
    if (token.length() < 5) {
      err = DEVICE_TOKEN_ERR;
      break;
    }
    std::string nick = data_share_mgr_->GetNick(rev.from_uid());
    if (nick == "") {
      DicValue dic;
      err = chat_mysql_->UserNickSelect(rev.from_uid(), &dic);
      if (err < 0)
        break;
      nick = dic.GetString(L"nickname_", &nick);
      data_share_mgr_->AddNick(rev.from_uid(), nick);
    }
    LOG(INFO)<< "token::" << token;
    LOG(INFO)<< "category::" << rev.body_str();
    util::PushApnChatMsg((char*) token.c_str(),
                         data_share_mgr_->AddUnReadCount(rev.to_uid()),
                         (char*) nick.c_str(), " 邀你同游！",
                         (char*) SpliceGtPushBody(rev.body_str(), 2).c_str());
  } while (0);
  return err;
}
//废弃的
int32 ChatInterface::PushChatMsg(ChatPacket rev) {
  int32 err = 0;
  LOG(INFO)<< "ChatInterface::PushChatMsg";
  std::string token = data_share_mgr_->GetDeviceToken(rev.to_uid());
  do {
    if (token == "") {
      err = chat_mysql_->DeviceTokenSelect(rev.to_uid(), &token);
      if (err < 0)
        break;
    }
    if (token.length() < 5) {
      err = DEVICE_TOKEN_ERR;
      break;
    }
    std::string nick = data_share_mgr_->GetNick(rev.from_uid());
    if (nick == "") {
      DicValue dic;
      err = chat_mysql_->UserNickSelect(rev.from_uid(), &dic);
      if (err < 0)
        break;
      nick = dic.GetString(L"nickname_", &nick);
      data_share_mgr_->AddNick(rev.from_uid(), nick);
    }
    LOG(INFO)<< "token::" << token;
    LOG(INFO)<< "content::" << rev.body_str();
    util::PushApnChatMsg((char*) token.c_str(),
                         data_share_mgr_->AddUnReadCount(rev.to_uid()),
                         (char*) nick.c_str(), (char*) rev.content().c_str(),
                         (char*) SpliceGtPushBody(rev.body_str(), 1).c_str());
  } while (0);
  return err;
}

std::string ChatInterface::SpliceGtPushBody(std::string json, int64 type) {
  int32 err = 0;
  std::string str;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &json, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  if (dic != NULL) {
    dic->SetBigInteger(L"push_msg_type_", type);
    serializer->Serialize(*dic, &str);
  } else {
    str = "{\"push_msg_type:\",-1}";
    LOG(ERROR)<< "SpliceGtPushBody json error";
  }
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return str;
}

int32 ChatInterface::CloseSocket(const int fd) {
  data_share_mgr_->UserOffline(fd);
  return 0;
}

void ChatInterface::SendPacket(const int socket, PacketHead* packet) {

  char* s = new char[packet->packet_length()];
  LOG(INFO)<< "packet body:" << packet->body_str();
  memset(s, 0, packet->packet_length());
  memcpy(s, &packet->head(), HEAD_LENGTH);
  memcpy(s + HEAD_LENGTH, packet->body_str().c_str(),
         packet->body_str().length());
  int total = util::SendFull(socket, s, packet->packet_length());
  delete[] s;
  s = NULL;
  LOG_IF(ERROR, total != packet->packet_length())
      << "send packet wrong:opcode[" << packet->operate_code() << "]";
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
