/***********************************************************************************
  Snes9x - Portable Super Nintendo Entertainment System (TM) emulator.

  (c) Copyright 1996 - 2002  Gary Henderson (gary.henderson@ntlworld.com),
                             Jerremy Koot (jkoot@snes9x.com)

  (c) Copyright 2002 - 2004  Matthew Kendora

  (c) Copyright 2002 - 2005  Peter Bortas (peter@bortas.org)

  (c) Copyright 2004 - 2005  Joel Yliluoma (http://iki.fi/bisqwit/)

  (c) Copyright 2001 - 2006  John Weidman (jweidman@slip.net)

  (c) Copyright 2002 - 2006  funkyass (funkyass@spam.shaw.ca),
                             Kris Bleakley (codeviolation@hotmail.com)

  (c) Copyright 2002 - 2010  Brad Jorsch (anomie@users.sourceforge.net),
                             Nach (n-a-c-h@users.sourceforge.net),
                             zones (kasumitokoduck@yahoo.com)

  (c) Copyright 2006 - 2007  nitsuja

  (c) Copyright 2009 - 2010  BearOso,
                             OV2


  BS-X C emulator code
  (c) Copyright 2005 - 2006  Dreamer Nom,
                             zones

  C4 x86 assembler and some C emulation code
  (c) Copyright 2000 - 2003  _Demo_ (_demo_@zsnes.com),
                             Nach,
                             zsKnight (zsknight@zsnes.com)

  C4 C++ code
  (c) Copyright 2003 - 2006  Brad Jorsch,
                             Nach

  DSP-1 emulator code
  (c) Copyright 1998 - 2006  _Demo_,
                             Andreas Naive (andreasnaive@gmail.com),
                             Gary Henderson,
                             Ivar (ivar@snes9x.com),
                             John Weidman,
                             Kris Bleakley,
                             Matthew Kendora,
                             Nach,
                             neviksti (neviksti@hotmail.com)

  DSP-2 emulator code
  (c) Copyright 2003         John Weidman,
                             Kris Bleakley,
                             Lord Nightmare (lord_nightmare@users.sourceforge.net),
                             Matthew Kendora,
                             neviksti

  DSP-3 emulator code
  (c) Copyright 2003 - 2006  John Weidman,
                             Kris Bleakley,
                             Lancer,
                             z80 gaiden

  DSP-4 emulator code
  (c) Copyright 2004 - 2006  Dreamer Nom,
                             John Weidman,
                             Kris Bleakley,
                             Nach,
                             z80 gaiden

  OBC1 emulator code
  (c) Copyright 2001 - 2004  zsKnight,
                             pagefault (pagefault@zsnes.com),
                             Kris Bleakley
                             Ported from x86 assembler to C by sanmaiwashi

  SPC7110 and RTC C++ emulator code used in 1.39-1.51
  (c) Copyright 2002         Matthew Kendora with research by
                             zsKnight,
                             John Weidman,
                             Dark Force

  SPC7110 and RTC C++ emulator code used in 1.52+
  (c) Copyright 2009         byuu,
                             neviksti

  S-DD1 C emulator code
  (c) Copyright 2003         Brad Jorsch with research by
                             Andreas Naive,
                             John Weidman

  S-RTC C emulator code
  (c) Copyright 2001 - 2006  byuu,
                             John Weidman

  ST010 C++ emulator code
  (c) Copyright 2003         Feather,
                             John Weidman,
                             Kris Bleakley,
                             Matthew Kendora

  Super FX x86 assembler emulator code
  (c) Copyright 1998 - 2003  _Demo_,
                             pagefault,
                             zsKnight

  Super FX C emulator code
  (c) Copyright 1997 - 1999  Ivar,
                             Gary Henderson,
                             John Weidman

  Sound emulator code used in 1.5-1.51
  (c) Copyright 1998 - 2003  Brad Martin
  (c) Copyright 1998 - 2006  Charles Bilyue'

  Sound emulator code used in 1.52+
  (c) Copyright 2004 - 2007  Shay Green (gblargg@gmail.com)

  SH assembler code partly based on x86 assembler code
  (c) Copyright 2002 - 2004  Marcus Comstedt (marcus@mc.pp.se)

  2xSaI filter
  (c) Copyright 1999 - 2001  Derek Liauw Kie Fa

  HQ2x, HQ3x, HQ4x filters
  (c) Copyright 2003         Maxim Stepin (maxim@hiend3d.com)

  NTSC filter
  (c) Copyright 2006 - 2007  Shay Green

  GTK+ GUI code
  (c) Copyright 2004 - 2010  BearOso

  Win32 GUI code
  (c) Copyright 2003 - 2006  blip,
                             funkyass,
                             Matthew Kendora,
                             Nach,
                             nitsuja
  (c) Copyright 2009 - 2010  OV2

  Mac OS GUI code
  (c) Copyright 1998 - 2001  John Stiles
  (c) Copyright 2001 - 2010  zones


  Specific ports contains the works of other authors. See headers in
  individual files.


  Snes9x homepage: http://www.snes9x.com/

  Permission to use, copy, modify and/or distribute Snes9x in both binary
  and source form, for non-commercial purposes, is hereby granted without
  fee, providing that this license information and copyright notice appear
  with all copies and any derived work.

  This software is provided 'as-is', without any express or implied
  warranty. In no event shall the authors be held liable for any damages
  arising from the use of this software or it's derivatives.

  Snes9x is freeware for PERSONAL USE only. Commercial users should
  seek permission of the copyright holders first. Commercial use includes,
  but is not limited to, charging money for Snes9x or software derived from
  Snes9x, including Snes9x or derivatives in commercial game bundles, and/or
  using Snes9x as a promotion for your commercial product.

  The copyright holders request that bug fixes and improvements to the code
  should be forwarded to them so everyone can benefit from the modifications
  in future versions.

  Super NES and Super Nintendo Entertainment System are trademarks of
  Nintendo Co., Limited and its subsidiary companies.
 ***********************************************************************************/

