/* BH314 application logic for the STM32F103 bare-metal port. */
#include <stdint.h>
#include <stdbool.h>
#include "bh314.h"
#include "bh314_platform.h"
#include "bsp_time.h"

/* Original source: BH314.ino, target comment: stm32f103rct6. */

/* Command */
#define NORMAL 0x01
#define SETTIME 0x02
#define SETTEMP 0x03
#define SETALARM 0x04
#define SETPICV 0x05
#define RX_FRAME_HEX_LEN 24

#define IDADDR 1    // ID Board
#define _SLEEPMODE  // Sleep Mode
// #define _CHECKLED             //Check led
// #define _SETALARM             //Setup alarm
#define _SERIAL    // use Serial
#define _SETTIME   // Setup time
#define _SETTEMP   // Setup temp
#define _SHOWTEMP  // Show temp
// #define _SHOWHUMIDITY // Show humidity
#define _SERVICE  //Service
#define _AIR      // Air
// #define _TV                 //TV
// #define _NIGHTMODE            //Night Mode
#define _MASTER  // Light master
#define _LAMP1   // Lamp 1
#define _LAMP2   // Lamp 2
#define _LAMP3   // Lamp 3
#define _LAMP4   // Lamp 4
#define _LAMP5   // Lamp 5
#define _LAMP6   // Lamp 6
// #define _LAMP7   // Lamp 7
// #define _LAMP8   // Lamp 8
// #define _LAMP9   //Lamp 9
// #define _LAMP10               //Lamp 10
// #define _LAMP11               //Lamp 11
// #define _LAMP12               //Lamp 12
// #define _LAMP13               //Lamp 13
// #define _LAMP14               //Lamp 14
// #define _LAMP15               //Lamp 15
// #define _LAMP16               //Lamp 16
// #define _LAMP17               //Lamp 17
// #define _LAMP18               //Lamp 18

/* Data bit */
#define Bit0 (unsigned char)0x01  // Bit 0
#define Bit1 (unsigned char)0x02  // Bit 1
#define Bit2 (unsigned char)0x04  // Bit 2
#define Bit3 (unsigned char)0x08  // Bit 3
#define Bit4 (unsigned char)0x10  // Bit 4
#define Bit5 (unsigned char)0x20  // Bit 5
#define Bit6 (unsigned char)0x40  // Bit 6
#define Bit7 (unsigned char)0x80  // Bit 7

/* SEGMENT */
#define sega 0x01   // digit A
#define segb 0x02   // digit B
#define segc 0x04   // digit C
#define segd 0x08   // digit D
#define sege 0x10   // digit E
#define segf 0x20   // digit F
#define segg 0x40   // digit G
#define segdp 0x80  // digit Dp

/* KEYBOARD */
#define Key1 0x00    // None
#define Key2 0x01    // None
#define Key3 0x02    // None
#define Key4 0x03    // None
#define Key5 0x04    // None
#define Key6 0x05    // None
#define Key7 0x06    // None
#define Key8 0x07    // None
#define Key9 0x08    // None
#define Key10 0x09   // None
#define Key11 0x0a   // None
#define Key12 0x0b   // None
#define Key13 0x0c   // None
#define Key14 0x0d   // None
#define Key15 0x0e   // None
#define Key16 0x0f   // None
#define Key17 0x10   // None
#define Key18 0x11   // None
#define Keyxx 0x12   // None
#define Hoursw 0x18  // None
#define Minsw 0x19   // None

/* Input pin */
#define IN1 BH_PIN_PC6
#define IN2 BH_PIN_PC8
#define IN3 BH_PIN_PA14
#define IN4 BH_PIN_PA12
#define IN5 BH_PIN_PB10
#define IN6 BH_PIN_PB0
#define IN7 BH_PIN_PB12
#define IN8 BH_PIN_PB14
#define IN9 BH_PIN_PA15
#define IN10 BH_PIN_PC10
#define IN11 BH_PIN_PC12
#define IN12 BH_PIN_PC5
#define IN13 BH_PIN_PA7
#define IN14 BH_PIN_PA4
#define IN15 BH_PIN_PA6
#define IN16 BH_PIN_PB4
#define IN17 BH_PIN_PB7
#define IN18 BH_PIN_PB5

/* Output pin */
#define OP1 BH_PIN_PC7
#define OP2 BH_PIN_PC9
#define OP3 BH_PIN_PA13
#define OP4 BH_PIN_PA11
#define OP5 BH_PIN_PB11
#define OP6 BH_PIN_PB1
#define OP7 BH_PIN_PB13
#define OP8 BH_PIN_PB15
#define OP9 BH_PIN_PB3
#define OP10 BH_PIN_PD2
#define OP11 BH_PIN_PC11
#define OP12 BH_PIN_PC4
#define OP13 BH_PIN_PA2
#define OP14 BH_PIN_PA3
#define OP15 BH_PIN_PA5
#define OP16 BH_PIN_PB9
#define OP17 BH_PIN_PB8
#define OP18 BH_PIN_PB6

/* Display */
#define A00 BH_PIN_PC0
#define A01 BH_PIN_PC1
#define A02 BH_PIN_PC2
#define DO BH_PIN_PA0
#define CLK BH_PIN_PC3
#define RCK BH_PIN_PA1

/* Serial */
#define DIR BH_PIN_PA8

/* Speaker */
#define SPK BH_PIN_PC13

/* Start Config */
/* Dspbuf[4] */
#define Led1 Bit0
#define Led2 Bit1
#define Led3 Bit2
#define Led4 Bit3
#define Led5 Bit4
#define Led6 Bit5
#define Led7 Bit6
#define Led8 Bit7

/* Dspbuf[5] */
#define Led9 Bit0
#define Led10 Bit1
#define Led11 Bit2
#define Led12 Bit3
#define Led13 Bit4
#define Led14 Bit5
#define Led15 Bit6
#define Led16 Bit7

/* Dspbuf[6] */
#define Led17 Bit0
#define Led18 Bit1
#define Led19 Bit2
#define Led20 Bit3
#define Led21 Bit4
#define Led22 Bit5
#define Led23 Bit6
#define Led24 Bit7

/* Inpbuf[0] SWITCH */
#define Sw1 Bit0
#define Sw2 Bit1
#define Sw3 Bit2
#define Sw4 Bit3
#define Sw5 Bit4
#define Sw6 Bit5
#define Sw7 Bit6
#define Sw8 Bit7

/* Inpbuf[1] SWITCH */
#define Sw9 Bit0
#define Sw10 Bit1
#define Sw11 Bit2
#define Sw12 Bit3
#define Sw13 Bit4
#define Sw14 Bit5
#define Sw15 Bit6
#define Sw16 Bit7

/* Inpbuf[2] SWITCH */
#define Sw17 Bit0
#define Sw18 Bit1

/* Outbuf[0] HOUR */
/* Outbuf[1] SEC  */
/* Outbuf[2] TEMP */

