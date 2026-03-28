# Bare-metal AVR SHT40
---
Tento projekt je implementací jednoduché meteostanice postavené na mikrokontroléru **ATmega328P**. Cílem projektu bylo vytvořit robustní embedded aplikaci v čistém jazyce C (bare-metal) **bez použití Arduino frameworku** nebo externích knihoven třetích stran.

Projekt demonstruje práci s registry, hardwarovými periferiemi, přerušeními a optimalizaci kódu pro 8-bitovou architekturu.

## Hlavní technické vlastnosti (Highlights)
---
* **Custom I2C (TWI) driver:** Plně hardwarově ovládaná I2C sběrnice přes registry AVR. Implementováno s **ochranou proti zamrznutí (timeouty)**, aby nedošlo k zablokování programu při chybě na sběrnici.
* **Non-blocking architektura:** Náhrada zdržujících `_delay_ms()` smyček vlastním asynchronním plánovačem pomocí `millis()`.
* **Interrupts & Atomicity:** Služba Timer0 běží na pozadí přes přerušení (`ISR`). Pro bezpečné čtení 32-bitové časové proměnné v hlavní smyčce je využit `ATOMIC_BLOCK`, čímž je zabráněno *race conditions* na 8-bitovém MCU.
* **Fixed-point aritmetika:** Kód záměrně **nevyužívá data type `float`**, který je pro 8-bitové procesory výpočetně náročný a zabírá zbytečně moc paměti. Hodnoty jsou počítány pomocí `int32_t` (např. teplota je udržována jako stonásobek reálné hodnoty).
* **CRC-8 Validace:** Implementován algoritmus pro kontrolu cyklickým redundantním součtem (CRC), který ověřuje integritu dat přečtených ze senzoru SHT40.
* **Vlastní Build systém:** Kompilace a flashování je řízeno vlastním `Makefile` s využitím `avr-gcc` a `avrdude`.

## Hardware
---
* **MCU:** ATmega328P (Taktováno na 16 MHz)
* **Senzor:** Sensirion SHT40 (High-accuracy I2C senzor teploty a vlhkosti, adresa `0x44`)
* **Displej:** 16x2 znakový LCD s I2C expandérem PCF8574 (adresa `0x4E`)

## Struktura projektu
---
```text
.
├── Makefile       # Definice build procesu a flashování
├── include/       # Hlavičkové soubory
│   ├── i2c.h      # Deklarace I2C rozhraní
│   ├── lcd_i2c.h  # Deklarace funkcí pro LCD
│   └── sht40.h    # Deklarace SHT40 API a datových struktur
└── src/           # Zdrojové kódy v C
    ├── i2c.c      # Low-level implementace I2C (registry TWCR, TWSR, atd.)
    ├── lcd_i2c.c  # Ovladač displeje (4-bitový režim přes I2C expandér)
    ├── main.c     # Hlavní smyčka a inicializace
    └── sht40.c    # Komunikace se senzorem, CRC check, fixed-point převody
```

## Kompilace a Flashování
---
Projekt nepoužívá žádné IDE, je plně ovladatelný z příkazové řádky.

### Prerekvizity
* GNU AVR toolchain (`avr-gcc`, `avr-libc`)
* `avrdude`
* `make`

### Příkazy
1. **Sestavení projektu** (vygeneruje `.elf` a `.hex` v adresáři `bin/`):
   ```bash
   make all
   ```
2. **Nahrání do MCU** (upravte `PORT` v Makefile dle vašeho OS, např. `/dev/ttyUSB0` nebo `COM3`):
   ```bash
   make flash
   ```
3. **Vyčištění zkompilovaných souborů**:
   ```bash
   make clean
   ```

## Zhodnocení a možná rozšíření
---
Aplikace běží stabilně a je odolná vůči odpojení senzoru za běhu (díky timeoutům na I2C a vypisování "SHT40 CHYBA!").
Potenciální budoucí vylepšení:
* Uspávání procesoru (Sleep modes) mezi cykly měření pro snížení spotřeby.
* Použití Watchdog timeru pro automatický reset při neočekávaném záseku.
