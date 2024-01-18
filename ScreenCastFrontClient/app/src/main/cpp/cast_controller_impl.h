#ifndef CAST_CONTROLLER_IMPL_H
#define CAST_CONTROLLER_IMPL_H

#include <atomic>
#include <string>
#include <functional>
#include <memory>
#include <unordered_map>
#include <iostream>
#include <thread>
#include <inttypes.h>
#include "libcast_common.h"
#include "cast_controller.h"
#include "istream_socket_client.h"
#define LOG_TAG "CastController"
using namespace std::chrono_literals;

namespace vcast {
    namespace client {

        class CastController::Impl {
        public:
            Impl(std::unique_ptr<IStreamSocketClient> tcp_sock_client, TcpConfigInfo info) 
                : socket_client_{std::move(tcp_sock_client)}, mInfo{std::move(info)} {

                LOGD("CastController::Impl info.tcp_conn_info.ip_addr: %s", mInfo.tcp_conn_info.ip_addr.c_str());
                LOGD("CastController::Impl info.tcp_conn_info.port: %d", mInfo.tcp_conn_info.port);
            }

            ~Impl() {
                    stop();
            }

            bool init() {

                mStatusCode = 0;
                return true;
            }

            IOResult start() {
                std::string error_msg = "";
                int try_times = 0;

                while (try_times < mConnectTimes && mStatusCode != 1 ) {
                    LOGD("try_times %d socket_client_->Connect() ",try_times);
                    auto[connected, error_msg] = socket_client_->Connect();
                    mStatusCode = connected;
                    if (!connected) {
                        LOGE("error: %s Failed to connect to Game Server. Retry after 1s...\n", error_msg.c_str());
                        std::this_thread::sleep_for(6s);
                        try_times++;
                        continue;
                    }
                    else
                        break;
                }
                // mStatusCode = 1;
                LOGD("start is: %s", "successful!");
                return {mStatusCode, error_msg};
            }

            IOResult stop() {
                std::string error_msg = "";
                //mWorkThread->join();
                socket_client_->Close();

                LOGD("stop is: %s", "successful!");
                return {0, error_msg};
            }
            IOResult startBackendApp(){
                LOGI("The connect status:3");
                std::string error_msg = "Disconnected";
                if(!socket_client_->Connected())
                    return {-1, error_msg};
                LOGI("The connect status:4");
                struct dpy_evt_header evt_hdr;
                evt_hdr.e_type = DPY_EVENT_START_CAST;
                evt_hdr.e_magic = DISPLAY_MAGIC_CODE;
                evt_hdr.e_size = 0;

                return socket_client_->Send((uint8_t*)&evt_hdr, sizeof(evt_hdr));

            }
            IOResult stopBackendApp(){
                std::string error_msg = "Disconnected";
                if(!socket_client_->Connected())
                    return {-1, error_msg};

                struct dpy_evt_header evt_hdr;
                evt_hdr.e_type = DPY_EVENT_STOP_CAST;
                evt_hdr.e_magic = DISPLAY_MAGIC_CODE;
                evt_hdr.e_size = 0;

                return socket_client_->Send((uint8_t*)&evt_hdr, sizeof(evt_hdr));
            }


        private:
            std::unique_ptr <IStreamSocketClient> socket_client_;
            struct TcpConfigInfo mInfo;
            int mStatusCode;
            const int mConnectTimes = 3;
        };
    }
}
#endif
