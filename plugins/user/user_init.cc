// Copyright (c) 2016 The user Authors. All rights reserved.
// user_init.cc
// Created on: 2016年8月8日
// Author: Paco

#include "user/user_init.h"

#include <assert.h>

#include "user/user_logic.h"
#include "core/common.h"
#include "core/plugins.h"
#include "pub/comm/comm_struct.h"

static void *OnUserStart() {
  signal(SIGPIPE, SIG_IGN);
  PluginInfo* user = reinterpret_cast<PluginInfo*>
    (calloc(1, sizeof(PluginInfo)));
  user->id = "user";
  user->name = "user";
  user->version = "1.0.0";
  user->provider = "Paco";
  if (!user::Userlogic::GetInstance())
    assert(0);
  return user;
}

static handler_t OnUserShutdown(struct server* srv, void* pd) {
  user::Userlogic::FreeInstance();
  return HANDLER_GO_ON;
}

static handler_t OnUserConnect(struct server *srv, int fd, void *data,
                               int len) {
  user::Userlogic::GetInstance()->OnUserConnect(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnUserMessage(struct server *srv, int fd, void *data,
                               int len) {
  bool r = false;
  r = user::Userlogic::GetInstance()->OnUserMessage(srv, fd, data, len);
  if (r) {
    return HANDLER_FINISHED;
  }
  return HANDLER_GO_ON;
}

static handler_t OnUserClose(struct server *srv, int fd) {
  user::Userlogic::GetInstance()->OnUserClose(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnUnknow(struct server *srv, int fd, void *data, int len) {
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastConnect(struct server* srv, int fd, void *data,
                                    int len) {
  user::Userlogic::GetInstance()->OnBroadcastConnect(srv, fd, data, len);
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastClose(struct server* srv, int fd) {
  user::Userlogic::GetInstance()->OnBroadcastClose(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastMessage(struct server* srv, int fd, void *data,
                                    int len) {
  user::Userlogic::GetInstance()->OnBroadcastMessage(srv, fd, data, len);
  return HANDLER_GO_ON;
}

static handler_t OnInitTimer(struct server* srv) {
  user::Userlogic::GetInstance()->OnInitTimer(srv);
  return HANDLER_GO_ON;
}

static handler_t OnTimeOut(struct server* srv, char* id, int opcode, int time) {
  user::Userlogic::GetInstance()->OnTimeout(srv, id, opcode, time);
  return HANDLER_GO_ON;
}

int user_plugin_init(struct plugin *pl) {
  pl->init = OnUserStart;
  pl->clean_up = OnUserShutdown;
  pl->connection = OnUserConnect;
  pl->connection_close = OnUserClose;
  pl->connection_close_srv = OnBroadcastClose;
  pl->connection_srv = OnBroadcastConnect;
  pl->handler_init_time = OnInitTimer;
  pl->handler_read = OnUserMessage;
  pl->handler_read_srv = OnBroadcastMessage;
  pl->handler_read_other = OnUnknow;
  pl->time_msg = OnTimeOut;
  pl->data = NULL;
  return 0;
}