/* Outbuf[3] LAMP */
#define Out1 Bit0
#define Out2 Bit1
#define Out3 Bit2
#define Out4 Bit3
#define Out5 Bit4
#define Out6 Bit5
#define Out7 Bit6
#define Out8 Bit7

/* Outbuf[4] LAMP */
#define Out9 Bit0
#define Out10 Bit1
#define Out11 Bit2
#define Out12 Bit3
#define Out13 Bit4
#define Out14 Bit5
#define Out15 Bit6
#define Out16 Bit7

/* Outbuf[5] LAMP */
#define Out17 Bit0
#define Out18 Bit1
#define Out19 Bit2
#define Out20 Bit3
#define Out21 Bit4
#define Out22 Bit5
#define Out23 Bit6
#define Out24 Bit7

/* Outbuf[6] LAMP */
#define Out25 Bit0
#define Out26 Bit1
#define Out27 Bit2
#define Out28 Bit3
#define Out29 Bit4
#define Out30 Bit5
#define Out31 Bit6
#define Out32 Bit7

/* Outbuf[7] CONTROL */
#define Dnd Bit0     //Dnd Output
#define Mur Bit1     //Mur Output
#define Ent Bit2     //Entran Output
#define Master Bit3  //Master Output
#define Door Bit4    //Door Output
#define Ccs Bit5     //CCS Output
#define Maid Bit6    //Maid Output
#define Guest Bit7   //Guest Output

/* Outbuf[8] AIR */
#define Temp 0x1f
#define FanSpeed 0x60
#define Airp Bit7
#define FanL 0x00
#define FanM 0x20
#define FanH 0x40
#define FanA 0x60

/* Outbuf[9] HOURS */
#define AlmHours 0x3f
#define Alarms Bit6
#define Alarmp Bit7

/* Outbuf[10] MINUTE */
#define AlmMin 0xff

/* Register Flag */
bool Timeflag;    // Timer flag
bool Halfflag;    // Half flag
bool Blkflag;     // Blink flag
bool Rcflag;      // Rxd flag
bool Rcvflag;     // Rxd flag
bool Rxdflag;     // Rxd flag
bool Txdflag;     // Txd flag
bool Dotflag;     // Dot flag
bool Alarmflag;   // Alarm flag
bool PowUpflag;   // Power Up flag
volatile bool Inpflag;  // Input flag
bool Inp1flag;    // Input flag
bool Inp2flag;    // Input flag
bool Inp3flag;    // Input flag
bool Inp4flag;    // Input flag
bool Inp5flag;    // Input flag
bool Inp6flag;    // Input flag
bool Inp7flag;    // Input flag
bool Inp8flag;    // Input flag
bool Inp9flag;    // Input flag
bool Inp10flag;   // Input flag
bool Inp11flag;   // Input flag
bool Inp12flag;   // Input flag
bool Inp13flag;   // Input flag
bool Inp14flag;   // Input flag
bool Inp15flag;   // Input flag
bool Inp16flag;   // Input flag
bool Inp17flag;   // Input flag
bool Inp18flag;   // Input flag
bool Updateflag;  // Update flag

/* Global - Variable */
uint16_t Timer;  // Timer Counter
bh_u8_t Txdaddr;  // Txd Address
bh_u8_t Txdcom;   // Txd Command
bh_u8_t Rxdaddr;  // Address Buffer
bh_u8_t Rxdcom;   // Rxd command

bh_u8_t Keybuf;    // Key Buffer
bh_u8_t Beepcnt;   // Beep Counter
bh_u8_t Beeplimit; // Beep duration limit
bh_u8_t Dimcnt;    // Port counter
bh_u8_t Dspcnt;    // Display counter
bh_u8_t Inp1cnt;   // Input counter
bh_u8_t Inp2cnt;   // Input counter
bh_u8_t Inp3cnt;   // Input counter
bh_u8_t Inp4cnt;   // Input counter
bh_u8_t Inp5cnt;   // Input counter
bh_u8_t Inp6cnt;   // Input counter
bh_u8_t Inp7cnt;   // Input counter
bh_u8_t Inp8cnt;   // Input counter
bh_u8_t Inp9cnt;   // Input counter
bh_u8_t Inp10cnt;  // Input counter
bh_u8_t Inp11cnt;  // Input counter
bh_u8_t Inp12cnt;  // Input counter
bh_u8_t Inp13cnt;  // Input counter
bh_u8_t Inp14cnt;  // Input counter
bh_u8_t Inp15cnt;  // Input counter
bh_u8_t Inp16cnt;  // Input counter
bh_u8_t Inp17cnt;  // Input counter
bh_u8_t Inp18cnt;  // Input counter

bh_u8_t Inpbuf[3];   // Input Buffer
bh_u8_t Inpmem[3];   // Input Buffer
bh_u8_t Ascbuf[4];   // Ascii Buffer
bh_u8_t Outbuf[11];  // Output buffer
volatile bh_u8_t Dspbuf[7];  // Display Buffer
volatile bh_u8_t *Dspscn;    // Display Pointer
bh_u8_t Colscn;      // Column Scan

bh_u8_t Outmem;  // Outbuf mem

#ifdef _SLEEPMODE  // Sleep Mode
bool Sleepflag;    // Sleep flag
bool SleepOn;      // Sleep flag
bh_u8_t Sleepcnt;     // Sleep counter
#endif

/* Uart.c */
bh_u8_t Rcvbuf[27];  // Rxd buffer
bh_u8_t *Rcvptr;     // Rxd pointer
bh_u8_t Rcvlen;      // Rxd length
bh_u8_t Rcvdly;      // Rxd delay

#define TIMER0_INTERVAL_MS 2
#define MS_TO_TICKS(ms) ((ms) / TIMER0_INTERVAL_MS)

/* Segment Table */
static const bh_u8_t SEGTAB[] = {
  sega + segb + segc + segd + sege + segf,         // digit 0
  segb + segc,                                     // digit 1
  sega + segb + segg + sege + segd,                // digit 2
  sega + segb + segc + segd + segg,                // digit 3
  segf + segg + segb + segc,                       // digit 4
  sega + segf + segg + segc + segd,                // digit 5
  sega + segf + segg + segc + segd + sege,         // digit 6
  sega + segb + segc,                              // digit 7
  sega + segb + segc + segd + sege + segf + segg,  // digit 8
  sega + segb + segc + segd + segf + segg,         // digit 9
  sega + segb + segc + sege + segf + segg,         // digit a
  segc + segd + sege + segf + segg,                // digit b
  sega + segd + sege + segf,                       // digit c
  segb + segc + segd + sege + segg,                // digit d
  sega + segd + sege + segf + segg,                // digit e
  sega + sege + segf + segg                        // digit f
};

static uint32_t bh314_irq_save(void) {
  uint32_t primask;
  __asm volatile ("mrs %0, primask\ncpsid i" : "=r" (primask) :: "memory");
  return primask;
}

static void bh314_irq_restore(uint32_t primask) {
  __asm volatile ("msr primask, %0" :: "r" (primask) : "memory");
}

