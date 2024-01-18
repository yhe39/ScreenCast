/**
 * @file unix_stream_socket_client_impl.h
 * @author Shakthi Prashanth M (shakthi.prashanth.m@intel.com)
 * @brief
 * @version 0.1
 * @date 2021-04-23
 *
 * Copyright (c) 2021 Intel Corporation
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#ifndef UNIX_STREAM_SOCKET_CLIENT_IMPL_H
#define UNIX_STREAM_SOCKET_CLIENT_IMPL_H

#include "tcp_stream_socket_client.h"
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <system_error>

extern "C"
{
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
}
#define LOG_TAG "TcpStreamSocketClient"
namespace vcast {
namespace client {

class TcpStreamSocketClient::Impl
{
public:
    Impl(const std::string& remote_server_ip,const int remote_server_port)
    {
        memset(&remote_, 0, sizeof(remote_));
        remote_.sin_family = AF_INET;
        remote_.sin_port=htons(remote_server_port);
        inet_pton(AF_INET,remote_server_ip.c_str(),&(remote_.sin_addr));
        LOGE( "Remote server ip_addr: %s, remote server port: %d", remote_server_ip.c_str(),remote_server_port);
    }
    ~Impl() { Close(); }

    ConnectionResult Connect()
    {
        std::string error_msg = "";
        if (fd_ >= 0) {
            Close();
        }
        fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
        if (fd_ < 0) {
            LOGE("failed to create socket fd");
        }
        LOGE( "Socket() args: fd: %d, remote server ip_addr: %x, remote server port: %d", fd_, remote_.sin_addr.s_addr,remote_.sin_port);
        connected_ = ::connect(fd_, (struct sockaddr*)&remote_, sizeof(struct sockaddr)) == 0;
        if (!connected_) {
            error_msg = std::strerror(errno);
            LOGE( "Connect() Failed args: fd: %d, remote server ip_addr: %x, "
                                   "remote server port: %d", fd_, remote_.sin_addr.s_addr,remote_.sin_port);
        }
        return { connected_, error_msg };
    }

    bool Connected() const { return connected_; }

    int GetNativeSocketFd() const { return fd_; }

    IOResult Send(const uint8_t* data, size_t size)
    {
        std::string error_msg = "";

        ssize_t sent;
        if ((sent = ::send(fd_, data, size, 0)) == -1) {
            std::cout << ". Send() args: fd: " << fd_ << ", data: " << data
                      << ", size: " << size << "\n";
            error_msg = std::strerror(errno);
        }
        return { sent, error_msg };
    }

    IOResult Recv(uint8_t* data, size_t size)
    {
        std::string error_msg = "";

        ssize_t received;
        if ((received = ::recv(fd_, data, size, 0)) == -1) {
            std::cout << ". Recv() args: fd: " << fd_ << ", data: " << data
                      << ", size: " << size << "\n";
            error_msg = std::strerror(errno);
        }
        return { received, error_msg };
    }

    void Close() {
        connected_ = false;
        if (fd_ < 0) return;
        shutdown(fd_, SHUT_RDWR);
        close(fd_);
        fd_ = -1;
    }

private:
    int  fd_ = -1;
    bool connected_ = false;
    struct sockaddr_in remote_;
};

} // namespace client
} // namespace vcast

#endif /* UNIX_STREAM_SOCKET_CLIENT_IMPL_H */
