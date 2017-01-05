// Copyright (c) 2016 The tourism Authors. All rights reserved.
// user_interface.cc
// Created on: 2016年8月10日.
// Author: Paco.
#include "user/user_interface.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/socket.h>

#include "public/basic/md5sum.h"
#include "glog/logging.h"

#include "user/user_proto.h"
#include "user/user_opcode.h"
#include "pub/util/util.h"
#include "pub/pay/wxpay/wx_order.h"

#define SHELL_SMS "./verify_code_sms.sh"
#define SMS_KEY "yd1742653sd"
namespace user {
UserInterface* UserInterface::instance_;

UserInterface* UserInterface::GetInstance() {
  if (instance_ == NULL)
    instance_ = new UserInterface();
  return instance_;
}

void UserInterface::FreeInstance() {
  if (instance_ == NULL) {
    delete instance_;
    instance_ = NULL;
  }
}

UserInterface::UserInterface() {
  user_mysql_ = NULL;
  data_share_mgr_ = NULL;
}

UserInterface::~UserInterface() {

}

void UserInterface::InitConfig(config::FileConfig* config) {
  user_mysql_ = new UserMysql(config);
//  data_share_mgr_ = share::DataShareMgr::GetInstance();
}

void UserInterface::InitShareDataMgr(share::DataShareMgr* data) {
  data_share_mgr_ = data;
}

int32 UserInterface::CheckHeartLoss() {
  int32 err = 0;
  do {
    data_share_mgr_->CheckHeartLoss();
  } while (0);

  return err;
}

int32 UserInterface::NopayOrderStatusCheck() {
  int32 err = 0;
  err = user_mysql_->CancelOrderStatusUpdate();
  return err;
}

int32 UserInterface::InitShareGuide() {
  int err = 0;
  InitShareType();
  InitShareDetails();
  InitShareSkills();
  return err;
}

int32 UserInterface::InitShareType() {
  int err = 0;
  do {
    DicValue type_dic;
    err = user_mysql_->ShareTourismTypeSelect(&type_dic);
    if (err < 0)
      break;
    if (!type_dic.empty()) {
      ListValue* info;
      type_dic.GetList(L"data_list_", &info);
      if (info != NULL && !info->empty()) {
        data_share_mgr_->InitShareType(info);
      }
    } else {
      break;
    }
  } while (0);
  return err;
}

int32 UserInterface::InitShareDetails() {
  int32 err = 0;
  do {
    DicValue detail_dic;
    err = user_mysql_->ShareTourismDetailsSelect(&detail_dic);
    if (err < 0)
      break;
    if (!detail_dic.empty()) {
      ListValue* info;
      detail_dic.GetList(L"data_list_", &info);
      if (info != NULL && !info->empty()) {
        data_share_mgr_->InitTourismShare(info);
      }
    }
  } while (0);
  return err;
}
int32 UserInterface::InitShareSkills() {
  int32 err = 0;
  do {
    DicValue skill_info;
    err = user_mysql_->ShareSkillDetailsSelect(&skill_info);
    if (err < 0)
      break;
    if (!skill_info.empty()) {
      ListValue* info;
      skill_info.GetList(L"data_list_", &info);
      if (info != NULL && !info->empty()) {
        data_share_mgr_->InitSkillShare(info);
      }
    }
  } while (0);
  return err;
}

int32 UserInterface::TourismShareTypeInfo(const int32 socket,
                                          PacketHead* packet) {
  int32 err = 0;
  do {
    DicValue type_dic;
    err = user_mysql_->ShareTourismTypeSelect(&type_dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &type_dic, SHARE_TOURISM_TYPE_RLY);
  } while (0);
  if (err < 0)
    SendError(socket, packet, err, SHARE_TOURISM_TYPE_RLY);
  return err;

}

int32 UserInterface::TourismShareRecommend(const int32 socket,
                                           PacketHead* packet) {
  int32 err = 0;
  do {
    PaggingRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = data_share_mgr_->QueryRecommendShare(recv.last_id(), recv.count(),
                                               recv.page_type(), &dic);
    if (err < 0)
      break;
    dic.SetBigInteger(L"page_type_", recv.page_type());
    SendMsg(socket, packet, &dic, SHARE_TOURISM_RECOMMEND_RLY);
  } while (0);
  if (err < 0)
    SendError(socket, packet, err, SHARE_TOURISM_RECOMMEND_RLY);
  return err;
}

//
int32 UserInterface::TourismShareList(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    PaggingRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = data_share_mgr_->QueryRecommendShare(recv.last_id(), recv.count(),
                                               recv.page_type(), &dic);
    if (err < 0)
      break;
    dic.SetBigInteger(L"page_type_", recv.page_type());
    SendMsg(socket, packet, &dic, SHARE_TOURISM_LIST_RLY);
  } while (0);
  if (err < 0)
    SendError(socket, packet, err, SHARE_TOURISM_LIST_RLY);
  return err;
}

int32 UserInterface::SkillShareList(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    PaggingRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = data_share_mgr_->QuerySkillShare(recv.last_id(), recv.count(), &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, SHARE_SKILL_LIST_RLY);
  } while (0);
  if (err < 0)
    SendError(socket, packet, err, SHARE_SKILL_LIST_RLY);
  return err;
}

int32 UserInterface::TourismShareDetail(const int32 socket,
                                        PacketHead* packet) {
  int32 err = 0;
  do {
    ShareTourismDetailRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = data_share_mgr_->QueryShareTourismDetail(recv.share_id(), &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, SHARE_TOURISM_DETAIL_REQ);
  } while (0);
  if (err < 0)
    SendError(socket, packet, err, SHARE_TOURISM_DETAIL_REQ);
  return err;
}

