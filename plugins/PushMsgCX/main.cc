#include "IGtPush.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include <string>
#include <vector>
#include "pub/storage/data_engine.h"
#include "pub/net/typedef.h"
#include "config/config.h"
#include "glog/logging.h"
#include "logic/logic_comm.h"
#ifdef WIN32
#include <windows.h>
#endif

using namespace std;

#define DEFAULT_CONFIG_PATH "config.xml"

static void printResult(IPushResult &result);

//https的域名
static char *host = "http://sdk.open.api.getui.net/apiex.htm";

//定义常量, appId、appKey、masterSecret 采用本文档 "第二步 获取访问凭证 "中获得的应用配置
static char *appId = "rx572izgdt5Sdfomxbel1";
static char *appKey = "LBW2Ex5cT79VIHunoC6Zg6";
static char *masterSecret = "ryvaqPpiXR96pYQgrLaJj7";

struct ChatInfo
{
	int64 nChatNo;                  //消息ID
	std::string dtNo;			    //设备号
	std::string msgFromName;		//消息从谁这里发送
	std::string msgText;			//消息正文
	int msgType;
};

base_logic::DataEngine* mysql_engine_;
vector<ChatInfo> vecChatInfo;
vector<int64> vecSuccessPushMsgId;
//apn单推
bool toapnsingletest(char* dt, char* chMsgTitle, char* MsgText, int nMsgCount){
	//准备数据
	LOG(INFO) << dt <<'    ' << chMsgTitle << "    "  <<MsgText << endl;;
	Message msg = { 0 };
	msg.isOffline = 1;//是否离线下发
	msg.offlineExpireTime = 1000 * 3600 * 2;//离线下发有效期 毫秒
	SingleMessage singleMsg = { 0 };
	singleMsg.msg = msg;

	//IOS自定义消息
	Entry cmsg = { 0 };
	strcpy(cmsg.key, "cmsg");
	strcpy(cmsg.value, "cmsg");

	//title-loc-args赋值
	ListItem titlelocargs[2] = { "titlelocargs1", "titlelocargs2" };

	//locargs赋值
	ListItem locargs[2] = { "locargs1", "locargs2" };

	//APN模板，只包含了父类模板
	APNTemplate templ = { 0 };
	templ.t.appId = appId;
	templ.t.appKey = appKey;
	templ.t.pushInfo.badge = nMsgCount;
	templ.t.pushInfo.body = MsgText;
	templ.t.pushInfo.title = chMsgTitle;
	IPushResult result = { 0 };

	result = pushAPNMessageToSingle(appKey, &templ, appId, dt);


	//如果成功就去出保存到成功列表,最后去数据库处理
	for (int i = 0; i < result.size; i++) {
		if (strcmp(result.entry[i].key, "result") == 0 && strcmp(result.entry[i].value, "ok") == 0)
		{
			vector<ChatInfo>::iterator _lit = vecChatInfo.begin();
			vecSuccessPushMsgId.push_back(_lit->nChatNo);
		}
	}
	//打印结果
	//printResult(result);
}


static void printResult(IPushResult &result) {
	cout << "print result:-------------" << endl;
	for (int i = 0; i < result.size; i++) {
		cout << result.entry[i].key << ": " << result.entry[i].value << endl;
	}
	cout << "print end:----------------" << endl;
}


void CallOfflineMessageSelect(void* param, Value* value)
{
	base_storage::DBStorageEngine* engine =
		(base_storage::DBStorageEngine*) (param);
	MYSQL_ROW rows;
	int32 num = engine->RecordCount();
	if (num > 0) {
		while (rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)) {
			ChatInfo myChatInfo;
			if (rows[0] != NULL)
				myChatInfo.dtNo = rows[0];
			if (rows[1] != NULL)
				myChatInfo.msgText = rows[1];
			if (rows[2] != NULL)
				myChatInfo.msgType = atoi(rows[2]);
			if (rows[3] != NULL)
				myChatInfo.msgFromName = rows[3];
			if (rows[4] != NULL)
				myChatInfo.nChatNo = atol(rows[4]);
			vecChatInfo.push_back(myChatInfo);
		}
	}
	else {
		LOG(WARNING) << "Call CallSpentCashUpdate count < 0";
	}
}

int main(){
	// 注意：接口传入字符必须为UTF-8编码，因ASCII字符UTF－8编码后与原先一样，所以可以不编码，但中文等非ASCII字符必须编码
	// 如果返回的类似错误"post http data failed, code=6"，错误码可百度CURL返回的错误码是什么意思，如http://www.cnblogs.com/wainiwann/p/3492939.html
	////程序运行前只需初始化一遍，若已经初始化过即可不用，"编码"两个字为固定写法，不需要做转换
	Result r = pushInit(host, appKey, masterSecret, "编码");
	if (r != SUCCESS){
		printf("pushInit for app failed: ret=%d\n", r);
		return -1;
	}

	//初始化数据库
	config::FileConfig* config = config::FileConfig::GetFileConfig();
	std::string path = DEFAULT_CONFIG_PATH;
	if (config == NULL) {
		LOG(ERROR) << "PushMsg config init error";
		return false;
	}

	bool res = config->LoadConfig(path);
	if (!res) {
		LOG(ERROR) << "PushMsg config load error";
		return false;
	}
	LOG(INFO) << "PushMsg  init success";
	mysql_engine_ = base_logic::DataEngine::Create(MYSQL_TYPE);
	mysql_engine_->InitParam(config->mysql_db_list_);


	while (1)
	{
		//查询数据库
		cout << "Running!\n" << endl;
		std::stringstream ss;
		ss << "call proc_Offline_Message_Querry(1)";
		LOG(INFO) << "sql:" << ss.str();
		DicValue* dict;
		res = mysql_engine_->ReadData(ss.str(), dict, CallOfflineMessageSelect);

		if (vecChatInfo.size() == 0)
		{
			sleep(60);
		}
		int nMsgCount = vecChatInfo.size();
		while (vecChatInfo.size() != 0)
		{
			vector<ChatInfo>::iterator _lit = vecChatInfo.begin();
			//APN单推			
			char* dtNo = const_cast<char*>(_lit->dtNo.c_str());
			char* msgFromName = const_cast<char*>(_lit->msgFromName.c_str());
			char* msgText = const_cast<char*>(_lit->msgText.c_str());

			toapnsingletest(dtNo, msgFromName, msgText, nMsgCount);
			vecChatInfo.erase(_lit);
		}

		//数据库的操作
		std::list<std::string> sql_list_;
		while (vecSuccessPushMsgId.size() != 0)
		{
			std::stringstream ss;
			ss << "call proc_Offline_Message_Update(" << vecSuccessPushMsgId.front() << ")";
			LOG(INFO) << ss.str().c_str();
			sql_list_.push_back(ss.str());
			vector<int64>::iterator l_it_msg = vecSuccessPushMsgId.begin();
			vecSuccessPushMsgId.erase(l_it_msg);
		}

		int32 err = 0;
		err = mysql_engine_->WriteDatas(sql_list_);
		if (err < 0)
			LOG_ERROR("数据库写入错误");
	}

	return 0;
}
