#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <android/log.h>

#ifndef LOGE
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "main", __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "main", __VA_ARGS__))
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "main", __VA_ARGS__))
#endif

static void write_str_to_file(const char* fn, const char* str) {
    int fd = open(fn, O_WRONLY);
    if (fd == -1) {
        LOGE("opening device file");
	return;
    }

    ssize_t bytes_written = write(fd, str, strlen(str));
    if (bytes_written == -1) {
        LOGE("writing to device file");
        close(fd);
        return;
    }

    if (close(fd) == -1) {
        LOGE("closing device file");
        return;
    }
}

int main() {
    if(setuid(0)){
            LOGI("set uid fail, %s\n", strerror(errno));
    }
    write_str_to_file("/sys/bus/pci/drivers/virtio-ivshmem/new_id", "1af4 1110");
    return 0;
}
