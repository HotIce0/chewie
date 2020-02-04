/****************************************************************************
 * Included Files
 ****************************************************************************/
#include "drv_pwm_out.h"

int init_pwm_device()
{
    int i = 0;
    int j = 0;
    int ret = 0;
    char devpath[50] = {0};

    if (!g_pwmstate.initialized) {
        // init g_pwmstate
        g_pwmstate.freq = CONFIG_CHEWIE_PWM_FREQUENCY;
        for (i = 0; i < CHEWIE_NUM_OF_PWM_DEV; ++i) {
            g_pwmstate.pwms_info[i].frequency = CONFIG_CHEWIE_PWM_FREQUENCY;
            g_pwmstate.pwm_devs_fd[i] = -1;
            for (j = 0; j < 4 && i * 4 + j < CONFIG_CHEWIE_NUM_OF_CHANNELS; ++j) {
                g_pwmstate.pwms_info[i].channels[j].channel = j + 1;
                g_pwmstate.pwms_info[i].channels[j].duty = CONFIG_CHEWIE_PWM_DUTY_INIT_VALUE;
            }
        }

        // init all pwm device
        for (i = 0; i < CHEWIE_NUM_OF_PWM_DEV; ++i) {
            sprintf(devpath, "%s%d", CONFIG_CHEWIE_PWM_DEVICE_PATH_PREFIX, i);
            printf("dev %s init\n", devpath);

            // open the pwm device
            g_pwmstate.pwm_devs_fd[i] = open(devpath, O_RDONLY);
            if (g_pwmstate.pwm_devs_fd[i] < 0) {
                printf("init_pwm_device: open %s failed: %d\n", devpath, errno);
                goto errout;
            }

            // send control info to pwm device
            ret = ioctl(g_pwmstate.pwm_devs_fd[i], PWMIOC_SETCHARACTERISTICS, (unsigned long)((uintptr_t)&g_pwmstate.pwms_info[i]));
            if (ret < 0) {
                printf("init_pwm_device: ioctl(PWMIOC_SETCHARACTERISTICS) failed: %d\n", errno);
                goto errout_with_dev;
            }

            // start the pwm device
            ret = ioctl(g_pwmstate.pwm_devs_fd[i], PWMIOC_START, 0);
            if (ret < 0) {
                printf("init_pwm_device: ioctl(PWMIOC_START) failed: %d\n", errno);
                goto errout_with_dev;
            }

            printf("success init and start pwm device : %s\n", devpath);
        }
        g_pwmstate.initialized = true;
    }
    else
    {
        printf("pwm device is already inited\n");
    }
    return OK;
errout_with_dev:
    for (i = 0; i < CHEWIE_NUM_OF_PWM_DEV; ++i)
    {
        if (g_pwmstate.pwm_devs_fd[i] >= 0)
            close(g_pwmstate.pwm_devs_fd[i]);
    }
errout:
    fflush(stdout);
    return ERROR;
}

int set_pwm_duty(uint8_t channel, ub16_t duty)
{
    int ret = 0;
    int i = 0;
    uint8_t index_dev = channel % 4 == 0 ? channel / 4 - 1 : channel / 4;

    g_pwmstate.pwms_info[index_dev].channels[channel].duty = duty;

    // send control info to pwm device
    ret = ioctl(g_pwmstate.pwm_devs_fd[index_dev], PWMIOC_SETCHARACTERISTICS, (unsigned long)((uintptr_t)&g_pwmstate.pwms_info[index_dev]));
    if (ret < 0) {
        printf("init_pwm_device: ioctl(PWMIOC_SETCHARACTERISTICS) failed: %d\n", errno);
        goto errout_with_dev;
    }
    return OK;

errout_with_dev:
    for (i = 0; i < CHEWIE_NUM_OF_PWM_DEV; ++i) {
        if (g_pwmstate.pwm_devs_fd[i] >= 0)
            close(g_pwmstate.pwm_devs_fd[i]);
    }
    fflush(stdout);
    return ERROR;
}

int uninit_pwm_device()
{
    int i = 0;
    g_pwmstate.initialized = false;
    for (i = 0; i < CHEWIE_NUM_OF_PWM_DEV; ++i) {
        if (g_pwmstate.pwm_devs_fd[i] >= 0)
            close(g_pwmstate.pwm_devs_fd[i]);
    }
    return OK;
}