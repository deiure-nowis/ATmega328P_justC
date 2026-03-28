#define F_CPU 16000000UL

#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#include "i2c.h"
#include "sht40.h"
#include "lcd_i2c.h"

volatile uint32_t sys_ticks = 0;

void timer0_init(void){
	TCCR0A = (1 << WGM01); // Režim CTC (Clear Timer on Compare Match)
	TCCR0B = (1 << CS01) | (1 << CS00); // Prescaler 64: 16 Mhz / 64 = 250 000 tiků za s
	OCR0A = 249; // 250 000 / 1 000 = 250 tiků na 1 milisekundu
	TIMSK0 |= (1 << OCIE0A);
}

ISR(TIMER0_COMPA_vect){
	sys_ticks++;
}

uint32_t millis(void){
	uint32_t ms;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		ms = sys_ticks;
	return ms;
}

int main(void){
	i2c_init();
	lcd_init();
	timer0_init();

	sei(); // Povolení přerušení

	char buffer[21];

	lcd_set_cursor(0, 0);
	lcd_print("Senzor SHT40: OK");

	uint32_t last_measure_time = 0;

	while(1){
		uint32_t current_time = millis();

		// Každé 2s
		if(current_time - last_measure_time >= 2000){
			last_measure_time = current_time;
			sht40_measurement_t meas;
			if(sht40_read_high_precision(&meas) == 0){
				int16_t t_whole = meas.temp_x100 / 100;
				int16_t t_frac  = meas.temp_x100 % 100;
				if(t_frac < 0) t_frac = -t_frac;

				int16_t h_whole = meas.hum_x100 / 100;
				int16_t h_frac  = meas.hum_x100 % 100;

				if(meas.temp_x100 < 0 && t_whole == 0){
					sprintf(buffer, "Teplota: -0.%02d C ", t_frac);
				}else{
					sprintf(buffer, "Teplota: %d.%02d C ", t_whole, t_frac);
				}
				lcd_set_cursor(0, 1);
				lcd_print(buffer);

				sprintf(buffer, "Vlhkost: %d.%02d %% ", h_whole, h_frac);
				lcd_set_cursor(0, 2);
				lcd_print(buffer);
			}else{
				lcd_set_cursor(0, 1);
				lcd_print("SHT40 CHYBA!    ");
				lcd_set_cursor(0, 2);
				lcd_print("                ");
			}
		}



	}

	return 0;
}
