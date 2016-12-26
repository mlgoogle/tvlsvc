#include "pub/storage/mysql_pool.h"
#include "base/logic/logic_comm.h"

#include "glog/logging.h"

namespace base_logic {

MYSQL_Pool::MYSQL_Pool()
    : db_pool_lock_(NULL) {
}

void MYSQL_Pool::Init(std::list<base::ConnAddr>& addrlist,
                      const int32 db_conn_num/* = 5*/) {
  addrlist_ = addrlist;
  bool r = false;
  InitThreadrw(&db_pool_lock_);
  for (int i = 0; i < db_conn_num; i++) {
    base_storage::DBStorageEngine* engine =
        base_storage::DBStorageEngine::Create(base_storage::IMPL_MYSQL);
    if (engine == NULL) {
      assert(0);
      continue;
    }

    r = engine->Connections(addrlist_);
    if (!r) {
      assert(0);
      continue;
    }
    db_conn_pool_.push_back(engine);

  }
}

void MYSQL_Pool::DBConnectionPush(base_storage::DBStorageEngine* engine) {
  base_logic::WLockGd lk(db_pool_lock_);
  db_conn_pool_.push_back(engine);
}

base_storage::DBStorageEngine* MYSQL_Pool::DBConnectionPop() {
  base_logic::WLockGd lk(db_pool_lock_);
  if (db_conn_pool_.empty()) {
    LOG(ERROR)<< "db_conn_pool_ empty";
    return NULL;
  }
  base_storage::DBStorageEngine* engine = NULL;
  engine = db_conn_pool_.front();
  db_conn_pool_.pop_front();
  if (engine == NULL) {
    LOG(ERROR)<< "DBConnectionPop engine NULL";
    engine = base_storage::DBStorageEngine::Create(base_storage::IMPL_MYSQL);
    engine->Connections(addrlist_);
  }
  if (!engine->CheckConnect()) {
    engine->Release();
    LOG(WARNING)<< "reconnect ";
    int reconnects = 3;
    do {
      if (engine->Connections(addrlist_))
      break;
      --reconnects;
    }while (reconnects > 0);
  }
  return engine;
}

void MYSQL_Pool::Dest() {
  base_logic::WLockGd lk(db_pool_lock_);
  while (!db_conn_pool_.empty()) {
    base_storage::DBStorageEngine* engine = db_conn_pool_.front();
    db_conn_pool_.pop_front();
    if (engine) {
      engine->Release();
      delete engine;
      engine = NULL;
    }
  }
  DeinitThreadrw(db_pool_lock_);
}

}
