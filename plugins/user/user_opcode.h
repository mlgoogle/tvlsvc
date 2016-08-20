// Copyright (c) 2016 The tourism Authors. All rights reserved.
// user_opcode.h
// Created on: 2016年8月10日.
// Author: Paco.
#ifndef PLUGINS_USER_USER_OPCODE_H_
#define PLUGINS_USER_USER_OPCODE_H_

#include "pub/comm/comm_head.h"

namespace user {

#define USER_LOGIN_REQ USER_TYPE*1000 + 1
#define USER_LOGIN_RLY USER_TYPE*1000 + 2

#define NEARBY_WAITER_REQ USER_TYPE*1000 + 3
#define NEARBY_WAITER_RLY USER_TYPE*1000 + 4



}  // namespace user



#endif  // PLUGINS_USER_USER_OPCODE_H_
