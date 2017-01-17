#include <sys/time.h>  
#include <sys/select.h>  
#include <iostream>
#include <time.h>  
#include <stdio.h>
#include <mysql.h>
#include <pthread.h>
#include <map>
#include "pub/storage/data_engine.h"
#include "config/config.h"
#include "http/http_api.h"
#include "glog/logging.h"
#include "pub/net/typedef.h"
using namespace std;

#define DEFAULT_CONFIG_PATH "config.xml"

base_logic::DataEngine* mysql_engine_;
struct IdCardInfo
{
	std::string strIdNum;
	std::string strIdName;
	std::string strIdUrlName;
	int IdCardId;
};
map<int, IdCardInfo> map_IdCard_Info_;

void CallIdCardIdentificatSelect(void* param, Value* value)
{
	base_storage::DBStorageEngine* engine =
		(base_storage::DBStorageEngine*) (param);
	MYSQL_ROW rows;
	int32 num = engine->RecordCount();
	IdCardInfo myIdCardInfo;
	if (num > 0) {
		while (rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)) {
			if (rows[0] != NULL)
				myIdCardInfo.strIdNum = rows[0];
			if (rows[1] != NULL)
				myIdCardInfo.strIdName = rows[1];
			if (rows[2] != NULL)
				myIdCardInfo.strIdUrlName = rows[2];
			if (rows[2] != NULL)
				myIdCardInfo.IdCardId = atoi(rows[3]);
			if (myIdCardInfo.strIdName != "")
			{
				map_IdCard_Info_.insert(pair<int, IdCardInfo>(1, myIdCardInfo));
			}
		}
	}
	else {
		LOG(WARNING) << "Call CallSpentCashUpdate count < 0";
	}
}

void DoWhileThings()
{	
	while (1)
	{
		cout << "runing" << endl;
		//��ѯ���ݿ�
		std::stringstream ss;
		ss << "call proc_IdCardIdentificatSelect()";
		LOG(INFO) << "sql:" << ss.str();
		DicValue* dict;
		int r = mysql_engine_->ReadData(ss.str(), dict, CallIdCardIdentificatSelect);

		if (map_IdCard_Info_.size() == 0)
		{
			sleep(60);
		}
		//ѭ����map��û��
		while (map_IdCard_Info_.size() != 0)
		{
			//�����ƽӿ�
			std::string strUrl = "http://idcardreturnphoto.haoservice.com/idcard/VerifyIdcardReturnPhoto";
			//�����ƽӿ�code
			std::string strHeader = "Authorization:APPCODE 900036feeee64ae089177dd06b25faa9";
			std::string strResult;
			base_logic::DictionaryValue dic;


			map<int, IdCardInfo>::iterator l_it;
			l_it = map_IdCard_Info_.begin();

			IdCardInfo tmp = l_it->second;
			dic.SetString(L"cardNo", tmp.strIdNum);
			dic.SetString(L"realName", tmp.strIdUrlName);

			//��֤���ؽ��
			base_http::HttpAPI::RequestGetMethod(strUrl, &dic, strResult, strHeader, 1);
			cout << strResult << endl;

			//������һ��
			base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
				base_logic::IMPL_JSON, &strResult, false);
			std::string err_str;
			DicValue* dicResultJosn;
			int32 err = 0;
			DicValue* dicJosn = (DicValue*)serializer->Deserialize(&err, &err_str);
	
			if (dicJosn != NULL){
				r = dicJosn->GetDictionary(L"result", &dicResultJosn);
				if (r)
				{
					//�����ڶ���
					int32 err = 0;
					bool bResultIsOk = false;
					if (dicResultJosn != NULL){
						r = dicResultJosn->GetBoolean(L"isok", &bResultIsOk);
						if (r)
						{
							//�������ݿ�
							std::stringstream strsql;
							if (bResultIsOk)
							{
								strsql << "call proc_IdCardIdentificatUpDate(" << tmp.IdCardId << "," "1)";
							}
							else
							{

								strsql << "call proc_IdCardIdentificatUpDate(" << tmp.IdCardId << "," "2)";
							}
							mysql_engine_->WriteData(strsql.str());

							map_IdCard_Info_.erase(l_it);
						}
					}

				}
			}
			else {
				LOG(ERROR) << "josn Deserialize error";
				break;
			}
		}
		sleep(60);
	}
	return;
}

int main(void)
{	
	int res;
	bool r = false;
	map_IdCard_Info_.clear();
	config::FileConfig* config = config::FileConfig::GetFileConfig();
	std::string path = DEFAULT_CONFIG_PATH;
	if (config == NULL) {
		LOG(ERROR) << "IdCard config init error";
		return false;
	}
	r = config->LoadConfig(path);
	if (!r) {
		LOG(ERROR) << "IdCard config load error";
		return false;
	}
	LOG(INFO) << "IdCard  init 111";
	mysql_engine_ = base_logic::DataEngine::Create(MYSQL_TYPE);
	mysql_engine_->InitParam(config->mysql_db_list_);

	//�����߳�
	DoWhileThings();
	return 0;
}
