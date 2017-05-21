# add_subdirectory(base)
# add_subdirectory(net)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	)

aux_source_directory(base BASE_SRCS)
aux_source_directory(net NET_SRCS)

add_library (fly STATIC ${NET_SRCS} ${BASE_SRCS})

install(TARGETS fly DESTINATION lib)

set(BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/base)
set(NET_DIR ${CMAKE_CURRENT_SOURCE_DIR}/net)

set(HEADERS 
	${BASE_DIR}/AsyncLogger.h
	${BASE_DIR}/Atomic.h
	${BASE_DIR}/BlockQueue.h
	${BASE_DIR}/BoundBlockQueue.h
	${BASE_DIR}/Buffer.h
	${BASE_DIR}/Condition.h
	${BASE_DIR}/CurrentThread.h
	${BASE_DIR}/Date.h
	${BASE_DIR}/Endian.h
	${BASE_DIR}/fly_base.h
	${BASE_DIR}/Logger.h
	${BASE_DIR}/LogStream.h
	${BASE_DIR}/Mutex.h
	${BASE_DIR}/StringView.h
	${BASE_DIR}/Thread.h
	${BASE_DIR}/Time.h
	${BASE_DIR}/Types.h

	${NET_DIR}/Accepter.h
	${NET_DIR}/Channel.h
	${NET_DIR}/Connecter.h
	${NET_DIR}/EventLoop.h
	${NET_DIR}/fly_net.h
	${NET_DIR}/help.h
	${NET_DIR}/NetAddr.h
	${NET_DIR}/Poller.h
	${NET_DIR}/SocketOps.h
	${NET_DIR}/TcpConnection.h
	${NET_DIR}/Timer.h
	${NET_DIR}/TimerQueue.h
	${NET_DIR}/Accepter.h
	${NET_DIR}/Channel.h
	${NET_DIR}/Connecter.h
	${NET_DIR}/EventLoop.h
	${NET_DIR}/fly_net.h
	${NET_DIR}/help.h
	${NET_DIR}/NetAddr.h
	${NET_DIR}/Poller.h
	${NET_DIR}/SocketOps.h
	${NET_DIR}/TcpConnection.h
	${NET_DIR}/Timer.h
	${NET_DIR}/TimerQueue.h
	)

install(FILES ${HEADERS} DESTINATION include/fly/net)