// Copyright (c) 2016 The tourism Authors. All rights reserved.
// user_proto.cc
// Created on: 2016年8月12日.
// Author: Paco.

#include "user/user_proto.h"

#include <typeinfo>

#include "glog/logging.h"
#include "base/logic/base_values.h"
#include "public/basic/md5sum.h"

#include "pub/comm/comm_head.h"
#include "pub/util/util.h"

namespace user {

LoginRecv::LoginRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  user_type_ = -1;
}

int32 LoginRecv::Deserialize() {
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  int32 err = 0;
  Value* v = serializer->Deserialize(&err, &err_str);
  DicValue* dic;
  do {
    if (v != NULL) {
      if (typeid(*v) == typeid(ListValue)) {
        LOG(INFO)<< "listvalue:";
        ((ListValue*)v)->GetDictionary(0, &dic);
      } else if (typeid(*v) == typeid(DicValue)) {
        LOG(INFO) << "dicvalue:";
        dic = (DicValue*) v;
      } else {
        err = USER_LOGIN_JSON_ERR;
        break;
      }
      r = dic->GetString(L"passwd_", &passwd_);
      LOG_IF(ERROR, !r) << "LoginRecv::passwd parse error";
      if (r) {
        base::MD5Sum md5(passwd_);
        passwd_ = md5.GetHash();
      }
      r = dic->GetString(L"phone_num_", &phone_num_);
      LOG_IF(ERROR, !r) << "LoginRecv::phone_num_ parse error";
      if (phone_num_.length() < 11) {
        LOG(ERROR) << "phone_num_ is wrong";
        err = PHONE_NUM_ERR;
        break;
      }
      r = dic->GetBigInteger(L"user_type_", &user_type_);
      LOG_IF(ERROR, !r) << "LoginRecv::user_type_ parse error";
    } else {
      LOG(ERROR) << "LoginRecv Deserialize error";
      err = USER_LOGIN_JSON_ERR;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

SMSCodeLoginRecv::SMSCodeLoginRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  timestamp_ = 0;
  verify_code_ = 0;
  user_type_ = 0;
}

int32 SMSCodeLoginRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"timestamp_", &timestamp_);
      LOG_IF(ERROR, !r) << "SMSCodeLoginRecv::timestamp_ parse error";
      r = dic->GetBigInteger(L"verify_code_", &verify_code_);
      LOG_IF(ERROR, !r) << "SMSCodeLoginRecv::verify_code_ parse error";
      r = dic->GetBigInteger(L"user_type_", &user_type_);
      LOG_IF(ERROR, !r) << "SMSCodeLoginRecv::user_type_ parse error";
      r = dic->GetString(L"phone_num_", &phone_num_);
      LOG_IF(ERROR, !r) << "SMSCodeLoginRecv::phone_num_ parse error";
      r = dic->GetString(L"token_", &token_);
      LOG_IF(ERROR, !r) << "SMSCodeLoginRecv::token_ parse error";
    } else {
      LOG(ERROR)<< "SMSCodeLoginRecv Deserialize error";
      err = SMS_CODE_LOGIN_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

ObtainGuideRecv::ObtainGuideRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  longitude_ = 0.0;
  latitude_ = 0.0;
  distance_ = 20.0;
}

int32 ObtainGuideRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetReal(L"distance_", &distance_);
      LOG_IF(ERROR, !r) << "ObtainGuideRecv::distance_ parse error";
      r = dic->GetReal(L"longitude_", &longitude_);
      LOG_IF(ERROR, !r) << "ObtainGuideRecv::longitude_ parse error";
      r = dic->GetReal(L"latitude_", &latitude_);
      LOG_IF(ERROR, !r) << "ObtainGuideRecv::latitude_ parse error";
    } else {
      LOG(ERROR)<< "ObtainGuideRecv Deserialize error";
      err = NEARBY_GUIDE_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

Heartbeat::Heartbeat(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  uid_ = -1;
}

int32 Heartbeat::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "Heartbeat::uid_ parse error";
    } else {
      LOG(ERROR)<< "Heartbeat Deserialize error";
      err = HEART_PACKET_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

RecommendGuideRecv::RecommendGuideRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  city_code_ = 0;
  recommend_type_ = 0;
}

int32 RecommendGuideRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"city_code_", &city_code_);
      LOG_IF(ERROR, !r) << "RecommendGuideRecv::city_code_ parse error";
      r = dic->GetBigInteger(L"recommend_type_", &recommend_type_);
      LOG_IF(ERROR, !r) << "RecommendGuideRecv::recommend_type_ parse error";
    } else {
      LOG(ERROR)<< "RecommendGuideRecv Deserialize error";
      err = RECOMMEND_GUIDE_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

ChangePasswdRecv::ChangePasswdRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  uid_ = -1;
}

int32 ChangePasswdRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "ChangePasswdRecv::uid_ parse error";
      r = dic->GetString(L"old_passwd_", &old_passwd_);
      if (r) {
        base::MD5Sum md5(old_passwd_);
        old_passwd_ = md5.GetHash();
      }
      LOG_IF(ERROR, !r) << "ChangePasswdRecv::old_passwd_ parse error";
      r = dic->GetString(L"new_passwd_", &new_passwd_);
      if (r) {
        base::MD5Sum md5(new_passwd_);
        new_passwd_ = md5.GetHash();
      }
      LOG_IF(ERROR, !r) << "ChangePasswdRecv::new_passwd_ parse error";
    } else {
      LOG(ERROR)<< "ChangePasswdRecv Deserialize error";
      err = CHANGE_PASSWD_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

ObtainVerifyCodeRecv::ObtainVerifyCodeRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  verify_type_ = 0;
}

