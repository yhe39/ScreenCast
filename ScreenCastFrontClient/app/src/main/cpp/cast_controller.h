#ifndef CAST_CONTROLLER_H
#define CAST_CONTROLLER_H
#include <atomic>
#include <string>
#include <functional>
#include <memory>
#include <map>
#include <thread>
#include "libcast_common.h"

namespace vcast {
namespace client {

//enum
//{
//    COMMAND_START_DOCKERD  = 1, // create frame
//    COMMAND_STOP_DOCKERD  = 2, // remove frame
//    COMMAND_SHOW_CONTAINER_LIST = 4, // display frame
//    COMMAND_START_CONTAINER    = 5,
//    COMMAND_STOP_CONTAINER = 6,
//};
enum TcpCommandType
{
    Tcp_FRAME_CREATE  = 0, // create frame
    Tcp_FRAME_REMOVE  = 1, // remove frame
    Tcp_FRAME_DISPLAY = 2, // display frame
    Tcp_ON_CONNECT    = 1001,
    Tcp_ON_DISCONNECT = 1002,
};

struct TcpConfigInfo
{
    TcpConnectionInfo tcp_conn_info;
    int user_id = 0;
};

// using CastControllerHandler = std::function<void(TcpCommandType cmd, const frame_info_t* frame)>;

class CastController
{
public:
    CastController(struct TcpConfigInfo info);
    ~CastController();
    IOResult start();
    IOResult stop();
    IOResult startBackendApp();
    IOResult stopBackendApp();

    // int getGameServerStatusCode();
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}
}
#endif
