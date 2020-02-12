#include <nuttx/config.h>
#include <fixedmath.h>

#define PLUSE_US_TO_DUTY(us) ((int)(us / (1 / CONFIG_CHEWIE_PWM_FREQUENCY * 0x000F4240) * 0x0000ffff))
#define CHEWIE_DRV_MOTOR_MAX_THROTTLE_DUTY PLUSE_US_TO_DUTY(CONFIG_CHEWIE_PWM_MAX_THROTTLE_PLUSE_US)
#define CHEWIE_DRV_MOTOR_MIN_THROTTLE_DUTY PLUSE_US_TO_DUTY(CONFIG_CHEWIE_PWM_MIN_THROTTLE_PLUSE_US)
#define CHEWIE_DRV_MOTOR_ARMED_THROTTLE_DUTY PLUSE_US_TO_DUTY(CONFIG_CHEWIE_PWM_ARMED_THROTTLE_PLUSE_US)

struct motor_states_s {
    ub16_t dutys[CONFIG_CHEWIE_NUM_OF_CHANNELS];
};

struct motor_states_s g_motor_state;

int drv_motor_init();

int drv_motor_esc_init();

int drv_motor_update();

void drv_motor_uninit();