int32 ObtainVerifyCodeRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"verify_type_", &verify_type_);
      LOG_IF(ERROR, !r) << "ObtainVerifyCodeRecv::verify_type_ parse error";
      r = dic->GetString(L"phone_num_", &phone_num_);
      LOG_IF(ERROR, !r) << "ObtainVerifyCodeRecv::phone_num_ parse error";
    } else {
      LOG(ERROR)<< "ObtainVerifyCodeRecv Deserialize error";
      err = OBTAIN_VERIFYCODE_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

RegisterAccountRecv::RegisterAccountRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  timestamp_ = 0;
  verify_code_ = 0;
  user_type_ = 0;
}

int32 RegisterAccountRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"timestamp_", &timestamp_);
      LOG_IF(ERROR, !r) << "RegisterAccountRecv::timestamp_ parse error";
      r = dic->GetBigInteger(L"verify_code_", &verify_code_);
      LOG_IF(ERROR, !r) << "RegisterAccountRecv::verify_code_ parse error";
      r = dic->GetBigInteger(L"user_type_", &user_type_);
      LOG_IF(ERROR, !r) << "RegisterAccountRecv::user_type_ parse error";
      r = dic->GetString(L"phone_num_", &phone_num_);
      LOG_IF(ERROR, !r) << "RegisterAccountRecv::phone_num_ parse error";
      r = dic->GetString(L"passwd_", &passwd_);
      if (r) {
        base::MD5Sum md5(passwd_);
        passwd_ = md5.GetHash();
      }
      LOG_IF(ERROR, !r) << "RegisterAccountRecv::passwd_ parse error";
      r = dic->GetString(L"token_", &token_);
      LOG_IF(ERROR, !r) << "RegisterAccount::token_ parse error";

	  r = dic->GetString(L"invitation_phone_num_", &invitation_phone_num_);
	  LOG(INFO) << invitation_phone_num_ << '\n';
	  LOG_IF(ERROR, !r) << "LoginRecv::invitation_phone_num_ parse error";
	  if (phone_num_.length() < 11) {
		  LOG(ERROR) << "invitation_phone_num_ is wrong";
		  err = INVITATION_PHONE_NUM_ERR;
		  break;
	  }
    } else {
      LOG(ERROR)<< "RegisterAccountRecv Deserialize error";
      err = REGISTER_ACCOUNT_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

ImproveDataRecv::ImproveDataRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  uid_ = 0;
  longitude_ = 0;
  latitude_ = 0;
  gender_ = 0;
}

int32 ImproveDataRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "ImproveDataRecv::uid_ parse error";
      r = dic->GetBigInteger(L"gender_", &gender_);
      LOG_IF(ERROR, !r) << "ImproveDataRecv::gender_ parse error";
      r = dic->GetString(L"nickname_", &nickname_);
      LOG_IF(ERROR, !r) << "ImproveDataRecv::nickname_ parse error";
      r = dic->GetString(L"head_url_", &head_url_);
      LOG_IF(ERROR, !r) << "ImproveDataRecv::head_url_ parse error";
      r = dic->GetString(L"address_", &address_);
      LOG_IF(ERROR, !r) << "ImproveDataRecv::address_ parse error";
      r = dic->GetReal(L"longitude_", &longitude_);
      LOG_IF(ERROR, !r) << "ImproveDataRecv::longitude_ parse error";
      r = dic->GetReal(L"latitude_", &latitude_);
      LOG_IF(ERROR, !r) << "ImproveDataRecv::latitude_ parse error";
    } else {
      LOG(ERROR)<< "ImproveDataRecv Deserialize error";
      err = IMPROVE_DATA_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

UserDetailRecv::UserDetailRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
}

int32 UserDetailRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetString(L"uid_str_", &uid_str_);
      LOG_IF(ERROR, !r) << "UserDetailRecv::uid_str_ parse error";

    } else {
      LOG(ERROR)<< "UserDetailRecv Deserialize error";
      err = REQUEST_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

ObtainTripRecv::ObtainTripRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  count_ = 10;
  uid_ = 0;
  order_id_ = 0;
}

int32 ObtainTripRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "ObtainTripRecv::uid_ parse error";
      r = dic->GetBigInteger(L"order_id_", &order_id_);
      LOG_IF(ERROR, !r) << "ObtainTripRecv::order_id_ parse error";
      r = dic->GetBigInteger(L"count_", &count_);
      LOG_IF(ERROR, !r) << "ObtainTripRecv::count_ parse error";
    } else {
      LOG(ERROR)<< "ObtainTripRecv Deserialize error";
      err = TRIP_RECORD_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

ServiceDetailsRecv::ServiceDetailsRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
}

int32 ServiceDetailsRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetString(L"oid_str_", &oid_str_);
      LOG_IF(ERROR, !r) << "ServiceDetailsRecv::oid_str_ parse error";

    } else {
      LOG(ERROR)<< "ServiceDetailsRecv Deserialize error";
      err = SERVICE_DETAILS_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

DrawBillRecv::DrawBillRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  invoice_type_ = 0;
  uid_ = 0;
}

