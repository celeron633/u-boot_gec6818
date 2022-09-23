#include <friendlyelec/1-wire.h>
#include "regs.h"
#include <i2c.h>

#define SAMPLE_BPS 9600

#define REQ_INFO          0x60U
#define REQ_BL            0x80U

#define GPH1CON_REG __REG(ELFIN_GPIO_BASE+GPH1CON_OFFSET)
#define GPH1DAT_REG __REG(ELFIN_GPIO_BASE+GPH1DAT_OFFSET)
#define GPH1PUD_REG __REG(ELFIN_GPIO_BASE+GPH1PUD_OFFSET)
#define GPH1DRV_REG __REG(ELFIN_GPIO_BASE+GPH1DRV_OFFSET)

static void init_1wire_pin(void)
{
#if 0
	Uart_SendString("BEFORE CONF 1wire pud: ");
	Uart_SendHexWORD( GPH1PUD_REG );
	Uart_SendString(", AFTER: ");
#endif
	GPH1PUD_REG = (GPH1PUD_REG & ~(0x03 << 4)) | (0x02<< 4);
#if 0
	Uart_SendHexWORD( GPH1PUD_REG );
	Uart_SendString("\r\n");
#endif
}
static void set_pin_as_input(void)
{
	unsigned tmp;
	tmp = GPH1CON_REG;
	tmp &= ~(0xFU << 8);
	GPH1CON_REG = tmp;
}

static void set_pin_as_output(void)
{
	unsigned tmp;
	tmp = GPH1CON_REG;
	tmp = (tmp & ~(0xFU << 8)) | (1U << 8);
	GPH1CON_REG = tmp;
}

static void set_pin_value(int v)
{
	unsigned tmp;
	tmp = GPH1DAT_REG;
	if (v) {
		tmp |= (1 << 2);
	} else {
		tmp &= ~(1<<2);
	}
	GPH1DAT_REG = tmp;
}

static int get_pin_value(void)
{
	int v;
	unsigned long tmp;
	tmp = GPH1DAT_REG;
	v = !!(tmp & (1<<2));
	return v;
}

#define rTCON   TCON_REG
#define rTCFG0  TCFG0_REG
#define rTCFG1  TCFG1_REG
#define rTCNTB0 TCNTB0_REG
#define rTCMPB0 TCMPB0_REG
#define rTCNTO0 TCNTO0_REG
#define rTCNTB1 TCNTB1_REG
#define rTCMPB1 TCMPB1_REG
#define rTCNTO1 TCNTO1_REG
#define rTCNTB2 TCNTB2_REG
#define rTCMPB2 TCMPB2_REG
#define rTCNTO2 TCNTO2_REG
#define rTCNTB3 TCNTB3_REG
#define rTCMPB3 TCMPB3_REG
#define rTCNTO3 TCNTO3_REG
#define rTCNTB4 TCNTB4_REG
#define rTCNTO4 TCNTO4_REG

#define rTINT_CSTAT TINT_CSTAT

#define TIMER4_PENDING_CLEAR	(1<<9)
#define TIMER3_PENDING_CLEAR	(1<<8)
#define TIMER2_PENDING_CLEAR	(1<<7)
#define TIMER1_PENDING_CLEAR	(1<<6)
#define TIMER0_PENDING_CLEAR	(1<<5)
#define TIMER4_INTERRUPT_ENABLE	(1<<4)
#define TIMER3_INTERRUPT_ENABLE	(1<<3)
#define TIMER2_INTERRUPT_ENABLE	(1<<2)
#define TIMER1_INTERRUPT_ENABLE	(1<<1)
#define TIMER0_INTERRUPT_ENABLE	(1<<0)

#define TINT_CSTAT_INTMASK(n)            ((n)&0x1F)    // Remove Pending Status Value from SFR

#define SYS_TIMER_PRESCALER	2
#define SYS_TIMER_DIVIDER	1
#define CHIP_PCLK 66700000
static void StartTimer(void)
{
	rTCON &= ~(0xf<<16);    // Timer3 Stop
    rTCON |= (1<<17);    // update TCNTB3
    rTCON &= ~(1<<17);
    rTCON |= ((1<<19)|(1<<16));    // AutoReload mode, Timer3 Start
}

static void StopTimer(void)
{
	unsigned tcon;
    tcon = rTCON;
	tcon &= ~(1<<16);
	rTCON = tcon;
}

static void WaitTimerTick(void)
{
	while ((rTINT_CSTAT & (1U<<8)) == 0);
	rTINT_CSTAT = TINT_CSTAT_INTMASK(rTINT_CSTAT) | TIMER3_PENDING_CLEAR;
}

int CheckTimerForSDFAT(void)
{
	static int inited;
	if (!inited) {
		rTCNTB4 = (CHIP_PCLK/SYS_TIMER_PRESCALER/SYS_TIMER_DIVIDER / 2 - 1);
		rTINT_CSTAT = TINT_CSTAT_INTMASK(rTINT_CSTAT) | TIMER4_PENDING_CLEAR;
		rTINT_CSTAT = TINT_CSTAT_INTMASK(rTINT_CSTAT) | TIMER4_INTERRUPT_ENABLE;
		rTCON &= ~(0x7<<20);    // Timer4 Stop
    	rTCON |= (1<<21);    // update TCNTB4
    	rTCON &= ~(1<<21);
    	rTCON |= ((1<<22)|(1<<20));    // AutoReload mode, Timer4 Start
		inited = 1;
	}
	
	if ((rTINT_CSTAT & (1U<<9)) == 0) {
		return 0;
	} else {
		rTINT_CSTAT = TINT_CSTAT_INTMASK(rTINT_CSTAT) | TIMER4_PENDING_CLEAR;
		return 1;
	}
}

