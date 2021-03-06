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

#define SERVICE_INFO_REQ USER_TYPE*1000 + 27 //开票信息里面的服务信息
#define SERVICE_INFO_RLY USER_TYPE*1000 + 28

#define DRAW_BILL_REQ USER_TYPE*1000 + 29
#define DRAW_BILL_RLY USER_TYPE*1000 + 30

#define DEVICE_TOKEN_REQ USER_TYPE*1000 + 31
#define DEVICE_TOKEN_RLY USER_TYPE*1000 + 32

#define INVOICE_RECORD_REQ USER_TYPE*1000 + 33
#define INVOICE_RECORD_RLY USER_TYPE*1000 + 34

#define BLACKCARD_PRIVILEGE_REQ USER_TYPE*1000 + 35
#define BLACKCARD_PRIVILEGE_RLY USER_TYPE*1000 + 36

#define BLACKCARD_INFO_REQ USER_TYPE*1000 + 37
#define BLACKCARD_INFO_RLY USER_TYPE*1000 + 38

#define BLACKCARD_CONSUME_RECORD_REQ USER_TYPE*1000 + 39
#define BLACKCARD_CONSUME_RECORD_RLY USER_TYPE*1000 + 40

#define SKILL_INFO_REQ USER_TYPE*1000 + 41
#define SKILL_INFO_RLY USER_TYPE*1000 + 42

#define NEW_APPOINTMENT_REQ USER_TYPE*1000 + 43
#define NEW_APPOINTMENT_RLY USER_TYPE*1000 + 44

#define INVOICE_DETAIL_REQ USER_TYPE*1000 + 45 //开票详情
#define INVOICE_DETAIL_RLY USER_TYPE*1000 + 46

#define IMG_TOKEN_REQ USER_TYPE*1000 + 47 //图片上传token
#define IMG_TOKEN_RLY USER_TYPE*1000 + 48

#define WX_PLACE_ORDER_REQ  USER_TYPE*1000 + 49 //微信下单
#define WX_PLACE_ORDER_RLY  USER_TYPE*1000 + 50 //微信下单

#define WXPAY_CLIENT_REQ USER_TYPE*1000 + 51 //微信客服端支付成功
#define WXPAY_CLIENT_RLY USER_TYPE*1000 + 52 //微信客服端支付成功

#define WXPAY_SERVER_REQ USER_TYPE*1000 + 53 //微信服务端支付成功
#define WXPAY_SERVER_RLY USER_TYPE*1000 + 54 //微信服务端支付成功

#define IDENTITY_PIC_REQ USER_TYPE*1000 + 55 //服务者身份证信息认证
#define IDENTITY_PIC_RLY USER_TYPE*1000 + 56 //

#define IDENTITY_STATUS_REQ USER_TYPE*1000 + 57 //获取身份认证状态
#define IDENTITY_STATUS_RLY USER_TYPE*1000 + 58 //获取身份认证状态

#define SHARE_TOURISM_TYPE_REQ USER_TYPE*1000 + 59 //获取旅游分享类别
#define SHARE_TOURISM_TYPE_RLY USER_TYPE*1000 + 60 //获取旅游分享类别

#define SHARE_TOURISM_RECOMMEND_REQ USER_TYPE*1000 + 61
#define SHARE_TOURISM_RECOMMEND_RLY USER_TYPE*1000 + 62

#define SHARE_TOURISM_LIST_REQ USER_TYPE*1000 + 63
#define SHARE_TOURISM_LIST_RLY USER_TYPE*1000 + 64

#define SHARE_TOURISM_DETAIL_REQ USER_TYPE*1000 + 65
#define SHARE_TOURISM_DETAIL_RLY USER_TYPE*1000 + 66

#define USER_CASH_REQ  USER_TYPE*1000 + 67 //用户余额
#define USER_CASH_RLY  USER_TYPE*1000 + 68

#define APPOINTMENT_RECORD_REQ USER_TYPE*1000 + 69 //我的预约记录
#define APPOINTMENT_RECORD_RLY USER_TYPE*1000 + 70 //我的预约记录

#define SHARE_SKILL_LIST_REQ USER_TYPE*1000 + 71
#define SHARE_SKILL_LIST_RLY USER_TYPE*1000 + 72

#define SHARE_SKILL_DETAIL_REQ USER_TYPE*1000 + 73
#define SHARE_SKILL_DETAIL_RLY USER_TYPE*1000 + 74