int32 UserInterface::SkillShareDetail(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    ShareSkillDetailRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = data_share_mgr_->QueryShareSkillDetail(recv.share_id(), &dic);
    if (err < 0)
      break;
    err = user_mysql_->ShareSkillEntrySelect(recv.share_id(), &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, SHARE_SKILL_DETAIL_RLY);
  } while (0);
  if (err < 0)
    SendError(socket, packet, err, SHARE_SKILL_DETAIL_RLY);
  return err;
}

int32 UserInterface::SkillShareDiscuss(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    ShareSkillDiscussRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->ShareSkillDiscussSelect(recv.share_id(), recv.last_id(),
                                               recv.count(), &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, SHARE_SKILL_DISCUSS_RLY);
  } while (0);
  if (err < 0)
    SendError(socket, packet, err, SHARE_SKILL_DISCUSS_RLY);
  return err;
}

int32 UserInterface::EntrySkillShare(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    EntryShareSkillRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->EntryShareSkillInsert(recv.share_id(), recv.uid(), &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, ENTRY_SHARE_SKILL_RLY);
  } while (0);
  if (err < 0)
    SendError(socket, packet, err, ENTRY_SHARE_SKILL_RLY);
  return err;
}

int32 UserInterface::UserCashInfo(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    UserCashRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->UserCashSelect(recv.uid(), &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, USER_CASH_RLY);
  } while (0);
  if (err < 0)
    SendError(socket, packet, err, USER_CASH_RLY);
  return err;
}

int32 UserInterface::OrderDetails(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    OrderDetailRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->OrderDetailsSelect(recv.order_id(), recv.order_type(),
                                          &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, ORDER_DETAILS_RLY);
  } while (0);
  if (err < 0)
    SendError(socket, packet, err, ORDER_DETAILS_RLY);
  return err;
}

int32 UserInterface::CheckSMSCode(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    CheckSMSCodeRecv rev(*packet);
    err = rev.Deserialize();
    if (err < 0)
      break;
    if (time(NULL) - rev.timestamp() > 15 * 60) {
      err = VERIFY_CODE_OVERDUE;
      break;
    }
    std::stringstream ss;
    ss << SMS_KEY << rev.timestamp() << rev.verify_code() << rev.phone_num();
    base::MD5Sum md5(ss.str());
    if (md5.GetHash() != rev.token()) {
      err = VERIFY_CODE_ERR;
      break;
    }
    ss.str("");
    ss.clear();
    ss << rev.phone_num() << ":" << rev.verify_type();
    if (!data_share_mgr_->CheckSMSToken(ss.str(), rev.token())) {
      err = VERIFY_CODE_NOT_NEW;
      break;
    }
    SendMsg(socket, packet, NULL, CHECK_SMS_CODE_RLY);
  } while (0);
  if (err < 0)
    SendError(socket, packet, err, CHECK_SMS_CODE_RLY);
  return err;
}

int32 UserInterface::VerifyPasswd(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    VerifyPasswdRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->CheckPasswdSelect(recv.uid(), recv.passwd(),
                                         recv.passwd_type(), &dic);
    if (err < 0)
      break;
    int64 result = 0;
    dic.GetBigInteger(L"result_", &result);
    if (result == -1) {
      err = VERIFY_PASSWD_ERR;
      break;
    }
    SendMsg(socket, packet, NULL, VERIFY_PASSWD_RLY);
  } while (0);
  if (err < 0)
    SendError(socket, packet, err, VERIFY_PASSWD_RLY);
  return err;
}

int32 UserInterface::ChangePayPasswd(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    ChangePayPasswdRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->ChangePasswdSelect(recv.uid(), recv.old_passwd(),
                                          recv.new_passwd(), recv.change_type(),
                                          recv.passwd_type(), &dic);
    if (err < 0)
      break;
    int64 result = 0;
    dic.GetBigInteger(L"result_", &result);
    if (result == -1) {
      err = HAS_SETTING_PAY_PASSWD;
      break;
    } else if (result == -2) {
      err = CANNOT_SET_LOGIN_PASSWD;
      break;
    } else if (result == -3) {
      err = PAY_PASSWD_ERR;
      break;
    }
    SendMsg(socket, packet, NULL, CHANGE_COMM_PASSWD_RLY);
  } while (0);
  if (err < 0)
    SendError(socket, packet, err, CHANGE_COMM_PASSWD_RLY);
  return err;
}

int32 UserInterface::GuideOrderRecord(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    GuideOrderRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->GuideOrderSelect(recv.uid(), recv.last_id(),
                                        recv.count(), &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, GUIDE_ORDER_RECORD_RLY);
  } while (0);
  if (err < 0)
    SendError(socket, packet, err, GUIDE_ORDER_RECORD_RLY);
  return err;
}

int32 UserInterface::GuideOrderDetail(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    GuideOrderDetailRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->GuideOrderDetailSelect(recv.order_id(), &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, GUIDE_ORDER_DETAIL_RLY);
  } while (0);
  if (err < 0)
    SendError(socket, packet, err, GUIDE_ORDER_DETAIL_RLY);
  return err;
}
int32 UserInterface::DefineGuideSkills(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    DefineGuideSkillsRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->DefineGuideSkillsUpdateSelect(recv.uid(),
                                                     recv.change_type(),
                                                     recv.skills(), &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, DEFINE_GUIDE_SKILL_RLY);
  } while (0);
  if (err < 0)
    SendError(socket, packet, err, DEFINE_GUIDE_SKILL_RLY);
  return err;
}

int32 UserInterface::GuidesInfo(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    UserDetailRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->GuidesInfoSelect(recv.uid_str(), &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, GUIDE_INFO_RLY);
  } while (0);
  if (err < 0)
    SendError(socket, packet, err, GUIDE_INFO_RLY);
  return err;
}

