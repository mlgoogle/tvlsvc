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
using namespace std;

#define DEFAULT_CONFIG_PATH "config.xml"

MYSQL conn;
struct IdCardInfo
{
	std::string strIdNum;
	std::string strIdName;
	std::string strIdUrlName;
};
map<int, IdCardInfo> map_IdCard_Info_;

void setTimer(int seconds, int mseconds)
{
	struct timeval temp;
	temp.tv_sec = seconds;
	temp.tv_usec = mseconds;
	select(0, NULL, NULL, NULL, &temp);
	printf("timer\n");
	return;
}

void* thread(void *arg)
{	
	while (1)
	{
		printf("This is a thread and arg = %d.\n", *(int*)arg);
		*(int*)arg = 0;

		//循环进行数据库的读写
		std::string query = "call proc_IdCardIdentificatInsert('sdasdsdsdsdsd','111111',11)";
		int res = mysql_query(&conn, query.c_str());
		if (res)
		{
			printf("error %d\n", res);
		}
		else
		{
			printf("OK\n");
		}

		//阿里云接口
		std::string strUrl = "http://idcardreturnphoto.haoservice.com/idcard/VerifyIdcardReturnPhoto?cardNo=13032119880401018&realName=%E5%BC%A0%E5%BC%BA";
		std::string strHeader = "Authorization:APPCODE 900036feeee64ae089177dd06b25faa9";
		std::string strResult;
		base_logic::DictionaryValue dic;
		//base_http::HttpAPI::RequestGetMethod(strUrl, &dic, strResult, strHeader, 1);
		//cout << strResult << endl;

		bool bIsAddIdCardInfo = false;
		if (!bIsAddIdCardInfo)
		{
			//sleep(1000 * 60);
		}
		sleep(1000 * 60);
	}
	return arg;
}

int main(void)
{	
	int res;
	mysql_init(&conn);
	//初始化mysql
	if (mysql_real_connect(&conn, "localhost", "tourism", "123456", "tourism", 0, NULL, CLIENT_FOUND_ROWS)) //"root":数据库管理员 "":root密码 "test":数据库的名字
	{
		printf("connect success!\n");
	}
	mysql_set_character_set(&conn, "gbk");

	//线程
	pthread_t th;
	int ret;
	int arg = 10;
	int *thread_ret = NULL;
	ret = pthread_create(&th, NULL, thread, &arg);
	if (ret != 0){
		printf("Create thread error!\n");
		return -1;
	}
	

	bool r = false;
	config::FileConfig* config = config::FileConfig::GetFileConfig();
	std::string path = DEFAULT_CONFIG_PATH;
	if (config == NULL) {
		LOG(ERROR) << "Chatlogic config init error";
		return false;
	}
	r = config->LoadConfig(path);
	if (!r) {
		LOG(ERROR) << "chat config load error";
		return false;
	}
	LOG(INFO) << "chat plugin init 111";
	base_logic::DataEngine* mysql_engine_;
	mysql_engine_ = base_logic::DataEngine::Create(MYSQL_TYPE);
	mysql_engine_->InitParam(config->mysql_db_list_);
	std::stringstream ss;
	ss << "call proc_IdCardIdentificatInsert('sdasdsdsdsdsd','111sdsds11',11)";
	LOG(INFO) << "sql:" << ss.str();
	r = mysql_engine_->WriteData(ss.str());
	

	//启动线程
	//printf("This is the main process.\n");
	//pthread_join(th, (void**)&thread_ret);
	//printf("thread_ret = %d.\n", *thread_ret);
	
	mysql_close(&conn);
	return 0;
}