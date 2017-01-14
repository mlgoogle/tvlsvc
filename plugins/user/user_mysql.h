// Copyright (c) 2016 The tourism Authors. All rights reserved.
// user_mysql.h
// Created on: 2016年8月16日.
// Author: Paco.
#ifndef PLUGINS_USER_USER_MYSQL_H_
#define PLUGINS_USER_USER_MYSQL_H_

#include "public/config/config.h"
#include "public/basic/basictypes.h"

#include "pub/storage/data_engine.h"

#include "pub/net/typedef.h"

namespace user {

class UserMysql {
 public:
  UserMysql(config::FileConfig* config);
  ~UserMysql();

 public:
  int32 UserLoginSelect(std::string phone, std::string pass, int32 type,
                        int64 time, DicValue* dic);

  int32 NearGuideSelect(double* point, DicValue* dic);
  int32 GuideDetailSelect(int64 uid, DicValue* dic);
  int32 GuideServiceSelect(int64 uid, DicValue* dic);
  int32 UserDetailSelect(std::string uids, DicValue* dic);
  int32 ServiceCitySelect(DicValue* dic);
  int32 RecommendGuideSelect(int64 city, int64 type, DicValue* dic);
  int32 GuidesInfoSelect(std::string uids, DicValue* dic);
  int32 RegisterInsertAndSelect(std::string phone, std::string pass, int64 type,
	  DicValue* dic);
  int32 ChangePwdUpdate(int64 uid, std::string pwd);
  int32 ImproveUserUpdate(int64 uid, int64 sex, std::string nickname,
                          std::string headurl, std::string addr, double lon,
                          double lat);

  int32 TripRecordSelect(int64 uid, int64 oid, int64 count, DicValue* dic);
  int32 ServiceInfoSelect(std::string oid_str, DicValue* dic);
  int32 InvoiceInfoInsert(int64 uid, std::string oids, std::string title,
                          std::string num, std::string comp, int64 type,
                          std::string name, std::string phone, std::string area,
                          std::string addr, std::string remarks, DicValue* dic);

  int32 InvoiceRecordSelect(int64 uid, int64 count, int64 iid, DicValue* dic);

  int32 InvoiceDetailSelect(int64 iid, DicValue* dic);

  int32 DeviceTokenUpdate(int64 uid, std::string dt);
  int32 BlackcardPrivilegeSelect(DicValue* dic);
  int32 BlackcardInfoSelect(int64 uid, DicValue* dic);

  //@brief 查询用户uid黑卡消费记录
  //@author paco
  //@param[in] uid 用户id
  //@param[out] dic 查询结果集
  //@return 返回mysql查询状态 正常返回0，异常返回负数 ，详情见pub/comm/comm_head.h
  //@warning
  int32 BlackcardConsumeRecordSelect(int64 uid, DicValue* dic);

  int32 BlackcardPriceInfoSelect(DicValue* dic);

  int32 BlackcardPlaceOrderInsertAndSelect(int64 uid, int64 lv, DicValue* dic);

  //@brief 查询服务技能
  //@author paco
  //@param[in]
  //@param[out] dic 技能结果集
  //@return 返回mysql查询状态 正常返回0，异常返回负数 ，详情见pub/comm/comm_head.h
  //@warning
  int32 SkillsInfoSelect(DicValue* dic);

  int32 NewAppointmentInsert(int64 uid, int64 city, int64 start, int64 end,
                             std::string skill, int64 other, std::string name,
                             int64 gender, std::string phone,
                             std::string remark, DicValue* dic);

  int32 AppointmentRecordSelect(int64 uid, int64 lastid, int64 count,
                                DicValue* dic);

  int32 ImgTokenSelect(DicValue* dic);

  int32 RechargeInfoInsertAndSelect(int64 uid, int64 price, DicValue* dic);

  int32 ChangeRechargeStatusAndSelect(int64 rid, int64 result, DicValue* dic);

  int32 IdentityInfoInsertAndSelect(int64 uid, std::string front,
                                    std::string back, DicValue* dic);

  int32 IdentityReviewStatusSelect(int64 uid, DicValue* dic);

  //读取旅游分享信息
  int32 ShareTourismDetailsSelect(DicValue* dic);
  //技能分享信息
  int32 ShareSkillDetailsSelect(DicValue* dic);
  int32 ShareTourismTypeSelect(DicValue* dic);

  int32 ShareSkillEntrySelect(int64 share_id, DicValue* dic);

  int32 ShareSkillDiscussSelect(int64 share_id, int64 lastid, int64 count,
                                DicValue* dic);

  int32 EntryShareSkillInsert(int64 share_id, int64 uid, DicValue* dic);

  int32 GuideServerUpdateAndSelect(int64 uid, std::list<std::string> sqls,
                                   DicValue* dic);

  int32 UserCashSelect(int64 uid, DicValue* dic);

  int32 OrderDetailsSelect(int64 oid, int64 type, DicValue* dic);
  
  int32 VerifyVleaderHeadInsert(int64 uid, std::string head_url, DicValue* dic);

  int32 ChangeUserInfoUpdate(int64 uid, std::string nickname, int64 gender, std::string, DicValue* dic);

  int32 ChangeBankCardInsertOrDelete(int64 type, int64 uid, std::string account,
					std::string bank_username, int64 bank, DicValue* dic);

