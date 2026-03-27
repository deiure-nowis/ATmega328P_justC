#include <util/delay.h>

#include "i2c.h"
#include "sht40.h"

#define SHT40_ADDR 0x44

// Command - high precision
#define SHT40_MEASURE_HIGH 0xFD

static uint8_t sht40_check_crc(uint8_t msb, uint8_t lsb, uint8_t expected_crc){
	uint8_t crc = 0xFF; // Inicializační hodnota
	uint8_t data[2] = {msb, lsb};

	for(uint8_t i=0; i<2; i++){
		crc ^= data[i];
		for(uint8_t bit=8; bit>0; --bit){
			if(crc & 0x80){
				crc = (crc << 1) ^ 0x31;
			}else{
				crc = (crc << 1);
			}
		}
	}

	return (crc == expected_crc) ? 0 : 1;
}

uint8_t sht40_read_high_precision(sht40_measurement_t *meas){
	uint8_t t_msb, t_lsb, t_crc;
	uint8_t h_msb, h_lsb, h_crc;
	uint8_t err = 0;

	// Zahájení měření
	err |= i2c_start();
	err |= i2c_write(SHT40_ADDR << 1); // write
	err |= i2c_write(SHT40_MEASURE_HIGH);
	i2c_stop();

	if(err) return 1; // Selhání I2C komunikace při odesílání příkazu

	_delay_ms(10); // max 8.2 ms podle datasheetu

	// Čtení 6 bajtů
	err |= i2c_start();
	err |= i2c_write((SHT40_ADDR << 1) | 1); // read

	err |=i2c_read_ack(&t_msb);
	err |=i2c_read_ack(&t_lsb);
	err |=i2c_read_ack(&t_crc);

	err |=i2c_read_ack(&h_msb);
	err |=i2c_read_ack(&h_lsb);
	err |=i2c_read_nack(&h_crc);

	i2c_stop();

	if(err) return 1; // Selhání I2C komunikace při čtení dat

	if(sht40_check_crc(t_msb, t_lsb, t_crc) != 0)
		return 1; // Chyba CRC u teploty
	if(sht40_check_crc(h_msb, h_lsb, h_crc) != 0)
		return 1; // Chyba CRC u vlhkosti

	uint16_t raw_temp = (t_msb << 8) | t_lsb;
	uint16_t raw_hum  = (h_msb  << 8) | h_lsb;

	meas->temp_x100 = -4500L + (int32_t)((17500UL * raw_temp) / 65535UL);
	meas->hum_x100  =  -600L + (int32_t)((12500UL * raw_hum)  / 65535UL);

	return 0; // OK
}