int32 DrawBillRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "DrawBillRecv::uid_ parse error";
      r = dic->GetString(L"oid_str_", &oid_str_);
      LOG_IF(ERROR, !r) << "DrawBillRecv::oid_str_ parse error";
      r = dic->GetString(L"title_", &title_);
      LOG_IF(ERROR, !r) << "DrawBillRecv::title_ parse error";
      r = dic->GetString(L"taxpayer_num_", &taxpayer_num_);
      LOG_IF(ERROR, !r) << "DrawBillRecv::taxpayer_num_ parse error";
      r = dic->GetString(L"company_addr_", &company_addr_);
      LOG_IF(ERROR, !r) << "DrawBillRecv::company_addr_ parse error";
      r = dic->GetBigInteger(L"invoice_type_", &invoice_type_);
      LOG_IF(ERROR, !r) << "DrawBillRecv::invoice_type_ parse error";
      r = dic->GetString(L"user_name_", &user_name_);
      LOG_IF(ERROR, !r) << "DrawBillRecv::user_name_ parse error";
      r = dic->GetString(L"user_mobile_", &user_mobile_);
      LOG_IF(ERROR, !r) << "DrawBillRecv::user_mobile_ parse error";
      r = dic->GetString(L"area_", &area_);
      LOG_IF(ERROR, !r) << "DrawBillRecv::area_ parse error";
      r = dic->GetString(L"addr_detail_", &addr_detail_);
      LOG_IF(ERROR, !r) << "DrawBillRecv::addr_detail_ parse error";
      r = dic->GetString(L"remarks_", &remarks_);
      LOG_IF(ERROR, !r) << "DrawBillRecv::remarks_ parse error";
    } else {
      LOG(ERROR)<< "ServiceDetailsRecv Deserialize error";
      err = DRAW_BILL_JSON_ERR;
      break;
    }
  }while (0);
  return err;
}

BillRecordRecv::BillRecordRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  uid_ = -1;
  count_ = 0;
  last_invoice_id_ = 0;
}

int32 BillRecordRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "BillRecordRecv::uid_ parse error";
      r = dic->GetBigInteger(L"count_", &count_);
      LOG_IF(ERROR, !r) << "BillRecordRecv::count_ parse error";
      if (count_ == 0 || count_ < 0 || count_ > 20)
        count_ = 20;
      r = dic->GetBigInteger(L"last_invoice_id_", &last_invoice_id_);
      LOG_IF(ERROR, !r) << "BillRecordRecv::last_invoice_id_ parse error";
    } else {
      LOG(ERROR)<< "BillRecordRecv Deserialize error";
      err = BILL_RECORD_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

InvoiceDetailRecv::InvoiceDetailRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  invoice_id_ = 0;
}

int32 InvoiceDetailRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"invoice_id_", &invoice_id_);
      LOG_IF(ERROR, !r) << "InvoiceDetailRecv::invoice_id_ parse error";
    } else {
      LOG(ERROR)<< "InvoiceDetailRecv Deserialize error";
      err = INVOICE_DETAIL_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

DeviceTokenRecv::DeviceTokenRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  uid_ = 0;
}

int32 DeviceTokenRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "DeviceTokenRecv::uid_ parse error";
      r = dic->GetString(L"device_token_", &device_token_);
      LOG_IF(ERROR, !r) << "DeviceTokenRecv::device_token_ parse error";
    } else {
      LOG(ERROR)<< "DeviceTokenRecv Deserialize error";
      err = SERVICE_DETAILS_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

BlackcardInfoRecv::BlackcardInfoRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  uid_ = 0;
}

int32 BlackcardInfoRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "BlackcardInfoRecv::uid_ parse error";
    } else {
      LOG(ERROR)<< "BlackcardInfoRecv Deserialize error";
      err = BLACKCARD_INFO_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

BlackcardConsumRecordRecv::BlackcardConsumRecordRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  uid_ = 0;
}

int32 BlackcardConsumRecordRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "BlackcardConsumRecordRecv::uid_ parse error";
    } else {
      LOG(ERROR)<< "BlackcardConsumRecordRecv Deserialize error";
      err = BLACKCARD_CONSUME_RECORD_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

BlackcardPlaceOrderRecv::BlackcardPlaceOrderRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  uid_ = 0;
  wanted_lv_ = 0;
}

int32 BlackcardPlaceOrderRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "BlackcardPlaceOrderRecv::uid_ parse error";
      r = dic->GetBigInteger(L"wanted_lv_", &wanted_lv_);
      LOG_IF(ERROR, !r) << "BlackcardPlaceOrderRecv::wanted_lv_ parse error";
    } else {
      LOG(ERROR)<< "BlackcardConsumRecordRecv Deserialize error";
      err = REQUEST_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

NewAppointmentRecv::NewAppointmentRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  uid_ = 0;
  city_code_ = 0;
  start_time_ = 0;
  other_gender_ = 1;
  end_time_ = 0;
  is_other_ = 0;
  ;
}