int32 UserInterface::VerifyVleaderHead(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    VerifyVleaderHeadRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->VerifyVleaderHeadInsert(recv.uid(), recv.head_url(),
                                               &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, VERIFY_VLEADER_HEAD_RLY);
  } while (0);
  if (err < 0)
    SendError(socket, packet, err, VERIFY_VLEADER_HEAD_RLY);
  return err;
}

int32 UserInterface::ChangeUserInfo(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    ChangeUserInfoRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->ChangeUserInfoUpdate(recv.uid(), recv.nickname(),
                                            recv.gender(), recv.address(),
                                            &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, CHANGE_USER_INFO_RLY);
  } while (0);
  if (err < 0)
    SendError(socket, packet, err, CHANGE_USER_INFO_RLY);
  return err;
}

int32 UserInterface::ChangeBankCard(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    ChangeBankCardRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->ChangeBankCardInsertOrDelete(recv.type(), recv.uid(),
                                                    recv.account(),
                                                    recv.bank_username(),
                                                    recv.bank(), &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, CHANGE_BANK_CARD_RLY);
  } while (0);
  if (err < 0)
    SendError(socket, packet, err, CHANGE_BANK_CARD_RLY);
  return err;
}

int32 UserInterface::BankCardInfo(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    BankCardInfoRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->BankCardInfoSelect(recv.uid(), &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, BANK_CARD_INFO_RLY);
  } while (0);
  if (err < 0)
    SendError(socket, packet, err, BANK_CARD_INFO_RLY);
  return err;
}

int32 UserInterface::ChangeDefaultBankCard(const int32 socket,
                                           PacketHead* packet) {
  int32 err = 0;
  do {
    ChangeDefaultBankCardRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->ChangeDefaultBankCardUpdate(recv.uid(), recv.account(),
                                                   &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, CHANGE_DEFAULT_BANK_CARD_RLY);
  } while (0);
  if (err < 0)
    SendError(socket, packet, err, CHANGE_DEFAULT_BANK_CARD_RLY);
  return err;
}

int32 UserInterface::UserWithdraw(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    UserWithdrawRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->UserWithdrawInsertAndSelect(recv.uid(), recv.account(),
                                                   recv.cash(), &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, USER_WITHDRAW_RLY);
  } while (0);
  if (err < 0)
    SendError(socket, packet, err, USER_WITHDRAW_RLY);
  return err;
}

int32 UserInterface::UserWithdrawRecord(const int32 socket,
                                        PacketHead* packet) {
  int32 err = 0;
  do {
    UserWithdrawRecordRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->UserWithdrawRecordSelect(recv.uid(), recv.account(),
                                                recv.size(), recv.num(), &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, USER_WITHDRAW_RECORD_RLY);
  } while (0);
  if (err < 0)
    SendError(socket, packet, err, USER_WITHDRAW_RECORD_RLY);
  return err;
}

int32 UserInterface::UserUploadPhoto(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    UserUploadPhotoRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    std::list<PhotoUrl*> list = recv.photo_list();
    std::list<std::string> sql_list;
    std::list<PhotoUrl*>::iterator it = list.begin();
    for (; it != list.end(); ++it) {
      PhotoUrl* data = *it;
      if (data != NULL) {
        std::stringstream ss;
        ss << "call proc_UserUploadPhotoInsert(" << recv.uid() << ",'"
           << data->photo_url_ << "','" << data->thumbnail_url_ << "')";
        sql_list.push_back(ss.str());
      }
    }
    DicValue dic;
    err = user_mysql_->UserUploadPhotoInsert(recv.uid(), sql_list, &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, USER_UPLOAD_PHOTO_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, USER_UPLOAD_PHOTO_RLY);
  }
  return err;
}

int32 UserInterface::UserPhotoAlbum(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    UserPhotoAlbumRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->UserPhotoAlbumSelect(recv.uid(), recv.size(), recv.num(),
                                            &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, USER_PHOTO_ALBUM_RLY);
  } while (0);
  if (err < 0)
    SendError(socket, packet, err, USER_PHOTO_ALBUM_RLY);
  return err;
}

int32 UserInterface::UserRegInvitationCode(const int32 socket, PacketHead* packet) {
	int32 err = 0;
	do {
		UserRegInvitationCodeRecv recv(*packet);
		err = recv.Deserialize();
		if (err < 0)
			break;
		DicValue dic;
		/*暂定为90天 AlwaysOnline*/
		err = user_mysql_->UserInvitationCodeUpDate(recv.phoneNum(),recv.invitationCode(), 90,  &dic);
		if (err < 0)
			break;
		SendMsg(socket, packet, &dic, USER_REG_INVITATIONCODE_RLY);
	} while (0);
	if (err < 0)
		SendError(socket, packet, err, USER_REG_INVITATIONCODE_RLY);
	return err;
}

int32 UserInterface::UserAppVersionInfo(const int32 socket, PacketHead* packet)
{
	int32 err = 0;
	do {
		UserAppVersionInfoeRecv recv(*packet);
		err = recv.Deserialize();
		if (err < 0)
			break;
		DicValue dic;
		err = user_mysql_->UserAppVersionInfo(recv.AppType(),&dic);
		if (err < 0)
			break;
		SendMsg(socket, packet, &dic, USER_APP_VERSION_INFO_RLY);
	} while (0);
	if (err < 0) {
		SendError(socket, packet, err, USER_APP_VERSION_INFO_RLY);
	}
	return err;
}

int32 UserInterface::CloseSocket(const int fd) {
  data_share_mgr_->UserOffline(fd);
  return 0;
}

int32 UserInterface::HeartPacket(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  Heartbeat rev(*packet);
  do {
    err = rev.Deserialize();
    if (err < 0)
      break;
    data_share_mgr_->UserHeart(rev.uid());
  } while (0);
  return err;
}

