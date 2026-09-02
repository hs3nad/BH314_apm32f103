/* BH314.ino */
/* stm32f103rct6 */
#include <Arduino.h>
#include <STM32TimerInterrupt.h>
#include <IWatchdog.h>

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
#define IN1 PC_6
#define IN2 PC_8
#define IN3 PA_14
#define IN4 PA_12
#define IN5 PB_10
#define IN6 PB_0
#define IN7 PB_12
#define IN8 PB_14
#define IN9 PA_15
#define IN10 PC_10
#define IN11 PC_12
#define IN12 PC_5
#define IN13 PA_7
#define IN14 PA_4
#define IN15 PA_6
#define IN16 PB_4
#define IN17 PB_7
#define IN18 PB_5

/* Output pin */
#define OP1 PC_7
#define OP2 PC_9
#define OP3 PA_13
#define OP4 PA_11
#define OP5 PB_11
#define OP6 PB_1
#define OP7 PB_13
#define OP8 PB_15
#define OP9 PB_3
#define OP10 PD_2
#define OP11 PC_11
#define OP12 PC_4
#define OP13 PA_2
#define OP14 PA_3
#define OP15 PA_5
#define OP16 PB_9
#define OP17 PB_8
#define OP18 PB_6

/* Display */
#define A00 PC_0
#define A01 PC_1
#define A02 PC_2
#define DO PA_0
#define CLK PC_3
#define RCK PA_1

/* Serial */
#define DIR PA_8

/* Speaker */
#define SPK PC_13

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
byte Txdaddr;  // Txd Address
byte Txdcom;   // Txd Command
byte Rxdaddr;  // Address Buffer
byte Rxdcom;   // Rxd command

byte Keybuf;    // Key Buffer
byte Beepcnt;   // Beep Counter
byte Beeplimit; // Beep duration limit
byte Dimcnt;    // Port counter
byte Dspcnt;    // Display counter
byte Inp1cnt;   // Input counter
byte Inp2cnt;   // Input counter
byte Inp3cnt;   // Input counter
byte Inp4cnt;   // Input counter
byte Inp5cnt;   // Input counter
byte Inp6cnt;   // Input counter
byte Inp7cnt;   // Input counter
byte Inp8cnt;   // Input counter
byte Inp9cnt;   // Input counter
byte Inp10cnt;  // Input counter
byte Inp11cnt;  // Input counter
byte Inp12cnt;  // Input counter
byte Inp13cnt;  // Input counter
byte Inp14cnt;  // Input counter
byte Inp15cnt;  // Input counter
byte Inp16cnt;  // Input counter
byte Inp17cnt;  // Input counter
byte Inp18cnt;  // Input counter

byte Inpbuf[3];   // Input Buffer
byte Inpmem[3];   // Input Buffer
byte Ascbuf[4];   // Ascii Buffer
byte Outbuf[11];  // Output buffer
volatile byte Dspbuf[7];  // Display Buffer
volatile byte *Dspscn;    // Display Pointer
byte Colscn;      // Column Scan

byte Outmem;  // Outbuf mem

#ifdef _SLEEPMODE  // Sleep Mode
bool Sleepflag;    // Sleep flag
bool SleepOn;      // Sleep flag
byte Sleepcnt;     // Sleep counter
#endif

/* Uart.c */
byte Rcvbuf[27];  // Rxd buffer
byte *Rcvptr;     // Rxd poiter
byte Rcvlen;      // Rxd length
byte Rcvdly;      // Rxd delay

// Init STM32 timer TIM1
STM32Timer ITimer0(TIM1);
#define TIMER0_INTERVAL_MS 2
#define MS_TO_TICKS(ms) ((ms) / TIMER0_INTERVAL_MS)

