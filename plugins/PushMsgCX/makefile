
#${LIB_DIR}/third_party/gtpush/lib/libgtpushsdk_gcc4.1.2_64.a
OUT_PUT = PushMsgCX.out

SRC_DIR = ../../
LIB_DIR = ./../../../mylib
SRC_PLUGINS = ${SRC_DIR}plugins
SO_DIR = ${SRC_DIR}bin/plugins
SRC_PUB_NET = ${SRC_DIR}pub/net
SRC_PUB_UTIL = ${SRC_DIR}pub/util
SRC_PUB_STORAGE = ${SRC_DIR}pub/storage
SRC_PUB_SHARE = ${SRC_DIR}pub/share
SRC_PUB_COMM = ${SRC_DIR}pub/comm

LINK_DYNAMIC = -Wl,-Bdynamic -lpthread
LINK_STATIC = -Wl,-Bstatic -L${LIB_DIR}/third_party/glog/lib/libglog.a
LINK_PATH = -L/usr/lib64/mysql

CXXFLAGS += -g -Wextra

INCLUDE_PATHS = -I${LIB_DIR} \
		-I${SRC_PLUGINS} \
		-I${SRC_DIR} \
		-I${LIB_DIR}/public \
		-I${LIB_DIR}/public/basic \
		-I${LIB_DIR}/../core \
		-I${LIB_DIR}/base \
		-I${LIB_DIR}/third_party/glog/include \
		-I${LIB_DIR}/third_party \
		-I${LIB_DIR}/third_party/gtpush/include\
		-I/usr/include/mysql

all : ${OUT_PUT} del-temp-files

del-temp-files:
	rm -rf *.o
	
clean :
	rm -f ${OUT_PUT}  del-temp-files
	
	
main.o : ./main.cc
	${CXX} ${CXXFLAGS} -c ./main.cc ${INCLUDE_PATHS}

util.o : ${SRC_PUB_UTIL}/util.cc  ${SRC_PUB_UTIL}/util.h 
	${CXX} ${CXXFLAGS} -c ${SRC_PUB_UTIL}/util.cc ${INCLUDE_PATHS}

data_engine.o : ${SRC_PUB_STORAGE}/data_engine.cc  ${SRC_PUB_STORAGE}/data_engine.h 
	${CXX} ${CXXFLAGS} -c ${SRC_PUB_STORAGE}/data_engine.cc ${INCLUDE_PATHS}

mysql_pool.o : ${SRC_PUB_STORAGE}/mysql_pool.cc  ${SRC_PUB_STORAGE}/mysql_pool.h 
	${CXX} ${CXXFLAGS} -c ${SRC_PUB_STORAGE}/mysql_pool.cc ${INCLUDE_PATHS}

data_mysql_engine.o : ${SRC_PUB_STORAGE}/data_mysql_engine.cc  ${SRC_PUB_STORAGE}/data_mysql_engine.h 
	${CXX} ${CXXFLAGS} -c ${SRC_PUB_STORAGE}/data_mysql_engine.cc ${INCLUDE_PATHS}

share_guide.o : ${SRC_PUB_COMM}/share_guide.cc  ${SRC_PUB_COMM}/share_guide.h 
	${CXX} ${CXXFLAGS} -c ${SRC_PUB_COMM}/share_guide.cc ${INCLUDE_PATHS}

data_share_mgr.o : ${SRC_PUB_SHARE}/data_share_mgr.cc  ${SRC_PUB_SHARE}/data_share_mgr.h 
	${CXX} ${CXXFLAGS} -c ${SRC_PUB_SHARE}/data_share_mgr.cc ${INCLUDE_PATHS}
	

${OUT_PUT} : main.o data_engine.o mysql_pool.o data_mysql_engine.o share_guide.o data_share_mgr.o util.o
	${CXX} ${CXXFLAGS} $^ -o ${OUT_PUT} ${LINK_PATH} -lpthread -lmysqlclient -lcurl\
		${LIB_DIR}/third_party/gtpush/lib/libgtpushsdk_gcc4.1.2_64.a \
		${LIB_DIR}/base/lib/mybase.a -lexpat -lmemcached -lmemcachedutil\
		${LIB_DIR}/third_party/glog/lib/libglog.a \
		${LIB_DIR}/third_party/dmg_fp/lib/dmg_fp.a \
		${LIB_DIR}/third_party/lib/libhiredis.a\
		/usr/lib64/libexpat.a\
		/usr/local/libmemcached/lib/libmemcached.a\
		/usr/local/libmemcached/lib/libmemcachedutil.a