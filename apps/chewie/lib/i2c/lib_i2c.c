#include <sys/ioctl.h>
#include <nuttx/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <nuttx/i2c/i2c_master.h>
#include "../lib_i2c.h"
#include "../lib_log.h"

struct lib_i2c_s {
    uint32_t frequency;         /* I2C frequency */
    uint16_t addr;              /* Slave address (7- or 10-bit) */
    int fd;
    int start;
};

/**
 * lib_i2c_open
 * @describe: 打开i2c设备，并且初始化从设备地址，频率
 * @author: hotice0
 * @param: devpath(i2c设备路经)
 *         frequency(i2c通信频率Hz)
 *         addr(从设备地址)
 * @return: 如果执行成功返回lib_i2c_s结构体，否则返回NULL
 */
FAR struct lib_i2c_s * lib_i2c_open(FAR const char *devpath, uint32_t frequency, uint16_t addr)
{
    FAR struct lib_i2c_s *_lib_i2c_s;
    _lib_i2c_s = (FAR struct lib_i2c_s *)malloc(sizeof(_lib_i2c_s));
    if (!_lib_i2c_s) {
        #ifdef CHEWIE_ENABLE_I2C_LOG_ERROR
        CHEWIE_LOG("ERROR: lib_i2c_open: malloc(%d) failed\n", sizeof(_lib_i2c_s));
        #endif
        goto errout;
    }
    _lib_i2c_s->frequency = frequency;
    _lib_i2c_s->addr = (uint8_t)addr;
    _lib_i2c_s->fd = open(devpath, O_RDONLY);
    if (_lib_i2c_s->fd < 0) {
        #ifdef CHEWIE_ENABLE_I2C_LOG_ERROR
        CHEWIE_LOG("ERROR: lib_i2c_open: open(%s, O_RDONLY) with erron %d\n", devpath, errno);
        #endif
        goto errout_with_free;
    }
    return _lib_i2c_s;
errout_with_free:
    free(_lib_i2c_s);
    _lib_i2c_s = NULL;
errout:
    return NULL;
}

/**
 * lib_i2c_read
 * @describe: 读取i2c设备指定地址上的2个字节内容
 * @author: hotice0
 * @param: regaddr(寄存器地址)
 *         result(读取的数据的存储缓冲区)
 * @return: 如果执行成功返回0，否则返回-1
 */
int lib_i2c_read(FAR struct lib_i2c_s *_lib_i2c_s, uint8_t regaddr,
                FAR uint16_t *result)
{
    struct i2c_msg_s msg[2];
    struct i2c_transfer_s xfer;
    int ret;

    /* Set up data structures */

    msg[0].frequency = _lib_i2c_s->frequency;
    msg[0].addr      = _lib_i2c_s->addr;
    msg[0].flags     = I2C_M_NOSTOP;
    msg[0].buffer    = &regaddr;
    msg[0].length    = 1;

    msg[1].frequency = _lib_i2c_s->frequency;
    msg[1].addr      = _lib_i2c_s->addr;
    msg[1].flags     = I2C_M_READ;
    msg[1].buffer = (uint8_t*)&result;
    msg[1].length = 2;

    xfer.msgv = msg;
    xfer.msgc = 2;

    ret = ioctl(_lib_i2c_s->fd, I2CIOC_TRANSFER, (unsigned long)((uintptr_t)&xfer));
    if (ret < 0) {
        #ifdef CHEWIE_ENABLE_I2C_LOG_ERROR
        CHEWIE_LOG("ERROR: lib_i2c_read: ioctl(fd: %d, I2CIOC_TRANSFER, %p) failed with errno: %d\n", _lib_i2c_s->fd, &xfer, errno);
        #endif
        goto errout;
    }

    return 0;
errout:
    return -1;
}

/**
 * lib_i2c_read
 * @describe: 读取i2c设备指定地址上的len个字节内容
 * @author: hotice0
 * @param: regaddr(寄存器地址)
 *         buffer(读取的数据的存储缓冲区)
 *         len(读取的字节个数)
 * @return: 如果执行成功返回0，否则返回-1
 */
int lib_i2c_read_mult_block(FAR struct lib_i2c_s *_lib_i2c_s, uint8_t regaddr,
                FAR uint8_t *buffer, ssize_t len)
{
    struct i2c_msg_s msg[2];
    struct i2c_transfer_s xfer;
    int ret;

    /* Set up data structures */

    msg[0].frequency = _lib_i2c_s->frequency;
    msg[0].addr      = _lib_i2c_s->addr;
    msg[0].flags     = I2C_M_NOSTOP;
    msg[0].buffer    = &regaddr;
    msg[0].length    = 1;

    msg[1].frequency = _lib_i2c_s->frequency;
    msg[1].addr      = _lib_i2c_s->addr;
    msg[1].flags     = I2C_M_READ;
    msg[1].buffer = buffer;
    msg[1].length = len;

    xfer.msgv = msg;
    xfer.msgc = 2;
    #ifdef CHEWIE_ENABLE_I2C_LOG_DEBUG
    CHEWIE_LOG("DEBUG: lib_i2c_read: addr: %02x regaddr: %02x\n", _lib_i2c_s->addr, regaddr);
    #endif
    ret = ioctl(_lib_i2c_s->fd, I2CIOC_TRANSFER, (unsigned long)((uintptr_t)&xfer));
    if (ret < 0) {
        #ifdef CHEWIE_ENABLE_I2C_LOG_ERROR
        CHEWIE_LOG("ERROR: lib_i2c_read: ioctl(fd: %d, I2CIOC_TRANSFER, %p) failed with errno: %d \n", _lib_i2c_s->fd, &xfer, errno);
        #endif
        goto errout;
    }
    
    return 0;
errout:
    return -1;
}

/**
 * lib_i2c_deinit
 * @describe: 反初始化i2c，释放资源，关闭设备
 * @author: hotice0
 * @param: _lib_i2c_s(i2c设备结构体指针)
 * @return: 无返回值
 */
void lib_i2c_deinit(FAR struct lib_i2c_s *_lib_i2c_s)
{
    if (_lib_i2c_s->fd >= 0)
        close(_lib_i2c_s->fd);
    if (_lib_i2c_s)
        free(_lib_i2c_s);
}