static void bh314_display_two_decimal(bh_u8_t value, bh_u8_t *tens, bh_u8_t *ones) {
  if (value >= 100U) {
    value = 99U;
  }

  *tens = (value >= 10U) ? SEGTAB[value / 10U] : 0x00;
  *ones = SEGTAB[value % 10U];
}

#if defined(_SERIAL)
/* ตรวจว่าตัวอักษรเป็นเลขฐาน 16 หรือไม่ */
static bool IsHex(uint8_t a) {
  return ((a >= '0' && a <= '9') || (a >= 'a' && a <= 'f') || (a >= 'A' && a <= 'F'));
}

/* แปลง ASCII hex หลักขวาให้เป็นค่า 0-15 */
static uint8_t atohr(uint8_t a) {  // change ascii to hex (R)
  if ((a >= 'a' && a <= 'f') || (a >= 'A' && a <= 'F'))
    return ((a + 9) & 0xf);
  else
    return (a & 0xf);
}

/* แปลง ASCII hex หลักซ้ายให้เป็นค่า nibble สูง */
static uint8_t atohl(uint8_t a) {  // change ascii to hex (L)
  a = atohr(a);
  a = a << 4;
  return (a);
}

#endif

/********** GPIO init **********/
/* ตั้งค่า GPIO ทั้งหมดที่ BH314 ใช้ ทั้ง input, output, display, serial direction และ speaker */
static void bh314_gpio_init(void) {
  // initialize input pins.
  bh_gpio_config(IN1, BH_GPIO_INPUT_PULLUP);
  bh_gpio_config(IN2, BH_GPIO_INPUT_PULLUP);
  bh_gpio_config(IN3, BH_GPIO_INPUT_PULLUP);
  bh_gpio_config(IN4, BH_GPIO_INPUT_PULLUP);
  bh_gpio_config(IN5, BH_GPIO_INPUT_PULLUP);
  bh_gpio_config(IN6, BH_GPIO_INPUT_PULLUP);
  bh_gpio_config(IN7, BH_GPIO_INPUT_PULLUP);
  bh_gpio_config(IN8, BH_GPIO_INPUT_PULLUP);
  bh_gpio_config(IN9, BH_GPIO_INPUT_PULLUP);
  bh_gpio_config(IN10, BH_GPIO_INPUT_PULLUP);
  bh_gpio_config(IN11, BH_GPIO_INPUT_PULLUP);
  bh_gpio_config(IN12, BH_GPIO_INPUT_PULLUP);
  bh_gpio_config(IN13, BH_GPIO_INPUT_PULLUP);
  bh_gpio_config(IN14, BH_GPIO_INPUT_PULLUP);
  bh_gpio_config(IN15, BH_GPIO_INPUT_PULLUP);
  bh_gpio_config(IN16, BH_GPIO_INPUT_PULLUP);
  bh_gpio_config(IN17, BH_GPIO_INPUT_PULLUP);
  bh_gpio_config(IN18, BH_GPIO_INPUT_PULLUP);

  // initialize output pins.
  bh_gpio_config(OP1, BH_GPIO_OUTPUT);
  bh_gpio_config(OP2, BH_GPIO_OUTPUT);
  bh_gpio_config(OP3, BH_GPIO_OUTPUT);
  bh_gpio_config(OP4, BH_GPIO_OUTPUT);
  bh_gpio_config(OP5, BH_GPIO_OUTPUT);
  bh_gpio_config(OP6, BH_GPIO_OUTPUT);
  bh_gpio_config(OP7, BH_GPIO_OUTPUT);
  bh_gpio_config(OP8, BH_GPIO_OUTPUT);
  bh_gpio_config(OP9, BH_GPIO_OUTPUT);
  bh_gpio_config(OP10, BH_GPIO_OUTPUT);
  bh_gpio_config(OP11, BH_GPIO_OUTPUT);
  bh_gpio_config(OP12, BH_GPIO_OUTPUT);
  bh_gpio_config(OP13, BH_GPIO_OUTPUT);
  bh_gpio_config(OP14, BH_GPIO_OUTPUT);
  bh_gpio_config(OP15, BH_GPIO_OUTPUT);
  bh_gpio_config(OP16, BH_GPIO_OUTPUT);
  bh_gpio_config(OP17, BH_GPIO_OUTPUT);
  bh_gpio_config(OP18, BH_GPIO_OUTPUT);

  // initialize display pins.
  bh_gpio_config(A00, BH_GPIO_OUTPUT);
  bh_gpio_config(A01, BH_GPIO_OUTPUT);
  bh_gpio_config(A02, BH_GPIO_OUTPUT);
  bh_gpio_config(DO, BH_GPIO_OUTPUT);
  bh_gpio_config(CLK, BH_GPIO_OUTPUT);
  bh_gpio_config(RCK, BH_GPIO_OUTPUT);
  bh_gpio_write(A00, BH_GPIO_LOW);
  bh_gpio_write(A01, BH_GPIO_LOW);
  bh_gpio_write(A02, BH_GPIO_LOW);
  bh_gpio_write(DO, BH_GPIO_LOW);
  bh_gpio_write(CLK, BH_GPIO_LOW);
  bh_gpio_write(RCK, BH_GPIO_LOW);

  // initialize serial direction pin.
  bh_gpio_config(DIR, BH_GPIO_OUTPUT);
  bh_gpio_write(DIR, BH_GPIO_LOW);

  // initialize speaker pin.
  bh_gpio_config(SPK, BH_GPIO_OUTPUT);
  bh_gpio_write(SPK, BH_GPIO_LOW);
}

/********** shift data output **********/
/* ส่งข้อมูล 8 บิตออกไปยัง shift register ของชุด display */
static void bh314_shift_write(bh_u8_t dataOut) {
  bh_u8_t cnt;
  for (cnt = 0; cnt < 8; cnt++) {
    if (dataOut & 0x80) {
      bh_gpio_write(DO, BH_GPIO_HIGH);
    } else
      bh_gpio_write(DO, BH_GPIO_LOW);
    dataOut <<= 1;
    bh_gpio_write(CLK, BH_GPIO_HIGH);
    bh_gpio_write(RCK, BH_GPIO_LOW);
    bh_gpio_write(CLK, BH_GPIO_LOW);
  }
  bh_gpio_write(RCK, BH_GPIO_HIGH);
  bh_gpio_write(CLK, BH_GPIO_LOW);
  bh_gpio_write(RCK, BH_GPIO_LOW);
}