int32 NewAppointmentRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "NewAppointmentRecv::uid_ parse error";
      r = dic->GetBigInteger(L"city_code_", &city_code_);
      LOG_IF(ERROR, !r) << "NewAppointmentRecv::city_code_ parse error";
      r = dic->GetBigInteger(L"start_time_", &start_time_);
      LOG_IF(ERROR, !r) << "NewAppointmentRecv::start_time_ parse error";
      r = dic->GetBigInteger(L"end_time_", &end_time_);
      LOG_IF(ERROR, !r) << "NewAppointmentRecv::end_time_ parse error";
      r = dic->GetString(L"skills_", &skills_);
      LOG_IF(ERROR, !r) << "NewAppointmentRecv::skills_ parse error";
      r = dic->GetString(L"remarks_", &remarks_);
      LOG_IF(ERROR, !r) << "NewAppointmentRecv::remarks_ parse error";
      r = dic->GetBigInteger(L"is_other_", &is_other_);
      LOG_IF(ERROR, !r) << "NewAppointmentRecv::is_other_ parse error";
      if (is_other_ == 1) {
        r = dic->GetString(L"other_name_", &other_name_);
        LOG_IF(ERROR, !r) << "NewAppointmentRecv::other_name_ parse error";
        r = dic->GetBigInteger(L"other_gender_", &other_gender_);
        LOG_IF(ERROR, !r) << "NewAppointmentRecv::other_gender_ parse error";
        r = dic->GetString(L"other_phone_", &other_phone_);
        LOG_IF(ERROR, !r) << "NewAppointmentRecv::other_phone_ parse error";
      } else {
        other_name_ = "";
        other_gender_ = 0;
        other_phone_ = "";
      }
    } else {
      LOG(ERROR)<< "NewAppointmentRecv Deserialize error";
      err = NEW_APPOINTMENT_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

WxPlaceOrderRecv::WxPlaceOrderRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  uid_ = 0;
  price_ = 0;  // 订单总价  单位 分
}

int32 WxPlaceOrderRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "WxPlaceOrderRecv::uid_ parse error";
      r = dic->GetString(L"title_", &title_);
      LOG_IF(ERROR, !r) << "WxPlaceOrderRecv::title_ parse error";
      r = dic->GetBigInteger(L"price_", &price_);
      LOG_IF(ERROR, !r) << "WxPlaceOrderRecv::price_ parse error";
    } else {
      LOG(ERROR)<< "WxPlaceOrderRecv Deserialize error";
      err = WX_PLACE_ORDER_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

WXPayClientRecv::WXPayClientRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  uid_ = 0;
  recharge_id_ = 0;
  pay_result_ = 0;
}

int32 WXPayClientRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "WXPayClientRecv::uid_ parse error";
      r = dic->GetBigInteger(L"recharge_id_", &recharge_id_);
      LOG_IF(ERROR, !r) << "WXPayClientRecv::recharge_id_ parse error";
      r = dic->GetBigInteger(L"pay_result_", &pay_result_);
      LOG_IF(ERROR, !r) << "WXPayClientRecv::pay_result_ parse error";
    } else {
      LOG(ERROR)<< "WxPlaceOrderRecv Deserialize error";
      err = WXPAY_CLIENT_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

WXPayServerRecv::WXPayServerRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  recharge_id_ = 0;
  pay_result_ = 0;
  total_fee_ = 0;
}

int32 WXPayServerRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetString(L"wxpay_result_", &xml_str_);
      LOG_IF(ERROR, !r) << "WXPayServerRecv::xml_str_ parse error";
      LOG(INFO)<< "WXPAY SERVER RESULT***" << xml_str_ << "***";
      if (r && xml_str_.length() > 5) {
        base_logic::ValueSerializer* deserializer =
            base_logic::ValueSerializer::Create(base_logic::IMPL_XML,
                                                &xml_str_);
        std::string err_str;
        int32 err = 0;
        DicValue* dic = (DicValue*) deserializer->Deserialize(&err, &err_str);
        if (dic != NULL) {
          dic->GetString(L"appid", &appid_);
          dic->GetString(L"mch_id", &mch_id_);
          appid_ = util::GetWxpayXmlValue(appid_);
          mch_id_ = util::GetWxpayXmlValue(mch_id_);
          std::string return_code;
          dic->GetString(L"return_code", &return_code);
          //通信成功标识
          if (return_code.find("SUCCESS") != std::string::npos) {
            std::string result_code;
            dic->GetString(L"result_code", &result_code);

            //订单总金额
            dic->GetBigInteger(L"total_fee", &total_fee_);
            //支付总金额
            dic->GetString(L"transaction_id", &transaction_id_);
            transaction_id_ = util::GetWxpayXmlValue(transaction_id_);
            //本平台订单号
            std::string out_trade_no;
            dic->GetString(L"out_trade_no", &out_trade_no);
            int npos1 = out_trade_no.find("<![CDATA[");
            int npos2 = out_trade_no.find("]]>");
            out_trade_no = out_trade_no.substr(npos1 + 9,
                                               npos2 - npos1 - 9 - 6);
            recharge_id_ = atol(out_trade_no.c_str());
            //支付成功标识
            if (result_code.find("SUCCESS") != std::string::npos) {
              pay_result_ = 1;
            } else {
              pay_result_ = -1;
            }
          }
        } else {
          LOG(ERROR)<< "WxPlaceOrderRecv xml  Deserialize error";
        }
        base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_XML,
                                                      deserializer);
      }

    } else {
      LOG(ERROR)<< "WxPlaceOrderRecv json Deserialize error";
      err = WXPAY_SERVER_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

IdentityPicRecv::IdentityPicRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  uid_ = 0;
}

