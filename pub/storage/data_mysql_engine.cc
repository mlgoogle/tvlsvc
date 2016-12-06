#include "data_mysql_engine.h"

namespace base_logic {

void DataMYSQLEngine::InitParam(std::list<base::ConnAddr>& addrlist) {
	db_pool_.Init(addrlist);
}

void DataMYSQLEngine::Release() {
	db_pool_.Dest();
}

bool DataMYSQLEngine::WriteData(const std::string& sql) {
  bool r = false;
  base_storage::DBStorageEngine* engine = db_pool_.DBConnectionPop();
  if (engine == NULL) {
    return false;
  }
  engine->FreeRes();
  r = engine->SQLExec(sql.c_str());
  if (!r) {
    return false;
  }
  db_pool_.DBConnectionPush(engine);
  return true;
}

bool DataMYSQLEngine::WriteDatas(std::list<std::string>& sqls) {
  bool r = false;
  base_storage::DBStorageEngine* engine = db_pool_.DBConnectionPop();
  if (engine == NULL) {
    return false;
  }
  engine->FreeRes();
  r = engine->SQLExecs(sqls);
  if (!r) {
    return false;
  }
  db_pool_.DBConnectionPush(engine);
  return true;
}


bool DataMYSQLEngine::ReadData(const std::string& sql, base_logic::Value* value,
        void (*storage_get)(void*, base_logic::Value*)) {
  bool r = false;
  base_storage::DBStorageEngine* engine = db_pool_.DBConnectionPop();
  do {
    if (engine == NULL) {
      r = false;
      break;
    }
    engine->FreeRes();
    r = engine->SQLExec(sql.c_str());
    if (!r) {
      r = false;
      break;
    }
    if (storage_get == NULL) {
      r = false;
      break;
    } else {
      storage_get(reinterpret_cast<void*>(engine), value);
      engine->FreeRes();
    }
  }while (0);
  db_pool_.DBConnectionPush(engine);
  return r;
}


}
