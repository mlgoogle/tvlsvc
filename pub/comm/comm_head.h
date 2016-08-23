// Copyright (c) 2016 The tourism Authors. All rights reserved.
// comm_head.h
// Created on: 2016年8月10日.
// Author: Paco.
#ifndef PUB_COMM_COMM_HEAD_H_
#define PUB_COMM_COMM_HEAD_H_

enum PACKET_TYPE {
  ERROR_TYPE = 0,
  USER_TYPE = 1,
  CHAT_TYPE = 2
};

enum PACKET_ERROR {
  SQL_EXEC_ERROR = -1000,
  USER_LOGIN_JSON_ERR = -1001,
  PHONE_NUM_ERR = -1002,
  PHONE_OR_PASSWD_ERR = -1003,
  NEARBY_GUIDE_JSON_ERR = -1004,
  NO_GUIDE_NEARBY = -1005,
  HEART_PACKET_JSON_ERR = -1006,
  NO_GUIDE = -1007,
  RECOMMEND_GUIDE_JSON_ERR = -1008,
  NO_RECOMMEND_GUIDE
};


#endif  // PUB_COMM_COMM_HEAD_H_