int32 IdentityPicRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "IdentityPicRecv::uid_ parse error";
      r = dic->GetString(L"front_pic_", &front_pic_);
      LOG_IF(ERROR, !r) << "IdentityPicRecv::front_pic_ parse error";
      r = dic->GetString(L"back_pic_", &back_pic_);
      LOG_IF(ERROR, !r) << "IdentityPicRecv::back_pic_ parse error";
    } else {
      LOG(ERROR)<< "IdentityPicRecv Deserialize error";
      err = IDENTITY_PIC_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

GuideDetailRecv::GuideDetailRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  uid_ = 0;
}

int32 GuideDetailRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "GuideDetailRecv::uid_ parse error";
    } else {
      LOG(ERROR)<< "GuideDetailRecv Deserialize error";
      err = GUIDE_DETAIL_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
  return err;
}

IdentityStatusRecv::IdentityStatusRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  uid_ = 0;
}

int32 IdentityStatusRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "IdentityStatusRecv::uid_ parse error";
    } else {
      LOG(ERROR)<< "IdentityStatusRecv Deserialize error";
      err = IDENTITY_STATUS_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

PaggingRecv::PaggingRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  last_id_ = 0;
  count_ = 10;
  page_type_ = 0;
}

int32 PaggingRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"last_id_", &last_id_);
      LOG_IF(ERROR, !r) << "PaggingRecv::last_id_ parse error";
      r = dic->GetBigInteger(L"count_", &count_);
      LOG_IF(ERROR, !r) << "PaggingRecv::count_ parse error";
      r = dic->GetBigInteger(L"page_type_", &page_type_);
      LOG_IF(ERROR, !r) << "PaggingRecv::page_type_ parse error";
      if (count_ <= 0 || count_ > 10)
        count_ = 10;
    } else {
      LOG(ERROR)<< "PaggingRecv Deserialize error";
      err = SHARE_PAGING_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

ShareTourismDetailRecv::ShareTourismDetailRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  share_id_ = 0;
}

int32 ShareTourismDetailRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"share_id_", &share_id_);
      LOG_IF(ERROR, !r) << "ShareTourismDetailRecv::share_id_ parse error";
    } else {
      LOG(ERROR)<< "ShareTourismDetailRecv Deserialize error";
      err = SHARE_TOURISM_DETAILS_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

ShareSkillDetailRecv::ShareSkillDetailRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  share_id_ = 0;
}

int32 ShareSkillDetailRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"share_id_", &share_id_);
      LOG_IF(ERROR, !r) << "ShareSkillDetailRecv::share_id_ parse error";
    } else {
      LOG(ERROR)<< "ShareSkillDetailRecv Deserialize error";
      err = SHARE_SKILL_DETAILS_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

ShareSkillDiscussRecv::ShareSkillDiscussRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  share_id_ = 0;
  last_id_ = 0;
  count_ = 0;
}

int32 ShareSkillDiscussRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"share_id_", &share_id_);
      LOG_IF(ERROR, !r) << "ShareSkillDiscussRecv::share_id_ parse error";
      r = dic->GetBigInteger(L"last_id_", &last_id_);
      LOG_IF(ERROR, !r) << "ShareSkillDiscussRecv::last_id_ parse error";
      r = dic->GetBigInteger(L"count_", &count_);
      LOG_IF(ERROR, !r) << "ShareSkillDiscussRecv::share_id_ parse error";
      if (count_ <= 0 || count_ > 50)
        count_ = 50;
    } else {
      LOG(ERROR)<< "ShareSkillDiscussRecv Deserialize error";
      err = SHARE_SKILL_DISCUSS_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

EntryShareSkillRecv::EntryShareSkillRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  share_id_ = 0;
  uid_ = 0;
}

int32 EntryShareSkillRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"share_id_", &share_id_);
      LOG_IF(ERROR, !r) << "EntryShareSkillRecv::share_id_ parse error";
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "EntryShareSkillRecv::uid_ parse error";
    } else {
      LOG(ERROR)<< "EntryShareSkillRecv Deserialize error";
      err = ENTRY_SHARE_SKILL_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

UserCashRecv::UserCashRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  uid_ = 0;
}

int32 UserCashRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "UserCashRecv::uid_ parse error";
    } else {
      LOG(ERROR)<< "UserCashRecv Deserialize error";
      err = USER_CASH_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

AppointmentRecordRecv::AppointmentRecordRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  uid_ = 0;
  last_id_ = 0;
  count_ = 10;
}

int32 AppointmentRecordRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "AppointmentRecordRecv::uid_ parse error";
      r = dic->GetBigInteger(L"last_id_", &last_id_);
      LOG_IF(ERROR, !r) << "AppointmentRecordRecv::last_id_ parse error";
      r = dic->GetBigInteger(L"count_", &count_);
      LOG_IF(ERROR, !r) << "AppointmentRecordRecv::count_ parse error";
      if (count_ > 20 || count_ < 0)
        count_ = 20;
    } else {
      LOG(ERROR)<< "AppointmentRecordRecv Deserialize error";
      err = APPOINTMENT_RECORD_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

ChangeGuideServiceRecv::ChangeGuideServiceRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  uid_ = 0;
  verify_code_ = 0;
  timestamp_ = 0;
}

