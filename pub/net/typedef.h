// Copyright (c) 2016 The tourism Authors. All rights reserved.
// typedef.h
// Created on: 2016年8月10日.
// Author: Paco.
#ifndef PUB_NET_TYPEDEF_H_
#define PUB_NET_TYPEDEF_H_

#include <map>

#include "pub/comm/user_info.h"
#include "base/logic/base_values.h"



typedef base_logic::DictionaryValue DicValue;
typedef base_logic::ListValue ListValue;
typedef base_logic::Value Value;

typedef std::map<int64, Guide*> GuideMap;
typedef std::map<int64, Visitor*> VisitorMap;
typedef std::map<int64, UserInfo*> UserMap;



#endif  // PUB_NET_TYPEDEF_H_
