// Copyright (c) 2016 The tourism Authors. All rights reserved.
// user_proto.h
// Created on: 2016年8月12日.
// Author: Paco.
#ifndef PLUGINS_USER_USER_PROTO_H_
#define PLUGINS_USER_USER_PROTO_H_

#include "pub/net/proto_buf.h"
namespace user {

//recv
class LoginRecv:public PacketHead {
 public:
  LoginRecv(PacketHead packet);
  int32 Deserialize();

  inline std::string phone_num() { return phone_num_; }
  inline std::string passwd() { return passwd_; }
  inline int64 user_type() { return user_type_; }
 private:
  std::string phone_num_;
  std::string passwd_;
  int64 user_type_;
};

class SMSCodeLoginRecv : public PacketHead {
 public:
  SMSCodeLoginRecv(PacketHead packet);
  int32 Deserialize();
  inline int64 timestamp() { return timestamp_; }
  inline int64 verify_code() { return verify_code_; }
  inline std::string phone_num() { return phone_num_; }
  inline std::string token() { return token_; }
  inline int64 user_type() { return user_type_; }
 private:
  int64 timestamp_;
  int64 verify_code_;
  int64 user_type_;
  std::string phone_num_;
  std::string token_;
};

class Heartbeat:public PacketHead {
 public:
  Heartbeat(PacketHead packet);
  int32 Deserialize();

  inline int64 uid() { return uid_; }
 private:
  int64 uid_;
};

class ObtainGuideRecv:public PacketHead {
 public:
  ObtainGuideRecv(PacketHead packet);
  int32 Deserialize();

  inline double longitude() { return longitude_; }
  inline double latitude() { return latitude_; }
  inline double distance() { return distance_; }
 private:
  double longitude_;
  double latitude_;
  double distance_;
};

class RecommendGuideRecv:public PacketHead {
 public:
  RecommendGuideRecv(PacketHead packet);
  int32 Deserialize();
  inline int64 city_code() { return city_code_; }
  inline int64 recommend_type() { return recommend_type_; }
 private:
  int64 city_code_;
  int64 recommend_type_; //1-推荐 2-预约
};

class ChangePasswdRecv:public PacketHead {
 public:
  ChangePasswdRecv(PacketHead packet);
  int32 Deserialize();
  inline int64 uid() { return uid_; }
  inline std::string old_passwd() { return old_passwd_; }
  inline std::string new_passwd() { return new_passwd_; }
 private:
  int64 uid_;
  std::string old_passwd_;
  std::string new_passwd_;
};

class ObtainVerifyCodeRecv : public PacketHead {
 public:
  ObtainVerifyCodeRecv(PacketHead packet);
  int32 Deserialize();
  inline int64 verify_type() { return verify_type_; }
  inline std::string phone_num() { return phone_num_; }
 private:
  int64 verify_type_;
  std::string phone_num_;
};

class RegisterAccountRecv : public PacketHead {
 public:
  RegisterAccountRecv(PacketHead packet);
  int32 Deserialize();
  inline int64 timestamp() { return timestamp_; }
  inline int64 verify_code() { return verify_code_; }
  inline std::string phone_num() { return phone_num_; }
  inline std::string passwd() { return passwd_; }
  inline std::string token() { return token_; }
  inline int64 user_type() { return user_type_; }
  inline std::string invitation_phone_num() { return invitation_phone_num_; }
 private:
  int64 timestamp_;
  int64 verify_code_;
  int64 user_type_;
  std::string phone_num_;
  std::string passwd_;
  std::string token_;
  std::string invitation_phone_num_;
};

class ImproveDataRecv : public PacketHead {
 public:
  ImproveDataRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 uid() { return uid_; }
  inline int64 gender() { return gender_; }
  inline std::string nickname() { return nickname_; }
  inline std::string head_url() { return head_url_; }
  inline std::string address() { return address_; }
  inline double longitude() { return longitude_; }
  inline double latitude() { return latitude_; }

 private:
  int64 uid_;
  int64 gender_;
  std::string nickname_;
  std::string head_url_;
  std::string address_;
  double longitude_;
  double latitude_;
};

class UserDetailRecv : public PacketHead {
public:
 UserDetailRecv(PacketHead packet);
 int32 Deserialize();