ChangeGuideServiceRecv::~ChangeGuideServiceRecv() {
  std::list<ServiceData*>::iterator it = service_list_.begin();
  for (; it != service_list_.end(); it++) {
    ServiceData* data = *it;
    if (data != NULL) {
      delete data;
      data = NULL;
    }
  }
  service_list_.clear();
}

int32 ChangeGuideServiceRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "ChangeGuideServiceRecv::uid_ parse error";
//      r = dic->GetBigInteger(L"verify_code_", &verify_code_);
//      LOG_IF(ERROR, !r) << "ChangeGuideServiceRecv::verify_code_ parse error";
//      r = dic->GetBigInteger(L"timestamp_", &timestamp_);
//      LOG_IF(ERROR, !r) << "ChangeGuideServiceRecv::phone_num_ parse error";
//      r = dic->GetString(L"token_", &token_);
//      LOG_IF(ERROR, !r) << "ChangeGuideServiceRecv::token_ parse error";
//      r = dic->GetString(L"phone_num_", &phone_num_);
//      LOG_IF(ERROR, !r) << "ChangeGuideServiceRecv::phone_num_ parse error";
      ListValue* list;
      r = dic->GetList(L"service_list_", &list);
      if (!r) {
        err = REQUEST_JSON_ERR;
        break;
      }
      int count = list->GetSize();
      for (int i = 0; i < count; i++) {
        DicValue* info;
        r = list->GetDictionary(i, &info);
        if (r) {
          ServiceData* data = new ServiceData;
          info->GetBigInteger(L"service_id_", &data->service_id_);
          info->GetBigInteger(L"service_type_", &data->service_type_);
          info->GetBigInteger(L"change_type_", &data->change_type_);
          info->GetString(L"service_name_", &data->service_name_);
          info->GetBigInteger(L"service_price_", &data->service_price_);
          info->GetBigInteger(L"service_start_", &data->service_start_);
          info->GetBigInteger(L"service_end_", &data->service_end_);
          service_list_.push_back(data);
        }
      }
    } else {
      LOG(ERROR)<< "AppointmentRecordRecv Deserialize error";
      err = REQUEST_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

OrderDetailRecv::OrderDetailRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  order_id_ = 0;
  order_type_ = 0;
}

int32 OrderDetailRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"order_id_", &order_id_);
      LOG_IF(ERROR, !r) << "OrderDetailRecv::order_id_ parse error";
      r = dic->GetBigInteger(L"order_type_", &order_type_);
      LOG_IF(ERROR, !r) << "OrderDetailRecv::order_type_ parse error";
    } else {
      LOG(ERROR)<< "OrderDetailRecv Deserialize error";
      err = REQUEST_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

VerifyPasswdRecv::VerifyPasswdRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  uid_ = 0;
  passwd_type_ = 0;
}

int32 VerifyPasswdRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "VerifyPasswdRecv::uid_ parse error";
      r = dic->GetBigInteger(L"passwd_type_", &passwd_type_);
      LOG_IF(ERROR, !r) << "VerifyPasswdRecv::passwd_type_ parse error";
      r = dic->GetString(L"passwd_", &passwd_);
      if (r) {
        base::MD5Sum md5(passwd_);
        passwd_ = md5.GetHash();
      }
      LOG_IF(ERROR, !r) << "VerifyPasswdRecv::passwd_ parse error";
    } else {
      LOG(ERROR)<< "VerifyPasswdRecv Deserialize error";
      err = REQUEST_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

ChangePayPasswdRecv::ChangePayPasswdRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  uid_ = 0;
  change_type_ = 0;
  passwd_type_ = 0;
}

int32 ChangePayPasswdRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "ChangePayPasswdRecv::uid_ parse error";
      r = dic->GetBigInteger(L"change_type_", &change_type_);
      LOG_IF(ERROR, !r) << "ChangePayPasswdRecv::change_type_ parse error";
      r = dic->GetBigInteger(L"passwd_type_", &passwd_type_);
      LOG_IF(ERROR, !r) << "ChangePayPasswdRecv::passwd_type_ parse error";
      r = dic->GetString(L"new_passwd_", &new_passwd_);
      if (r) {
        base::MD5Sum md5(new_passwd_);
        new_passwd_ = md5.GetHash();
      }
      LOG_IF(ERROR, !r) << "ChangePayPasswdRecv::new_passwd_ parse error";
      r = dic->GetString(L"old_passwd_", &old_passwd_);
      if (r) {
        base::MD5Sum md5(old_passwd_);
        old_passwd_ = md5.GetHash();
      }
      LOG_IF(ERROR, !r) << "ChangePayPasswdRecv::old_passwd_ parse error";
    } else {
      LOG(ERROR)<< "ChangePayPasswdRecv Deserialize error";
      err = REQUEST_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

GuideOrderRecv::GuideOrderRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  uid_ = 0;
  last_id_ = 0;
  count_ = 0;
}