int32 UserInterface::AlipayServer(const int32 socket, PacketHead* packet) {
  LOG(INFO) << "alipay server req";
  return 0;
}

int32 UserInterface::AlipayClient(const int32 socket, PacketHead* packet) {
  LOG(INFO) << "alipay client req";
  return 0;
}

int32 UserInterface::ImproveUserData(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    ImproveDataRecv rev(*packet);
    err = rev.Deserialize();
    if (err < 0)
      break;
    err = user_mysql_->ImproveUserUpdate(rev.uid(), rev.gender(),
                                         rev.nickname(), rev.head_url(),
                                         rev.address(), rev.longitude(),
                                         rev.latitude());
    if (err < 0)
      break;
    SendMsg(socket, packet, NULL, IMPROVE_DATA_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, IMPROVE_DATA_RLY);
  }
  return err;
}

int32 UserInterface::ObtainTripRecord(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    ObtainTripRecv rev(*packet);
    err = rev.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->TripRecordSelect(rev.uid(), rev.order_id(), rev.count(),
                                        &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, OBTAIN_TRIP_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, OBTAIN_TRIP_RLY);
  }
  return err;
}

int32 UserInterface::ObtainServiceInfo(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    ServiceDetailsRecv rev(*packet);
    err = rev.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->ServiceInfoSelect(rev.oid_str(), &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, SERVICE_INFO_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, SERVICE_INFO_RLY);
  }
  return err;
}

int32 UserInterface::DrawBillTrip(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    DrawBillRecv rev(*packet);
    err = rev.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->InvoiceInfoInsert(rev.uid(), rev.oid_str(), rev.title(),
                                         rev.taxpayer_num(), rev.company_addr(),
                                         rev.invoice_type(), rev.user_name(),
                                         rev.user_mobile(), rev.area(),
                                         rev.addr_detail(), rev.remarks(),
                                         &dic);
    if (err < 0)
      break;
    int64 result = 1;
    dic.GetBigInteger(L"result_", &result);
    if (result == -2) {
      err = ALREADY_INVOICE_RECORD;
      break;
    } else if (result == -3) {
      err = ORDER_NOT_PAY;  //订单未支付
      break;
    }

    dic.SetString(L"oid_str_", rev.oid_str());
    SendMsg(socket, packet, &dic, DRAW_BILL_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, DRAW_BILL_RLY);
  }
  return err;
}

int32 UserInterface::BillingRecord(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    BillRecordRecv rev(*packet);
    err = rev.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->InvoiceRecordSelect(rev.uid(), rev.count(),
                                           rev.last_invoice_id(), &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, INVOICE_RECORD_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, INVOICE_RECORD_RLY);
  }
  return err;
}

int32 UserInterface::InvoiceDetail(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    InvoiceDetailRecv rev(*packet);
    err = rev.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->InvoiceDetailSelect(rev.invoice_id(), &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, INVOICE_DETAIL_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, INVOICE_DETAIL_RLY);
  }
  return err;
}

int32 UserInterface::DeviceToken(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  LOG(INFO) << "DeviceToken";
  do {
    DeviceTokenRecv rev(*packet);
    err = rev.Deserialize();
    LOG(INFO) << "DeviceToken Deserialize err:" << err;
    if (err < 0)
      break;
    int result = data_share_mgr_->AddDeviceToken(rev.uid(), rev.device_token());
    LOG(INFO) << "AddDeviceToken result:" << result;
    if (result >= 0)
      err = user_mysql_->DeviceTokenUpdate(rev.uid(), rev.device_token());
    if (err < 0)
      break;
    SendMsg(socket, packet, NULL, DEVICE_TOKEN_RLY);
  } while (0);
  if (err < 0) {
    LOG(INFO) << "DeviceToken SendError err:" << err;
    SendError(socket, packet, err, DEVICE_TOKEN_RLY);
  }
  return err;
}

int32 UserInterface::BlackcardPrivilege(const int32 socket,
                                        PacketHead* packet) {
  int32 err = 0;
  do {
    DicValue dic;
    err = user_mysql_->BlackcardPrivilegeSelect(&dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, BLACKCARD_PRIVILEGE_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, BLACKCARD_PRIVILEGE_RLY);
  }
  return err;
}

int32 UserInterface::BlackcardInfo(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    BlackcardInfoRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->BlackcardInfoSelect(recv.uid(), &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, BLACKCARD_INFO_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, BLACKCARD_INFO_RLY);
  }
  return err;
}

int32 UserInterface::BlackcardConsumeRecord(const int32 socket,
                                            PacketHead* packet) {
  int32 err = 0;
  do {
    BlackcardConsumRecordRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->BlackcardConsumeRecordSelect(recv.uid(), &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, BLACKCARD_CONSUME_RECORD_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, BLACKCARD_CONSUME_RECORD_RLY);
  }
  return err;
}

int32 UserInterface::BlackcardPriceInfo(const int32 socket,
                                        PacketHead* packet) {
  int32 err = 0;
  do {
    DicValue dic;
    err = user_mysql_->BlackcardPriceInfoSelect(&dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, BLACKCARD_PRICE_INFO_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, BLACKCARD_PRICE_INFO_RLY);
  }
  return err;
}

int32 UserInterface::BlackcardPlaceOrder(const int32 socket,
                                         PacketHead* packet) {
  int32 err = 0;
  do {
    BlackcardPlaceOrderRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->BlackcardPlaceOrderInsertAndSelect(recv.uid(),
                                                          recv.wanted_lv(),
                                                          &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, PLACE_BLACKCARD_ORDER_RLY);
    if (err < 0)
      break;
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, PLACE_BLACKCARD_ORDER_RLY);
  }
  return err;
}

int32 UserInterface::SkillsInfo(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    DicValue dic;
    err = user_mysql_->SkillsInfoSelect(&dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, SKILL_INFO_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, SKILL_INFO_RLY);
  }
  return err;
}

