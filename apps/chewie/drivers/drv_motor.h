#include <nuttx/config.h>
#include <fixedmath.h>

#define PLUSE_US_TO_DUTY(us)    ((ub16_t)(us / (1.0 / CONFIG_CHEWIE_MOTOR_PWM_FREQUENCY * 1000000.0) * 0x0000FFFF))

#define CHEWIE_DRV_MOTOR_MAX_THROTTLE_DUTY  PLUSE_US_TO_DUTY(CONFIG_CHEWIE_MOTOR_PWM_MAX_THROTTLE_PLUSE_US)
#define CHEWIE_DRV_MOTOR_MIN_THROTTLE_DUTY  PLUSE_US_TO_DUTY(CONFIG_CHEWIE_MOTOR_PWM_MIN_THROTTLE_PLUSE_US)
#define CHEWIE_DRV_MOTOR_ARMED_THROTTLE_DUTY    PLUSE_US_TO_DUTY(CONFIG_CHEWIE_MOTOR_PWM_ARMED_THROTTLE_PLUSE_US)

struct motor_state_s {
    ub16_t dutys[CONFIG_CHEWIE_NUM_OF_CHANNELS];
};

// motor的全局配置(用于读取或者设置当前的每个通道的duty值)
struct motor_state_s g_motor_state;

int drv_motor_init(void);
int drv_motor_esc_init(void);
int drv_motor_update(void);
void drv_motor_deinit(void);
