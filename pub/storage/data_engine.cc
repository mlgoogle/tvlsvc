//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2015年9月17日 Author: kerry

#include "pub/storage/data_engine.h"
#include "pub/storage/data_mysql_engine.h"


namespace base_logic {

DataEngine* DataEngine::Create(int32 type) {
	DataEngine* engine = NULL;
    switch (type) {
     /* case REIDS_TYPE: {
        engine = new RedisController();
        break;
      }*/
      case MYSQL_TYPE: {
        engine = new DataMYSQLEngine();
        break;
      }
      /*case MEM_TYPE: {
        engine = new MemController();
        break;
      }*/
      default:
        break;
    }
    return engine;
}

void DataEngine::Init(config::FileConfig* config) {
}

void DataEngine::Dest() {

}

}  // namespace base_logic