int32 UserInterface::NewAppointment(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    NewAppointmentRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->NewAppointmentInsert(recv.uid(), recv.city_code(),
                                            recv.start_time(), recv.end_time(),
                                            recv.skills(), recv.is_other(),
                                            recv.other_name(),
                                            recv.other_gender(),
                                            recv.ohter_phone(), recv.remarks(),
                                            &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, NEW_APPOINTMENT_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, NEW_APPOINTMENT_RLY);
  }
  return err;
}

int32 UserInterface::AppointmentRecord(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    AppointmentRecordRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->AppointmentRecordSelect(recv.uid(), recv.last_id(),
                                               recv.count(), &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, APPOINTMENT_RECORD_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, APPOINTMENT_RECORD_RLY);
  }
  return err;
}

int32 UserInterface::UpImgToken(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    DicValue dic;
    std::string token;
    int64 tim;
    data_share_mgr_->GetImgToken(&token, &tim);
    if (token.length() > 0 && (tim - time(NULL)) > 60 * 2) {
      dic.SetString(L"img_token_", token);
      dic.SetBigInteger(L"valid_time_", tim);
    } else {
      err = user_mysql_->ImgTokenSelect(&dic);
      if (err < 0)
        break;
      dic.GetString(L"img_token_", &token);
      dic.GetBigInteger(L"valid_time_", &tim);
      data_share_mgr_->SetImgToken(token, tim);
    }
    SendMsg(socket, packet, &dic, IMG_TOKEN_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, IMG_TOKEN_RLY);
  }
  return err;
}

void UserInterface::test() {
  LOG(INFO) << "UserInterface::test()";

////  DicValue dic;
////  std::string temp;
////  user_mysql_->GuideOrderSelect(16, 0, 10, &dic);
////  if (dic.empty())
////    LOG(INFO) << "dic empty";
////  base_logic::ValueSerializer* serializer =
////  base_logic::ValueSerializer::Create(base_logic::IMPL_JSON);
////  serializer->Serialize(dic, &temp);
////  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
////      serializer);
////  LOG(INFO) << temp;
//  LOG(INFO) << "----------------";
//
//  DicValue dic;
//  user_mysql_->GuideOrderSelect(16, 0, 10, &dic);
//  PacketHead packet;
//  SendMsg(1, &packet, &dic, GUIDE_ORDER_RECORD_RLY);
//  return ;

  /*

   DicValue dic1;
   std::string temp;
   data_share_mgr_->QueryRecommendShare(0, 10, 0, &dic1);
   base_logic::ValueSerializer* serializer =
   base_logic::ValueSerializer::Create(base_logic::IMPL_JSON);
   serializer->Serialize(dic1, &temp);
   base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
   serializer);
   LOG(INFO) << temp;
   LOG(INFO) << "----------------";
   DicValue dic2;
   std::string temp2;
   data_share_mgr_->QueryRecommendShare(126, 10, 1, &dic2);
   base_logic::ValueSerializer* serializer2 =
   base_logic::ValueSerializer::Create(base_logic::IMPL_JSON);
   serializer2->Serialize(dic2, &temp2);
   base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
   serializer2);
   LOG(INFO) << temp2;
   LOG(INFO) << "----------------";
   DicValue dic3;
   std::string temp3;
   data_share_mgr_->QueryRecommendShare(4, 10, 2, &dic3);
   base_logic::ValueSerializer* serializer3 =
   base_logic::ValueSerializer::Create(base_logic::IMPL_JSON);
   serializer3->Serialize(dic3, &temp3);
   base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
   serializer3);
   LOG(INFO) << temp3;
   LOG(INFO) << "----------------";

   DicValue dic4;
   std::string temp4;
   data_share_mgr_->QuerySkillShare(0, 10, &dic4);
   base_logic::ValueSerializer* serializer4 =
   base_logic::ValueSerializer::Create(base_logic::IMPL_JSON);
   serializer4->Serialize(dic4, &temp4);
   base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
   serializer4);
   LOG(INFO) << temp4;
   LOG(INFO) << "----------------";

   DicValue dic5;
   std::string temp5;
   data_share_mgr_->QuerySkillShare(15, 10, &dic5);
   base_logic::ValueSerializer* serializer5 =
   base_logic::ValueSerializer::Create(base_logic::IMPL_JSON);
   serializer5->Serialize(dic5, &temp5);
   base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
   serializer5);
   LOG(INFO) << temp5;
   LOG(INFO) << "----------------";

   return;

   WXOrder wx_order;
   wx_order.set_spbill_create_ip("127.0.0.1");
   wx_order.set_body("V领队-高级服务");
   wx_order.set_out_trade_no("1");
   wx_order.set_total_fee(100);
   std::string wx_result = wx_order.PlaceOrder();
   LOG(INFO)<< "test::" << wx_result;
   base_logic::ValueSerializer* deserializer =
   base_logic::ValueSerializer::Create(base_logic::IMPL_XML, &wx_result);
   std::string err_str;
   int32 err = 0;
   DicValue* dic = (DicValue*) deserializer->Deserialize(&err, &err_str);
   do {
   if (dic != NULL) {
   std::string return_code;
   dic->GetString(L"return_code", &return_code);
   LOG(INFO)<< "return_code:" << return_code;
   //下单成功
   if (return_code.find("SUCCESS") != std::string::npos) {
   std::string result_code;
   dic->GetString(L"result_code", &result_code);
   LOG(INFO)<< "result_code:" << result_code;
   //业务逻辑成功
   if (result_code.find("SUCCESS") != std::string::npos) {
   std::string prepay_id;
   dic->GetString(L"prepay_id", &prepay_id);
   LOG(INFO)<< "prepay_id:" << prepay_id;
   int npos1 = prepay_id.find("<![CDATA[");
   int npos2 = prepay_id.find("]]>");
   prepay_id = prepay_id.substr(npos1 + 9, npos2 - npos1 - 9);
   LOG(INFO)<< "prepay_id:" << prepay_id;
   } else {

   }
   } else {

   }
   }
   }while (0);*/
}

