#include "../drv_motor.h"
#include "../pwm_out/drv_pwm_out.h"

#define CHEWIE_NUM_OF_MOTOR_PWM_DEV (int)((CONFIG_CHEWIE_NUM_OF_CHANNELS - 1) / 4)
/**
 * The path of the pwm out device is named like,
 * CHEWIE_PWM_OUT_DEFAULT_DEVICE_PATH_PREFIX + "0" | + "1" ...
 * /dev/pwm_out_motor_0
 * /dev/pwm_out_motor_1
 */
#define CHEWIE_MOTOR_DEFAULT_DEVICE_PATH_PREFIX "/dev/pwm_out_motor_"

struct motor_state_s
{
  struct pwm_info_s pwms_info[CHEWIE_NUM_OF_MOTOR_PWM_DEV];
  int pwm_devs_fd[CHEWIE_NUM_OF_MOTOR_PWM_DEV];
  uint32_t freq;
};

struct motor_state_s _g_motors_state;

int drv_motor_init()
{
    uint8_t i = 0;
    uint8_t j = 0;
    int ret = 0;
    char devpath[50] = {0};
    // init _g_motors_state
    _g_motors_state.freq = CONFIG_CHEWIE_PWM_FREQUENCY;
    for (i = 0; i < CHEWIE_NUM_OF_MOTOR_PWM_DEV; ++i) {
        _g_motors_state.pwms_info[i].frequency = CONFIG_CHEWIE_PWM_FREQUENCY;
        _g_motors_state.pwm_devs_fd[i] = -1;
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
        printf("dev %s init\n", devpath);

        // init
        _g_motors_state.pwm_devs_fd[i] = drv_pwm_init(devpath);
        if (_g_motors_state.pwm_devs_fd[i] < 0) {
            goto errout_with_dev;
        }
        // update duty
        ret = drv_pwm_update_info(_g_motors_state.pwm_devs_fd[i], &_g_motors_state.pwms_info[i]);
        if (ret < 0) {
            printf("drv_motor_init: drv_pwm_update_info(fd: %d, ...) failed\n", _g_motors_state.pwm_devs_fd[i]);
            goto errout_with_dev;
        }
        // start pwm out
        drv_pwm_start(_g_motors_state.pwm_devs_fd[i]);
        if (ret < 0) {
            printf("drv_motor_init: drv_pwm_start(fd: %d) failed\n", _g_motors_state.pwm_devs_fd[i]);
            goto errout_with_dev;
        }
        printf(">> success init and start pwm device : %s\n", devpath);
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

int drv_motor_esc_init()
{
    // TODO
    return -1;
}

int drv_motor_update()
{
    static uint8_t i = 0;
    static uint8_t j = 0;
    static uint8_t k = 0;
    // update dutys into _g_motors_state
    for (i = 0, k = 0, j = 0; i < CONFIG_CHEWIE_NUM_OF_CHANNELS; ++i, ++k) {
        if (k >= CONFIG_PWM_NCHANNELS) {
            k = 0;
            drv_pwm_update_info(_g_motors_state.pwm_devs_fd[j], &_g_motors_state.pwms_info[j]);
            ++j;
        }
        _g_motors_state.pwms_info[j].channels[k].duty = g_motor_state.dutys[i];
        // limit range of duty
        if (g_motor_state.dutys[i] > CHEWIE_DRV_MOTOR_MAX_THROTTLE_DUTY)
            _g_motors_state.pwms_info[j].channels[k].duty = CHEWIE_DRV_MOTOR_MAX_THROTTLE_DUTY;
        if (g_motor_state.dutys[i] < CHEWIE_DRV_MOTOR_ARMED_THROTTLE_DUTY)
            _g_motors_state.pwms_info[j].channels[k].duty = CHEWIE_DRV_MOTOR_ARMED_THROTTLE_DUTY;
    }
    return 0;
}

void drv_motor_uninit()
{
    int i = 0;
    for (i = 0; i < CHEWIE_NUM_OF_MOTOR_PWM_DEV; ++i) {
        drv_pwm_stop(_g_motors_state.pwm_devs_fd[i]);
    }
    drv_pwm_uninit(_g_motors_state.pwm_devs_fd[i]);
}