#ifdef _CHECKLED
/********** Function CheckLed **********/
/* โหมดทดสอบ LED: toggle LED ตามปุ่มที่กด */
static void CheckLed(void) {
  if (Keybuf == Key1)
    Dspbuf[4] ^= Led1;
  else if (Keybuf == Key2)
    Dspbuf[4] ^= Led2;
  else if (Keybuf == Key3)
    Dspbuf[4] ^= Led3;
  else if (Keybuf == Key4)
    Dspbuf[4] ^= Led4;
  else if (Keybuf == Key5)
    Dspbuf[4] ^= Led5;
  else if (Keybuf == Key6)
    Dspbuf[4] ^= Led6;
  else if (Keybuf == Key7)
    Dspbuf[4] ^= Led7;
  else if (Keybuf == Key8)
    Dspbuf[4] ^= Led8;
  else if (Keybuf == Key9)
    Dspbuf[5] ^= Led9;
  else if (Keybuf == Key10)
    Dspbuf[5] ^= Led10;
  else if (Keybuf == Key11)
    Dspbuf[5] ^= Led11;
  else if (Keybuf == Key12)
    Dspbuf[5] ^= Led12;
  else if (Keybuf == Key13)
    Dspbuf[5] ^= Led13;
  else if (Keybuf == Key14)
    Dspbuf[5] ^= Led14;
  else if (Keybuf == Key15)
    Dspbuf[5] ^= Led15;
  else if (Keybuf == Key16)
    Dspbuf[5] ^= Led16;
  else if (Keybuf == Key17)
    Dspbuf[6] ^= Led17;
  else if (Keybuf == Key18)
    Dspbuf[6] ^= Led18;
}
#endif

