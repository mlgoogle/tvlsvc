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

}  // namespace user

