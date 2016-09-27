// Copyright (c) 2016 The tourism Authors. All rights reserved.
// user_proto.cc
// Created on: 2016年8月12日.
// Author: Paco.

#include "user/user_proto.h"

#include <typeinfo>

#include "glog/logging.h"
#include "base/logic/base_values.h"

#include "pub/comm/comm_head.h"

namespace user {

LoginRecv::LoginRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  user_type_ = -1;
}

int32 LoginRecv::Deserialize() {
  bool r = false;
  base_logic::ValueSerializer* serializer =
      base_logic::ValueSerializer::Create(base_logic::IMPL_JSON, &body_str_);
  std::string err_str;
  int32 err = 0;
  Value* v = serializer->Deserialize(&err, &err_str);
  DicValue* dic;
  do {
    if (v != NULL) {
      if (typeid(*v) == typeid(ListValue)) {
        LOG(INFO) << "listvalue:" ;
       ((ListValue*)v)->GetDictionary(0, &dic);
      } else if (typeid(*v) == typeid(DicValue)) {
        LOG(INFO) << "dicvalue:" ;
        dic = (DicValue*) v;
      } else {
        err = USER_LOGIN_JSON_ERR;
        break;
      }
      r = dic->GetString(L"passwd_", &passwd_);
      LOG_IF(ERROR, !r) << "LoginRecv::passwd parse error";
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
  } while (0);
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
   base_logic::ValueSerializer* serializer =
       base_logic::ValueSerializer::Create(base_logic::IMPL_JSON, &body_str_);
   std::string err_str;
   DicValue* dic = (DicValue*)serializer->Deserialize(&err, &err_str);
   do {
     if (dic != NULL) {
       r = dic->GetReal(L"distance_", &distance_);
       LOG_IF(ERROR, !r) << "ObtainGuideRecv::distance_ parse error";
       r = dic->GetReal(L"longitude_", &longitude_);
       LOG_IF(ERROR, !r) << "ObtainGuideRecv::longitude_ parse error";
       r = dic->GetReal(L"latitude_", &latitude_);
       LOG_IF(ERROR, !r) << "ObtainGuideRecv::latitude_ parse error";
     } else {
       LOG(ERROR) << "ObtainGuideRecv Deserialize error";
       err = NEARBY_GUIDE_JSON_ERR;
     break;
     }
   } while (0);
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
  base_logic::ValueSerializer* serializer =
     base_logic::ValueSerializer::Create(base_logic::IMPL_JSON, &body_str_);
  std::string err_str;
  DicValue* dic = (DicValue*)serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "Heartbeat::uid_ parse error";
    } else {
      LOG(ERROR) << "Heartbeat Deserialize error";
      err = HEART_PACKET_JSON_ERR;
    break;
    }
  } while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                               serializer);
  return err;
}

RecommendGuideRecv::RecommendGuideRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  city_code_ = 0;
}

