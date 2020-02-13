#include <stdio.h>
#include "../pwm_out/drv_pwm_out.h"
#include "../../lib/lib_log.h"

#include "../drv_motor.h"

#define CHEWIE_NUM_OF_MOTOR_PWM_DEV ((int)(CONFIG_CHEWIE_NUM_OF_CHANNELS / 4) + (CONFIG_CHEWIE_NUM_OF_CHANNELS % 4 == 0 ? 0 : 1))

struct _motor_state_s
{
  struct pwm_info_s pwms_info[CHEWIE_NUM_OF_MOTOR_PWM_DEV];
  int pwm_devs_fd[CHEWIE_NUM_OF_MOTOR_PWM_DEV];
  uint32_t freq;
};

struct _motor_state_s _g_motors_state;

/**
 * drv_motor_init
 * @describe: 初始化motor设备
 * @author: hotice0
 * @param: 
 * @return: 如果执行成功返回0，否则返回-1
 */
int drv_motor_init(void)
{
    uint8_t i = 0;
    uint8_t j = 0;
    int ret = 0;
    char devpath[50] = {0};
    #ifdef CONFIG_CHEWIE_ENABLE_MOTOR_LOG_DEBUG
    CHEWIE_LOG("DEBUG:: drv_motor_init(void)\n");
    #endif
    // init _g_motors_state
    _g_motors_state.freq = CONFIG_CHEWIE_MOTOR_PWM_FREQUENCY;
    for (i = 0; i < CHEWIE_NUM_OF_MOTOR_PWM_DEV; ++i) {
        _g_motors_state.pwm_devs_fd[i] = -1;
        _g_motors_state.pwms_info[i].frequency = CONFIG_CHEWIE_MOTOR_PWM_FREQUENCY;
        for (j = 0; j < CONFIG_PWM_NCHANNELS && i * CONFIG_PWM_NCHANNELS + j < CONFIG_CHEWIE_NUM_OF_CHANNELS; ++j) {
            _g_motors_state.pwms_info[i].channels[j].channel = j + 1;
            _g_motors_state.pwms_info[i].channels[j].duty = CHEWIE_DRV_MOTOR_ARMED_THROTTLE_DUTY;
        }
    }

    // init g_motor_state
    for (i = 0; i < CONFIG_CHEWIE_NUM_OF_CHANNELS; ++i)
        g_motor_state.dutys[i] = CHEWIE_DRV_MOTOR_ARMED_THROTTLE_DUTY;

    // init motor pwm dev
    for (i = 0; i < CHEWIE_NUM_OF_MOTOR_PWM_DEV; ++i) {
        sprintf(devpath, "%s%d", CONFIG_CHEWIE_MOTOR_PWM_OUT_DEVICE_PATH_PREFIX, i);
        #ifdef CONFIG_CHEWIE_ENABLE_MOTOR_LOG_DEBUG
        CHEWIE_LOG("DEBUG:: dev %s init\n", devpath);
        #endif

        // init
        _g_motors_state.pwm_devs_fd[i] = drv_pwm_init(devpath);
        if (_g_motors_state.pwm_devs_fd[i] < 0) {
            #ifdef CONFIG_CHEWIE_ENABLE_MOTOR_LOG_ERROR
            CHEWIE_LOG("ERROR:: drv_motor_init: drv_pwm_init(devpath: %s) failed\n", devpath);
            #endif
            goto errout_with_dev;
        }
        // update duty
        ret = drv_pwm_update_info(_g_motors_state.pwm_devs_fd[i], &(_g_motors_state.pwms_info[i]));
        if (ret < 0) {
            #ifdef CONFIG_CHEWIE_ENABLE_MOTOR_LOG_ERROR
            CHEWIE_LOG("ERROR:: drv_motor_init: drv_pwm_update_info(fd: %d, ...) failed\n", _g_motors_state.pwm_devs_fd[i]);
            #endif
            goto errout_with_dev;
        }
        // start pwm out
        ret = drv_pwm_start(_g_motors_state.pwm_devs_fd[i]);
        if (ret < 0) {
            #ifdef CONFIG_CHEWIE_ENABLE_MOTOR_LOG_ERROR
            CHEWIE_LOG("ERROR: drv_motor_init: drv_pwm_start(fd: %d) failed\n", _g_motors_state.pwm_devs_fd[i]);
            #endif
            goto errout_with_dev;
        }
        #ifdef CONFIG_CHEWIE_ENABLE_MOTOR_LOG_INFO
        CHEWIE_LOG("INFO:: >> success init and start motor pwm device. devpath: %s ,fd: %d \n", devpath, _g_motors_state.pwm_devs_fd[i]);
        #endif
    }
    return 0;
errout_with_dev:
    for (i = 0; i < CHEWIE_NUM_OF_MOTOR_PWM_DEV; ++i)
    {
        if (_g_motors_state.pwm_devs_fd[i] >= 0)
            close(_g_motors_state.pwm_devs_fd[i]);
    }
    fflush(stdout);
    return -1;
}