int32 UserInterface::WXPlaceOrder(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    WxPlaceOrderRecv recv(*packet);
    err = recv.Deserialize();
    //记录订单信息
    DicValue recharge_dic;
    err = user_mysql_->RechargeInfoInsertAndSelect(recv.uid(), recv.price(),
                                                   &recharge_dic);
    if (err < 0)
      break;
    std::string recharge_id;
    recharge_dic.GetString(L"recharge_id_", &recharge_id);
    std::string ip;
    int port;
    //访问微信下单接口
    WXOrder wx_order;
    if (util::GetIPAddress(socket, &ip, &port))
      wx_order.set_spbill_create_ip(ip);
    wx_order.set_body(recv.title());
    wx_order.set_out_trade_no(recharge_id);
    wx_order.set_total_fee(recv.price());
    std::string wx_result = wx_order.PlaceOrder();
    base_logic::ValueSerializer* deserializer =
        base_logic::ValueSerializer::Create(base_logic::IMPL_XML, &wx_result);
    std::string err_str;
    int32 err = 0;
    DicValue* dic = (DicValue*) deserializer->Deserialize(&err, &err_str);
    do {
      if (dic != NULL) {
        std::string return_code;
        dic->GetString(L"return_code", &return_code);
        LOG(INFO) << "return_code:" << return_code;
        //下单成功
        if (return_code.find("SUCCESS") != std::string::npos) {
          std::string result_code;
          dic->GetString(L"result_code", &result_code);
          LOG(INFO) << "result_code:" << result_code;
          //业务逻辑成功
          if (result_code.find("SUCCESS") != std::string::npos) {
            std::string prepay_id;
            dic->GetString(L"prepay_id", &prepay_id);
            LOG(INFO) << "prepay_id:" << prepay_id;
            int npos1 = prepay_id.find("<![CDATA[");
            int npos2 = prepay_id.find("]]>");
            prepay_id = prepay_id.substr(npos1 + 9, npos2 - npos1 - 9);
            wx_order.set_prepayid(prepay_id);
            wx_order.PreSign();
            wx_order.PreSerialize(&recharge_dic);
            SendMsg(socket, packet, &recharge_dic, WX_PLACE_ORDER_RLY);
            // todo 下单成功 ，记录微信订单信息

          } else {
            err = WX_PLACE_ORDER_ERR;
            break;
          }
        } else {
          err = WX_PLACE_ORDER_ERR;
          break;
        }
      }
    } while (0);
    base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_XML,
                                                  deserializer);

  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, WX_PLACE_ORDER_RLY);
  }
  return err;
}

int32 UserInterface::WXPayClientResponse(const int32 socket,
                                         PacketHead* packet) {
  int32 err = 0;
  do {
    WXPayClientRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->ChangeRechargeStatusAndSelect(recv.recharge_id(),
                                                     recv.pay_result(), &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, WXPAY_CLIENT_RLY);
    if (recv.pay_result() == 1) {
      user_mysql_->ChangeRechargeStatusAndSelect(recv.recharge_id(), 1, &dic);
    } else {
      user_mysql_->ChangeRechargeStatusAndSelect(recv.recharge_id(), 2, &dic);
    }
    SendMsg(socket, packet, &dic, WXPAY_SERVER_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, WXPAY_CLIENT_RLY);
  }
  return err;
}

int32 UserInterface::WXPayServerResponse(const int32 socket,
                                         PacketHead* packet) {
  int32 err = 0;
  do {
    WXPayServerRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    //支付成功
    DicValue dic;
    if (recv.appid() != APPID && recv.mch_id() != MCH_ID) {
      LOG(ERROR) << "WXPAY SERVER RESULT appid:[" << recv.appid() << "]";
      LOG(ERROR) << "WXPAY SERVER RESULT mch_id:[" << recv.mch_id() << "]";
      break;
    }
    if (recv.pay_result() == 1) {
      user_mysql_->ChangeRechargeStatusAndSelect(recv.recharge_id(), 3, &dic);
    } else {
      user_mysql_->ChangeRechargeStatusAndSelect(recv.recharge_id(), 4, &dic);
    }
    int64 user_id = 0;
    dic.GetBigInteger(L"uid_", &user_id);
    UserInfo* user = data_share_mgr_->GetUser(user_id);
    if (user != NULL && user->is_login()) {
      SendMsg(user->socket_fd(), packet, &dic, WXPAY_SERVER_RLY);
    }
  } while (0);
  return err;
}

int32 UserInterface::IdentityPicInfo(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    IdentityPicRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->IdentityInfoInsertAndSelect(recv.uid(), recv.front_pic(),
                                                   recv.back_pic(), &dic);
    SendMsg(socket, packet, &dic, IDENTITY_PIC_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, IDENTITY_PIC_RLY);
  }
  return err;
}

int32 UserInterface::IdentityReviewStatus(const int32 socket,
                                          PacketHead* packet) {
  int32 err = 0;
  do {
    IdentityStatusRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->IdentityReviewStatusSelect(recv.uid(), &dic);
    SendMsg(socket, packet, &dic, IDENTITY_STATUS_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, IDENTITY_STATUS_RLY);
  }
  return err;
}