 inline std::string uid_str() { return uid_str_; }
private:
 std::string uid_str_; // "1,2,3,4"
};

class ObtainTripRecv : public PacketHead {
 public:
  ObtainTripRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 uid() { return uid_; }
  inline int64 order_id() { return order_id_; }
  inline int64 count() { return count_; }
 private:
  int64 uid_;
  int64 order_id_; //上次请求的最小orderid
  int64 count_; // 请求条数
};

class ServiceDetailsRecv : public PacketHead {
 public:
  ServiceDetailsRecv(PacketHead packet);
  int32 Deserialize();

  inline std::string oid_str() { return oid_str_; }
 private:
  std::string oid_str_; // "1,2,3,4"
};

class DrawBillRecv : public PacketHead {
 public:
  DrawBillRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 uid() { return uid_; }
  inline std::string oid_str() { return oid_str_; }
  inline std::string title() { return title_; }
  inline std::string taxpayer_num() { return taxpayer_num_; }
  inline std::string company_addr() { return company_addr_; }
  inline int64 invoice_type() { return invoice_type_; }
  inline std::string user_name() { return user_name_; }
  inline std::string user_mobile() { return user_mobile_; }
  inline std::string area() { return area_; }
  inline std::string addr_detail() { return addr_detail_; }
  inline std::string remarks() { return remarks_; }
 private:
  int64 uid_;
  std::string oid_str_; // 订单号集合"1,2,3,4"
  std::string title_; //发票抬头
  std::string taxpayer_num_; //纳税号
  std::string company_addr_; // 注册地址
  int64 invoice_type_; //发票类型
  std::string user_name_; //联系人
  std::string user_mobile_; //联系电话
  std::string area_; //邮寄区域
  std::string addr_detail_; //详细地址
  std::string remarks_; //备注
};

class BillRecordRecv : public PacketHead {
 public:
  BillRecordRecv(PacketHead packet);
  int32 Deserialize();
  inline int64 uid() { return uid_; }
  inline int64 count() { return count_; }
  inline int64 last_invoice_id() { return last_invoice_id_; }
 private:
  int64 uid_;
  int64 count_;
  int64 last_invoice_id_;
};

class InvoiceDetailRecv : public PacketHead {
 public:
  InvoiceDetailRecv(PacketHead packet);
  int32 Deserialize();
  inline int64 invoice_id() { return invoice_id_; }
 private:
  int64 invoice_id_;
};

class DeviceTokenRecv : public PacketHead {
 public:
  DeviceTokenRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 uid() { return uid_; }
  inline std::string device_token() { return device_token_; }
 private:
  int64 uid_;
  std::string device_token_;
};

class BlackcardInfoRecv : public PacketHead {
 public:
  BlackcardInfoRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 uid() { return uid_; }
 private:
  int64 uid_;
};

class BlackcardConsumRecordRecv : public PacketHead {
 public:
  BlackcardConsumRecordRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 uid() { return uid_; }
 private:
  int64 uid_;
};

class BlackcardPlaceOrderRecv : public PacketHead {
 public:
  BlackcardPlaceOrderRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 uid() { return uid_; }
  inline int64 wanted_lv() { return wanted_lv_; }
 private:
  int64 uid_;
  int64 wanted_lv_;
};

class NewAppointmentRecv : public PacketHead {
 public:
  NewAppointmentRecv(PacketHead packet);
  int32 Deserialize();
  inline int64 uid() { return uid_; }
  inline int64 city_code() { return city_code_; }
  inline int64 start_time() { return start_time_; }
  inline int64 end_time() { return end_time_; }
  inline std::string skills() { return skills_; }
  inline int64 is_other() { return is_other_; }
  inline std::string other_name() { return other_name_; }
  int64 other_gender() { return other_gender_; }
  inline std::string ohter_phone() { return other_phone_; }
  inline std::string remarks() { return remarks_; }
 private:
  int64 uid_;
  int64 city_code_;
  int64 start_time_;
  int64 end_time_;
  std::string skills_;
  int64 is_other_;
  std::string other_name_;
  int64 other_gender_;
  std::string other_phone_;
  std::string remarks_;
};

class WxPlaceOrderRecv : public PacketHead {
 public:
  WxPlaceOrderRecv(PacketHead packet);
  int32 Deserialize();
  inline int64 uid() { return uid_; }
  inline int64 price() { return price_; }
  inline std::string title() { return title_; }
 private:
  int64 uid_;
  std::string title_; //应用名-商品名 eg.V领队-高级服务
  int64 price_; // 订单总价  单位 分
};

class WXPayClientRecv : public PacketHead {
 public:
  WXPayClientRecv(PacketHead packet);
  int32 Deserialize();
  inline int64 uid() { return uid_; }
  inline int64 recharge_id() { return recharge_id_; }
  inline int64 pay_result() { return pay_result_; }
 private:
  int64 uid_;
  int64 recharge_id_;
  int64 pay_result_; //1-成功 2-取消
};

class WXPayServerRecv : public PacketHead {
 public:
  WXPayServerRecv(PacketHead packet);
  int32 Deserialize();
  inline int64 total_fee() { return total_fee_; }
  inline int64 recharge_id() { return recharge_id_; }
  inline int64 pay_result() { return pay_result_; }
  inline std::string appid() { return appid_; }
  inline std::string mch_id() { return mch_id_; }
 private:
  std::string appid_;
  std::string mch_id_;
  std::string xml_str_;
  int64 recharge_id_;
  int64 pay_result_; //1 - 支付成功
  int64 total_fee_;
  std::string transaction_id_;
  std::string time_end_;
};

class IdentityPicRecv : public PacketHead {
 public:
  IdentityPicRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 uid() { return uid_; }
  inline std::string front_pic() { return front_pic_; }
  inline std::string back_pic() { return back_pic_; }
 private:
  int64 uid_;
  std::string front_pic_;
  std::string back_pic_;
};

class GuideDetailRecv : public PacketHead {
 public:
  GuideDetailRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 uid() { return uid_; }
 private:
  int64 uid_;
};

class IdentityStatusRecv : public PacketHead {
 public:
  IdentityStatusRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 uid() { return uid_; }
 private:
  int64 uid_;
};

class PaggingRecv : public PacketHead {
 public:
  PaggingRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 last_id() { return last_id_; }
  inline int64 count() { return count_; }
  inline int64 page_type() { return page_type_; }
 private:
  int64 last_id_;
  int64 count_;
  int64 page_type_;
};

class ShareTourismDetailRecv : public PacketHead {
 public:
  ShareTourismDetailRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 share_id() { return share_id_; }
 private:
  int64 share_id_;
};

class ShareSkillDetailRecv : public PacketHead {
 public:
  ShareSkillDetailRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 share_id() { return share_id_; }
 private:
  int64 share_id_;
};

class ShareSkillDiscussRecv : public PacketHead {
 public:
  ShareSkillDiscussRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 share_id() { return share_id_; }
  inline int64 last_id() { return last_id_; }
  inline int64 count() { return count_; }
 private:
  int64 share_id_;
  int64 last_id_;
  int64 count_;
};

class EntryShareSkillRecv : public PacketHead {
 public:
  EntryShareSkillRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 share_id() { return share_id_; }
  inline int64 uid() { return uid_; }
 private:
  int64 share_id_;
  int64 uid_;
};

class UserCashRecv : public PacketHead {
 public:
  UserCashRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 uid() { return uid_; }
 private:
  int64 uid_;
};
//send

class AppointmentRecordRecv : public PacketHead {
 public:
  AppointmentRecordRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 uid() { return uid_; }
  inline int64 last_id() { return last_id_; }
  inline int64 count() { return count_; }
 private:
  int64 uid_;
  int64 last_id_;
  int64 count_;
};

struct ServiceData {
  int64 service_id_;
  std::string service_name_;
  int64 service_price_;
  int64 service_start_;
  int64 service_type_;
  int64 change_type_;
  int64 service_end_;
};

class ChangeGuideServiceRecv : public PacketHead {
 public:

