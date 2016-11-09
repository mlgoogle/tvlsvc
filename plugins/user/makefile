

OUT_PUT = ${SO_DIR}/user/user.so 

SRC_DIR = ../../
LIB_DIR = ./../../../baselib
SRC_PLUGINS = ${SRC_DIR}plugins
SO_DIR = ${SRC_DIR}bin/plugins
SRC_PUB_NET = ${SRC_DIR}pub/net
SRC_PUB_UTIL = ${SRC_DIR}pub/util
SRC_PUB_STORAGE = ${SRC_DIR}pub/storage
SRC_PUB_SHARE = ${SRC_DIR}pub/share
SRC_PUB_COMM = ${SRC_DIR}pub/comm
SRC_PUB_PAY = ${SRC_DIR}pub/pay

LINK_DYNAMIC = -Wl,-Bdynamic -lpthread
LINK_STATIC = -Wl,-Bstatic -L${LIB_DIR}/third_party/glog/lib/libglog.a
LINK_PATH = -L/usr/lib64/mysql

CXXFLAGS += -g -O0 -Wextra -w -shared -fPIC 

INCLUDE_PATHS = -I${LIB_DIR} \
		-I${SRC_PLUGINS} \
		-I${SRC_DIR} \
		-I${LIB_DIR}/public \
		-I${LIB_DIR}/base \
		-I${LIB_DIR}/third_party/glog/include \
		-I${LIB_DIR}/third_party/gtpush/include

all : ${OUT_PUT} del-temp-files

del-temp-files:
	rm -rf *.o
	
clean :
	rm -f ${OUT_PUT}  del-temp-files
	
user_init.o : ./user_init.cc  ./user_init.h
	${CXX} ${CXXFLAGS} -c ./user_init.cc ${INCLUDE_PATHS}

user_logic.o : ./user_logic.cc  ./user_logic.h
	${CXX} ${CXXFLAGS} -c ./user_logic.cc ${INCLUDE_PATHS}

user_manager.o : ./user_manager.cc  ./user_manager.h
	${CXX} ${CXXFLAGS} -c ./user_manager.cc ${INCLUDE_PATHS}
	
user_interface.o : ./user_interface.cc  ./user_interface.h
	${CXX} ${CXXFLAGS} -c ./user_interface.cc ${INCLUDE_PATHS}
	
user_proto.o : ./user_proto.cc  ./user_proto.h
	${CXX} ${CXXFLAGS} -c ./user_proto.cc ${INCLUDE_PATHS}

user_mysql.o : ./user_mysql.cc  ./user_mysql.h
	${CXX} ${CXXFLAGS} -c ./user_mysql.cc ${INCLUDE_PATHS}

proto_buf.o : ${SRC_PUB_NET}/proto_buf.cc  ${SRC_PUB_NET}/proto_buf.h 
	${CXX} ${CXXFLAGS} -c ${SRC_PUB_NET}/proto_buf.cc ${INCLUDE_PATHS}

util.o : ${SRC_PUB_UTIL}/util.cc  ${SRC_PUB_UTIL}/util.h 
	${CXX} ${CXXFLAGS} -c ${SRC_PUB_UTIL}/util.cc ${INCLUDE_PATHS}

data_engine.o : ${SRC_PUB_STORAGE}/data_engine.cc  ${SRC_PUB_STORAGE}/data_engine.h 
	${CXX} ${CXXFLAGS} -c ${SRC_PUB_STORAGE}/data_engine.cc ${INCLUDE_PATHS}

mysql_pool.o : ${SRC_PUB_STORAGE}/mysql_pool.cc  ${SRC_PUB_STORAGE}/mysql_pool.h 
	${CXX} ${CXXFLAGS} -c ${SRC_PUB_STORAGE}/mysql_pool.cc ${INCLUDE_PATHS}

data_mysql_engine.o : ${SRC_PUB_STORAGE}/data_mysql_engine.cc  ${SRC_PUB_STORAGE}/data_mysql_engine.h 
	${CXX} ${CXXFLAGS} -c ${SRC_PUB_STORAGE}/data_mysql_engine.cc ${INCLUDE_PATHS}

user_info.o : ${SRC_PUB_COMM}/user_info.cc  ${SRC_PUB_COMM}/user_info.h 
	${CXX} ${CXXFLAGS} -c ${SRC_PUB_COMM}/user_info.cc ${INCLUDE_PATHS}

share_guide.o : ${SRC_PUB_COMM}/share_guide.cc  ${SRC_PUB_COMM}/share_guide.h 
	${CXX} ${CXXFLAGS} -c ${SRC_PUB_COMM}/share_guide.cc ${INCLUDE_PATHS}

data_share_mgr.o : ${SRC_PUB_SHARE}/data_share_mgr.cc  ${SRC_PUB_SHARE}/data_share_mgr.h 
	${CXX} ${CXXFLAGS} -c ${SRC_PUB_SHARE}/data_share_mgr.cc ${INCLUDE_PATHS}

wx_order.o : ${SRC_PUB_PAY}/wxpay/wx_order.cc  ${SRC_PUB_PAY}/wxpay/wx_order.h 
	${CXX} ${CXXFLAGS} -c ${SRC_PUB_PAY}/wxpay/wx_order.cc ${INCLUDE_PATHS}

${OUT_PUT} : user_init.o user_logic.o proto_buf.o user_manager.o \
		user_interface.o user_proto.o user_mysql.o util.o data_engine.o \
		mysql_pool.o data_mysql_engine.o user_info.o share_guide.o data_share_mgr.o wx_order.o 
	${CXX} ${CXXFLAGS} $^ -o ${OUT_PUT} ${LINK_PATH} -lpthread -lmysqlclient \
		-lcurl ${LIB_DIR}/third_party/glog/lib/libglog.a \
		${LIB_DIR}/third_party/gtpush/lib/libgtpushsdk_gcc4.1.2_64.a \
		${LIB_DIR}/base/lib/mybase.a \
		${LIB_DIR}/third_party/dmg_fp/lib/dmg_fp.a
