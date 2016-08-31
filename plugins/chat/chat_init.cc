// Copyright (c) 2016 The chat Authors. All rights reserved.
// chat_init.cc
// Created on: 2016年8月24日
// Author: Paco

#include "chat/chat_init.h"

#include <assert.h>

#include "chat/chat_logic.h"
#include "core/common.h"
#include "core/plugins.h"
#include "pub/comm/comm_struct.h"

static void *OnChatStart() {
  signal(SIGPIPE, SIG_IGN);
  PluginInfo* chat = reinterpret_cast<PluginInfo*>
    (calloc(1, sizeof(PluginInfo)));
  chat->id = "chat";
  chat->name = "chat";
  chat->version = "1.0.0";
  chat->provider = "Paco";
  if (!chat::Chatlogic::GetInstance())
    assert(0);
  return chat;
}

static handler_t OnChatShutdown(struct server* srv, void* pd) {
  chat::Chatlogic::FreeInstance();
  return HANDLER_GO_ON;
}

static handler_t OnChatConnect(struct server *srv, int fd, void *data,
                               int len) {
  chat::Chatlogic::GetInstance()->OnChatConnect(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnChatMessage(struct server *srv, int fd, void *data,
                               int len) {
  chat::Chatlogic::GetInstance()->OnChatMessage(srv, fd, data, len);
  return HANDLER_GO_ON;
}

static handler_t OnChatClose(struct server *srv, int fd) {
  chat::Chatlogic::GetInstance()->OnChatClose(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnUnknow(struct server *srv, int fd, void *data, int len) {
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastConnect(struct server* srv, int fd, void *data,
                                    int len) {
  chat::Chatlogic::GetInstance()->OnBroadcastConnect(srv, fd, data, len);
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastClose(struct server* srv, int fd) {
  chat::Chatlogic::GetInstance()->OnBroadcastClose(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastMessage(struct server* srv, int fd, void *data,
                                    int len) {
  chat::Chatlogic::GetInstance()->OnBroadcastMessage(srv, fd, data, len);
  return HANDLER_GO_ON;
}

static handler_t OnInitTimer(struct server* srv) {
  chat::Chatlogic::GetInstance()->OnInitTimer(srv);
  return HANDLER_GO_ON;
}

static handler_t OnTimeOut(struct server* srv, char* id, int opcode, int time) {
  chat::Chatlogic::GetInstance()->OnTimeout(srv, id, opcode, time);
  return HANDLER_GO_ON;
}

int chat_plugin_init(struct plugin *pl) {
  pl->init = OnChatStart;
  pl->clean_up = OnChatShutdown;
  pl->connection = OnChatConnect;
  pl->connection_close = OnChatClose;
  pl->connection_close_srv = OnBroadcastClose;
  pl->connection_srv = OnBroadcastConnect;
  pl->handler_init_time = OnInitTimer;
  pl->handler_read = OnChatMessage;
  pl->handler_read_srv = OnBroadcastMessage;
  pl->handler_read_other = OnUnknow;
  pl->time_msg = OnTimeOut;
  pl->data = NULL;
  return 0;
}