static int OneWireSession(unsigned char req, unsigned char res[])
{
	unsigned Req;
	unsigned *Res;
	unsigned int i;
	
	Req = (req << 24) | (crc8(req << 24, 8) << 16);
	Res = (unsigned *)res;
	
	set_pin_value(1);
	set_pin_as_output();
	StartTimer();
	for (i = 0; i < 60; i++) {
		WaitTimerTick();
	}
	set_pin_value(0);
	for (i = 0; i < 2; i++) {
		WaitTimerTick();
	}
	for (i = 0; i < 16; i++) {
		int v = !!(Req & (1U <<31));
		Req <<= 1;
		set_pin_value(v);
		WaitTimerTick();
	}
	WaitTimerTick();
	set_pin_as_input();
	WaitTimerTick();
	for (i = 0; i < 32; i++) {
		(*Res) <<= 1;
		(*Res) |= get_pin_value();
		WaitTimerTick();
	}
	StopTimer();
	set_pin_value(1);
	set_pin_as_output();

	return crc8(*Res, 24) == res[0];
}

static int TryOneWireSession(unsigned char req, unsigned char res[])
{
	int i;
	for (i = 0; i < 7; i++) {
		if (OneWireSession(req, res)) {
			return 1;
		}
		{
			volatile int j = 275*1000;
			while(j--);
		}
	}
	return 0;
}

static void OneWire_BaseInit(void)
{
	// Init Timer 3
	StopTimer();
	rTCFG0 = 0x0101;
	rTCFG1 &= ~(0xF<12); 
	rTCNTB3 = (CHIP_PCLK/SYS_TIMER_PRESCALER/SYS_TIMER_DIVIDER / SAMPLE_BPS - 1);
	rTINT_CSTAT = TINT_CSTAT_INTMASK(rTINT_CSTAT) | TIMER3_PENDING_CLEAR;
	rTINT_CSTAT = TINT_CSTAT_INTMASK(rTINT_CSTAT) | TIMER3_INTERRUPT_ENABLE;

	init_1wire_pin();
}

static int OneWire_GetInfo(unsigned char *Lcd, unsigned short *FirmwareVer)
{
	unsigned char res[4];
	
#if 0
    // Debug loop
	for (;;) {
		int r = TryOneWireSession(0x40U, res);
		Uart_SendString(r ? "GetInfo " : "Dont GetInfo ");
		Uart_SendHexWORD( *(unsigned *)res );
		Uart_SendString("\r\n");
	}
#endif
	if (!TryOneWireSession(REQ_INFO, res)) {
		return 0;
	}
	*Lcd = res[3];
	*FirmwareVer = res[2] * 0x100 + res[1];
	return 1;
}

static int OneWire_SetBacklightOfLCD(unsigned Brightness)
{
	unsigned char res[4];
	int ret;
	if (Brightness > 127) {
		Brightness = 127;
	}
	printf("Backlight: %d\n", Brightness);
	ret = TryOneWireSession(Brightness|0x80U, res);
	return ret;
}

unsigned char crc8(unsigned v, unsigned len)
{
	unsigned char crc = 0xACU;
	while (len--) {
		if (( crc & 0x80U) != 0) {
			crc <<= 1;
			crc ^= 0x7U;
		} else {
			crc <<= 1;
		}
		if ( (v & (1U << 31)) != 0) {
			crc ^= 0x7U;
		}
		v <<= 1;
	}
	return crc;
}

/* -------------------------------------------------------------- */
#include "i2c.h"

static int bus_type = -1;

#define ONEWIRE_I2C_ADDR	0x2f

#ifndef DBGOUT
#define DBGOUT(msg...)		do {} while (0)
#endif

static int onewire_i2c_do_request(unsigned char req, unsigned char *buf)
{
	unsigned char tx[4];
	int ret;

	tx[0] = req;
	tx[1] = crc8(req << 24, 8);

	if (i2c_write(ONEWIRE_I2C_ADDR, 0, 0, tx, 2))
		return -EIO;

	if (!buf) /* NO READ */
		return 0;

	if (i2c_read(ONEWIRE_I2C_ADDR, 0, 0, buf, 4))
		return -EIO;

	ret = crc8((buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8), 24);
	DBGOUT("req = %02X, res = %02X%02X%02X%02X, ret = %02x\n",
			req, buf[0], buf[1], buf[2], buf[3], ret);

	return (ret == buf[3]) ? 0 : -EIO;
}

void InitOneWire(void)
{
	i2c_init(CFG_I2C_SPEED, CFG_I2C_SLAVE);

	if (i2c_probe(ONEWIRE_I2C_ADDR))
		OneWire_BaseInit();
}

int GetInfo(unsigned char *lcd, unsigned short *fw_ver)
{
	unsigned char buf[4];
	int ret;

	ret = onewire_i2c_do_request(REQ_INFO, buf);
	if (!ret) {
		*lcd = buf[0];
		*fw_ver = buf[1] * 0x100 + buf[2];

		bus_type = BUS_I2C;
		return 1;
	}

	return OneWire_GetInfo(lcd, fw_ver);
}

int SetBacklightOfLCD(unsigned brightness)
{
	if (brightness > 127) {
		brightness = 127;
	}

	if (bus_type == BUS_I2C) {
		onewire_i2c_do_request((REQ_BL | brightness), 0);
		return 1;
	}

	return OneWire_SetBacklightOfLCD(brightness);
}


