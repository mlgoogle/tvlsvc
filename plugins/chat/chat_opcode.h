// Copyright (c) 2016 The tvlsvc Authors. All rights reserved.
// chat_opcode.h
// Created on: 2016年8月24日.
// Author: Paco.
#ifndef PLUGINS_CHAT_OPCODE_H_
#define PLUGINS_CHAT_OPCODE_H_

#include "pub/comm/comm_head.h"

namespace chat {

#define ASK_INVITATION_REQ CHAT_TYPE*1000 + 1
#define ASK_INVITATION_RLY CHAT_TYPE*1000 + 2

#define CHAT_MESSAGE_REQ CHAT_TYPE*1000 + 3
#define CHAT_MESSAGE_RLY CHAT_TYPE*1000 + 4


#define RECORD_CHAT_MSG 200
}  // namespace chat



#endif  // PLUGINS_CHAT_OPCODE_H_
