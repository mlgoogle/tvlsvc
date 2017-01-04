// Copyright (c) 2016 The tourism Authors. All rights reserved.
// comm_head.h
// Created on: 2016年8月10日.
// Author: Paco.
#ifndef PUB_COMM_COMM_HEAD_H_
#define PUB_COMM_COMM_HEAD_H_

enum TIME_TASK {
  CONNECT_CKECK = 100,
  SHARE_DATA_INIT = 101,//初始化共享数据
  SHARE_DATA_INIT_TEN = 102,//定时更新共享数据
  ORDER_STATUS_CHECK = 103, //定时监测未支付的订单
};

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
  NO_RECOMMEND_GUIDE = -1009,
  ASK_INVITATION_JSON_ERR = -1010,
  CHANGE_PASSWD_JSON_ERR = -1011,
  USER_NOT_IN_CACHE = -1012,
  CHANGE_OLD_PWD_ERR = -1013,
  CHAT_PACKET_JSON_ERR = -1014,
  NEW_ORDER_ERR = -1015,
  CHAT_RECORD_JSON_ERR = -1016,
  NO_CHAT_RECORD = -1017,
  OBTAIN_VERIFYCODE_JSON_ERR = -1018,
  REGISTER_ACCOUNT_JSON_ERR = -1019,
  VERIFY_CODE_OVERDUE = -1020,
  VERIFY_CODE_ERR = -1021,
  IMPROVE_DATA_JSON_ERR = -1022,
  TRIP_RECORD_JSON_ERR = -1023,
  SERVICE_DETAILS_JSON_ERR = -1024,
  NO_USER = -1025,
  DRAW_BILL_JSON_ERR = -1026,
  NO_USER_DEVICE_TOKEN = -1027, //没有该用户devicetoken
  DEVICE_TOKEN_ERR = -1028,
  MSG_READ_JSON_ERR = -1029,
  EVALUATE_TRIP_JSON_ERR = -1030,
  ANSWER_INVITATION_JSON_ERR = -1031,
  ORDER_STATUS_ERR = -1032,
  BILL_RECORD_JSON_ERR = -1033,
  BLACK_PRIVILEGE_EMPTY = -1034,
  BLACKCARD_INFO_JSON_ERR = -1035,
  BLACKCARD_CONSUME_RECORD_JSON_ERR = -1036,
  NEW_APPOINTMENT_JSON_ERR = -1037,
  ALREADY_INVOICE_RECORD = -1038, //已经请求过开票了
  ORDER_NOT_PAY = -1039,//订单未支付
  NO_ONLINE_COORDINATOR = -1040, //没有在线客服
  INVOICE_DETAIL_JSON_ERR = -1041,
  WX_PLACE_ORDER_JSON_ERR = -1042,
  ORDER_PRICE_ERR = -1043, // 下单金额有误
  WXPAY_CLIENT_JSON_ERR = -1044,
  IDENTITY_PIC_JSON_ERR = -1045,
  GUIDE_DETAIL_JSON_ERR = -1046,
  IDENTITY_STATUS_JSON_ERR = -1047,
  SHARE_PAGING_JSON_ERR = -1048,
  SHARE_TOURISM_DETAILS_JSON_ERR = -1049,
  USER_CASH_JSON_ERR = -1050,
  SMS_CODE_LOGIN_JSON_ERR = -1051,
  EVALUATE_INFO_JSON_ERR = -1052,
  NO_EVALUTE_INFO = -1053,
  APPOINTMENT_RECORD_JSON_ERR = -1054,
  APPOINTMENT_RECORD_EMPTY = -1055,
  SHARE_SKILL_DETAILS_JSON_ERR = -1056,
  SHARE_SKILL_DISCUSS_JSON_ERR = -1057,
  ENTRY_SHARE_SKILL_JSON_ERR = -1058,
  REQUEST_JSON_ERR = -1059,
  WXPAY_SERVER_JSON_ERR = -1060, //微信支付服务端回调参数有误
  WX_PLACE_ORDER_ERR = -1061,  //微信下单失败
  VERIFY_PASSWD_ERR = -1062, //密码错误
  HAS_SETTING_PAY_PASSWD = -1063,
  CANNOT_SET_LOGIN_PASSWD = -1064,
  PAY_PASSWD_ERR = -1065, //支付密码错误
  VERIFY_CODE_NOT_NEW = -1066, //验证码不是最新的
  INVITATION_PHONE_NUM_ERR = -1037 //邀请人手机号码错误
};


#endif  // PUB_COMM_COMM_HEAD_H_