/********** Input scan **********/
/* อ่าน input ทั้ง 18 จุด พร้อม debounce และตั้ง Keybuf/Txdflag เมื่อมีปุ่มใหม่ */
static void bh314_read_inputs(void) {

  if (!bh_gpio_read(IN1)) {  // Input
    if (!Inp1flag) {
      if (++Inp1cnt >= MS_TO_TICKS(50)) {
        Inp1flag = true;
        Inpbuf[0] |= Sw1;
        if (!(Inpmem[0] & Sw1)) {
          Inpmem[0] |= Sw1;
          Keybuf = Key1;
          Txdflag = true;
        }
      }
    } else
      Inp1cnt = 0x00;
  } else {
    if (Inp1flag) {
      if (++Inp1cnt >= MS_TO_TICKS(100)) {
        Inp1flag = false;
        Inpbuf[0] &= ~Sw1;
        Inpmem[0] &= ~Sw1;
      }
    } else
      Inp1cnt = 0x00;
  }

  if (!bh_gpio_read(IN2)) {  // Input
    if (!Inp2flag) {
      if (++Inp2cnt >= MS_TO_TICKS(50)) {
        Inp2flag = true;
        Inpbuf[0] |= Sw2;
        if (!(Inpmem[0] & Sw2)) {
          Inpmem[0] |= Sw2;
          Keybuf = Key2;
          Txdflag = true;
        }
      }
    } else
      Inp2cnt = 0x00;
  } else {
    if (Inp2flag) {
      if (++Inp2cnt >= MS_TO_TICKS(100)) {
        Inp2flag = false;
        Inpbuf[0] &= ~Sw2;
        Inpmem[0] &= ~Sw2;
      }
    } else
      Inp2cnt = 0x00;
  }

  if (!bh_gpio_read(IN3)) {  // Input
    if (!Inp3flag) {
      if (++Inp3cnt >= MS_TO_TICKS(50)) {
        Inp3flag = true;
        Inpbuf[0] |= Sw3;
        if (!(Inpmem[0] & Sw3)) {
          Inpmem[0] |= Sw3;
          Keybuf = Key3;
          Txdflag = true;
        }
      }
    } else
      Inp3cnt = 0x00;
  } else {
    if (Inp3flag) {
      if (++Inp3cnt >= MS_TO_TICKS(100)) {
        Inp3flag = false;
        Inpbuf[0] &= ~Sw3;
        Inpmem[0] &= ~Sw3;
      }
    } else
      Inp3cnt = 0x00;
  }

  if (!bh_gpio_read(IN4)) {  // Input
    if (!Inp4flag) {
      if (++Inp4cnt >= MS_TO_TICKS(50)) {
        Inp4flag = true;
        Inpbuf[0] |= Sw4;
        if (!(Inpmem[0] & Sw4)) {
          Inpmem[0] |= Sw4;
          Keybuf = Key4;
          Txdflag = true;
        }
      }
    } else
      Inp4cnt = 0x00;
  } else {
    if (Inp4flag) {
      if (++Inp4cnt >= MS_TO_TICKS(100)) {
        Inp4flag = false;
        Inpbuf[0] &= ~Sw4;
        Inpmem[0] &= ~Sw4;
      }
    } else
      Inp4cnt = 0x00;
  }

  if (!bh_gpio_read(IN5)) {  // Input
    if (!Inp5flag) {
      if (++Inp5cnt >= MS_TO_TICKS(50)) {
        Inp5flag = true;
        Inpbuf[0] |= Sw5;
        if (!(Inpmem[0] & Sw5)) {
          Inpmem[0] |= Sw5;
          Keybuf = Key5;
          Txdflag = true;
        }
      }
    } else
      Inp5cnt = 0x00;
  } else {
    if (Inp5flag) {
      if (++Inp5cnt >= MS_TO_TICKS(100)) {
        Inp5flag = false;
        Inpbuf[0] &= ~Sw5;
        Inpmem[0] &= ~Sw5;
      }
    } else
      Inp5cnt = 0x00;
  }

  if (!bh_gpio_read(IN6)) {  // Input
    if (!Inp6flag) {
      if (++Inp6cnt >= MS_TO_TICKS(50)) {
        Inp6flag = true;
        Inpbuf[0] |= Sw6;
        if (!(Inpmem[0] & Sw6)) {
          Inpmem[0] |= Sw6;
          Keybuf = Key6;
          Txdflag = true;
        }
      }
    } else
      Inp6cnt = 0x00;
  } else {
    if (Inp6flag) {
      if (++Inp6cnt >= MS_TO_TICKS(100)) {
        Inp6flag = false;
        Inpbuf[0] &= ~Sw6;
        Inpmem[0] &= ~Sw6;
      }
    } else
      Inp6cnt = 0x00;
  }

  if (!bh_gpio_read(IN7)) {  // Input
    if (!Inp7flag) {
      if (++Inp7cnt >= MS_TO_TICKS(50)) {
        Inp7flag = true;
        Inpbuf[0] |= Sw7;
        if (!(Inpmem[0] & Sw7)) {
          Inpmem[0] |= Sw7;
          Keybuf = Key7;
          Txdflag = true;
        }
      }
    } else
      Inp7cnt = 0x00;
  } else {
    if (Inp7flag) {
      if (++Inp7cnt >= MS_TO_TICKS(100)) {
        Inp7flag = false;
        Inpbuf[0] &= ~Sw7;
        Inpmem[0] &= ~Sw7;
      }
    } else
      Inp7cnt = 0x00;
  }

  if (!bh_gpio_read(IN8)) {  // Input
    if (!Inp8flag) {
      if (++Inp8cnt >= MS_TO_TICKS(50)) {
        Inp8flag = true;
        Inpbuf[0] |= Sw8;
        if (!(Inpmem[0] & Sw8)) {
          Inpmem[0] |= Sw8;
          Keybuf = Key8;
          Txdflag = true;
        }
      }
    } else
      Inp8cnt = 0x00;
  } else {
    if (Inp8flag) {
      if (++Inp8cnt >= MS_TO_TICKS(100)) {
        Inp8flag = false;
        Inpbuf[0] &= ~Sw8;
        Inpmem[0] &= ~Sw8;
      }
    } else
      Inp8cnt = 0x00;
  }

  if (!bh_gpio_read(IN9)) {  // Input
    if (!Inp9flag) {
      if (++Inp9cnt >= MS_TO_TICKS(50)) {
        Inp9flag = true;
        Inpbuf[1] |= Sw9;
        if (!(Inpmem[1] & Sw9)) {
          Inpmem[1] |= Sw9;
          Keybuf = Key9;
          Txdflag = true;
        }
      }
    } else
      Inp9cnt = 0x00;
  } else {
    if (Inp9flag) {
      if (++Inp9cnt >= MS_TO_TICKS(100)) {
        Inp9flag = false;
        Inpbuf[1] &= ~Sw9;
        Inpmem[1] &= ~Sw9;
      }
    } else
      Inp9cnt = 0x00;
  }

  if (!bh_gpio_read(IN10)) {  // Input
    if (!Inp10flag) {
      if (++Inp10cnt >= MS_TO_TICKS(50)) {
        Inp10flag = true;
        Inpbuf[1] |= Sw10;
        if (!(Inpmem[1] & Sw10)) {
          Inpmem[1] |= Sw10;
          Keybuf = Key10;
          Txdflag = true;
        }
      }
    } else
      Inp10cnt = 0x00;
  } else {
    if (Inp10flag) {
      if (++Inp10cnt >= MS_TO_TICKS(100)) {
        Inp10flag = false;
        Inpbuf[1] &= ~Sw10;
        Inpmem[1] &= ~Sw10;
      }
    } else
      Inp10cnt = 0x00;
  }

  if (!bh_gpio_read(IN11)) {  // Input
    if (!Inp11flag) {
      if (++Inp11cnt >= MS_TO_TICKS(50)) {
        Inp11flag = true;
        Inpbuf[1] |= Sw11;
        if (!(Inpmem[1] & Sw11)) {
          Inpmem[1] |= Sw11;
          Keybuf = Key11;
          Txdflag = true;
        }
      }
    } else
      Inp11cnt = 0x00;
  } else {
    if (Inp11flag) {
      if (++Inp11cnt >= MS_TO_TICKS(100)) {
        Inp11flag = false;
        Inpbuf[1] &= ~Sw11;
        Inpmem[1] &= ~Sw11;
      }
    } else
      Inp11cnt = 0x00;
  }

  if (!bh_gpio_read(IN12)) {  // Input
    if (!Inp12flag) {
      if (++Inp12cnt >= MS_TO_TICKS(50)) {
        Inp12flag = true;
        Inpbuf[1] |= Sw12;
        if (!(Inpmem[1] & Sw12)) {
          Inpmem[1] |= Sw12;
          Keybuf = Key12;
          Txdflag = true;
        }
      }
    } else
      Inp12cnt = 0x00;
  } else {
    if (Inp12flag) {
      if (++Inp12cnt >= MS_TO_TICKS(100)) {
        Inp12flag = false;
        Inpbuf[1] &= ~Sw12;
        Inpmem[1] &= ~Sw12;
      }
    } else
      Inp12cnt = 0x00;
  }

  if (!bh_gpio_read(IN13)) {  // Input
    if (!Inp13flag) {
      if (++Inp13cnt >= MS_TO_TICKS(50)) {
        Inp13flag = true;
        Inpbuf[1] |= Sw13;
        if (!(Inpmem[1] & Sw13)) {
          Inpmem[1] |= Sw13;
          Keybuf = Key13;
          Txdflag = true;
        }
      }
    } else
      Inp13cnt = 0x00;
  } else {
    if (Inp13flag) {
      if (++Inp13cnt >= MS_TO_TICKS(100)) {
        Inp13flag = false;
        Inpbuf[1] &= ~Sw13;
        Inpmem[1] &= ~Sw13;
      }
    } else
      Inp13cnt = 0x00;
  }

  if (!bh_gpio_read(IN14)) {  // Input
    if (!Inp14flag) {
      if (++Inp14cnt >= MS_TO_TICKS(50)) {
        Inp14flag = true;
        Inpbuf[1] |= Sw14;
        if (!(Inpmem[1] & Sw14)) {
          Inpmem[1] |= Sw14;
          Keybuf = Key14;
          Txdflag = true;
        }
      }
    } else
      Inp14cnt = 0x00;
  } else {
    if (Inp14flag) {
      if (++Inp14cnt >= MS_TO_TICKS(100)) {
        Inp14flag = false;
        Inpbuf[1] &= ~Sw14;
        Inpmem[1] &= ~Sw14;
      }
    } else
      Inp14cnt = 0x00;
  }

  if (!bh_gpio_read(IN15)) {  // Input
    if (!Inp15flag) {
      if (++Inp15cnt >= MS_TO_TICKS(50)) {
        Inp15flag = true;
        Inpbuf[1] |= Sw15;
        if (!(Inpmem[1] & Sw15)) {
          Inpmem[1] |= Sw15;
          Keybuf = Key15;
          Txdflag = true;
        }
      }
    } else
      Inp15cnt = 0x00;
  } else {
    if (Inp15flag) {
      if (++Inp15cnt >= MS_TO_TICKS(100)) {
        Inp15flag = false;
        Inpbuf[1] &= ~Sw15;
        Inpmem[1] &= ~Sw15;
      }
    } else
      Inp15cnt = 0x00;
  }

  if (!bh_gpio_read(IN16)) {  // Input
    if (!Inp16flag) {
      if (++Inp16cnt >= MS_TO_TICKS(50)) {
        Inp16flag = true;
        Inpbuf[1] |= Sw16;
        if (!(Inpmem[1] & Sw16)) {
          Inpmem[1] |= Sw16;
          Keybuf = Key16;
          Txdflag = true;
        }
      }
    } else
      Inp16cnt = 0x00;
  } else {
    if (Inp16flag) {
      if (++Inp16cnt >= MS_TO_TICKS(100)) {
        Inp16flag = false;
        Inpbuf[1] &= ~Sw16;
        Inpmem[1] &= ~Sw16;
      }
    } else
      Inp16cnt = 0x00;
  }

  if (!bh_gpio_read(IN17)) {  // Input
    if (!Inp17flag) {
      if (++Inp17cnt >= MS_TO_TICKS(50)) {
        Inp17flag = true;
        Inpbuf[2] |= Sw17;
        if (!(Inpmem[2] & Sw17)) {
          Inpmem[2] |= Sw17;
          Keybuf = Key17;
          Txdflag = true;
        }
      }
    } else
      Inp17cnt = 0x00;
  } else {
    if (Inp17flag) {
      if (++Inp17cnt >= MS_TO_TICKS(100)) {
        Inp17flag = false;
        Inpbuf[2] &= ~Sw17;
        Inpmem[2] &= ~Sw17;
      }
    } else
      Inp17cnt = 0x00;
  }

  if (!bh_gpio_read(IN18)) {  // Input
    if (!Inp18flag) {
      if (++Inp18cnt >= MS_TO_TICKS(50)) {
        Inp18flag = true;
        Inpbuf[2] |= Sw18;
        if (!(Inpmem[2] & Sw18)) {
          Inpmem[2] |= Sw18;
          Keybuf = Key18;
          Txdflag = true;
        }
      }
    } else
      Inp18cnt = 0x00;
  } else {
    if (Inp18flag) {
      if (++Inp18cnt >= MS_TO_TICKS(100)) {
        Inp18flag = false;
        Inpbuf[2] &= ~Sw18;
        Inpmem[2] &= ~Sw18;
      }
    } else
      Inp18cnt = 0x00;
  }
}

