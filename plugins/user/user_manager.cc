// Copyright (c) 2016 The tourism Authors. All rights reserved.
// user_manager.cc
// Created on: 2016年8月10日.
// Author: Paco.

#include "user/user_manager.h"

#include <stdio.h>

#include "glog/logging.h"

#include "user/user_opcode.h"
#include "user/user_interface.h"

namespace user {
UserManager* UserManager::instance_ = NULL;

UserManager* UserManager::GetInstance() {
  if (instance_ == NULL)
    instance_ = new UserManager();
  return instance_;
}

void UserManager::FreeInstance() {
  if (instance_ == NULL) {
    delete instance_;
    instance_ = NULL;
  }
}

UserManager::UserManager() {

}

UserManager::~UserManager() {

}

int32 UserManager::AssignPacket(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  UserInterface* interface = UserInterface::GetInstance();
  LOG(INFO) << "AssignPacket opcode" << (int)packet->operate_code();
  switch (packet->operate_code()) {
    case USER_LOGIN_REQ: {
      interface->UserLogin(socket, packet);
      break;
    }
    case NEARBY_GUIDE_REQ: {
      interface->NearbyGuide(socket, packet);
      break;
    }
    case GUIDE_RECOMMEND_REQ: {
      interface->RecommendGuide(socket, packet);
      break;
    }
    case GUIDE_DETAIL_REQ: {
      interface->GuideDetail(socket, packet);
      break;
    }
    case PACKET_HEART_REQ: {
      interface->HeartPacket(socket, packet);
      break;
    }
    case SERVICE_CITY_REQ: {
      interface->ServiceCity(socket, packet);
      break;
    }

  }
  return err;
}

int32 UserManager::CheckHeartLoss() {
  UserInterface* interface = UserInterface::GetInstance();
  interface->CheckHeartLoss();
  return 0;
}

}  // namespace user

