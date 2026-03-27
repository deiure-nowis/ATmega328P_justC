#ifndef SHT40_H_
#define SHT40_H_

#include <stdint.h>
#include <util/delay.h>

typedef struct{
    int32_t temp_x100;
    int32_t hum_x100;
} sht40_measurement_t;

void	sht40_init(void);   // zatím prázdné, případně pro jiné režimy
uint8_t	sht40_read_high_precision(sht40_measurement_t *meas);

#endif
