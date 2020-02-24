#include <fixedmath.h>
#include <nuttx/config.h>
#include <stdio.h>

struct mpu_params {
    // 角加速度
	float	acc_pitch;
	float	acc_roll;
	float	acc_yaw;
    // 角速度
	float	gyro_pitch;
	float   gyro_roll;
	float   gyro_yaw;
    // 磁力计
    float   mag_pitch;
    float   mag_roll;
    float   mag_yaw;
    // 角度
    float   angle_pitch; // pitch
    float   angle_roll; // roll
    float   angle_yaw; // yaw
};

struct imu_interface {
    FAR int (*update)(void);
};

struct mpu_params drv_imu_date;
struct imu_interface drv_imu_interface;

void drv_imu_init(void);
void drv_imu_deinit(void);