#define SHARE_SKILL_DISCUSS_REQ USER_TYPE*1000 + 75
#define SHARE_SKILL_DISCUSS_RLY USER_TYPE*1000 + 76

#define ENTRY_SHARE_SKILL_REQ USER_TYPE*1000 + 77
#define ENTRY_SHARE_SKILL_RLY USER_TYPE*1000 + 78

#define GUIDE_INFO_REQ USER_TYPE*1000 + 79
#define GUIDE_INFO_RLY USER_TYPE*1000 + 80

#define ORDER_DETAILS_REQ USER_TYPE*1000 + 81
#define ORDER_DETAILS_RLY USER_TYPE*1000 + 82

#define BLACKCARD_PRICE_INFO_REQ USER_TYPE*1000 + 83
#define BLACKCARD_PRICE_INFO_RLY USER_TYPE*1000 + 84

#define PLACE_BLACKCARD_ORDER_REQ USER_TYPE*1000 + 85
#define PLACE_BLACKCARD_ORDER_RLY USER_TYPE*1000 + 86

#define VERIFY_PASSWD_REQ USER_TYPE*1000 + 87
#define VERIFY_PASSWD_RLY USER_TYPE*1000 + 88

#define CHANGE_COMM_PASSWD_REQ USER_TYPE*1000 + 89 // 修改密码
#define CHANGE_COMM_PASSWD_RLY USER_TYPE*1000 + 90

#define VERIFY_VLEADER_HEAD_REQ USER_TYPE*1000 + 91
#define VERIFY_VLEADER_HEAD_RLY USER_TYPE*1000 + 92
	
#define CHANGE_USER_INFO_REQ USER_TYPE*1000 + 93
#define CHANGE_USER_INFO_RLY USER_TYPE*1000 + 94
	
#define CHANGE_BANK_CARD_REQ USER_TYPE*1000 + 95
#define CHANGE_BANK_CARD_RLY USER_TYPE*1000 + 96
	
#define BANK_CARD_INFO_REQ USER_TYPE*1000 + 97
#define BANK_CARD_INFO_RLY USER_TYPE*1000 + 98
	
#define CHANGE_DEFAULT_BANK_CARD_REQ USER_TYPE*1000 + 99
#define CHANGE_DEFAULT_BANK_CARD_RLY USER_TYPE*1000 + 100
	
#define CHECK_SMS_CODE_REQ USER_TYPE*1000 + 101 //验证短信验证码
#define CHECK_SMS_CODE_RLY USER_TYPE*1000 + 102 //验证短信验证码

#define USER_WITHDRAW_REQ USER_TYPE*1000 + 103
#define USER_WITHDRAW_RLY USER_TYPE*1000 + 104
	
#define USER_WITHDRAW_RECORD_REQ USER_TYPE*1000 + 105
#define USER_WITHDRAW_RECORD_RLY USER_TYPE*1000 + 106

#define USER_UPLOAD_PHOTO_REQ USER_TYPE*1000 + 107
#define USER_UPLOAD_PHOTO_RLY USER_TYPE*1000 + 108

#define USER_PHOTO_ALBUM_REQ USER_TYPE*1000 + 109
#define USER_PHOTO_ALBUM_RLY USER_TYPE*1000 + 110

//服务者接口
#define GUIDE_SERVICE_LIST_REQ USER_TYPE*1000 + 501
#define GUIDE_SERVICE_LIST_RLY USER_TYPE*1000 + 502

#define CHANGE_GUIDE_SERVICE_REQ USER_TYPE*1000 + 503
#define CHANGE_GUIDE_SERVICE_RLY USER_TYPE*1000 + 504

#define GUIDE_ORDER_RECORD_REQ USER_TYPE*1000 + 505
#define GUIDE_ORDER_RECORD_RLY USER_TYPE*1000 + 506

#define GUIDE_ORDER_DETAIL_REQ USER_TYPE*1000 + 507
#define GUIDE_ORDER_DETAIL_RLY USER_TYPE*1000 + 508

#define DEFINE_GUIDE_SKILL_REQ USER_TYPE*1000 + 509
#define DEFINE_GUIDE_SKILL_RLY USER_TYPE*1000 + 510

}  // namespace user



#endif  // PLUGINS_USER_USER_OPCODE_H_
