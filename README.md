# ATmega328P Weather Station (SHT40 & I2C LCD)

## O projektu
Tento projekt je napsaný v čistém C pro mikrokontrolér ATmega328p. Slouží k vyčítání teploty a vlhkosti ze senzoru SHT40 přes sběrnici I2C a k zobrazování naměřených hodnot na LCD displeji. Data jsou zároveň odesílána přes UART pro logování do PC.

## Vlastnosti
* **Bare-metal C:** Přímý zápis do registrů bez použití Arduino knihoven.
* **Vlastní drivery:** Napsané ovladače pro I2C (hardwarové TWI), UART a komunikaci se senzorem SHT40.
* **Automatizace buildu:** Konfigurovaný `Makefile` pro kompilaci přes `avr-gcc` a flashování přes `avrdude`.

## Hardware
* MCU: ATmega328P (16 MHz)
* Senzor: SHT40 (Teplota a vlhkost)
* Displej: 16x2 LCD s I2C převodníkem
* Programátor: Arduino as ISP / USB-to-UART (CH340/FTDI)

## Jak projekt přeložit a nahrát
Ujistěte se, že máte nainstalovaný toolchain `avr-libc`, `gcc-avr` a `avrdude`.

1. Klonování repozitáře: `git clone [URL]`
2. Kompilace projektu: `make`
3. Nahrání do MCU: `make flash` (v `Makefile` případně upravte `PORT=/dev/ttyUSBx`)