/* Segment Table */
byte const SEGTAB[] = {
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

#if defined(_SERIAL)
/* Function IsHex */
bool IsHex(uint8_t a) {
  return ((a >= '0' && a <= '9') || (a >= 'a' && a <= 'f') || (a >= 'A' && a <= 'F'));
}

/* Function atohr */
uint8_t atohr(uint8_t a) {  // change ascii to hex (R)
  if ((a >= 'a' && a <= 'f') || (a >= 'A' && a <= 'F'))
    return ((a + 9) & 0xf);
  else
    return (a & 0xf);
}

/* Function atohl */
uint8_t atohl(uint8_t a) {  // change ascii to hex (L)
  a = atohr(a);
  a = a << 4;
  return (a);
}

/* Function htoar */
uint8_t htoar(uint8_t h) {  // change hex to ascii (R)
  h = h & 0xf;
  if (h > 9)
    return ((h - 9) | 0x40);
  else
    return (h | 0x30);
}

/* Function htoal */
uint8_t htoal(uint8_t h) {  // change hex to ascii (L)
  return (htoar(h >> 4));
}
#endif

/********** SetupPin **********/
void SetupPin() {
  // initialize digital pin as an input.
  pinMode(IN1, INPUT_PULLUP);
  pinMode(IN2, INPUT_PULLUP);
  pinMode(IN3, INPUT_PULLUP);
  pinMode(IN4, INPUT_PULLUP);
  pinMode(IN5, INPUT_PULLUP);
  pinMode(IN6, INPUT_PULLUP);
  pinMode(IN7, INPUT_PULLUP);
  pinMode(IN8, INPUT_PULLUP);
  pinMode(IN9, INPUT_PULLUP);
  pinMode(IN10, INPUT_PULLUP);
  pinMode(IN11, INPUT_PULLUP);
  pinMode(IN12, INPUT_PULLUP);
  pinMode(IN13, INPUT_PULLUP);
  pinMode(IN14, INPUT_PULLUP);
  pinMode(IN15, INPUT_PULLUP);
  pinMode(IN16, INPUT_PULLUP);
  pinMode(IN17, INPUT_PULLUP);
  pinMode(IN18, INPUT_PULLUP);

  // initialize digital pin as an output.
  pinMode(OP1, OUTPUT);
  pinMode(OP2, OUTPUT);
  pinMode(OP3, OUTPUT);
  pinMode(OP4, OUTPUT);
  pinMode(OP5, OUTPUT);
  pinMode(OP6, OUTPUT);
  pinMode(OP7, OUTPUT);
  pinMode(OP8, OUTPUT);
  pinMode(OP9, OUTPUT);
  pinMode(OP10, OUTPUT);
  pinMode(OP11, OUTPUT);
  pinMode(OP12, OUTPUT);
  pinMode(OP13, OUTPUT);
  pinMode(OP14, OUTPUT);
  pinMode(OP15, OUTPUT);
  pinMode(OP16, OUTPUT);
  pinMode(OP17, OUTPUT);
  pinMode(OP18, OUTPUT);

  // initialize digital pin as an display
  pinMode(A00, OUTPUT);
  pinMode(A01, OUTPUT);
  pinMode(A02, OUTPUT);
  pinMode(DO, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(RCK, OUTPUT);
  digitalWrite(A00, LOW);
  digitalWrite(A01, LOW);
  digitalWrite(A02, LOW);
  digitalWrite(DO, LOW);
  digitalWrite(CLK, LOW);
  digitalWrite(RCK, LOW);

  // initialize digital pin as an serial
  pinMode(DIR, OUTPUT);
  digitalWrite(DIR, LOW);

  // initialize digital pin as an specker
  pinMode(SPK, OUTPUT);
  digitalWrite(SPK, LOW);
}

/********** shift data output **********/
void WriteSPI(byte dataOut) {
  byte cnt;
  for (cnt = 0; cnt < 8; cnt++) {
    if (dataOut & 0x80) {
      digitalWrite(DO, HIGH);
    } else
      digitalWrite(DO, LOW);
    dataOut <<= 1;
    digitalWrite(CLK, HIGH);
    digitalWrite(RCK, LOW);
    digitalWrite(CLK, LOW);
  }
  digitalWrite(RCK, HIGH);
  digitalWrite(CLK, LOW);
  digitalWrite(RCK, LOW);
}

// /********** Function WriteOutput **********/
// void WriteOutput() {
// }

#ifdef _CHECKLED
/********** Function CheckLed **********/
void CheckLed() {
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

/********** Function ReadInput **********/
void ReadInput() {

  if (!digitalRead(IN1)) {  // Input
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

  if (!digitalRead(IN2)) {  // Input
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

  if (!digitalRead(IN3)) {  // Input
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

  if (!digitalRead(IN4)) {  // Input
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

  if (!digitalRead(IN5)) {  // Input
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

  if (!digitalRead(IN6)) {  // Input
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

  if (!digitalRead(IN7)) {  // Input
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

  if (!digitalRead(IN8)) {  // Input
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

  if (!digitalRead(IN9)) {  // Input
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

  if (!digitalRead(IN10)) {  // Input
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

  if (!digitalRead(IN11)) {  // Input
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

  if (!digitalRead(IN12)) {  // Input
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

  if (!digitalRead(IN13)) {  // Input
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

  if (!digitalRead(IN14)) {  // Input
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

  if (!digitalRead(IN15)) {  // Input
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

  if (!digitalRead(IN16)) {  // Input
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

  if (!digitalRead(IN17)) {  // Input
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

  if (!digitalRead(IN18)) {  // Input
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

/********** Function Beep **********/
void Beep() {  // Beep
  digitalWrite(SPK, HIGH);
  Beeplimit = MS_TO_TICKS(50);
  Beepcnt = 0x00;
}

// Timer
void TimerHandler0() {
  Inpflag = true;

  WriteSPI(*Dspscn);
  if (Colscn & Bit0)
    digitalWrite(A00, HIGH);
  else
    digitalWrite(A00, LOW);
  if (Colscn & Bit1)
    digitalWrite(A01, HIGH);
  else
    digitalWrite(A01, LOW);
  Dspscn++;
  if (++Colscn >= 4) {
    Dspscn = &Dspbuf[0];
    Colscn = 0x00;
  }

  if (Dimcnt == 0) {
    digitalWrite(OP1, (Dspbuf[4] & Led1));
    digitalWrite(OP2, (Dspbuf[4] & Led2));
    digitalWrite(OP3, (Dspbuf[4] & Led3));
    digitalWrite(OP4, (Dspbuf[4] & Led4));
    digitalWrite(OP5, (Dspbuf[4] & Led5));
    digitalWrite(OP6, (Dspbuf[4] & Led6));
    digitalWrite(OP7, (Dspbuf[4] & Led7));
    digitalWrite(OP8, (Dspbuf[4] & Led8));
    digitalWrite(OP9, (Dspbuf[5] & Led9));
    digitalWrite(OP10, (Dspbuf[5] & Led10));
    digitalWrite(OP11, (Dspbuf[5] & Led11));
    digitalWrite(OP12, (Dspbuf[5] & Led12));
    digitalWrite(OP13, (Dspbuf[5] & Led13));
    digitalWrite(OP14, (Dspbuf[5] & Led14));
    digitalWrite(OP15, (Dspbuf[5] & Led15));
    digitalWrite(OP16, (Dspbuf[5] & Led16));
    digitalWrite(OP17, (Dspbuf[6] & Led17));
    digitalWrite(OP18, (Dspbuf[6] & Led18));
  } else if (Dimcnt == 1) {
    digitalWrite(OP1, 0);
    digitalWrite(OP2, 0);
    digitalWrite(OP3, 0);
    digitalWrite(OP4, 0);
    digitalWrite(OP5, 0);
    digitalWrite(OP6, 0);
    digitalWrite(OP7, 0);
    digitalWrite(OP8, 0);
    digitalWrite(OP9, 0);
    digitalWrite(OP10, 0);
    digitalWrite(OP11, 0);
    digitalWrite(OP12, 0);
    digitalWrite(OP13, 0);
    digitalWrite(OP14, 0);
    digitalWrite(OP15, 0);
    digitalWrite(OP16, 0);
    digitalWrite(OP17, 0);
    digitalWrite(OP18, 0);
  }
  if (++Dimcnt >= 6) Dimcnt = 0;
}

/**
    @brief  System Clock Configuration
    @param  None
    @retval None
*/
void SystemClock_Config(void)
#if defined(USBCON)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {};

  /** Initializes the RCC Oscillators according to the specified parameters
    in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
    Error_Handler();
  }
}
#else
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {};

  /** Initializes the RCC Oscillators according to the specified parameters
    in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
    Error_Handler();
  }
}
#endif /* USBCON */

/********** setup **********/
void setup() {
  //  HAL_Init();
  //  SystemClock_Config();
  SetupPin();

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
  Serial.begin(2400);
  ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, TimerHandler0);
  PowUpflag = false;
  IWatchdog.begin(8000000);  // init 8 second wd timer
  while (!IWatchdog.isEnabled())
    ;
}

/********** loop **********/
void loop() {
  IWatchdog.reload();

  // Receiver
  if (Rxdflag) {
    byte rxHeader;

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
        Dotflag = true;
      } else if (Rxdcom == SETTIME) {  // Clock
        Dspbuf[0] = SEGTAB[Outbuf[0] >> 4];
        Dspbuf[1] = SEGTAB[Outbuf[0] & 0x0f];
        Dspbuf[2] = SEGTAB[Outbuf[1] >> 4];
        Dspbuf[3] = SEGTAB[Outbuf[1] & 0x0f];
        Dspbuf[1] |= segdp;
        Dspbuf[2] |= segdp;
        Dotflag = false;
        Dspcnt = 0;
        Timeflag = false;
        Timer = 0;
      } else if (Rxdcom == SETTEMP) {  // Temp
        sprintf((char *)Ascbuf, "%2d", (Outbuf[8] & Temp));
        Dspbuf[0] = 0x00;
        Dspbuf[1] = SEGTAB[Ascbuf[0] & 0x0f];
        Dspbuf[2] = SEGTAB[Ascbuf[1] & 0x0f];
        Dspbuf[3] = sega + segf + segg + segdp;
        Dotflag = false;
        Dspcnt = 0;
        Timeflag = false;
        Timer = 0;
      } else if (Rxdcom == SETPICV) {  // Picv time
        sprintf((char *)Ascbuf, "%2d", Outbuf[10]);
        Dspbuf[0] = 0x00;
        Dspbuf[1] = SEGTAB[Ascbuf[0] & 0x0f];
        Dspbuf[2] = SEGTAB[Ascbuf[1] & 0x0f];
        Dspbuf[3] = sega + segb + sege + segf + segg;
        Dotflag = false;
        Dspcnt = 0;
        Timeflag = false;
        Timer = 0;
      }
    }
  }

  // Update Led
  if (Updateflag) {
    Updateflag = false;
    Outmem = Outbuf[7];

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
    Timeflag = false;
    PowUpflag = true;

#ifdef _SETALARM
    // Alarm
    if (Outbuf[9] & Alarmp) {  // Check Alarm
      if ((Outbuf[0] == (Outbuf[9] & AlmHours)) && (Outbuf[1] == Outbuf[10])) {
        digitalWrite(SPK, HIGH);
        Beeplimit = 200;
        Beepcnt = 0;
      }
    }

    if (Outbuf[9] & Alarms) {
      Dspbuf[0] = SEGTAB[(Outbuf[9] >> 4) & 0x03];
      Dspbuf[1] = SEGTAB[Outbuf[9] & 0x0f] + segdp;
      Dspbuf[2] = SEGTAB[Outbuf[10] >> 4] + segdp;
      Dspbuf[3] = SEGTAB[Outbuf[10] & 0x0f];
    } else
    // End Alarm
#endif

      if (!SleepOn) {
#ifdef _SHOWTEMP
      if (Dspcnt == 10) {  // Show Temp
        sprintf((char *)Ascbuf, "%2d", Outbuf[2]);
        Dspbuf[0] = 0x00;
        Dspbuf[1] = SEGTAB[Ascbuf[0] & 0x0f];
        Dspbuf[2] = SEGTAB[Ascbuf[1] & 0x0f];
        Dspbuf[3] = sega + segf + segg + segdp;
        Dotflag = false;
      } else
#endif
#ifdef _SHOWHUMIDITY
        if (Dspcnt == 11) {  // Show humidity
        sprintf((char *)Ascbuf, "%3d", Outbuf[9]);
        Dspbuf[0] = 0;
        Dspbuf[1] = SEGTAB[Ascbuf[0] & 0x0f];
        Dspbuf[2] = SEGTAB[Ascbuf[1] & 0x0f];
        Dspbuf[3] = segb + segc + sege + segf + segg;
        Dotflag = false;
      } else
#endif
        if (Dspcnt < 10) {
        if ((Outbuf[0] >> 4) == 0)  // Show Clock
          Dspbuf[0] = 0;
        else
          Dspbuf[0] = SEGTAB[Outbuf[0] >> 4];
        Dspbuf[1] = SEGTAB[Outbuf[0] & 0x0f];
        Dspbuf[2] = SEGTAB[Outbuf[1] >> 4];
        Dspbuf[3] = SEGTAB[Outbuf[1] & 0x0f];
        Dotflag = true;
      }
    } else {
      Dspbuf[0] = 0;
      Dspbuf[1] = segdp;
      Dspbuf[2] = segdp;
      Dspbuf[3] = 0;
      Dspbuf[4] = 0;
      Dspbuf[5] = 0;
      Dspbuf[6] = 0;
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
        digitalWrite(DIR, HIGH);  // Tx on
        while (!digitalRead(DIR))
          ;
        Serial.printf("\n:%02X%02X\r", IDADDR, Keybuf);
        Serial.flush();
        digitalWrite(DIR, LOW);  // Rx on
        Beep();
      }
    } else {
      Updateflag = true;
      Sleepflag = false;
      SleepOn = false;
    }
#ifdef _CHECKLED
    CheckLed();
#endif
  }

  // Check Serial
  if (Serial.available() > 0) {
    char inChar = (char)Serial.read();
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

      if ((SleepOn && Blkflag) || (Dotflag)) {
        Dspbuf[1] |= segdp;
        Dspbuf[2] |= segdp;
      } else {
        Dspbuf[1] &= ~segdp;
        Dspbuf[2] &= ~segdp;
      }
    } else if (Timer >= (MS_TO_TICKS(1000))) {
      Halfflag = true;
      Timeflag = true;
      Timer = 0;
    }

    if (++Rcvdly >= (MS_TO_TICKS(20))) {  // Uart 20 mSec
      Rcflag = false;
    }

    if (++Beepcnt >= Beeplimit) {  // Beep
      digitalWrite(SPK, LOW);
    }

    // WriteOutput();
    ReadInput();
  }  // End Input
}