int32 UserInterface::RegisterAccount(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  //LOG(INFO) << "AlwaysOnline test:";
  do {
    RegisterAccountRecv rev(*packet);
    err = rev.Deserialize();
    if (err < 0)
      break;
    if (time(NULL) - rev.timestamp() > 15 * 60) {
      err = VERIFY_CODE_OVERDUE;
      break;
    }
    std::stringstream ss;
    ss << SMS_KEY << rev.timestamp() << rev.verify_code() << rev.phone_num();
    base::MD5Sum md5(ss.str());
    if (md5.GetHash() != rev.token()) {
      err = VERIFY_CODE_ERR;
      break;
    }
    DicValue dic;

    err = user_mysql_->RegisterInsertAndSelect(rev.phone_num(), rev.passwd(),
		rev.user_type(), &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, REGISTER_ACCOUNT_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, REGISTER_ACCOUNT_RLY);
  }
  return err;
}

int32 UserInterface::ObtainVerifyCode(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    ObtainVerifyCodeRecv rev(*packet);
    err = rev.Deserialize();
    if (err < 0)
      break;
    std::stringstream ss;
    int64 timestamp_ = time(NULL);
    int64 rand_code_ = util::Random(100000, 999999);

    DicValue dic;
    dic.SetBigInteger(L"timestamp_", timestamp_);

    ss << SMS_KEY << timestamp_ << rand_code_ << rev.phone_num();
    base::MD5Sum md5(ss.str());
    dic.SetString(L"token_", md5.GetHash().c_str());
    LOG(INFO) << "token:" << ss.str();
    LOG(INFO) << "md5 token:" << md5.GetHash();
    ss.str("");
    ss.clear();
    ss << rev.phone_num() << ":" << rev.verify_type();
    data_share_mgr_->UpdateSMSToken(ss.str(), md5.GetHash());
    SendMsg(socket, packet, &dic, QUERY_VERIFY_CODE_RLY);

    ss.str("");
    ss.clear();
    ss << SHELL_SMS << " " << rev.phone_num() << " " << rand_code_ << " "
       << rev.verify_type();
    LOG(INFO) << ss.str();
    system(ss.str().c_str());
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, QUERY_VERIFY_CODE_RLY);
  }
  return err;
}

int32 UserInterface::ObtainUserInfo(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    UserDetailRecv rev(*packet);
    err = rev.Deserialize();
    if (err < 0)
      break;
    DicValue dict;
//    UserInfo* u = data_share_mgr_->GetUser(rev.uid());
//    if (u != NULL) {
//      dict.SetBigInteger(L"uid_", u->uid());
//      dict.SetString(L"phone_num_", u->phone_num());
//      dict.SetString(L"nickname_", u->nickname());
//      dict.SetBigInteger(L"credit_lv_", u->credit_lv());
//      dict.SetBigInteger(L"praise_lv_", u->praise_lv());
//      dict.SetBigInteger(L"cash_lv_", u->cash_lv());
//      dict.SetString(L"head_url_", u->head_url());
//      dict.SetString(L"address_", u->usual_addr());
//      dict.SetReal(L"longitude_", u->usual_lon());
//      dict.SetReal(L"latitude_", u->usual_lat());
//      SendMsg(socket, packet, &dict, USER_INFO_RLY);
//      break;
//    }
    err = user_mysql_->UserDetailSelect(rev.uid_str(), &dict);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dict, USER_INFO_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, USER_INFO_RLY);
  }
  return err;
}

int32 UserInterface::GuideDetail(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    //获取导游详
    GuideDetailRecv rev(*packet);
    err = rev.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->GuideDetailSelect(rev.uid(), &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, GUIDE_DETAIL_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, GUIDE_DETAIL_RLY);
  }
  return err;
}

int32 UserInterface::GuideServiceList(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    GuideDetailRecv rev(*packet);
    err = rev.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->GuideServiceSelect(rev.uid(), &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, GUIDE_SERVICE_LIST_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, GUIDE_SERVICE_LIST_RLY);
  }
  return err;
}

int32 UserInterface::ChangeGuideService(const int32 socket,
                                        PacketHead* packet) {
  int32 err = 0;
  do {
    ChangeGuideServiceRecv recv(*packet);
    err = recv.Deserialize();
    if (err < 0)
      break;
//    if (time(NULL) - recv.timestamp() > 15 * 60) {
//      err = VERIFY_CODE_OVERDUE;
//      break;
//    }
//    std::stringstream ss_token;
//    ss_token << SMS_KEY << recv.timestamp() << recv.verify_code()
//             << recv.phone_num();
//    base::MD5Sum md5(ss_token.str());
//    if (md5.GetHash() != recv.token()) {
//      err = VERIFY_CODE_ERR;
//      break;
//    }
    std::list<ServiceData*> list = recv.service_list();
    std::list<std::string> sql_list;
    std::list<ServiceData*>::iterator it = list.begin();
    for (; it != list.end(); ++it) {
      ServiceData* data = *it;
      if (data != NULL) {
        std::stringstream ss;
        ss << "call proc_GuideServerUpdate(" << recv.uid() << ","
           << data->service_id_ << "," << data->change_type_ << ","
           << data->service_type_ << ",'" << data->service_name_ << "',"
           << data->service_start_ << "," << data->service_end_ << ","
           << data->service_price_ << ")";
        sql_list.push_back(ss.str());
      }
    }
    DicValue dic;
    err = user_mysql_->GuideServerUpdateAndSelect(recv.uid(), sql_list, &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, CHANGE_GUIDE_SERVICE_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, CHANGE_GUIDE_SERVICE_RLY);
  }
  return err;
}

