/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <sys/types.h>
#include <sys/ioctl.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <debug.h>
#include <string.h>

#include "../../lib/lib_log.h"

#include "drv_pwm_out.h"

/**
 * drv_pwm_init
 * @describe: 初始化pwm设备，即打开设备获取设备fd号，用于后续操作。
 * @author: hotice0
 * @param: devpath(pwm设备路经)
 * @return: 如果执行成功返回设备fd号，否则返回-1
 */
int drv_pwm_init(const char *devpath)
{
    int fd = 0;
    #ifdef CONFIG_CHEWIE_ENABLE_PWM_LOG_DEBUG
    CHEWIE_LOG("DEBUG:: drv_pwm_init(devpath: %s)\n", devpath);
    #endif
    if (!devpath) {
        #ifdef CONFIG_CHEWIE_ENABLE_PWM_LOG_ERROR
        CHEWIE_LOG("ERROR:: drv_pwm_init(devpath: NULL) failed\n");
        #endif
        goto errout;
    }
    fd = open(devpath, O_RDONLY);
    if (fd < 0) {
        #ifdef CONFIG_CHEWIE_ENABLE_PWM_LOG_ERROR
        CHEWIE_LOG("ERROR:: drv_pwm_init: open(devpath: %s, flag: O_RDONLY) pwm dev failed with errno : %d\n", devpath, errno);
        #endif
        goto errout;
    }
    return fd;
errout:
    fflush(stdout);
    return -1;
}

/**
 * drv_pwm_start
 * @describe: 启动对应pwm设备的pwm信号输出
 * @author: hotice0
 * @param: fd(pwm设备初始化后得到的fd号)
 * @return: 如果执行成功返回0，否则返回-1
 */
int drv_pwm_start(int fd)
{
    int ret = 0;
    #ifdef CONFIG_CHEWIE_ENABLE_PWM_LOG_DEBUG
    CHEWIE_LOG("DEBUG:: drv_pwm_start(fd: %d)\n", fd);
    #endif
    // start the pwm device
    ret = ioctl(fd, PWMIOC_START, 0);
    if (ret < 0) {
        #ifdef CONFIG_CHEWIE_ENABLE_PWM_LOG_ERROR
        CHEWIE_LOG("ERROR:: drv_pwm_start: ioctl(PWMIOC_START) failed: %d\n", errno);
        #endif
        goto errout;
    }
    return 0;
errout:
    fflush(stdout);
    return -1;
}

/**
 * drv_pwm_stop
 * @describe: 停止对应pwm设备的pwm信号输出
 * @author: hotice0
 * @param: fd(pwm设备初始化后得到的fd号)
 * @return: 如果执行成功返回0，否则返回-1
 */
int drv_pwm_stop(int fd)
{
    int ret = 0;
    #ifdef CONFIG_CHEWIE_ENABLE_PWM_LOG_DEBUG
    CHEWIE_LOG("DEBUG:: drv_pwm_stop(fd: %d)\n", fd);
    #endif
    
    ret = ioctl(fd, PWMIOC_STOP, 0);
    if (ret < 0)
    {
        #ifdef CONFIG_CHEWIE_ENABLE_PWM_LOG_ERROR
        CHEWIE_LOG("ERROR:: drv_pwm_stop: ioctl(PWMIOC_STOP) failed: %d\n", errno);
        #endif
        goto errout;
    }
    return 0;
errout:
    fflush(stdout);
    return -1;
}

/**
 * drv_pwm_update_info
 * @describe: 更新对应pwm设备的pwm相关配置
 * @author: hotice0
 * @param: fd(pwm设备初始化后得到的fd号)
 *         p_init_info_s(pwm设备配置信息结构体)
 * @return: 如果执行成功返回0，否则返回-1
 */
int drv_pwm_update_info(int fd, const struct pwm_info_s *p_init_info_s)
{
    int ret = 0;
    #ifdef CONFIG_CHEWIE_ENABLE_PWM_LOG_DEBUG
    CHEWIE_LOG("DEBUG:: drv_pwm_update_info(fd: %d, p_init_info_s: ...)\n", fd);
    #endif
    // send control info to pwm device
    ret = ioctl(fd, PWMIOC_SETCHARACTERISTICS, (unsigned long)((uintptr_t)p_init_info_s));
    if (ret < 0) {
        #ifdef CONFIG_CHEWIE_ENABLE_PWM_LOG_ERROR
        CHEWIE_LOG("ERROR:: drv_pwm_init: ioctl(fd: %d) failed with code %d\n", fd, errno);
        #endif
        goto errout;
    }
    return 0;
errout:
    fflush(stdout);
    return -1;
}

/**
 * drv_pwm_deinit
 * @describe: 关闭pwm设备(并不会关闭pwm输出)
 * @author: hotice0
 * @param: fd(pwm设备初始化后得到的fd号)
 * @return: 如果执行成功返回0，否则返回-1
 */
void drv_pwm_deinit(int fd)
{
    #ifdef CONFIG_CHEWIE_ENABLE_PWM_LOG_DEBUG
    CHEWIE_LOG("DEBUG:: drv_pwm_deinit(fd: %d)\n", fd);
    #endif
    close(fd);
}
