// Copyright (c) 2016 The tourism Authors. All rights reserved.
// user_manager.cc
// Created on: 2016年8月10日.
// Author: Paco.

#include "user/user_manager.h"

#include <stdio.h>

#include "glog/logging.h"
#include "logic/logic_comm.h"

#include "user/user_opcode.h"
#include "user/user_interface.h"

namespace user {
UserManager* UserManager::instance_ = NULL;

UserManager* UserManager::GetInstance() {
  if (instance_ == NULL)
    instance_ = new UserManager();
  return instance_;
}

void UserManager::FreeInstance() {
  if (instance_ == NULL) {
    delete instance_;
    instance_ = NULL;
  }
}

UserManager::UserManager() {

}

UserManager::~UserManager() {

}

int32 UserManager::AssignPacket(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  UserInterface* interface = UserInterface::GetInstance();
  //LOG(INFO)<< "AssignPacket opcode" << (int)packet->operate_code();
  LOG_MSG2("AssignPacket opcode %d\n", (int)packet->operate_code());
  switch (packet->operate_code()) {
    case USER_LOGIN_REQ: {
      interface->UserLogin(socket, packet);
      break;
    }
    case NEARBY_GUIDE_REQ: {
      interface->NearbyGuide(socket, packet);
      break;
    }
    case GUIDE_RECOMMEND_REQ: {
      interface->RecommendGuide(socket, packet);
      break;
    }
    case GUIDE_DETAIL_REQ: {
      interface->GuideDetail(socket, packet);
      break;
    }
    case PACKET_HEART_REQ: {
      interface->HeartPacket(socket, packet);
      break;
    }
    case SERVICE_CITY_REQ: {
      interface->ServiceCity(socket, packet);
      break;
    }
    case CHANGE_PASSWD_REQ: {
      interface->ChangePasswd(socket, packet);
      break;
    }
    case USER_INFO_REQ: {
      interface->ObtainUserInfo(socket, packet);
      break;
    }
    case ALIPAY_SEVER_REQ: {
      interface->AlipayServer(socket, packet);
      break;
    }
    case ALIPAY_CLIENT_REQ: {
      interface->AlipayClient(socket, packet);
      break;
    }
    case QUERY_VERIFY_CODE_REQ: {
      interface->ObtainVerifyCode(socket, packet);
      break;
    }
    case REGISTER_ACCOUNT_REQ: {
      interface->RegisterAccount(socket, packet);
      break;
    }
    case IMPROVE_DATA_REQ: {
      interface->ImproveUserData(socket, packet);
      break;
    }
    case OBTAIN_TRIP_REQ: {
      interface->ObtainTripRecord(socket, packet);
      break;
    }
    case SERVICE_INFO_REQ: {
      interface->ObtainServiceInfo(socket, packet);
      break;
    }
    case DRAW_BILL_REQ: {
      interface->DrawBillTrip(socket, packet);
      break;
    }
    case DEVICE_TOKEN_REQ: {
      interface->DeviceToken(socket, packet);
      break;
    }
    case INVOICE_RECORD_REQ: {
      interface->BillingRecord(socket, packet);
      break;
    }
    case INVOICE_DETAIL_REQ: {
      interface->InvoiceDetail(socket, packet);
      break;
    }
    case BLACKCARD_PRIVILEGE_REQ: {
      interface->BlackcardPrivilege(socket, packet);
      break;
    }
    case BLACKCARD_INFO_REQ: {
      interface->BlackcardInfo(socket, packet);
      break;
    }
    case BLACKCARD_CONSUME_RECORD_REQ: {
      interface->BlackcardConsumeRecord(socket, packet);
      break;
    }
    case BLACKCARD_PRICE_INFO_REQ: {
      interface->BlackcardPriceInfo(socket, packet);
      break;
    }
    case PLACE_BLACKCARD_ORDER_REQ: {
      interface->BlackcardPlaceOrder(socket, packet);
      break;
    }
    case SKILL_INFO_REQ: {
      interface->SkillsInfo(socket, packet);
      break;
    }
    case NEW_APPOINTMENT_REQ: {
      interface->NewAppointment(socket, packet);
      break;
    }
    case APPOINTMENT_RECORD_REQ: {
      interface->AppointmentRecord(socket, packet);
      break;
    }
    case IMG_TOKEN_REQ: {
      interface->UpImgToken(socket, packet);
      break;
    }
    case WX_PLACE_ORDER_REQ: {
      interface->WXPlaceOrder(socket, packet);
      break;
    }
    case WXPAY_CLIENT_REQ: {
      interface->WXPayClientResponse(socket, packet);
      break;
    }
    case WXPAY_SERVER_REQ: {
      interface->WXPayServerResponse(socket, packet);
      break;
    }
    case IDENTITY_PIC_REQ: {
      interface->IdentityPicInfo(socket, packet);
      break;
    }
    case IDENTITY_STATUS_REQ: {
      interface->IdentityReviewStatus(socket, packet);
      break;
    }
    case SHARE_TOURISM_TYPE_REQ: {
      interface->TourismShareTypeInfo(socket, packet);
      break;
    }
    case SHARE_TOURISM_RECOMMEND_REQ: {
      interface->TourismShareRecommend(socket, packet);
      break;
    }
    case SHARE_TOURISM_LIST_REQ: {
      interface->TourismShareList(socket, packet);
      break;
    }
    case SHARE_SKILL_LIST_REQ: {
      interface->SkillShareList(socket, packet);
      break;
    }
    case SHARE_TOURISM_DETAIL_REQ: {
      interface->TourismShareDetail(socket, packet);
      break;
    }
    case USER_CASH_REQ: {
      interface->UserCashInfo(socket, packet);
      break;
    }
    case SHARE_SKILL_DETAIL_REQ: {
      interface->SkillShareDetail(socket, packet);
      break;
    }
    case SHARE_SKILL_DISCUSS_REQ: {
      interface->SkillShareDiscuss(socket, packet);
      break;
    }
    case ENTRY_SHARE_SKILL_REQ: {
      interface->EntrySkillShare(socket, packet);
      break;
    }
    case GUIDE_INFO_REQ: {
      interface->GuidesInfo(socket, packet);
      break;
    }
    case ORDER_DETAILS_REQ: {
      interface->OrderDetails(socket, packet);
      break;
    }
    case VERIFY_PASSWD_REQ: {
      interface->VerifyPasswd(socket, packet);
      break;
    }
    case CHECK_SMS_CODE_REQ: {
      interface->CheckSMSCode(socket, packet);
      break;
    }
    case CHANGE_COMM_PASSWD_REQ: {
      interface->ChangePayPasswd(socket, packet);
      break;
    }
    case VERIFY_VLEADER_HEAD_REQ: {
      interface->VerifyVleaderHead(socket, packet);
      break;
    }
    case CHANGE_USER_INFO_REQ: {
	  interface->ChangeUserInfo(socket, packet);
	  break;
	} 
    case CHANGE_BANK_CARD_REQ: {
      interface->ChangeBankCard(socket, packet);
	  break;
    }
    case BANK_CARD_INFO_REQ: {
      interface->BankCardInfo(socket, packet);
      break;
    }
    case CHANGE_DEFAULT_BANK_CARD_REQ: {
      interface->ChangeDefaultBankCard(socket, packet);
      break;
    }
    case USER_WITHDRAW_REQ: {
      interface->UserWithdraw(socket, packet);
      break;
    }
    case USER_WITHDRAW_RECORD_REQ: {
      interface->UserWithdrawRecord(socket, packet);
      break;
    }
    case USER_UPLOAD_PHOTO_REQ: {
      interface->UserUploadPhoto(socket, packet);
      break;
    }
    case USER_PHOTO_ALBUM_REQ: {
      interface->UserPhotoAlbum(socket, packet);
      break;
	}
	case UPLOAD_CONTACTS_REQ: {
	  interface->UploadContacts(socket, packet);
	  break;
	}
	case USER_REG_INVITATIONCODE_REQ: {
      interface->UserRegInvitationCode(socket, packet);
      break;
	}
	case USER_APP_VERSION_INFO_REQ: {
       interface->UserAppVersionInfo(socket, packet);
       break;
	}
	case USER_INSURANCE_PRICE_REQ: {
      interface->UserInsurancePrice(socket, packet);
      break;
	}
	case USER_INSURANCE_PAY_REQ: {
      interface->UserInsurancePay(socket, packet);
      break;
	}
	case USER_IDCARD_INFO_REQ: {
        interface->UserIdCardInfo(socket, packet);
		break;
	}
    case GUIDE_SERVICE_LIST_REQ: {
      interface->GuideServiceList(socket, packet);
      break;
    }
    case CHANGE_GUIDE_SERVICE_REQ: {
      interface->ChangeGuideService(socket, packet);
      break;
    }
    case GUIDE_ORDER_RECORD_REQ: {
      interface->GuideOrderRecord(socket, packet);
      break;
    }
    case GUIDE_ORDER_DETAIL_REQ: {
      interface->GuideOrderDetail(socket, packet);
      break;
    }
    case DEFINE_GUIDE_SKILL_REQ: {
      interface->DefineGuideSkills(socket, packet);
      break;
    }
  }
  return err;
}

int32 UserManager::CheckHeartLoss() {
  UserInterface* interface = UserInterface::GetInstance();
  interface->CheckHeartLoss();
  return 0;
}

int32 UserManager::InitShareGuideData() {
  UserInterface* interface = UserInterface::GetInstance();
  interface->InitShareGuide();
  return 0;
}

int32 UserManager::OnSockClose(const int fd) {
  UserInterface* interface = UserInterface::GetInstance();
  interface->CloseSocket(fd);
  return 0;
}

int32 UserManager::OrderStatusCheck() {
  UserInterface* interface = UserInterface::GetInstance();
  interface->NopayOrderStatusCheck();
  return 0;
}

}  // namespace user