int32 UserInterface::ChangePasswd(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  //LOG(INFO) << "AlwaysOnline test:";
  do {
    ChangePasswdRecv rev(*packet);
    err = rev.Deserialize();
    if (err < 0)
      break;
    UserInfo* p = data_share_mgr_->GetUser(rev.uid());
    if (p == NULL || !p->is_login()) {
      err = USER_NOT_IN_CACHE;
      break;
    }

	/************TEST***********/
	int a[100];
	std::string sInvitation_code = "13819158789";
	int n, i = 0;
	int64 x = 13819158789;
	LOG(INFO) << "Invitation_code:" << sInvitation_code << '\n';
	while (x != 0)
	{
		a[i] = x % 32;
		i++;
		n = i;
		x = x / 32;
	}
	for (i = (n - 1); i >= 0; i--)
	{
		if (a[i] <= 9)
		{ 
			LOG(INFO) << "Invitation_code11111:" << (char)(a[i] + 48);
			sInvitation_code = sInvitation_code + (char)(a[i] + 48);
		}			
		else
		{
			sInvitation_code = sInvitation_code + (char)(a[i] - 10 + 'a');
		}

	}
	LOG(INFO) << "Invitation_code:" << sInvitation_code << '\n';
	/************TEST***********/


    LOG(INFO) << "oldpwd:" << rev.old_passwd();
    LOG(INFO) << "pwd" << p->passwd();
    if (p->passwd() != rev.old_passwd()) {
      err = CHANGE_OLD_PWD_ERR;
      break;
    } else {
      err = user_mysql_->ChangePwdUpdate(rev.uid(), rev.new_passwd());
      if (err < 0)
        break;
      p->set_passwd(rev.new_passwd());
      SendMsg(socket, packet, NULL, CHANGE_PASSWD_RLY);
    }
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, CHANGE_PASSWD_RLY);
  }
  return err;
}

int32 UserInterface::ServiceCity(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    DicValue dic;
    err = user_mysql_->ServiceCitySelect(&dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, SERVICE_CITY_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, SERVICE_CITY_RLY);
  }
  return err;
}

int32 UserInterface::RecommendGuide(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  RecommendGuideRecv rev(*packet);
  do {
    err = rev.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    err = user_mysql_->RecommendGuideSelect(rev.city_code(),
                                            rev.recommend_type(), &dic);
    if (err < 0)
      break;
    dic.SetBigInteger(L"recommend_type_", rev.recommend_type());
    SendMsg(socket, packet, &dic, GUIDE_RECOMMEND_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, GUIDE_RECOMMEND_RLY);
  }
  return err;
}

int32 UserInterface::NearbyGuide(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  ObtainGuideRecv rev(*packet);
  do {
    err = rev.Deserialize();
    if (err < 0)
      break;
    DicValue dic;
    double point[4];
    util::BonderOfCoordinate(rev.longitude(), rev.latitude(), rev.distance(),
                             point);
    err = user_mysql_->NearGuideSelect(point, &dic);
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, NEARBY_GUIDE_RLY);
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, NEARBY_GUIDE_RLY);
  }
  return err;
}

int32 UserInterface::UserLogin(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  LoginRecv rev(*packet);
  do {
    err = rev.Deserialize();
    if (err < 0)
      break;
    if (UserIsLogin(rev.phone_num())) {
      // todo
    }
    DicValue dic;
    err = AuthorUser(rev.phone_num(), rev.passwd(), rev.user_type(), &dic);
    LOG(INFO) << "UserLogin err:" << err;
    if (err < 0)
      break;
    SendMsg(socket, packet, &dic, USER_LOGIN_RLY);
    AddUser(socket, &dic, rev.user_type(), rev.passwd());
  } while (0);
  if (err < 0) {
    SendError(socket, packet, err, USER_LOGIN_RLY);
  }
  LOG(INFO) << "UserLogin finish err:" << err;
  return err;
}

bool UserInterface::UserIsLogin(std::string u) {
  return false;
}

void UserInterface::AddUser(int32 fd, DicValue* v, int64 type,
                            std::string pwd) {
  UserInfo* user = NULL;
//游客
  if (type == 1)
    user = new Visitor();
  else if (type == 2)
    user = new Guide();
  else
    user = new Coordinator();
  user->Serialization(v);
  user->set_user_type(type);
  user->set_is_login(true);
  user->set_socket_fd(fd);
  user->set_passwd(pwd);
  LOG(INFO) << "adduser login :" << user->uid();
  data_share_mgr_->AddUser(user);
}

int32 UserInterface::AuthorUser(std::string phone, std::string passwd,
                                int32 type, DicValue* v) {
  int32 err = 0;
  do {
    err = user_mysql_->UserLoginSelect(phone, passwd, type, time(NULL), v);
    if (err < 0)
      break;
  } while (0);
  return err;
}

void UserInterface::SendPacket(const int socket, PacketHead* packet) {

  char* s = new char[packet->packet_length()];
  LOG(INFO) << "packet body:" << packet->body_str();
  memset(s, 0, packet->packet_length());
  memcpy(s, &packet->head(), HEAD_LENGTH);
  memcpy(s + HEAD_LENGTH, packet->body_str().c_str(),
         packet->body_str().length());
  int total = util::SendFull(socket, s, packet->packet_length());
  delete[] s;
  s = NULL;
  LOG_IF(ERROR, total != packet->packet_length()) << "send packet wrong:opcode["
      << packet->operate_code() << "]";
}

void UserInterface::SendError(const int socket, PacketHead* packet, int32 err,
                              int16 opcode) {
  PacketErr p_err;
  p_err.set_head(packet->head());
  p_err.set_type(ERROR_TYPE);
  p_err.set_error(err);
  p_err.Serialize();
  p_err.AdapterLen();
  p_err.set_operate_code(opcode);
  SendPacket(socket, &p_err);
}

void UserInterface::SendMsg(const int socket, PacketHead* packet, DicValue* dic,
                            int16 opcode) {
  PacketHead send;
  send.set_head(packet->head());
  send.Serialize(dic);
  send.AdapterLen();
  send.set_operate_code(opcode);
  SendPacket(socket, &send);
}
}  // namespace user