/********** Beep **********/
/* เปิดเสียง beep และตั้งเวลาปิดเสียงด้วย counter ใน tick 2 ms */
static void bh314_beep(void) {
  bh_gpio_write(SPK, BH_GPIO_HIGH);
  Beeplimit = MS_TO_TICKS(50);
  Beepcnt = 0x00;
}

// Timer
/* งานที่ต้องทำทุก 2 ms: scan display, dimmer output และตั้ง flag อ่าน input */
void bh314_timer_tick_2ms(void) {
  Inpflag = true;

  bh314_shift_write(*Dspscn);
  if (Colscn & Bit0)
    bh_gpio_write(A00, BH_GPIO_HIGH);
  else
    bh_gpio_write(A00, BH_GPIO_LOW);
  if (Colscn & Bit1)
    bh_gpio_write(A01, BH_GPIO_HIGH);
  else
    bh_gpio_write(A01, BH_GPIO_LOW);
  Dspscn++;
  if (++Colscn >= 4) {
    Dspscn = &Dspbuf[0];
    Colscn = 0x00;
  }

  if (Dimcnt == 0) {
    bh_gpio_write(OP1, (Dspbuf[4] & Led1));
    bh_gpio_write(OP2, (Dspbuf[4] & Led2));
    bh_gpio_write(OP3, (Dspbuf[4] & Led3));
    bh_gpio_write(OP4, (Dspbuf[4] & Led4));
    bh_gpio_write(OP5, (Dspbuf[4] & Led5));
    bh_gpio_write(OP6, (Dspbuf[4] & Led6));
    bh_gpio_write(OP7, (Dspbuf[4] & Led7));
    bh_gpio_write(OP8, (Dspbuf[4] & Led8));
    bh_gpio_write(OP9, (Dspbuf[5] & Led9));
    bh_gpio_write(OP10, (Dspbuf[5] & Led10));
    bh_gpio_write(OP11, (Dspbuf[5] & Led11));
    bh_gpio_write(OP12, (Dspbuf[5] & Led12));
    bh_gpio_write(OP13, (Dspbuf[5] & Led13));
    bh_gpio_write(OP14, (Dspbuf[5] & Led14));
    bh_gpio_write(OP15, (Dspbuf[5] & Led15));
    bh_gpio_write(OP16, (Dspbuf[5] & Led16));
    bh_gpio_write(OP17, (Dspbuf[6] & Led17));
    bh_gpio_write(OP18, (Dspbuf[6] & Led18));
  } else if (Dimcnt == 1) {
    bh_gpio_write(OP1, 0);
    bh_gpio_write(OP2, 0);
    bh_gpio_write(OP3, 0);
    bh_gpio_write(OP4, 0);
    bh_gpio_write(OP5, 0);
    bh_gpio_write(OP6, 0);
    bh_gpio_write(OP7, 0);
    bh_gpio_write(OP8, 0);
    bh_gpio_write(OP9, 0);
    bh_gpio_write(OP10, 0);
    bh_gpio_write(OP11, 0);
    bh_gpio_write(OP12, 0);
    bh_gpio_write(OP13, 0);
    bh_gpio_write(OP14, 0);
    bh_gpio_write(OP15, 0);
    bh_gpio_write(OP16, 0);
    bh_gpio_write(OP17, 0);
    bh_gpio_write(OP18, 0);
  }
  if (++Dimcnt >= 6) Dimcnt = 0;
}

/********** Init **********/
/* เริ่มต้นระบบ BH314: GPIO, buffer, UART, timer และ watchdog */
void bh314_init(void) {
  bh314_gpio_init();

  Inpbuf[0] = 0;
  Inpbuf[1] = 0;
  Inpbuf[2] = 0;
  Inpmem[0] = 0;
  Inpmem[1] = 0;
  Inpmem[2] = 0;
  Dspbuf[0] = 0;
  Dspbuf[1] = 0;
  Dspbuf[2] = 0;
  Dspbuf[3] = 0;
  Dspbuf[4] = 0;
  Dspbuf[5] = 0;
  Dspbuf[6] = 0;
  Dspscn = &Dspbuf[0];
  Colscn = 0;
  Dimcnt = 0;
  bh_serial_begin_default();
  bsp_time_init();
  PowUpflag = false;
  bh_watchdog_begin(8000);
}

