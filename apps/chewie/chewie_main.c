/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <stdio.h>
#include "drivers/drv_motor.h"
#include "chewie_main.h"
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/
/****************************************************************************
 * Private Functions
 ****************************************************************************/
/****************************************************************************
 * Public Functions
 ****************************************************************************/
int test_motor(void)
{
  int ret = 0;
  int i = 0;
  printf("INFO:: chewie started\n");
  printf("INFO:: motor init started\n");
  ret = drv_motor_init();
  if (ret < 0) {
    printf(">> test motor: drv_motor_init with error ret: %d\n", ret);
    goto errout;
  }

  printf("INFO:: esc started\n");
  ret = drv_motor_esc_init();
  if (ret < 0) {
    printf(">> test motor: drv_motor_esc_init with error ret: %d\n", ret);
    goto errout;
  }

  // speed up to min throttle
  for (i = 0; i < CONFIG_CHEWIE_NUM_OF_CHANNELS; ++i)
    g_motor_state.dutys[i] = CHEWIE_DRV_MOTOR_ARMED_THROTTLE_DUTY + 874;
  drv_motor_update();
  sleep(1);
  for (i = 0; i < CONFIG_CHEWIE_NUM_OF_CHANNELS; ++i)
    g_motor_state.dutys[i] = CHEWIE_DRV_MOTOR_ARMED_THROTTLE_DUTY + 874 + 874;
  drv_motor_update();
  sleep(1);
  for (i = 0; i < CONFIG_CHEWIE_NUM_OF_CHANNELS; ++i)
    g_motor_state.dutys[i] = CHEWIE_DRV_MOTOR_ARMED_THROTTLE_DUTY + 874 + 874 + 874;
  drv_motor_update();
  sleep(1);
  // 

  printf("INFO:: test throttle 50%% speed started(5s)\n");
  for (i = 0; i < CONFIG_CHEWIE_NUM_OF_CHANNELS; ++i)
    g_motor_state.dutys[i] =  (int)(CHEWIE_DRV_MOTOR_MIN_THROTTLE_DUTY + (CHEWIE_DRV_MOTOR_MAX_THROTTLE_DUTY - CHEWIE_DRV_MOTOR_MIN_THROTTLE_DUTY) * 0.5);
  ret = drv_motor_update();
  if (ret < 0) {
    printf(">> test motor: drv_motor_update with error ret: %d\n", ret);
    goto errout;
  }
  sleep(5);

  printf("INFO:: test arme motor started (5s)\n");
  for (i = 0; i < CONFIG_CHEWIE_NUM_OF_CHANNELS; ++i)
    g_motor_state.dutys[i] = CHEWIE_DRV_MOTOR_ARMED_THROTTLE_DUTY;
  ret = drv_motor_update();
  if (ret < 0) {
    printf(">> test motor: drv_motor_update with error ret: %d\n", ret);
    goto errout;
  }
  sleep(5);

  drv_motor_deinit();
  printf(">> test motor done\n");
  return 0;
errout:
  printf(">> test motor [failed]\n");
  drv_motor_deinit();
  return -1;
}

#if defined(CONFIG_CHEWIE_ENABLE_UNIT_TEST)
#if defined(BUILD_MODULE)
int main(int argc, char *argv[])
#else
int chewie_main(int argc, char *argv[])
#endif
{
  test_motor();
  return 0;
}
#else
/****************************************************************************
 * chewie_main
 ****************************************************************************/

#include "drivers/drv_imu.h"
#include "lib/lib_rtc.h"

#if defined(BUILD_MODULE)
int main(int argc, FAR char *argv[])
#else
int chewie_main(int argc, char *argv[])
#endif
{
  char input;
  clock_t time_last = g_system_timer;
  drv_imu_init();
  while(1) {
    drv_imu_interface.update();
    printf("###delta: %d\n", g_system_timer - time_last);
    time_last = g_system_timer;
    //printf("system_timer: %d\n", g_system_timer);
    //input = getchar();
    //if (input == '1')
    // break;
  }
  drv_imu_deinit();
  return 0;
}

#endif