int32 RecommendGuideRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer =
     base_logic::ValueSerializer::Create(base_logic::IMPL_JSON, &body_str_);
  std::string err_str;
  DicValue* dic = (DicValue*)serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"city_code_", &city_code_);
      LOG_IF(ERROR, !r) << "RecommendGuideRecv::city_code_ parse error";
    } else {
      LOG(ERROR) << "RecommendGuideRecv Deserialize error";
      err = RECOMMEND_GUIDE_JSON_ERR;
    break;
    }
  } while (0);
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
  base_logic::ValueSerializer* serializer =
     base_logic::ValueSerializer::Create(base_logic::IMPL_JSON, &body_str_);
  std::string err_str;
  DicValue* dic = (DicValue*)serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "ChangePasswdRecv::uid_ parse error";
      r = dic->GetString(L"old_passwd_", &old_passwd_);
      LOG_IF(ERROR, !r) << "ChangePasswdRecv::old_passwd_ parse error";
      r = dic->GetString(L"new_passwd_", &new_passwd_);
      LOG_IF(ERROR, !r) << "ChangePasswdRecv::new_passwd_ parse error";
    } else {
      LOG(ERROR) << "ChangePasswdRecv Deserialize error";
      err = CHANGE_PASSWD_JSON_ERR;
    break;
    }
  } while (0);
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
  base_logic::ValueSerializer* serializer =
     base_logic::ValueSerializer::Create(base_logic::IMPL_JSON, &body_str_);
  std::string err_str;
  DicValue* dic = (DicValue*)serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"verify_type_", &verify_type_);
      LOG_IF(ERROR, !r) << "ObtainVerifyCodeRecv::verify_type_ parse error";
      r = dic->GetString(L"phone_num_", &phone_num_);
      LOG_IF(ERROR, !r) << "ObtainVerifyCodeRecv::phone_num_ parse error";
    } else {
      LOG(ERROR) << "ObtainVerifyCodeRecv Deserialize error";
      err = OBTAIN_VERIFYCODE_JSON_ERR;
    break;
    }
  } while (0);
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
  base_logic::ValueSerializer* serializer =
     base_logic::ValueSerializer::Create(base_logic::IMPL_JSON, &body_str_);
  std::string err_str;
  DicValue* dic = (DicValue*)serializer->Deserialize(&err, &err_str);
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
      LOG_IF(ERROR, !r) << "RegisterAccountRecv::passwd_ parse error";
      r = dic->GetString(L"token_", &token_);
      LOG_IF(ERROR, !r) << "RegisterAccount::token_ parse error";
    } else {
      LOG(ERROR) << "RegisterAccountRecv Deserialize error";
      err = REGISTER_ACCOUNT_JSON_ERR;
    break;
    }
  } while (0);
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
  base_logic::ValueSerializer* serializer =
     base_logic::ValueSerializer::Create(base_logic::IMPL_JSON, &body_str_);
  std::string err_str;
  DicValue* dic = (DicValue*)serializer->Deserialize(&err, &err_str);
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
      LOG(ERROR) << "ImproveDataRecv Deserialize error";
      err = IMPROVE_DATA_JSON_ERR;
    break;
    }
  } while (0);
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
  base_logic::ValueSerializer* serializer =
     base_logic::ValueSerializer::Create(base_logic::IMPL_JSON, &body_str_);
  std::string err_str;
  DicValue* dic = (DicValue*)serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetString(L"uid_str_", &uid_str_);
      LOG_IF(ERROR, !r) << "UserDetailRecv::uid_str_ parse error";

    } else {
      LOG(ERROR) << "UserDetailRecv Deserialize error";
      err = IMPROVE_DATA_JSON_ERR;
    break;
    }
  } while (0);
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
  base_logic::ValueSerializer* serializer =
     base_logic::ValueSerializer::Create(base_logic::IMPL_JSON, &body_str_);
  std::string err_str;
  DicValue* dic = (DicValue*)serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "ObtainTripRecv::uid_ parse error";
      r = dic->GetBigInteger(L"order_id_", &order_id_);
      LOG_IF(ERROR, !r) << "ObtainTripRecv::order_id_ parse error";
      r = dic->GetBigInteger(L"count_", &count_);
      LOG_IF(ERROR, !r) << "ObtainTripRecv::count_ parse error";
    } else {
      LOG(ERROR) << "ObtainTripRecv Deserialize error";
      err = TRIP_RECORD_JSON_ERR;
    break;
    }
  } while (0);
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
  base_logic::ValueSerializer* serializer =
     base_logic::ValueSerializer::Create(base_logic::IMPL_JSON, &body_str_);
  std::string err_str;
  DicValue* dic = (DicValue*)serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetString(L"sid_str_", &sid_str_);
      LOG_IF(ERROR, !r) << "ServiceDetailsRecv::uid_str_ parse error";

    } else {
      LOG(ERROR) << "ServiceDetailsRecv Deserialize error";
      err = SERVICE_DETAILS_JSON_ERR;
    break;
    }
  } while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                               serializer);
  return err;
}

DrawBillRecv::DrawBillRecv(PacketHead packet) {
  head_ = packet.head();
  body_str_ = packet.body_str();
  order_id_ = 0;
  invoice_type_ = 0;
}

int32 DrawBillRecv::Deserialize() {
  int32 err = 0;
  bool r = false;
  base_logic::ValueSerializer* serializer =
     base_logic::ValueSerializer::Create(base_logic::IMPL_JSON, &body_str_);
  std::string err_str;
  DicValue* dic = (DicValue*)serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"order_id_", &order_id_);
      LOG_IF(ERROR, !r) << "DrawBillRecv::order_id_ parse error";
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
      LOG(ERROR) << "ServiceDetailsRecv Deserialize error";
      err = DRAW_BILL_JSON_ERR;
      break;
    }
  } while (0);
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
  base_logic::ValueSerializer* serializer =
     base_logic::ValueSerializer::Create(base_logic::IMPL_JSON, &body_str_);
  std::string err_str;
  DicValue* dic = (DicValue*)serializer->Deserialize(&err, &err_str);
  do {
    if (dic != NULL) {
      r = dic->GetBigInteger(L"uid_", &uid_);
      LOG_IF(ERROR, !r) << "DeviceTokenRecv::uid_ parse error";
      r = dic->GetString(L"device_token_", &device_token_);
      LOG_IF(ERROR, !r) << "DeviceTokenRecv::device_token_ parse error";
    } else {
      LOG(ERROR) << "DeviceTokenRecv Deserialize error";
      err = SERVICE_DETAILS_JSON_ERR;
    break;
    }
  } while (0);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                               serializer);
  return err;
}

}  // namespace user

