// Copyright (c) 2016 The tourism Authors. All rights reserved.
// typedef.h
// Created on: 2016年8月10日.
// Author: Paco.
#ifndef PUB_NET_TYPEDEF_H_
#define PUB_NET_TYPEDEF_H_

#include <map>

#include "pub/comm/user_info.h"
#include "pub/comm/share_guide.h"
#include "base/logic/base_values.h"



typedef base_logic::DictionaryValue DicValue;
typedef base_logic::ListValue ListValue;
typedef base_logic::Value Value;

typedef std::map<int64, Coordinator*> CoordinatorMap;
typedef std::map<int64, Guide*> GuideMap;
typedef std::map<int64, Visitor*> VisitorMap;
typedef std::map<int64, UserInfo*> UserMap;
typedef std::map<int64, std::string> DeviceTokenMap;
typedef std::map<int64, int32> UnReadMap;
typedef std::map<int64, std::string> NickMap;
typedef std::map<std::string, std::string> SMSTokenMap;
typedef std::map<int64, ShareTourism*> ShareTourismMap;
typedef std::map<int64, ShareSkill*> ShareSkillMap;
typedef std::map<int64, int64> ShareIdMap;
typedef std::map<int64, ShareIdMap> ShareTypeMap;



#endif  // PUB_NET_TYPEDEF_H_
