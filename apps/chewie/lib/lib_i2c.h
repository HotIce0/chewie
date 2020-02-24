#include <fixedmath.h>

struct lib_i2c_s;

FAR struct lib_i2c_s * lib_i2c_open(FAR const char *devpath, uint32_t frequency, uint16_t addr);
int lib_i2c_read(FAR struct lib_i2c_s *_lib_i2c_s, uint8_t regaddr, FAR uint16_t *result);
int lib_i2c_read_mult_block(FAR struct lib_i2c_s *_lib_i2c_s, uint8_t regaddr,
                FAR uint8_t *buffer, ssize_t len);
void lib_i2c_deinit(FAR struct lib_i2c_s *_lib_i2c_s);