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
#define APP_TYPE              2   //1 代表出行 2助理
static void printResult(IPushResult &result);
void OrderMsgPushed();

//https的域名
static char *host = "http://sdk.open.api.getui.net/apiex.htm";

//定义常量, appId、appKey、masterSecret 采用本文档 "第二步 获取访问凭证 "中获得的应用配置
static char *appId = "ym6gRyDFK08iP0seFqYmm3";
static char *appKey = "Ut4rL2IDMP8Gj0biBFc3UA";
static char *masterSecret = "zuoDKEcIheAaIs2Pr0GCr8";

struct ChatInfo
{
	int64 nChatNo;                  //消息ID
	std::string dtNo;			    //设备号
	std::string msgFromName;		//消息从谁这里发送
	std::string msgText;			//消息正文
	int msgType;
};

struct OrderMsgInfo
{
	int64 nMsgId;					//消息号
	int64 from_uid_;				//发消息的Id
	int64 to_uid_;					//推送Id
	int64 msg_type_;				//消息类型
	int64 msg_time_;				//消息时间
	std::string servant_id_;		//服务者Id
	int64 appointment_id_;			//订单号
	std::string content_;			//消息正文
	std::string dtNo;			    //设备号
};

base_logic::DataEngine* mysql_engine_;
vector<ChatInfo> vecChatInfo;
vector<OrderMsgInfo> vecOrderMsgInfo;
vector<int64> vecSuccessPushMsgId;
//apn单推
bool toapnsingletest(std::string Indt, int64 nMsgId, std::string strInMsgTitle, std::string strInMsgText, int nMsgCount, std::string strCategory){
	//准备数据
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
	templ.t.pushInfo.body = const_cast<char*>(strInMsgText.c_str());
	templ.t.pushInfo.title = const_cast<char*>(strInMsgTitle.c_str());
	templ.t.pushInfo.category = const_cast<char*>(strCategory.c_str());
	IPushResult result = { 0 };

	char* dt = const_cast<char*>(Indt.c_str());
	result = pushAPNMessageToSingle(appKey, &templ, appId, dt);


	//如果成功就去出保存到成功列表,最后去数据库处理
	for (int i = 0; i < result.size; i++) {
		if (strcmp(result.entry[i].key, "result") == 0 && strcmp(result.entry[i].value, "ok") == 0)
		{
			vecSuccessPushMsgId.push_back(nMsgId);
		}
	}
	//打印结果
	printResult(result);
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
		LOG(WARNING) << "Call CallOfflineMessageSelect count < 0";
	}
}

void CallOrderMsgPushedSelect(void* param, Value* value)
{
	base_storage::DBStorageEngine* engine =
		(base_storage::DBStorageEngine*) (param);
	MYSQL_ROW rows;
	int32 num = engine->RecordCount();
	if (num > 0) {
		while (rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)) {
			OrderMsgInfo myOrderMsgInfo;
			if (rows[0] != NULL)
				myOrderMsgInfo.from_uid_ = atoi(rows[0]);
			if (rows[1] != NULL)
				myOrderMsgInfo.to_uid_ = atoi(rows[1]);
			if (rows[2] != NULL)
				myOrderMsgInfo.msg_type_ = atoi(rows[2]);
			if (rows[3] != NULL)
				myOrderMsgInfo.msg_time_ = atoi(rows[3]);
			if (rows[4] != NULL)
				myOrderMsgInfo.servant_id_ = rows[4];
			if (rows[5] != NULL)
				myOrderMsgInfo.appointment_id_ = atol(rows[5]);
			if (rows[6] != NULL)
				myOrderMsgInfo.content_ = rows[6];
			if (rows[7] != NULL)
				myOrderMsgInfo.nMsgId = atol(rows[7]);
			if (rows[8] != NULL)
				myOrderMsgInfo.dtNo = rows[8];
			vecOrderMsgInfo.push_back(myOrderMsgInfo);
		}
	}
	else {
		LOG(WARNING) << "Call CallOrderMsgPushedSelect count < 0";
	}
}

//用户聊天消息推送
void UserMsgPush(){
	//查询数据库
	vecSuccessPushMsgId.clear();
	cout << "Running UserMsgPush!\n" << endl;
	std::stringstream ss;
	ss << "call proc_Offline_Message_Querry(" << APP_TYPE << ")";
	LOG(INFO) << "sql:" << ss.str();
	DicValue* dict;
	bool res = mysql_engine_->ReadData(ss.str(), dict, CallOfflineMessageSelect);

	int nMsgCount = vecChatInfo.size();
	while (vecChatInfo.size() != 0)
	{
		vector<ChatInfo>::iterator _lit = vecChatInfo.begin();
		//APN单推
		toapnsingletest(_lit->dtNo, _lit->nChatNo, _lit->msgFromName, _lit->msgText, nMsgCount, "");
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

	OrderMsgPushed();
}

//订单消息推送
void OrderMsgPushed(){
	//查询数据库
	cout << "Running OrderMsgPushed!\n" << endl;
	vecSuccessPushMsgId.clear();
	vecOrderMsgInfo.clear();
	std::stringstream ss;
	ss << "call proc_OrderMsgSelete(" << APP_TYPE << ")";
	LOG(INFO) << "sql:" << ss.str();
	DicValue* dict;
	bool res = mysql_engine_->ReadData(ss.str(), dict, CallOrderMsgPushedSelect);

	if (vecOrderMsgInfo.size() == 0)
	{
		sleep(30);
	}
	int nMsgCount = vecOrderMsgInfo.size();
	while (vecOrderMsgInfo.size() != 0)
	{
		vector<OrderMsgInfo>::iterator _lit = vecOrderMsgInfo.begin();
		std::stringstream strBody;
		strBody << "{\"from_uid_\":" << _lit->from_uid_ << ",\"to_uid_\":" << _lit->to_uid_ << ", \"msg_type_\": " << _lit->msg_type_ << ", \"msg_time_\": " << _lit->msg_time_ <<
			",\"servant_id_\": \"" << _lit->servant_id_ << "\",\"appointment_id_\": " << _lit->appointment_id_ << ",\"content_\": \"" << _lit->content_ << "\"}";
		//cout << strBody.str().c_str() << endl;

		//cout << "设备编号" << _lit->dtNo << endl;
		//APN单推			
		//APN单推	
		std::string strName = "系统消息";
		cout << "标题" << strName << endl;
		toapnsingletest(_lit->dtNo, _lit->nMsgId, strName, _lit->content_, nMsgCount, strBody.str());
		vecOrderMsgInfo.erase(_lit);
	}

	//数据库的操作
	std::list<std::string> sql_list_;
	while (vecSuccessPushMsgId.size() != 0)
	{
		std::stringstream ss;
		ss << "call proc_OrderMsgUpdate(" << vecSuccessPushMsgId.front() << ")";
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
	if (config == NULL){
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
		UserMsgPush();
	}

	return 0;
}