/*****
 * S-RTC emulation code
 * Copyright (c) byuu
 *****/


#include <limits>

#include "snes9x.h"
#include "memmap.h"
#include "srtc.h"
#include "display.h"

#define memory_cartrtc_read(a)		RTCData.reg[(a)]
#define memory_cartrtc_write(a, b)	{ RTCData.reg[(a)] = (b); }
#define cpu_regs_mdr			OpenBus

static inline unsigned max (unsigned a, unsigned b)
{
	return ((a > b) ? a : b);
}

static inline unsigned min (unsigned a, unsigned b)
{
	return ((a < b) ? a : b);
}

static enum RTC_Mode { RTCM_Ready, RTCM_Command, RTCM_Read, RTCM_Write } rtc_mode;
static signed rtc_index;

static const unsigned months[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

static void srtcemu_update_time()
{
	time_t rtc_time
		= (memory_cartrtc_read(16) <<  0)
		| (memory_cartrtc_read(17) <<  8)
		| (memory_cartrtc_read(18) << 16)
		| (memory_cartrtc_read(19) << 24);
	time_t current_time = time(0);

	//sizeof(time_t) is platform-dependent; though memory::cartrtc needs to be platform-agnostic.
	//yet platforms with 32-bit signed time_t will overflow every ~68 years. handle this by
	//accounting for overflow at the cost of 1-bit precision (to catch underflow). this will allow
	//memory::cartrtc timestamp to remain valid for up to ~34 years from the last update, even if
	//time_t overflows. calculation should be valid regardless of number representation, time_t size,
	//or whether time_t is signed or unsigned.
	time_t diff
		= (current_time >= rtc_time)
		? (current_time - rtc_time)
		: (std::numeric_limits<time_t>::max() - rtc_time + current_time + 1);  //compensate for overflow
	if(diff > std::numeric_limits<time_t>::max() / 2) diff = 0;            //compensate for underflow

	if(diff > 0) {
		unsigned second  = memory_cartrtc_read( 0) + memory_cartrtc_read( 1) * 10;
		unsigned minute  = memory_cartrtc_read( 2) + memory_cartrtc_read( 3) * 10;
		unsigned hour    = memory_cartrtc_read( 4) + memory_cartrtc_read( 5) * 10;
		unsigned day     = memory_cartrtc_read( 6) + memory_cartrtc_read( 7) * 10;
		unsigned month   = memory_cartrtc_read( 8);
		unsigned year    = memory_cartrtc_read( 9) + memory_cartrtc_read(10) * 10 + memory_cartrtc_read(11) * 100;
		unsigned weekday = memory_cartrtc_read(12);

		day--;
		month--;
		year += 1000;

		second += diff;
		while(second >= 60) {
			second -= 60;

			minute++;
			if(minute < 60) continue;
			minute = 0;

			hour++;
			if(hour < 24) continue;
			hour = 0;

			day++;
			weekday = (weekday + 1) % 7;
			unsigned days = months[month % 12];
			if(days == 28) {
				bool leapyear = false;
				if((year % 4) == 0) {
					leapyear = true;
					if((year % 100) == 0 && (year % 400) != 0) leapyear = false;
				}
				if(leapyear) days++;
			}
			if(day < days) continue;
			day = 0;

			month++;
			if(month < 12) continue;
			month = 0;

			year++;
		}

		day++;
		month++;
		year -= 1000;

		memory_cartrtc_write( 0, second % 10);
		memory_cartrtc_write( 1, second / 10);
		memory_cartrtc_write( 2, minute % 10);
		memory_cartrtc_write( 3, minute / 10);
		memory_cartrtc_write( 4, hour % 10);
		memory_cartrtc_write( 5, hour / 10);
		memory_cartrtc_write( 6, day % 10);
		memory_cartrtc_write( 7, day / 10);
		memory_cartrtc_write( 8, month);
		memory_cartrtc_write( 9, year % 10);
		memory_cartrtc_write(10, (year / 10) % 10);
		memory_cartrtc_write(11, year / 100);
		memory_cartrtc_write(12, weekday % 7);
	}

	memory_cartrtc_write(16, current_time >>  0);
	memory_cartrtc_write(17, current_time >>  8);
	memory_cartrtc_write(18, current_time >> 16);
	memory_cartrtc_write(19, current_time >> 24);
}

#define srtcemu_reset() \
  rtc_mode = RTCM_Read; \
  rtc_index = -1; \
  srtcemu_update_time();

//returns day of week for specified date
//eg 0 = Sunday, 1 = Monday, ... 6 = Saturday
//usage: weekday(2008, 1, 1) returns weekday of January 1st, 2008
static unsigned srtcemu_weekday(unsigned year, unsigned month, unsigned day)
{
	unsigned y = 1900, m = 1;  //epoch is 1900-01-01
	unsigned sum = 0;          //number of days passed since epoch

	year = max(1900, year);
	month = max(1, min(12, month));
	day = max(1, min(31, day));

	while(y < year) {
		bool leapyear = false;
		if((y % 4) == 0) {
			leapyear = true;
			if((y % 100) == 0 && (y % 400) != 0) leapyear = false;
		}
		sum += leapyear ? 366 : 365;
		y++;
	}

	while(m < month) {
		unsigned days = months[m - 1];
		if(days == 28) {
			bool leapyear = false;
			if((y % 4) == 0) {
				leapyear = true;
				if((y % 100) == 0 && (y % 400) != 0) leapyear = false;
			}
			if(leapyear) days++;
		}
		sum += days;
		m++;
	}

	sum += day - 1;
	return (sum + 1) % 7;  //1900-01-01 was a Monday
}

static uint8 srtcemu_mmio_read(unsigned addr)
{
	addr &= 0xffff;

	if(addr == 0x2800)
	{
		if(rtc_mode != RTCM_Read) return 0x00;

		if(rtc_index < 0)
		{
			srtcemu_update_time();
			rtc_index++;
			return 0x0f;
		} else if(rtc_index > 12)
		{
			rtc_index = -1;
			return 0x0f;
		}
		else
			return memory_cartrtc_read(rtc_index++);
	}

	return cpu_regs_mdr;
}

static void srtcemu_mmio_write(unsigned addr, uint8 data)
{
	addr &= 0xffff;

	if(addr == 0x2801) {
		data &= 0x0f;  //only the low four bits are used

		if(data == 0x0d)
		{
			rtc_mode = RTCM_Read;
			rtc_index = -1;
			return;
		}

		if(data == 0x0e) {
			rtc_mode = RTCM_Command;
			return;
		}

		if(data == 0x0f) return;  //unknown behavior

		if(rtc_mode == RTCM_Write) {
			if(rtc_index >= 0 && rtc_index < 12) {
				memory_cartrtc_write(rtc_index++, data);

				if(rtc_index == 12) {
					//day of week is automatically calculated and written
					unsigned day   = memory_cartrtc_read( 6) + memory_cartrtc_read( 7) * 10;
					unsigned month = memory_cartrtc_read( 8);
					unsigned year  = memory_cartrtc_read( 9) + memory_cartrtc_read(10) * 10 + memory_cartrtc_read(11) * 100;
					year += 1000;

					memory_cartrtc_write(rtc_index++, srtcemu_weekday(year, month, day));
				}
			}
		} else if(rtc_mode == RTCM_Command) {
			if(data == 0) {
				rtc_mode = RTCM_Write;
				rtc_index = 0;
			} else if(data == 4) {
				rtc_mode = RTCM_Ready;
				rtc_index = -1;
				for(unsigned i = 0; i < 13; i++) memory_cartrtc_write(i, 0);
			} else {
				//unknown behavior
				rtc_mode = RTCM_Ready;
			}
		}
	}
}

void S9xInitSRTC (void)
{
	srtcemu_reset();
	memset(RTCData.reg, 0, 20);
}

void S9xResetSRTC (void)
{
	srtcemu_reset();
}

void S9xSetSRTC (uint8 data, uint16 address)
{
	srtcemu_mmio_write(address, data);
}

uint8 S9xGetSRTC (uint16 address)
{
	return (srtcemu_mmio_read(address));
}

void S9xSRTCPreSaveState (void)
{
	srtcsnap.rtc_mode  = (int32) rtc_mode;
	srtcsnap.rtc_index = (int32) rtc_index;
}

void S9xSRTCPostLoadState (int)
{
	rtc_mode  = (RTC_Mode) srtcsnap.rtc_mode;
	rtc_index = (signed)         srtcsnap.rtc_index;

	srtcemu_update_time();
}
