// Copyright (c) 2016 The tourism Authors. All rights reserved.
// user_opcode.h
// Created on: 2016年8月10日.
// Author: Paco.
#ifndef PLUGINS_USER_USER_OPCODE_H_
#define PLUGINS_USER_USER_OPCODE_H_

#include "pub/comm/comm_head.h"

namespace user {
#define PACKET_HEART_REQ 1000
#define USER_LOGIN_REQ USER_TYPE*1000 + 1
#define USER_LOGIN_RLY USER_TYPE*1000 + 2

#define NEARBY_GUIDE_REQ USER_TYPE*1000 + 3
#define NEARBY_GUIDE_RLY USER_TYPE*1000 + 4

#define GUIDE_DETAIL_REQ USER_TYPE*1000 + 5
#define GUIDE_DETAIL_RLY USER_TYPE*1000 + 6

#define GUIDE_RECOMMEND_REQ USER_TYPE*1000 + 7
#define GUIDE_RECOMMEND_RLY USER_TYPE*1000 + 8

#define SERVICE_CITY_REQ USER_TYPE*1000 + 9
#define SERVICE_CITY_RLY USER_TYPE*1000 + 10

#define CHANGE_PASSWD_REQ USER_TYPE*1000 + 11
#define CHANGE_PASSWD_RLY USER_TYPE*1000 + 12

#define USER_INFO_REQ USER_TYPE*1000 + 13
#define USER_INFO_RLY USER_TYPE*1000 + 14

#define ALIPAY_SEVER_REQ USER_TYPE*1000 + 15
#define ALIPAY_SEVER_RLY USER_TYPE*1000 + 16

#define ALIPAY_CLIENT_REQ USER_TYPE*1000 + 17
#define ALIPAY_CLIENT_RLY USER_TYPE*1000 + 18

#define QUERY_VERIFY_CODE_REQ USER_TYPE*1000 + 19
#define QUERY_VERIFY_CODE_RLY USER_TYPE*1000 + 20

#define REGISTER_ACCOUNT_REQ USER_TYPE*1000 + 21
#define REGISTER_ACCOUNT_RLY USER_TYPE*1000 + 22

#define IMPROVE_DATA_REQ USER_TYPE*1000 + 23
#define IMPROVE_DATA_RLY USER_TYPE*1000 + 24

#define OBTAIN_TRIP_REQ USER_TYPE*1000 +25
#define OBTAIN_TRIP_RLY USER_TYPE*1000 +26

#define SERVICE_INFO_REQ USER_TYPE*1000 + 27
#define SERVICE_INFO_RLY USER_TYPE*1000 + 28

#define DRAW_BILL_REQ USER_TYPE*1000 + 29
#define DRAW_BILL_RLY USER_TYPE*1000 + 30

#define DEVICE_TOKEN_REQ USER_TYPE*1000 + 31
#define DEVICE_TOKEN_RLY USER_TYPE*1000 + 32

}  // namespace user



#endif  // PLUGINS_USER_USER_OPCODE_H_
