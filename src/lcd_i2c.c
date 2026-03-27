#include <util/delay.h>
#include <stdint.h>

#include "i2c.h"
#include "lcd_i2c.h"

#define LCD_ADDR 0x4E
#define LCD_BACKLIGHT 0x08
#define LCD_ENABLE 0x04

static void pcf8574_write(uint8_t data){
	i2c_start();
	i2c_write(LCD_ADDR);
	i2c_write(data | LCD_BACKLIGHT);
	i2c_stop();
}

static void lcd_pulse_enable(uint8_t data){
	pcf8574_write(data | LCD_ENABLE);
	_delay_us(1);
	pcf8574_write(data & ~LCD_ENABLE);
	_delay_us(50);
}

static void lcd_send(uint8_t value, uint8_t rs_mode){
	uint8_t high = value & 0xF0;
	uint8_t low  = (value << 4) & 0xF0;

	lcd_pulse_enable(high | rs_mode);
	lcd_pulse_enable(low  | rs_mode);
}

void lcd_init(void){
	_delay_ms(50);

	lcd_pulse_enable(0x30); _delay_ms(5);
	lcd_pulse_enable(0x30); _delay_us(150);
	lcd_pulse_enable(0x30);
	lcd_pulse_enable(0x20);

	lcd_send(0x28, 0); // 4-bit, 2 řádky, 5×8 font
	lcd_send(0x0C, 0); // display ON, cursor OFF
	lcd_send(0x01, 0); // clear
	_delay_ms(2);
}

void lcd_set_cursor(uint8_t col, uint8_t row){
	static const uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
	lcd_send(0x80 | (col + row_offsets[row]), 0);
}

void lcd_print(const char* str){
	while(*str){
		lcd_send((uint8_t)*str, 1);
		str++;
	}
}