int32 GuideOrderRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "GuideOrderRecv::uid_ parse error";
      r = dic->GetBigInteger(L"last_id_", &last_id_);
      LOG_IF(ERROR, !r) << "GuideOrderRecv::last_id_ parse error";
      r = dic->GetBigInteger(L"count_", &count_);
      LOG_IF(ERROR, !r) << "GuideOrderRecv::count_ parse error";
    } else {
      LOG(ERROR)<< "GuideOrderRecv Deserialize error";
      err = REQUEST_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

VerifyVleaderHeadRecv::VerifyVleaderHeadRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  uid_ = 0;
}  
  
int32 VerifyVleaderHeadRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "VerifyVleaderHeadRecv::uid_ parse error";
      r = dic->GetString(L"head_url_", &head_url_);
      LOG_IF(ERROR, !r) << "VerifyVleaderHeadRecv::head_ parse error";
    } else {
      LOG(ERROR)<< "VerifyVleaderHeadRecv Deserialize error";
      err = REQUEST_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

ChangeUserInfoRecv::ChangeUserInfoRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  uid_ = 0;
  gender_ = 0;
}  

int32 ChangeUserInfoRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "ChangUserInfoRecv::uid_ parse error";
      r = dic->GetString(L"nickname_", &nickname_);
      LOG_IF(ERROR, !r) << "ChangUserInfoRecv::nickname_ parse error";
      r = dic->GetBigInteger(L"gender_", &gender_);
      LOG_IF(ERROR, !r) << "ChangUserInfoRecv::gender_ parse error";
      r = dic->GetString(L"address_", &address_);
      LOG_IF(ERROR, !r) << "ChangUserInfoRecv::address_ parse error";
    } else {
      LOG(ERROR)<< "ChangUserInfoRecv Deserialize error";
      err = REQUEST_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

ChangeBankCardRecv::ChangeBankCardRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  uid_ = 0;
  type_ = 0;
  bank_ = 0;
}  

int32 ChangeBankCardRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "ChangeBankCardRecv::uid_ parse error";
      r = dic->GetBigInteger(L"type_", &type_);
      r = dic->GetString(L"account_", &account_);
      LOG_IF(ERROR, !r) << "ChangeBankCardRecv::account_ parse error";
      r = dic->GetString(L"bank_username_", &bank_username_);
      LOG_IF(ERROR, !r && type_ == CHANGE_BANK_CARD_ADD_UPDATE)
				<< "ChangeBankCardRecv::bank_username_ parse error";
      r = dic->GetBigInteger(L"bank_", &bank_);
      LOG_IF(ERROR, !r && type_ == CHANGE_BANK_CARD_ADD_UPDATE)
				<< "ChangeBankCardRecv::bank_ parse error";
    } else {
      LOG(ERROR)<< "ChangeBankCardRecv Deserialize error";
      err = REQUEST_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

BankCardInfoRecv::BankCardInfoRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  uid_ = 0;
}  

int32 BankCardInfoRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "BankCardInfoRecv::uid_ parse error";
    } else {
      LOG(ERROR)<< "BankCardInfoRecv Deserialize error";
      err = REQUEST_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}


ChangeDefaultBankCardRecv::ChangeDefaultBankCardRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  uid_ = 0;
}  

int32 ChangeDefaultBankCardRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "ChangeDefaultBankCardRecv::uid_ parse error";
      r = dic->GetString(L"account_", &account_);
      LOG_IF(ERROR, !r) << "ChangeDefaultBankCardRecv::account_ parse error";
    } else {
      LOG(ERROR)<< "ChangeDefaultBankCardRecv Deserialize error";
      err = REQUEST_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

GuideOrderDetailRecv::GuideOrderDetailRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  order_id_ = 0;
}

int32 GuideOrderDetailRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"order_id_", &order_id_);
      LOG_IF(ERROR, !r) << "GuideOrderRecv::order_id_ parse error";
    } else {
      LOG(ERROR)<< "GuideOrderDetailRecv Deserialize error";
      err = REQUEST_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

UserWithdrawRecv::UserWithdrawRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  uid_ = 0;
}  

int32 UserWithdrawRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "UserWithdrawRecv::uid_ parse error";
      r = dic->GetString(L"account_", &account_);
      LOG_IF(ERROR, !r) << "UserWithdrawRecv::account_ parse error";
      r = dic->GetBigInteger(L"cash_", &cash_);
      LOG_IF(ERROR, !r) << "UserWithdrawRecv::cash_ parse error";
    } else {
      LOG(ERROR)<< "UserWithdrawRecv Deserialize error";
      err = REQUEST_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

UserWithdrawRecordRecv::UserWithdrawRecordRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  uid_ = 0;
  size_ = 0;
  num_ = 0;
}  

int32 UserWithdrawRecordRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "UserWithdrawRecordRecv::uid_ parse error";
      r = dic->GetString(L"account_", &account_);
	  if (!r)
		  account_ = ""; 
      //LOG_IF(ERROR, !r) << "UserWithdrawRecordRecv::account_ parse error";
      r = dic->GetBigInteger(L"size_", &size_);
      LOG_IF(ERROR, !r) << "UserWithdrawRecordRecv::size_ parse error";
      r = dic->GetBigInteger(L"num_", &num_);
      LOG_IF(ERROR, !r) << "UserWithdrawRecordRecv::num_ parse error";
    } else {
      LOG(ERROR)<< "UserWithdrawRecordRecv Deserialize error";
      err = REQUEST_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

CheckSMSCodeRecv::CheckSMSCodeRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  verify_code_ = 0;
  verify_type_ = 0;  //0-注册 1-登录 2-更新服务
  timestamp_ = 0;  //时间戳
}

