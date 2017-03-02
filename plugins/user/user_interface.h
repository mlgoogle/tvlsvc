// Copyright (c) 2016 The tourism Authors. All rights reserved.
// user_interface.h
// Created on: 2016年8月10日.
// Author: Paco.
#ifndef PLUGINS_USER_USER_INTERFACE_H_
#define PLUGINS_USER_USER_INTERFACE_H_

#include "public/basic/basictypes.h"
#include "public/config/config.h"
#include "pub/net/proto_buf.h"
#include "pub/share/data_share_mgr.h"

#include "user/user_mysql.h"
namespace user {

class UserInterface {
 private:
  UserInterface();
  ~UserInterface();

 public:
  static UserInterface* GetInstance();
  static void FreeInstance();
  void test();
 public:
  void InitConfig(config::FileConfig* config);

  int32 UserLogin(const int32 socket, PacketHead* packet);

  int32 NearbyGuide(const int32 socket, PacketHead* packet);

  int32 HeartPacket(const int32 socket, PacketHead* packet);

  int32 GuideDetail(const int32 socket, PacketHead* packet);

  int32 ObtainUserInfo(const int32 socket, PacketHead* packet);

  int32 RecommendGuide(const int32 socket, PacketHead* packet);

  int32 ServiceCity(const int32 socket, PacketHead* packet);

  int32 ChangePasswd(const int32 socket, PacketHead* packet);

  int32 AlipayServer(const int32 socket, PacketHead* packet);

  int32 AlipayClient(const int32 socket, PacketHead* packet);

  int32 ObtainVerifyCode(const int32 socket, PacketHead* packet);

  int32 RegisterAccount(const int32 socket, PacketHead* packet);

  int32 ImproveUserData(const int32 socket, PacketHead* packet);

  int32 ObtainTripRecord(const int32 socket, PacketHead* packet);

  int32 ObtainServiceInfo(const int32 socket, PacketHead* packet);

  int32 DrawBillTrip(const int32 socket, PacketHead* packet);

  int32 BillingRecord(const int32 socket, PacketHead* packet);

  int32 InvoiceDetail(const int32 socket, PacketHead* packet);

  int32 DeviceToken(const int32 socket, PacketHead* packet);

  int32 BlackcardPrivilege(const int32 socket, PacketHead* packet);

  int32 BlackcardInfo(const int32 socket, PacketHead* packet);

  int32 BlackcardConsumeRecord(const int32 socket, PacketHead* packet);

  int32 BlackcardPriceInfo(const int32 socket, PacketHead* packet);

  int32 BlackcardPlaceOrder(const int32 socket, PacketHead* packet);

  int32 SkillsInfo(const int32 socket, PacketHead* packet);


  int32 NewAppointment(const int32 socket, PacketHead* packet);
  int32 AppointmentRecord(const int32 socket, PacketHead* packet);

  int32 UpImgToken(const int32 socket, PacketHead* packet);

  int32 WXPlaceOrder(const int32 socket, PacketHead* packet);

  int32 WXPayClientResponse(const int32 socket, PacketHead* packet);

  int32 WXPayServerResponse(const int32 socket, PacketHead* packet);

  //服务者上传身份证信息
  int32 IdentityPicInfo(const int32 socket, PacketHead* packet);

  int32 IdentityReviewStatus(const int32 socket, PacketHead* packet);

  int32 TourismShareTypeInfo(const int32 socket, PacketHead* packet);

  int32 TourismShareRecommend(const int32 socket, PacketHead* packet);

  int32 TourismShareList(const int32 socket, PacketHead* packet);

  int32 SkillShareList(const int32 socket, PacketHead* packet);

  int32 TourismShareDetail(const int32 socket, PacketHead* packet);

  int32 SkillShareDetail(const int32 socket, PacketHead* packet);

  int32 SkillShareDiscuss(const int32 socket, PacketHead* packet);

  int32 EntrySkillShare(const int32 socket, PacketHead* packet);

  int32 GuideServiceList(const int32 socket, PacketHead* packet);

  int32 ChangeGuideService(const int32 socket, PacketHead* packet);

  int32 UserCashInfo(const int32 socket, PacketHead* packet);

  int32 GuidesInfo(const int32 socket, PacketHead* packet);

  int32 OrderDetails(const int32 socket, PacketHead* packet);
  
  int32 VerifyVleaderHead(const int32 socket, PacketHead* packet);

  int32 ChangeUserInfo(const int32 socket, PacketHead* packet);

  int32 ChangeBankCard(const int32 socket, PacketHead* packet);

  int32 BankCardInfo(const int32 socket, PacketHead* packet);

  int32 ChangeDefaultBankCard(const int32 socket, PacketHead* packet);

  int32 UserWithdraw(const int32 socket, PacketHead* packet);

  int32 UserWithdrawRecord(const int32 socket, PacketHead* packet);

  int32 UserUploadPhoto(const int32 socket, PacketHead* packet);

  int32 UserPhotoAlbum(const int32 socket, PacketHead* packet);

  int32 UploadContacts(const int32 socket, PacketHead* packet);

  int32 UserRegInvitationCode(const int32 socket, PacketHead* packet);

  int32 UserInsurancePrice(const int32 socket, PacketHead* packet);

  int32 UserInsurancePay(const int32 socket, PacketHead* packet);

  int32 UserIdCardInfo(const int32 socket, PacketHead* packet);

  int32 UserServicrPrice(const int32 socket, PacketHead* packet);

  int32 FollowType(const int32 socket, PacketHead* packet);

  int32 FollowList(const int32 socket, PacketHead* packet);

  int32 FollowNumber(const int32 socket, PacketHead* packet);

  int32 UserAppVersionInfo(const int32 socket, PacketHead* packet);

  int32 VerifyPasswd(const int32 socket, PacketHead* packet);

  int32 CheckSMSCode(const int32 socket, PacketHead* packet);

  int32 ChangePayPasswd(const int32 socket, PacketHead* packet);

  int32 GuideOrderRecord(const int32 socket, PacketHead* packet);

  int32 GuideOrderDetail(const int32 socket, PacketHead* packet);

  int32 DefineGuideSkills(const int32 socket, PacketHead* packet);

  int32 AuthorUser(std::string phone, std::string passwd, int32 type,
                   DicValue* v);

  int32 CheckHeartLoss();
//更新共享数据，旅游分享和技能分享
  int32 InitShareGuide();
  //定时监测订单状态
  int32 NopayOrderStatusCheck();
  int32 CloseSocket(const int fd);

  void AddUser(int32 fd, DicValue* v, int64 type, std::string pwd);

  //判断用户是否登陆
  bool UserIsLogin(std::string u);

  void SendPacket(const int socket, PacketHead* packet);

  void SendError(const int socket, PacketHead* packet, int32 err, int16 opcode);

  void SendMsg(const int socket, PacketHead* packet, DicValue* dic,
               int16 opcode);

  void InitShareDataMgr(share::DataShareMgr* data);
 private:
  int32 InitShareType();
  int32 InitShareDetails();
  int32 InitShareSkills();

  static UserInterface* instance_;
  UserMysql* user_mysql_;
  share::DataShareMgr* data_share_mgr_;
};

}  // namespace user



#endif  // PLUGINS_USER_USER_INTERFACE_H_