/********** Main task **********/
/* loop หลักของ BH314: รับ packet, อัปเดต output/display, ส่ง key และอ่าน input */
void bh314_task(void) {
  bh_watchdog_reload();

  // Receiver
  if (Rxdflag) {
    bh_u8_t rxHeader;

    Rxdflag = false;
    rxHeader = atohl(Rcvbuf[0]) | atohr(Rcvbuf[1]);  // Command
    Rxdaddr = rxHeader & 0x0f;
    Rxdcom = rxHeader >> 4;

    if (((Rxdaddr == IDADDR) || (Rxdaddr == 0x00)) && (Rxdcom != 0)) {
      Outbuf[0] = atohl(Rcvbuf[2]) | atohr(Rcvbuf[3]);
      Outbuf[1] = atohl(Rcvbuf[4]) | atohr(Rcvbuf[5]);
      Outbuf[2] = atohl(Rcvbuf[6]) | atohr(Rcvbuf[7]);
      Outbuf[3] = atohl(Rcvbuf[8]) | atohr(Rcvbuf[9]);
      Outbuf[4] = atohl(Rcvbuf[10]) | atohr(Rcvbuf[11]);
      Outbuf[5] = atohl(Rcvbuf[12]) | atohr(Rcvbuf[13]);
      Outbuf[6] = atohl(Rcvbuf[14]) | atohr(Rcvbuf[15]);
      Outbuf[7] = atohl(Rcvbuf[16]) | atohr(Rcvbuf[17]);
      Outbuf[8] = atohl(Rcvbuf[18]) | atohr(Rcvbuf[19]);
      Outbuf[9] = atohl(Rcvbuf[20]) | atohr(Rcvbuf[21]);
      Outbuf[10] = atohl(Rcvbuf[22]) | atohr(Rcvbuf[23]);

      if (Rxdcom == NORMAL) {
        if (!SleepOn) Updateflag = true;
        else if ((Outbuf[7] & (Guest | Maid)) != (Outmem & (Guest | Maid))) {
          if (Outbuf[7] & (Guest | Maid)) {
            Updateflag = true;
            Sleepflag = false;
            SleepOn = false;
          }
        }
      } else if (Rxdcom == SETALARM) {
        uint32_t irq_state = bh314_irq_save();
        if ((Outbuf[9] & Alarms) && (!(Outbuf[9] & Alarmp))) {  // Show alarm
          Dspbuf[0] = SEGTAB[(Outbuf[9] >> 4) & 0x03];
          Dspbuf[1] = SEGTAB[Outbuf[9] & 0x0f] + segdp;
          Dspbuf[2] = SEGTAB[Outbuf[10] >> 4] + segdp;
          Dspbuf[3] = SEGTAB[Outbuf[10] & 0x0f];
        } else if ((Outbuf[9] & Alarms) && (Outbuf[9] & Alarmp)) {  // Show alarm
          Dspbuf[0] = SEGTAB[Outbuf[0] >> 4];
          Dspbuf[1] = SEGTAB[Outbuf[0] & 0x0f];
          Dspbuf[2] = SEGTAB[Outbuf[1] >> 4];
          Dspbuf[3] = SEGTAB[Outbuf[1] & 0x0f];
        } else {
          if ((Outbuf[0] >> 4) == 0)  // Show clock
            Dspbuf[0] = 0;
          else
            Dspbuf[0] = SEGTAB[Outbuf[0] >> 4];
          Dspbuf[1] = SEGTAB[Outbuf[0] & 0x0f];
          Dspbuf[2] = SEGTAB[Outbuf[1] >> 4];
          Dspbuf[3] = SEGTAB[Outbuf[1] & 0x0f];
        }
        bh314_irq_restore(irq_state);
        Dotflag = true;
      } else if (Rxdcom == SETTIME) {  // Clock
        uint32_t irq_state = bh314_irq_save();
        Dspbuf[0] = SEGTAB[Outbuf[0] >> 4];
        Dspbuf[1] = SEGTAB[Outbuf[0] & 0x0f];
        Dspbuf[2] = SEGTAB[Outbuf[1] >> 4];
        Dspbuf[3] = SEGTAB[Outbuf[1] & 0x0f];
        Dspbuf[1] |= segdp;
        Dspbuf[2] |= segdp;
        bh314_irq_restore(irq_state);
        Dotflag = false;
        Dspcnt = 0;
        Timeflag = false;
        Timer = 0;
      } else if (Rxdcom == SETTEMP) {  // Temp
        uint32_t irq_state;
        bh314_display_two_decimal((Outbuf[8] & Temp), &Ascbuf[0], &Ascbuf[1]);
        irq_state = bh314_irq_save();
        Dspbuf[0] = 0x00;
        Dspbuf[1] = Ascbuf[0];
        Dspbuf[2] = Ascbuf[1];
        Dspbuf[3] = sega + segf + segg + segdp;
        bh314_irq_restore(irq_state);
        Dotflag = false;
        Dspcnt = 0;
        Timeflag = false;
        Timer = 0;
      } else if (Rxdcom == SETPICV) {  // Picv time
        uint32_t irq_state;
        bh314_display_two_decimal(Outbuf[10], &Ascbuf[0], &Ascbuf[1]);
        irq_state = bh314_irq_save();
        Dspbuf[0] = 0x00;
        Dspbuf[1] = Ascbuf[0];
        Dspbuf[2] = Ascbuf[1];
        Dspbuf[3] = sega + segb + sege + segf + segg;
        bh314_irq_restore(irq_state);
        Dotflag = false;
        Dspcnt = 0;
        Timeflag = false;
        Timer = 0;
      }
    }
  }

  // Update Led
  if (Updateflag) {
    uint32_t irq_state;

    Updateflag = false;
    Outmem = Outbuf[7];

    irq_state = bh314_irq_save();
    Dspbuf[4] = 0;  // Led1  -> Led8
    Dspbuf[5] = 0;  // Led9  -> Led16
    Dspbuf[6] = 0;  // Led17 -> Led24

    if (Outbuf[7] & (Guest | Maid)) {
      Dspbuf[4] |= Led3 | Led4;
      // Dspbuf[5] |= Led9;

      // if (!(Outbuf[7] & Master)) {
#ifdef _LAMP1
        if (Outbuf[3] & Out3) {  // Out1
          Dspbuf[4] |= Led7;
        }
#endif

#ifdef _LAMP2
        if (Outbuf[3] & Out4) {  // Out2
          Dspbuf[4] |= Led8;
        }
#endif

#ifdef _LAMP3
        if (Outbuf[3] & Out5) {  // Out3
          Dspbuf[5] |= Led9;
        }
#endif

#ifdef _LAMP4
        if (Outbuf[3] & Out6) {  // Out4
          Dspbuf[5] |= Led14;
        }
#endif

#ifdef _LAMP5
        if (Outbuf[3] & Out7) {  // Out5
          Dspbuf[5] |= Led15;
        }
#endif

#ifdef _LAMP6
        if (Outbuf[3] & Out8) {  // Out6
          Dspbuf[5] |= Led16;
        }
#endif

#ifdef _LAMP7
        if (Outbuf[4] & Out9) {  // Out7
          Dspbuf[5] |= Led15;
        }
#endif

#ifdef _LAMP8
        if (Outbuf[4] & Out10) {  // Out8
          Dspbuf[5] |= Led16;
        }
#endif

#ifdef _LAMP9
        if (Outbuf[5] & Out17) {  // Out8
          Dspbuf[6] |= Led17;
        }
#endif
      // }

#ifdef _MASTER
      if ((Outbuf[7] & (Guest | Maid)) && (Outbuf[7] & Master)) {
        Dspbuf[6] |= (Led17 | Led18);
      }
#endif

#ifdef _AIR
      if (IDADDR == 0x01) {
        if (Outbuf[8] & Airp) {  // Air (Guest | Maid)
          Dspbuf[4] |= Led2;
          if ((Outbuf[8] & FanSpeed) == FanL) {  // Fan low
            Dspbuf[5] |= Led12;
          } else if ((Outbuf[8] & FanSpeed) == FanM) {  // Fan med
            Dspbuf[4] |= Led5;
          } else if ((Outbuf[8] & FanSpeed) == FanH) {  // Fan high
            Dspbuf[4] |= Led1;
          }
        }
      }
#endif

#ifdef _TV
      if (IDADDR == 0x01) {
        if (Outbuf[7] & Tvp) {  // Tv
          Dspbuf[5] |= Led12;
        }
      }
#endif

#ifdef _SETALARM
      if (Outbuf[9] & Alarms) {
        Dspbuf[6] |= Led18;
      }

      if (Outbuf[9] & Alarmp) {  // Alarm
        Dspbuf[5] |= Led15;
      }
#endif
    }

#ifdef _SERVICE
    if (Outbuf[7] & Mur) {  // Make Up Room
      Dspbuf[5] |= Led10;
    }
    if (Outbuf[7] & Dnd) {  // Donot Disturb
      Dspbuf[5] |= Led11;
    }
#endif

    bh314_irq_restore(irq_state);
  }  // Update

  // Half flag
  if (Halfflag) {
    Halfflag = false;

#ifdef _SLEEPMODE
    if ((((Outbuf[3] & (Out3 | Out4 | Out5 | Out6 | Out7 | Out8)) == 0) && (Outbuf[4] == 0) && (Outbuf[5] == 0))\
      || (Outbuf[7] & Master)) {
      // if ((Outbuf[0] >= 0x21) || (Outbuf[0] < 0x06)) {  // Sleep on time 21:00 > 6.00
      if (!Sleepflag) {
        Sleepflag = true;
        Sleepcnt = 0;
      } else {
        if (++Sleepcnt >= (30 * 2)) {  // 30 sec
          Sleepflag = false;
          SleepOn = true;
        }
      }
      // }
    } else {
      Sleepflag = false;
      SleepOn = false;
    }
#endif
  }

  // Timer
  if (Timeflag) {
    uint32_t irq_state;

    Timeflag = false;
    PowUpflag = true;

#ifdef _SETALARM
    // Alarm
    if (Outbuf[9] & Alarmp) {  // Check Alarm
      if ((Outbuf[0] == (Outbuf[9] & AlmHours)) && (Outbuf[1] == Outbuf[10])) {
        bh_gpio_write(SPK, BH_GPIO_HIGH);
        Beeplimit = 200;
        Beepcnt = 0;
      }
    }

    if (Outbuf[9] & Alarms) {
      irq_state = bh314_irq_save();
      Dspbuf[0] = SEGTAB[(Outbuf[9] >> 4) & 0x03];
      Dspbuf[1] = SEGTAB[Outbuf[9] & 0x0f] + segdp;
      Dspbuf[2] = SEGTAB[Outbuf[10] >> 4] + segdp;
      Dspbuf[3] = SEGTAB[Outbuf[10] & 0x0f];
      bh314_irq_restore(irq_state);
    } else
    // End Alarm
#endif

      if (!SleepOn) {
#ifdef _SHOWTEMP
      if (Dspcnt == 10) {  // Show Temp
        bh314_display_two_decimal(Outbuf[2], &Ascbuf[0], &Ascbuf[1]);
        irq_state = bh314_irq_save();
        Dspbuf[0] = 0x00;
        Dspbuf[1] = Ascbuf[0];
        Dspbuf[2] = Ascbuf[1];
        Dspbuf[3] = sega + segf + segg + segdp;
        bh314_irq_restore(irq_state);
        Dotflag = false;
      } else
#endif
#ifdef _SHOWHUMIDITY
        if (Dspcnt == 11) {  // Show humidity
        bh314_display_two_decimal(Outbuf[9], &Ascbuf[0], &Ascbuf[1]);
        irq_state = bh314_irq_save();
        Dspbuf[0] = 0;
        Dspbuf[1] = Ascbuf[0];
        Dspbuf[2] = Ascbuf[1];
        Dspbuf[3] = segb + segc + sege + segf + segg;
        bh314_irq_restore(irq_state);
        Dotflag = false;
      } else
#endif
        if (Dspcnt < 10) {
        irq_state = bh314_irq_save();
        if ((Outbuf[0] >> 4) == 0)  // Show Clock
          Dspbuf[0] = 0;
        else
          Dspbuf[0] = SEGTAB[Outbuf[0] >> 4];
        Dspbuf[1] = SEGTAB[Outbuf[0] & 0x0f];
        Dspbuf[2] = SEGTAB[Outbuf[1] >> 4];
        Dspbuf[3] = SEGTAB[Outbuf[1] & 0x0f];
        bh314_irq_restore(irq_state);
        Dotflag = true;
      }
    } else {
      irq_state = bh314_irq_save();
      Dspbuf[0] = 0;
      Dspbuf[1] = segdp;
      Dspbuf[2] = segdp;
      Dspbuf[3] = 0;
      Dspbuf[4] = 0;
      Dspbuf[5] = 0;
      Dspbuf[6] = 0;
      bh314_irq_restore(irq_state);
    }

#ifdef _SHOWHUMIDITY
    if (++Dspcnt > 11) {
      Dspcnt = 0;
    }
#else
    if (++Dspcnt > 10) {
      Dspcnt = 0;
    }
#endif
  }  // End Timer

  if ((Txdflag) && (!Rcflag)) {
    Txdflag = false;
    if (!SleepOn) {
      if (PowUpflag) {
        bh_gpio_write(DIR, BH_GPIO_HIGH);  // Tx on
        while (!bh_gpio_read(DIR)) bh_watchdog_reload();
        bh_serial_send_key(IDADDR, Keybuf);
        bh_serial_flush();
        bh_gpio_write(DIR, BH_GPIO_LOW);  // Rx on
        bh314_beep();
      }
    } else {
      Updateflag = true;
      Sleepflag = false;
      SleepOn = false;
    }
#ifdef _CHECKLED
    {
      uint32_t irq_state = bh314_irq_save();
      CheckLed();
      bh314_irq_restore(irq_state);
    }
#endif
  }

  // Check Serial
  if (bh_serial_available() > 0) {
    char inChar = (char)bh_serial_read();
    if (inChar == ':') {
      Rcvptr = &Rcvbuf[0];
      Rcvlen = 0;
      Rcvflag = true;
    } else if (Rcvflag) {
      if (inChar == '\r') {
        *Rcvptr = '\0';
        Rcvflag = false;
        if (Rcvlen == RX_FRAME_HEX_LEN) {
          Rxdflag = true;
        }
      } else if ((Rcvlen < RX_FRAME_HEX_LEN) && IsHex(inChar)) {
        *Rcvptr = inChar;
        Rcvptr++;
        Rcvlen++;
      } else {
        Rcvflag = false;
      }
    }
    Rcvdly = 0;
    Rcflag = true;
  }  // End Serial

  // Read Input
  if (Inpflag) {
    Inpflag = false;

    if (++Timer == (MS_TO_TICKS(500))) {
      Halfflag = true;
      Blkflag ^= true;

      uint32_t irq_state = bh314_irq_save();
      if ((SleepOn && Blkflag) || (Dotflag)) {
        Dspbuf[1] |= segdp;
        Dspbuf[2] |= segdp;
      } else {
        Dspbuf[1] &= ~segdp;
        Dspbuf[2] &= ~segdp;
      }
      bh314_irq_restore(irq_state);
    } else if (Timer >= (MS_TO_TICKS(1000))) {
      Halfflag = true;
      Timeflag = true;
      Timer = 0;
    }

    if (++Rcvdly >= (MS_TO_TICKS(20))) {  // Uart 20 mSec
      Rcflag = false;
    }

    if (++Beepcnt >= Beeplimit) {  // Beep
      bh_gpio_write(SPK, BH_GPIO_LOW);
    }

    bh314_read_inputs();
  }  // End Input
}