  ChangeGuideServiceRecv(PacketHead packet);
  int32 Deserialize();
  virtual ~ChangeGuideServiceRecv();

  inline int64 uid() { return uid_; }
  inline int64 timestamp() { return timestamp_; }
  inline int64 verify_code() { return verify_code_; }
  inline std::string token() { return token_; }
  inline std::string phone_num() { return phone_num_; }
  inline std::list<ServiceData*> service_list() { return service_list_; }
 private:
  std::list<ServiceData*> service_list_;
  int64 timestamp_;
  int64 verify_code_;
  std::string token_;
  std::string phone_num_;
  int64 uid_;
};

class OrderDetailRecv : public PacketHead {
 public:
  OrderDetailRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 order_id() { return order_id_; }
  inline int64 order_type() { return order_type_; }

 private:
  int64 order_id_;
  int64 order_type_;//0-邀约 1-预约
};

class VerifyPasswdRecv : public PacketHead {
 public:
  VerifyPasswdRecv (PacketHead packet);
  int32 Deserialize();

  inline int64 uid() { return uid_; }
  inline std::string passwd() { return passwd_; }
  inline int64 passwd_type() { return passwd_type_; }
 private:
  int64 uid_;
  std::string passwd_;
  int64 passwd_type_;// 1-支付密码 2-登录密码
};

class CheckSMSCodeRecv : public PacketHead {
 public:
  CheckSMSCodeRecv (PacketHead packet);
  int32 Deserialize();

