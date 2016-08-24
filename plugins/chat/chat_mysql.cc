// Copyright (c) 2016 The tvlsvc Authors. All rights reserved.
// chat_mysql.cc
// Created on: 2016年8月24日.
// Author: Paco.
#include "chat/chat_mysql.h"

#include <mysql/mysql.h>

#include "pub/storage/data_engine.h"
#include "pub/comm/comm_head.h"

#include "glog/logging.h"

namespace chat {
ChatMysql::ChatMysql(config::FileConfig* config) {
  mysql_engine_ = base_logic::DataEngine::Create(MYSQL_TYPE);
  mysql_engine_->InitParam(config->mysql_db_list_);
}

ChatMysql::~ChatMysql() {
  if (mysql_engine_) {
    delete mysql_engine_;
  }
  mysql_engine_ = NULL;
}

}  // namespace chat