int32 CheckSMSCodeRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"timestamp_", &timestamp_);
      LOG_IF(ERROR, !r) << "CheckSMSCodeRecv::timestamp_ parse error";
      r = dic->GetBigInteger(L"verify_code_", &verify_code_);
      LOG_IF(ERROR, !r) << "CheckSMSCodeRecv::verify_code_ parse error";
      r = dic->GetBigInteger(L"verify_type_", &verify_type_);
      LOG_IF(ERROR, !r) << "CheckSMSCodeRecv::verify_type_ parse error";
      r = dic->GetString(L"phone_num_", &phone_num_);
      LOG_IF(ERROR, !r) << "CheckSMSCodeRecv::phone_num_ parse error";
      r = dic->GetString(L"token_", &token_);
      LOG_IF(ERROR, !r) << "CheckSMSCodeRecv::token_ parse error";
    } else {
      LOG(ERROR)<< "CheckSMSCodeRecv Deserialize error";
      err = REQUEST_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

UserUploadPhotoRecv::UserUploadPhotoRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  uid_ = 0;
}

UserUploadPhotoRecv::~UserUploadPhotoRecv() {
  std::list<PhotoUrl*>::iterator it = photo_list_.begin();
  for (; it != photo_list_.end(); it++) {
    PhotoUrl* data = *it;
    if (data != NULL) {
      delete data;
      data = NULL;
    }
  }
  photo_list_.clear();
}

int32 UserUploadPhotoRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "UserUploadPhotoRecv::uid_ parse error";
      ListValue* list;
      r = dic->GetList(L"photo_list_", &list);
      LOG_IF(ERROR, !r) << "UserUploadPhotoRecv::photo_list_ parse error:";
	  if (r && typeid(*list) == typeid(ListValue)) {
        int count = list->GetSize();
        for (int i = 0; i < count; i++) {
          DicValue* info;
          r = list->GetDictionary(i, &info);
          if (r) {
            PhotoUrl* data = new PhotoUrl;
            info->GetString(L"photo_url_", &data->photo_url_);
            info->GetString(L"thumbnail_url_", &data->thumbnail_url_);
            photo_list_.push_back(data);
          }
        }
	  } else {
        if (!r) {
          err = REQUEST_JSON_ERR;
          break;
        }
      }
    } else {
      LOG(ERROR)<< "UserUploadPhotoRecv Deserialize error";
      if (!r) {
        err = REQUEST_JSON_ERR;
        break;
      }
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

DefineGuideSkillsRecv::DefineGuideSkillsRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  uid_ = 0;
  change_type_ = 0;
}

int32 DefineGuideSkillsRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "DefineGuideSkillsRecv::uid_ parse error";
      r = dic->GetBigInteger(L"change_type_", &change_type_);
      LOG_IF(ERROR, !r) << "DefineGuideSkillsRecv::change_type_ parse error";
      r = dic->GetString(L"skills_", &skills_);
      LOG_IF(ERROR, !r) << "DefineGuideSkillsRecv::skills_ parse error";
    } else {
      LOG(ERROR)<< "DefineGuideSkillsRecv Deserialize error";
      err = REQUEST_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

UserPhotoAlbumRecv::UserPhotoAlbumRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  uid_ = 0;
  size_ = 0;
  num_ = 0;
}  

int32 UserPhotoAlbumRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON, &body_str_, false);
  std::string err_str;
  DicValue* dic = (DicValue*) serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "UserPhotoAlbumRecv::uid_ parse error";
      r = dic->GetBigInteger(L"size_", &size_);
      LOG_IF(ERROR, !r) << "UserPhotoAlbumRecv::size_ parse error";
      r = dic->GetBigInteger(L"num_", &num_);
      LOG_IF(ERROR, !r) << "UserPhotoAlbumRecv::num_ parse error";
    } else {
      LOG(ERROR)<< "UserPhotoAlbumRecv Deserialize error";
      err = REQUEST_JSON_ERR;
      break;
    }
  }while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                serializer);
  return err;
}

UserRegInvitationCodeRecv::UserRegInvitationCodeRecv(PacketHead packet) {
	head_ = packet.head();
	body_str_ = packet.body_str();
}

int32 UserRegInvitationCodeRecv::Deserialize()
{
	int32 err = 0;
	bool r = false;
	base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
		base_logic::IMPL_JSON, &body_str_, false);
	std::string err_str;
	DicValue* dic = (DicValue*)serializer->Deserialize(&err, &err_str);
	do {
		if (dic != NULL) {
			r = dic->GetString(L"phoneNum_", &phoneNum_);
			LOG_IF(ERROR, !r) << "UserRegInvitationCodeRecv::uid_ parse error";
			r = dic->GetString(L"invitationCode_", &invitationCode_);
			LOG_IF(ERROR, !r) << "UserRegInvitationCodeRecv::invitationCode_ parse error";
		}
		else {
			LOG(ERROR) << "UserRegInvitationCodeRecv Deserialize error";
			err = REQUEST_JSON_ERR;
			break;
		}
	} while (0);
	base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
		serializer);
	return err;
}

}  // namespace user