  inline int64 verify_code() { return verify_code_; }
  inline int64 verify_type() { return verify_type_; }
  inline int64 timestamp() { return timestamp_; }
  inline std::string phone_num() { return phone_num_; }
  inline std::string token() { return token_; }
 private:
  int64 verify_code_;
  int64 verify_type_; //0-注册 1-登录 2-更新服务
  int64 timestamp_; //时间戳
  std::string phone_num_;
  std::string token_;
};

class ChangePayPasswdRecv : public PacketHead {
 public:
  ChangePayPasswdRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 uid() { return uid_; }
  inline std::string new_passwd() { return new_passwd_; }
  inline std::string old_passwd() { return old_passwd_; }
  inline int64 change_type() { return change_type_; }
  inline int64 passwd_type() { return passwd_type_; }
 private:
  int64 uid_;
  std::string new_passwd_;
  std::string old_passwd_;
  int64 change_type_; // 0-初次设置密码 1-修改密码
  int64 passwd_type_; // 1-支付 2-登陆
};

class GuideOrderRecv : public PacketHead {
 public:
  GuideOrderRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 uid() { return uid_; }
  inline int64 last_id() { return last_id_; }
  inline int64 count() { return count_; }
 private:
  int64 uid_;
  int64 last_id_; //上一次最小订单号 第一次取0
  int64 count_; //请求条数
};

class GuideOrderDetailRecv : public PacketHead {
 public:
  GuideOrderDetailRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 order_id() { return order_id_; }
 private:
  int64 order_id_;
};

class DefineGuideSkillsRecv : public PacketHead {
 public:
  DefineGuideSkillsRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 uid() { return uid_; }
  inline int64 change_type() { return change_type_; }
  inline std::string skills() { return skills_; }
 private:
  int64 uid_;
  std::string skills_; //"1,2,3,4"
  int64 change_type_; // 0-拉取 1-修改
};

class VerifyVleaderHeadRecv : public PacketHead {
 public:
  VerifyVleaderHeadRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 uid() { return uid_; }
  inline std::string head_url() { return head_url_; }
  
 private:
  int64 uid_;
  std::string head_url_;
};

class ChangeUserInfoRecv : public PacketHead {
 public:
  ChangeUserInfoRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 uid() { return uid_; }
  inline std::string nickname() { return nickname_; }
  inline int64 gender() { return gender_; }
  inline std::string address() { return address_; }
  
 private:
  int64 uid_;
  std::string nickname_;
  int64 gender_;
  std::string address_;
};

#define CHANGE_BANK_CARD_ADD_UPDATE 0
#define CHNAGE_BANK_CARD_DELETE 1
class ChangeBankCardRecv : public PacketHead {
 public:
  ChangeBankCardRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 uid() { return uid_; }
  inline int64 type() { return type_; }
  inline std::string account() { return account_; }
  inline std::string bank_username() { return bank_username_; }
  inline int64 bank() { return bank_; }
  
 private:
  int64 uid_;
  int64 type_;
  std::string account_;
  std::string bank_username_;
  int64 bank_;
};

class BankCardInfoRecv : public PacketHead {
 public:
  BankCardInfoRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 uid() { return uid_; }
  
 private:
  int64 uid_;
};

class ChangeDefaultBankCardRecv : public PacketHead {
 public:
  ChangeDefaultBankCardRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 uid() { return uid_; }
  inline std::string account() { return account_; }
  
 private:
  int64 uid_;
  std::string account_;
};

class UserWithdrawRecv : public PacketHead {
 public:
  UserWithdrawRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 uid() { return uid_; }
  inline std::string account() { return account_; }
  inline int64 cash() { return cash_; }
  
 private:
  int64 uid_;
  std::string account_;
  int64 cash_;
};

class UserWithdrawRecordRecv : public PacketHead {
 public:
  UserWithdrawRecordRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 uid() { return uid_; }
  inline std::string account() { return account_; }
  inline int64 size() { return size_; }
  inline int64 num() { return num_; }
  
