//  Copyright (c) 2015-2015 The george Authors. All rights reserved.
//  Created on: 2016年5月18日 Author: kerry

#include "storage/storage.h"

#include <list>

#include "public/basic/basictypes.h"
#include "base/thread/base_thread_handler.h"
#include "base/thread/base_thread_lock.h"

namespace base_logic {

class MYSQL_Pool {
 public:
	MYSQL_Pool();
	virtual ~MYSQL_Pool(){};
 public:
	void Init(std::list<base::ConnAddr>& addrlist, const int32 db_conn_num = 10);
	void Dest();
 public:
	base_storage::DBStorageEngine* DBConnectionPop(void);
	void DBConnectionPush(base_storage::DBStorageEngine* engine);
 public:
	std::list<base_storage::DBStorageEngine*>     db_conn_pool_;
	threadrw_t*                                   db_pool_lock_;
	std::list<base::ConnAddr>                     addrlist_;
};

}
