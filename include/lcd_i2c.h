#ifndef LCD_I2C_H_
#define LCD_I2C_H_

void lcd_init(void);
void lcd_set_cursor(uint8_t col, uint8_t row);
void lcd_print(const char* str);

#endif