/**
 * drv_motor_esc_init
 * @describe: 进行电调行程校准
 * @author: hotice0
 * @param: 
 * @return: 如果执行成功返回0，否则返回-1
 */
int drv_motor_esc_init(void)
{
    uint8_t i = 0;
    int ret = 0;

    // 设置油门信号到最大值
    for(i = 0; i < CONFIG_CHEWIE_NUM_OF_CHANNELS; ++i)
        g_motor_state.dutys[i] = CHEWIE_DRV_MOTOR_MAX_THROTTLE_DUTY;
    ret = drv_motor_update();
    if (ret < 0) {
        #ifdef CONFIG_CHEWIE_ENABLE_MOTOR_LOG_ERROR
        CHEWIE_LOG("ERROR:: drv_motor_esc_init: drv_motor_update(void) failed\n");
        #endif
        goto errout;
    }
    sleep(3);
    // 设置油门信号到最小值
    for(i = 0; i < CONFIG_CHEWIE_NUM_OF_CHANNELS; ++i)
        g_motor_state.dutys[i] = CHEWIE_DRV_MOTOR_MIN_THROTTLE_DUTY;
    ret = drv_motor_update();
    if (ret < 0) {
        #ifdef CONFIG_CHEWIE_ENABLE_MOTOR_LOG_ERROR
        CHEWIE_LOG("ERROR:: drv_motor_esc_init: drv_motor_update(void) failed\n");
        #endif
        goto errout;
    }
    sleep(3);
    return 0;
errout:
    return -1;
}

/**
 * drv_motor_update
 * @describe: 将存储于g_motor_state的duty配置，更新到motor设备
 * @author: hotice0
 * @param: 
 * @return: 如果执行成功返回0，否则返回-1
 */
int drv_motor_update(void)
{
    static uint8_t i = 0;
    static uint8_t j = 0;
    static uint8_t k = 0;
    #ifdef CONFIG_CHEWIE_ENABLE_MOTOR_LOG_ERROR
    int ret = 0;
    #endif
    // update dutys into _g_motors_state
    for (i = 0, k = 0, j = 0; i < CONFIG_CHEWIE_NUM_OF_CHANNELS; ++i, ++k) {
        _g_motors_state.pwms_info[j].channels[k].duty = g_motor_state.dutys[i];
        // limit range of duty
        if (g_motor_state.dutys[i] > CHEWIE_DRV_MOTOR_MAX_THROTTLE_DUTY)
            _g_motors_state.pwms_info[j].channels[k].duty = CHEWIE_DRV_MOTOR_MAX_THROTTLE_DUTY;
        if (g_motor_state.dutys[i] < CHEWIE_DRV_MOTOR_ARMED_THROTTLE_DUTY)
            _g_motors_state.pwms_info[j].channels[k].duty = CHEWIE_DRV_MOTOR_ARMED_THROTTLE_DUTY;
        if (k >= CONFIG_PWM_NCHANNELS - 1) {
            k = -1;
            #ifdef CONFIG_CHEWIE_ENABLE_MOTOR_LOG_ERROR
            ret = drv_pwm_update_info(_g_motors_state.pwm_devs_fd[j], &(_g_motors_state.pwms_info[j]));
            if (ret < 0) {
                CHEWIE_LOG("ERROR:: drv_motor_update: drv_pwm_update_info(fd: %d, p_init_info_s: ...) failed\n", _g_motors_state.pwm_devs_fd[j]);
            }
            #else
            drv_pwm_update_info(_g_motors_state.pwm_devs_fd[j], &(_g_motors_state.pwms_info[j]));
            #endif
            ++j;
        }
    }
    return 0;
}

/**
 * drv_motor_deinit
 * @describe: 停止motor设备的pwm输出，并关闭设备fd
 * @author: hotice0
 * @param: 
 * @return: void
 */
void drv_motor_deinit(void)
{
    int i = 0;
    #ifdef CONFIG_CHEWIE_ENABLE_MOTOR_LOG_ERROR
    int ret = 0;
    #endif
    for (i = 0; i < CHEWIE_NUM_OF_MOTOR_PWM_DEV; ++i) {
        #ifdef CONFIG_CHEWIE_ENABLE_MOTOR_LOG_ERROR
        ret = drv_pwm_stop(_g_motors_state.pwm_devs_fd[i]);
        if (ret < 0) {
            CHEWIE_LOG("ERROR:: drv_motor_deinit: drv_pwm_stop(fd: %d) failed\n", _g_motors_state.pwm_devs_fd[i]);
        }
        #else
        drv_pwm_stop(_g_motors_state.pwm_devs_fd[i]);
        #endif

        drv_pwm_deinit(_g_motors_state.pwm_devs_fd[i]);
        _g_motors_state.pwm_devs_fd[i] = -1;
    }
}