  int32 BankCardInfoSelect(int64 uid, DicValue* dic);

  int32 ChangeDefaultBankCardUpdate(int64 uid, std::string account, DicValue* dic);

  int32 UserWithdrawInsertAndSelect(int64 uid, std::string account, int64 cash, DicValue* dic);

  int32 UserWithdrawRecordSelect(int64 uid, std::string account, int64 size, int64 num, DicValue* dic);

  int32 UserUploadPhotoInsert(int64 uid, std::list<std::string> sqls, DicValue* dic);

  int32 UserPhotoAlbumSelect(int64 uid, int64 size, int64 num, DicValue* dic);

  int32 WriteDatas(std::list<std::string> sqls);

  int32 UserInvitationCodeUpDate(std::string phoneNum, std::string invitationCode, int invitationDate, DicValue* dic);

  int32 UserAppVersionInfo(int64 appType,DicValue* dic);

  int32 UserIdCardInfo(std::string IdCardNum, std::string IdCardName, std::string IdCardUrlName, int64 uid, DicValue* dic);

  int32 CheckPasswdSelect(int64 uid, std::string pass, int64 type,
                          DicValue* dic);

  int32 ChangePasswdSelect(int64 uid, std::string oldpass, std::string newpass,
                           int64 ctype, int64 ptype, DicValue* dic);

  int32 GuideOrderSelect(int64 uid, int64 lastid, int64 count, DicValue* dic);

  int32 GuideOrderDetailSelect(int64 oid, DicValue* dic);

  int32 DefineGuideSkillsUpdateSelect(int64 uid, int64 type, std::string skills,
                                      DicValue* dic);

  int32 CancelOrderStatusUpdate();

  static void CallUserLoginSelect(void* param, Value* value);
  static void CallUserDetailSelect(void* param, Value* value);
  static void CallNearGuideSelect(void* param, Value* value);
  static void CallGuideDetailSelect(void* param, Value* value);
  static void CallGuideServiceSelect(void* param, Value* value);
  static void CallServiceCitySelect(void* param, Value* value);
  static void CallRecommendGuideSelect(void* param, Value* value);
  static void CallRegisterInsertAndSelect(void* param, Value* value);
  static void CallTripRecordSelect(void* param, Value* value);
  static void CallServiceInfoSelect(void* param, Value* value);
  static void CallBlackServiceInfoSelect(void* param, Value* value);
  static void CallBlackBuyInfoSelect(void* param, Value* value);
  static void CallOrderStatusSelect(void* param, Value* value);
  static void CallInvoiceInfoInsert(void* param, Value* value);
  static void CallInvoiceRecordSelect(void* param, Value* value);
  static void CallInvoiceDetailSelect(void* param, Value* value);
  static void CallBlackcardPrivilegeSelect(void* param, Value* value);
  static void CallBlackcardInfoSelect(void* param, Value* value);
  static void CallBlackcardConsumeRecordSelect(void* param, Value* value);
  static void CallBlackcardBuyOrderSelect(void* param, Value* value);
  static void CallSkillsInfoSelect(void* param, Value* value);
  static void CallImgTokenSelect(void* param, Value* value);
  static void CallRechargeInfoInsertAndSelect(void* param, Value* value);
  static void CallChangeRechargeStatusAndSelect(void* param, Value* value);
  static void CallIdentityInfoInsertAndSelect(void* param, Value* value);
  static void CallIdentityReviewStatusSelect(void* param, Value* value);
  static void CallShareTourismDetailsSelect(void* param, Value* value);
  static void CallShareTourismTypeSelect(void* param, Value* value);
  static void CallShareSkillDetailsSelect(void* param, Value* value);
  static void CallUserCashSelect(void* param, Value* value);
  static void CallAppointmentRecordSelect(void* param, Value* value);
  static void CallShareSkillEntrySelect(void* param, Value* value);
  static void CallShareSkillDiscussSelect(void* param, Value* value);
  static void CallEntryShareSkillInsert(void* param, Value* value);
  static void CallNewAppointmentInsert(void* param, Value* value);
  static void CallOrderDetailsSelect(void* param, Value* value);
  static void CallBlackcardPriceInfoSelect(void* param, Value* value);
  static void CallBlackcardPlaceOrderInsertAndSelect(void* param, Value* value);
  static void CallCheckPasswdSelect(void* param, Value* value);
  static void CallChangePasswdSelect(void* param, Value* value);
  static void CallGuideOrderSelect(void* param, Value* value);
  static void CallBankCardInfoSelect(void* param, Value* value);
  static void CallUserWithdrawInsertAndSelect(void* param, Value* value);
  static void CallUserWithdrawRecordSelect(void* param, Value* value);
  static void CallUserPhotoAlbumSelect(void* param, Value* value);
  static void CallGuideOrderDetailSelect(void* param, Value* value);
  static void CallDefineGuideSkillsUpdateSelect(void* param, Value* value);
  static void CallUserInvitationCodeUpDate(void* param, Value* value);
  static void CallUserAppVersionInfo(void* param, Value* value);
 private:
  base_logic::DataEngine* mysql_engine_;
};

}  // namespace user

#endif  // PLUGINS_USER_USER_MYSQL_H_
