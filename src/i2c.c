#include <avr/io.h>
#include "i2c.h"

#define I2C_TIMEOUT 10000

void i2c_init(void){
	TWSR = 0x00;
	TWBR = 72;          // ~100 kHz při 16 MHz
	TWCR = (1 << TWEN);
}

uint8_t i2c_start(void){
	uint16_t timeout = I2C_TIMEOUT;
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while(!(TWCR & (1 << TWINT)))
		if(--timeout == 0) return 1;
	return 0;
}

void i2c_stop(void){
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

uint8_t i2c_write(uint8_t data){
	uint16_t timeout = I2C_TIMEOUT;
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)))
		if(--timeout == 0) return 1;
	return 0;
}

uint8_t i2c_read_ack(uint8_t *data){
	uint16_t timeout = I2C_TIMEOUT;
	TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)))
		if(--timeout == 0) return 1;
	*data = TWDR;
	return 0;
}

uint8_t i2c_read_nack(uint8_t *data){
	uint16_t timeout = I2C_TIMEOUT;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)))
		if(--timeout == 0) return 1;
	*data = TWDR;
	return 0;
}
