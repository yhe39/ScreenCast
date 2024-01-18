#include "cast_controller.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <stdio.h>

#include "tcp_stream_socket_client.h"
#include "cast_controller_impl.h"

namespace vcast {
    namespace client {

        CastController::CastController(struct TcpConfigInfo info) {
            auto ip_addr   = info.tcp_conn_info.ip_addr;
            auto port = info.tcp_conn_info.port;

            LOGD("======CastController info.tcp_conn_info.ip_addr: %s", info.tcp_conn_info.ip_addr.c_str());
            LOGD("======CastController info.tcp_conn_info.port: %d", info.tcp_conn_info.port);
            
            if (ip_addr.length() == 0) {
                LOGE("Please set a valid game server ip_addr");
            }
            //Creating interface to communicate to VHAL via libvhal
            auto tcp_sock_client =
                    std::make_unique<TcpStreamSocketClient>(ip_addr,port);

            impl_ = std::make_unique<Impl>(std::move(tcp_sock_client),
                                           std::move(info));
            if (!impl_->init()) {
                LOGE("failed to create docker controller");
            }
        }

        CastController::~CastController() { impl_->stop(); }

        IOResult CastController::start() {
            return impl_->start();
        }

        IOResult CastController::stop() {
            return impl_->stop();
        }

        IOResult CastController::startBackendApp(){
            LOGI("The connect status:2");
            return impl_->startBackendApp();
        }
        IOResult CastController::stopBackendApp(){
            return impl_->stopBackendApp();
        }
    }
}
