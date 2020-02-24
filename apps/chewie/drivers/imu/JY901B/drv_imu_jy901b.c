#include <fixedmath.h>
#include <nuttx/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <nuttx/pthread.h>
#include "../../../lib/lib_i2c.h"
#include "../../drv_imu.h"

#ifdef CONFIG_CHEWIE_IMU_JY901B

#define SAVE 			0x00
#define CALSW 		0x01
#define RSW 			0x02
#define RRATE			0x03
#define BAUD 			0x04
#define AXOFFSET	0x05
#define AYOFFSET	0x06
#define AZOFFSET	0x07
#define GXOFFSET	0x08
#define GYOFFSET	0x09
#define GZOFFSET	0x0a
#define HXOFFSET	0x0b
#define HYOFFSET	0x0c
#define HZOFFSET	0x0d
#define D0MODE		0x0e
#define D1MODE		0x0f
#define D2MODE		0x10
#define D3MODE		0x11
#define D0PWMH		0x12
#define D1PWMH		0x13
#define D2PWMH		0x14
#define D3PWMH		0x15
#define D0PWMT		0x16
#define D1PWMT		0x17
#define D2PWMT		0x18
#define D3PWMT		0x19
#define IICADDR		0x1a
#define LEDOFF 		0x1b
#define GPSBAUD		0x1c

#define YYMM				0x30
#define DDHH				0x31
#define MMSS				0x32
#define MS					0x33
#define AX					0x34
#define AY					0x35
#define AZ					0x36
#define GX					0x37
#define GY					0x38
#define GZ					0x39
#define HX					0x3a
#define HY					0x3b
#define HZ					0x3c			
#define Roll				0x3d
#define Pitch				0x3e
#define Yaw					0x3f
#define TEMP				0x40
#define D0Status		0x41
#define D1Status		0x42
#define D2Status		0x43
#define D3Status		0x44
#define PressureL		0x45
#define PressureH		0x46
#define HeightL			0x47
#define HeightH			0x48
#define LonL				0x49
#define LonH				0x4a
#define LatL				0x4b
#define LatH				0x4c
#define GPSHeight   0x4d
#define GPSYAW      0x4e
#define GPSVL				0x4f
#define GPSVH				0x50
      
#define DIO_MODE_AIN 0
#define DIO_MODE_DIN 1
#define DIO_MODE_DOH 2
#define DIO_MODE_DOL 3
#define DIO_MODE_DOPWM 4
#define DIO_MODE_GPS 5	

struct g_imu_jy901b {
    struct lib_i2c_s *_lib_i2c_s;
};

struct g_imu_jy901b _g_imu_jy901b_info;

int _init(void)
{
    _g_imu_jy901b_info._lib_i2c_s = lib_i2c_open(CONFIG_CHEWIE_IMU_JY901B_I2C_DEV_PATH, 
                 CONFIG_CHEWIE_IMU_JY901B_I2C_FREQUENCY,
                 0x50);
    if (!_g_imu_jy901b_info._lib_i2c_s) {
        printf("lib_i2c_open(%s, %d, %02x) failed\n", 
                        CONFIG_CHEWIE_IMU_JY901B_I2C_DEV_PATH, 
                        (int)CONFIG_CHEWIE_IMU_JY901B_I2C_FREQUENCY,
                        CONFIG_CHEWIE_IMU_JY901B_I2C_SLAVE_ADDRESS);
        goto errout;
    }
    return 0;
errout:
    lib_i2c_deinit(_g_imu_jy901b_info._lib_i2c_s);
    _g_imu_jy901b_info._lib_i2c_s = NULL;
    return -1;
}

#define REVERT_REG_DATA(x)  (  (short) ( ((unsigned char *)(x))[1]<<8 | ((unsigned char *)(x))[0] )  )

int _update(void)
{
    unsigned char chrTemp[30];
    int ret = 0;

    ret = lib_i2c_read_mult_block(_g_imu_jy901b_info._lib_i2c_s, AX, &chrTemp[0], 24);
    if (ret < 0) {
        goto errout;
    }

    drv_imu_date.acc_pitch = REVERT_REG_DATA(&chrTemp[0])/32768.0*16;
    drv_imu_date.acc_roll = REVERT_REG_DATA(&chrTemp[2])/32768.0*16;
    drv_imu_date.acc_yaw = -1 * REVERT_REG_DATA(&chrTemp[4])/32768.0*16;

    drv_imu_date.gyro_pitch = REVERT_REG_DATA(&chrTemp[6])/32768.0*2000;
    drv_imu_date.gyro_roll = REVERT_REG_DATA(&chrTemp[8])/32768.0*2000;
    drv_imu_date.gyro_yaw = -1 * REVERT_REG_DATA(&chrTemp[10])/32768.0*2000;

    drv_imu_date.mag_pitch = REVERT_REG_DATA(&chrTemp[12]);
    drv_imu_date.mag_roll = REVERT_REG_DATA(&chrTemp[14]);
    drv_imu_date.mag_yaw = -1 * REVERT_REG_DATA(&chrTemp[16]);

    drv_imu_date.angle_pitch = REVERT_REG_DATA(&chrTemp[18])/32768.0*180;
    drv_imu_date.angle_roll = REVERT_REG_DATA(&chrTemp[20])/32768.0*180;
    drv_imu_date.angle_yaw = -1 * REVERT_REG_DATA(&chrTemp[22])/32768.0*180;

    #ifdef CHEWIE_ENABLE_IMU_LOG_DEBUG
    CHEWIE_LOG("acc (%d, %d, %d)\n", 
                (int)(drv_imu_date.acc_pitch*1000),
                (int)(drv_imu_date.acc_roll*1000),
                (int)(drv_imu_date.acc_yaw*1000)
                );
    CHEWIE_LOG("gyro (%d, %d, %d)\n", 
                (int)(drv_imu_date.gyro_pitch*1000),
                (int)(drv_imu_date.gyro_roll*1000),
                (int)(drv_imu_date.gyro_yaw*1000)
                );

    CHEWIE_LOG("mag (%d, %d, %d)\n", 
                (int)drv_imu_date.mag_pitch,
                (int)drv_imu_date.mag_roll,
                (int)drv_imu_date.mag_yaw
                );
    CHEWIE_LOG("angle (%d, %d, %d)\n", 
                (int)(drv_imu_date.angle_pitch*1000),
                (int)(drv_imu_date.angle_roll*1000),
                (int)(drv_imu_date.angle_yaw*1000)
                );
    #endif
    return 0;
errout:
    return -1;
}

void drv_imu_init(void)
{
    drv_imu_interface.update = _update;

    _init();
}

void drv_imu_deinit(void)
{
    lib_i2c_deinit(_g_imu_jy901b_info._lib_i2c_s);
}

#endif