 private:
  int64 uid_;
  std::string account_;
  int64 size_;
  int64 num_;
};
 
struct PhotoUrl {
  std::string photo_url_;
  std::string thumbnail_url_;
};

class UserUploadPhotoRecv : public PacketHead {
 public:
  UserUploadPhotoRecv(PacketHead packet);
  int32 Deserialize();
  virtual ~UserUploadPhotoRecv();

  inline int64 uid() { return uid_; }
  inline std::list<PhotoUrl*> photo_list() { return photo_list_; }
 private:
  int64 uid_;
  std::list<PhotoUrl*> photo_list_;
};

class UserPhotoAlbumRecv : public PacketHead {
 public:
  UserPhotoAlbumRecv(PacketHead packet);
  int32 Deserialize();

  inline int64 uid() { return uid_; }
  inline int64 size() { return size_; }
  inline int64 num() { return num_; }
  
 private:
  int64 uid_;
  int64 size_;
  int64 num_;
};

class UploadContactsRecv : public PacketHead {
public:
	UploadContactsRecv(PacketHead packet);
	int32 Deserialize();

	inline int64 uid() { return uid_; }
	inline std::list<std::string> sql_list() { return sql_list_; }
private:
	int64 uid_;
	std::list<std::string> sql_list_;
};

class  UserRegInvitationCodeRecv :public PacketHead {
public:
	UserRegInvitationCodeRecv(PacketHead packet);
	int32 Deserialize();

	inline std::string phoneNum() { return phoneNum_; }
	inline std::string invitationCode() { return invitationCode_; }

private:
	std::string phoneNum_;
	std::string invitationCode_;
};
class UserAppVersionInfoeRecv :public PacketHead {
public:
	UserAppVersionInfoeRecv(PacketHead packet);
	int32 Deserialize();

	inline int64 AppType() { return app_type_; }

private:
	int64 app_type_;
};

class UserInsurancePriceRecv :public PacketHead {
public:
	UserInsurancePriceRecv(PacketHead packet);
	int32 Deserialize();

	inline int64 InsuranceType() { return insurance_type_; }
	inline int64 OrderPrice() { return order_price_; }

private:
	int64 insurance_type_;
	int64 order_price_;
};

class UserInsurancePayRecv :public PacketHead {
public:
	UserInsurancePayRecv(PacketHead packet);
	int32 Deserialize();

	inline int64 InsurancePrice() { return insurance_price_; }
	inline std::string InsuranceUsername() { return insurance_username_; }

private:
	int64 insurance_price_;
	std::string insurance_username_;
};

class UserIdCardInfoRecv :public PacketHead {
public:
	UserIdCardInfoRecv(PacketHead packet);
	int32 Deserialize();

	inline std::string IdCardNum() { return IdCardNum_; }
	inline std::string IdCardName() { return IdCardName_; }
	inline std::string IdCardUrlName() { return IdCardUrlName_; }
	inline int64 Uid() { return uid_; }

private:
	int64 uid_;
	std::string IdCardNum_;
	std::string IdCardName_;
	std::string IdCardUrlName_;
};

class FollowTypeRecv :public PacketHead {
public:
	FollowTypeRecv(PacketHead packet);
	int32 Deserialize();

	inline int64 FollowFrom() { return follow_from_; }
	inline int64 FollowTo() { return follow_to_; }
	inline int64 FollowType() { return follow_type_; }

private:
	int64 follow_from_;
	int64 follow_to_;
	int64 follow_type_;
};

class FollowListRecv :public PacketHead {
public:
	FollowListRecv(PacketHead packet);
	int32 Deserialize();

	inline int64 uid() { return uid_; }
	inline int64 followType() { return follow_type_; }
	inline int64 page() { return page_num_; }
	inline int64 page_count() { return page_count_; }

private:
	int64 uid_;
	int64 follow_type_;
	int64 page_num_;
	int64 page_count_;
}; 

class FollowNumberRecv :public PacketHead {
public:
	FollowNumberRecv(PacketHead packet);
	int32 Deserialize();

	inline int64 uid() { return uid_; }
	inline int64 type() { return type_; }

private:
	int64 uid_;
	int64 type_;
};
}  // namespace user

#endif  // PLUGINS_USER_USER_PROTO_H_
