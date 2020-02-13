#include <nuttx/config.h>
#include <nuttx/drivers/pwm.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/
int drv_pwm_init(const char *devpath);
int drv_pwm_start(int fd);
int drv_pwm_stop(int fd);
int drv_pwm_update_info(int fd, const struct pwm_info_s *p_init_info_s);
void drv_pwm_deinit(int fd);