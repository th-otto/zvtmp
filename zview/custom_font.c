#include "general.h"
#include "custom_font.h"

static MFDB screen;

static short const ofwf[256] =
{
	0, 7, 7, 7, 7, 8, 8, 8, 7, 8, 7, 7, 7, 7, 6, 7,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7,
	3, 3, 5, 7, 6, 6, 7, 3, 4, 4, 6, 6, 3, 6, 2, 5,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 2, 3, 4, 5, 4, 6,
	7, 6, 6, 6, 6, 5, 5, 6, 6, 2, 5, 6, 5, 7, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 8, 6, 6, 6, 3, 5, 3, 6, 6,
	3, 5, 5, 5, 5, 5, 3, 5, 5, 4, 4, 5, 4, 8, 5, 5,
	5, 5, 4, 5, 3, 5, 5, 6, 6, 5, 6, 5, 2, 5, 7, 6,
	6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 6, 6,
	5, 8, 8, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 5,
	5, 4, 5, 5, 5, 6, 5, 5, 6, 7, 7, 8, 8, 3, 7, 7,
	5, 5, 8, 7, 8, 8, 6, 6, 6, 6, 5, 4, 8, 8, 8, 8,
	6, 6, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 8, 6, 8,
	7, 6, 6, 8, 7, 6, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 8, 8, 6, 7, 5, 5, 4, 7, 5, 5, 5, 6
};

static short const my_font_8[1280] =
{
	0x0018, 0x3C7E, 0x1818, 0x1800, 0x0018, 0x1818, 0x7E3C, 0x1800, 
	0x0008, 0x0C7E, 0x7E0C, 0x0800, 0x0010, 0x307E, 0x7E30, 0x1000, 
	0x3C99, 0xC3E7, 0xC399, 0x3C00, 0xFFFF, 0xFEFC, 0xF9F3, 0xE700, 
	0xE7C3, 0x993C, 0x99C3, 0xE700, 0x0002, 0x068C, 0xD870, 0x2000, 
	0x7EC3, 0x9195, 0x9981, 0xC37E, 0x183C, 0x3C3C, 0x7E10, 0x3810, 
	0x000E, 0x0B08, 0x0838, 0x7830, 0xF080, 0xDC90, 0x9810, 0x1000, 
	0x6080, 0x9C92, 0x7C12, 0x1200, 0x0A0A, 0x0A1A, 0x1A32, 0xF2E2, 
	0xA0A0, 0xA0B0, 0xB098, 0x9E8E, 0x3844, 0x4400, 0x4444, 0x3800, 
	0x0404, 0x0400, 0x0404, 0x0400, 0x3804, 0x0438, 0x4040, 0x3800, 
	0x7008, 0x0870, 0x0808, 0x7000, 0x4444, 0x4438, 0x0404, 0x0400, 
	0x3840, 0x4038, 0x0404, 0x3800, 0x3840, 0x4038, 0x4444, 0x3800, 
	0x7008, 0x0800, 0x0808, 0x0800, 0x3844, 0x4438, 0x4444, 0x3800, 
	0x3844, 0x4438, 0x0404, 0x3800, 0x0000, 0x7804, 0x7844, 0x3800, 
	0x7040, 0x7040, 0x7C10, 0x1C00, 0x0F1F, 0x3F30, 0x3020, 0x3C2E, 
	0xE0F0, 0xD808, 0x0808, 0x78A8, 0x2216, 0x1B0C, 0x0F5C, 0x7370, 
	0x0850, 0xB050, 0xA020, 0xC000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x4040, 0x4040, 0x4000, 0x4000, 0x4848, 0x0000, 0x0000, 0x0000, 
	0x0024, 0x7E24, 0x247E, 0x2400, 0x103C, 0x5038, 0x1478, 0x1000, 
	0x0048, 0x0810, 0x1020, 0x2400, 0x1824, 0x1830, 0x4A44, 0x3A00, 
	0x2020, 0x4000, 0x0000, 0x0000, 0x1020, 0x4040, 0x4040, 0x2010, 
	0x4020, 0x1010, 0x1010, 0x2040, 0x0028, 0x107C, 0x1028, 0x0000, 
	0x0010, 0x107C, 0x1010, 0x0000, 0x0000, 0x0000, 0x0020, 0x2040, 
	0x0000, 0x007C, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x4000, 
	0x0808, 0x1010, 0x2020, 0x4040, 0x3844, 0x4444, 0x4444, 0x3800, 
	0x1030, 0x1010, 0x1010, 0x3800, 0x3844, 0x0408, 0x1020, 0x7C00, 
	0x7C08, 0x1038, 0x0444, 0x3800, 0x0818, 0x2848, 0x7C08, 0x0800, 
	0x7C40, 0x7804, 0x0444, 0x3800, 0x1820, 0x4078, 0x4444, 0x3800, 
	0x7C04, 0x0408, 0x1010, 0x1000, 0x3844, 0x4438, 0x4444, 0x3800, 
	0x3844, 0x443C, 0x0408, 0x3000, 0x0000, 0x4000, 0x0040, 0x0000, 
	0x0000, 0x2000, 0x0020, 0x2040, 0x0010, 0x2040, 0x2010, 0x0000, 
	0x0000, 0x7800, 0x7800, 0x0000, 0x0040, 0x2010, 0x2040, 0x0000, 
	0x3844, 0x0408, 0x1000, 0x1000, 0x3C42, 0x5A52, 0x5C40, 0x3E00, 
	0x1028, 0x4444, 0x7C44, 0x4400, 0x7844, 0x4478, 0x4444, 0x7800, 
	0x3844, 0x4040, 0x4044, 0x3800, 0x7048, 0x4444, 0x4448, 0x7000, 
	0x7840, 0x4070, 0x4040, 0x7800, 0x7840, 0x4070, 0x4040, 0x4000, 
	0x3844, 0x404C, 0x4444, 0x3800, 0x4444, 0x447C, 0x4444, 0x4400, 
	0x4040, 0x4040, 0x4040, 0x4000, 0x0808, 0x0808, 0x0848, 0x3000, 
	0x4448, 0x5060, 0x5048, 0x4400, 0x4040, 0x4040, 0x4040, 0x7800, 
	0x4266, 0x5A42, 0x4242, 0x4200, 0x4464, 0x544C, 0x4444, 0x4400, 
	0x3844, 0x4444, 0x4444, 0x3800, 0x7844, 0x4478, 0x4040, 0x4000, 
	0x3844, 0x4444, 0x5448, 0x3400, 0x7844, 0x4478, 0x5048, 0x4400, 
	0x3844, 0x4038, 0x0444, 0x3800, 0x7C10, 0x1010, 0x1010, 0x1000, 
	0x4444, 0x4444, 0x4444, 0x3800, 0x4444, 0x4444, 0x2828, 0x1000, 
	0x4141, 0x4149, 0x2A2A, 0x1400, 0x4444, 0x2810, 0x2844, 0x4400, 
	0x4444, 0x4428, 0x1010, 0x1000, 0x7C04, 0x0810, 0x2040, 0x7C00, 
	0x6040, 0x4040, 0x4040, 0x6000, 0x4040, 0x2020, 0x1010, 0x0808, 
	0x6020, 0x2020, 0x2020, 0x6000, 0x1028, 0x4400, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x7C00, 0x4040, 0x2000, 0x0000, 0x0000, 
	0x0000, 0x3008, 0x3848, 0x3800, 0x4040, 0x7048, 0x4848, 0x7000, 
	0x0000, 0x3048, 0x4048, 0x3000, 0x0808, 0x3848, 0x4848, 0x3800, 
	0x0000, 0x3048, 0x7840, 0x3800, 0x3040, 0x4060, 0x4040, 0x4000, 
	0x0000, 0x3848, 0x4838, 0x0870, 0x4040, 0x7048, 0x4848, 0x4800, 
	0x2000, 0x6020, 0x2020, 0x7000, 0x1000, 0x1010, 0x1010, 0x5020, 
	0x4040, 0x4850, 0x6050, 0x4800, 0x6020, 0x2020, 0x2020, 0x7000, 
	0x0000, 0x7649, 0x4949, 0x4900, 0x0000, 0x7048, 0x4848, 0x4800, 
	0x0000, 0x3048, 0x4848, 0x3000, 0x0000, 0x7048, 0x4848, 0x7040, 
	0x0000, 0x3848, 0x4848, 0x3808, 0x0000, 0x5060, 0x4040, 0x4000, 
	0x0000, 0x3840, 0x3008, 0x7000, 0x4040, 0x6040, 0x4040, 0x2000, 
	0x0000, 0x4848, 0x4848, 0x3800, 0x0000, 0x4848, 0x4850, 0x2000, 
	0x0000, 0x4444, 0x5438, 0x2800, 0x0000, 0x4428, 0x1028, 0x4400, 
	0x0000, 0x4848, 0x4838, 0x0870, 0x0000, 0x7C08, 0x1020, 0x7C00, 
	0x1820, 0x2040, 0x2020, 0x1800, 0x4040, 0x4040, 0x4040, 0x4040, 
	0x6010, 0x1008, 0x1010, 0x6000, 0x0020, 0x724E, 0x0400, 0x0000, 
	0x0010, 0x2828, 0x447C, 0x0000, 0x3844, 0x4040, 0x4438, 0x1020, 
	0x4800, 0x4848, 0x4848, 0x3800, 0x1020, 0x3048, 0x7840, 0x3800, 
	0x3048, 0x3008, 0x3848, 0x3800, 0x2800, 0x3008, 0x3848, 0x3800, 
	0x1008, 0x3008, 0x3848, 0x3800, 0x1010, 0x3008, 0x3848, 0x3800, 
	0x0030, 0x4840, 0x4830, 0x1060, 0x1028, 0x3048, 0x7840, 0x3800, 
	0x2800, 0x3048, 0x7840, 0x3800, 0x2010, 0x3048, 0x7840, 0x3800, 
	0x5000, 0x6020, 0x2020, 0x7000, 0x2050, 0x6020, 0x2020, 0x7000, 
	0x2010, 0x6020, 0x2020, 0x7000, 0x2800, 0x1028, 0x447C, 0x4400, 
	0x1000, 0x1028, 0x447C, 0x4400, 0x1020, 0x7840, 0x7040, 0x7800, 
	0x0000, 0x3E09, 0x3F48, 0x3E00, 0x1F38, 0x484E, 0x7848, 0x4F00, 
	0x3048, 0x3048, 0x4848, 0x3000, 0x4800, 0x3048, 0x4848, 0x3000, 
	0x2010, 0x3048, 0x4848, 0x3000, 0x3048, 0x0048, 0x4848, 0x3800, 
	0x2010, 0x0048, 0x4848, 0x3800, 0x4800, 0x4848, 0x4838, 0x0870, 
	0x2800, 0x3844, 0x4444, 0x3800, 0x4400, 0x4444, 0x4444, 0x3800, 
	0x1038, 0x5450, 0x5438, 0x1000, 0x1824, 0x2078, 0x2020, 0x7800, 
	0x4444, 0x3810, 0x7C10, 0x1000, 0x1824, 0x4478, 0x4444, 0x7840, 
	0x1820, 0x2070, 0x2020, 0x4000, 0x1020, 0x3008, 0x3848, 0x3800, 
	0x2040, 0x0060, 0x2020, 0x7000, 0x1020, 0x0030, 0x4848, 0x3000, 
	0x1020, 0x0048, 0x4848, 0x3800, 0x2850, 0x0070, 0x4848, 0x4800, 
	0x3458, 0x0064, 0x544C, 0x4400, 0x0030, 0x0838, 0x4838, 0x0078, 
	0x0030, 0x4848, 0x4830, 0x0078, 0x1000, 0x1020, 0x4044, 0x3800, 
	0x0000, 0x0000, 0x1E10, 0x1000, 0x0000, 0x0000, 0x7808, 0x0800, 
	0x4244, 0x4816, 0x2142, 0x0700, 0x4244, 0x4816, 0x2A4F, 0x0200, 
	0x0040, 0x0040, 0x4040, 0x4040, 0x0012, 0x2448, 0x2412, 0x0000, 
	0x0048, 0x2412, 0x2448, 0x0000, 0x2850, 0x0030, 0x0878, 0x3800, 
	0x2850, 0x0030, 0x4848, 0x3000, 0x1D22, 0x262A, 0x3222, 0x5C00, 
	0x0000, 0x1A24, 0x3C24, 0x5800, 0x0000, 0x3E49, 0x4F48, 0x3E00, 
	0x3F48, 0x484E, 0x4848, 0x3F00, 0x1008, 0x1028, 0x447C, 0x4400, 
	0x3458, 0x1028, 0x447C, 0x4400, 0x3458, 0x0038, 0x4444, 0x3800, 
	0x2400, 0x0000, 0x0000, 0x0000, 0x1020, 0x4000, 0x0000, 0x0000, 
	0x2070, 0x2020, 0x0000, 0x0000, 0x3A4A, 0x4A4A, 0x3A0A, 0x0A0A, 
	0x3C42, 0x99A1, 0xA199, 0x423C, 0x3C42, 0xB9A5, 0xB9A5, 0x423C, 
	0xEA4E, 0x4A4A, 0x0000, 0x0000, 0x2400, 0x6424, 0x2474, 0x0418, 
	0x7424, 0x2424, 0x2474, 0x0418, 0x0000, 0x1038, 0x7C10, 0x1010, 
	0x1010, 0x107C, 0x3810, 0x0000, 0x0010, 0x18FC, 0x1810, 0x0000, 
	0x0008, 0x183F, 0x1808, 0x0000, 0x1010, 0x101F, 0x0000, 0x0000, 
	0x0000, 0x00F0, 0x1010, 0x1010, 0x1010, 0x10F0, 0x0000, 0x0000, 
	0x0000, 0x001F, 0x1010, 0x1010, 0x0000, 0x00FF, 0x0000, 0x0000, 
	0x1010, 0x1010, 0x1010, 0x1010, 0x1010, 0x10FF, 0x1010, 0x1010, 
	0x1010, 0x101F, 0x1010, 0x1010, 0x0000, 0x00FF, 0x1010, 0x1010, 
	0x1010, 0x10F0, 0x1010, 0x1010, 0x1010, 0x10FF, 0x0000, 0x0000, 
	0x0010, 0x181C, 0x1C18, 0x1000, 0x0008, 0x1838, 0x3818, 0x0800, 
	0x0024, 0x343C, 0x3C34, 0x2400, 0x0024, 0x2C3C, 0x3C2C, 0x2400, 
	0x0048, 0x6C7E, 0x7E6C, 0x4800, 0x0012, 0x367E, 0x7E36, 0x1200, 
	0x0010, 0x387C, 0x007C, 0x0000, 0x007C, 0x7C7C, 0x7C7C, 0x0000, 
	0x0038, 0x7C7C, 0x7C38, 0x0000, 0x3C42, 0x8181, 0x8250, 0x3810, 
	0x1054, 0x9282, 0x8244, 0x3800, 0x0004, 0x0404, 0x247C, 0x2000, 
	0x0609, 0x1C22, 0x221C, 0x4830, 0x0000, 0x1028, 0x4400, 0x0000, 
	0x0036, 0x4949, 0x3600, 0x0000, 0x0000, 0x324C, 0x484C, 0x3200, 
	0x1824, 0x4478, 0x4444, 0x7840, 0x7C24, 0x2020, 0x2020, 0x7000, 
	0x0001, 0x3E54, 0x1414, 0x1400, 0x7E22, 0x1008, 0x1022, 0x7E00, 
	0x001C, 0x3048, 0x4848, 0x3000, 0x0000, 0x2424, 0x2424, 0x3A40, 
	0x0004, 0x3850, 0x1010, 0x1000, 0x3810, 0x3844, 0x4438, 0x1038, 
	0x0038, 0x447C, 0x4444, 0x3800, 0x0038, 0x4444, 0x4428, 0x6C00, 
	0x3020, 0x1038, 0x4444, 0x3800, 0x0408, 0x3854, 0x5438, 0x2040, 
	0x1038, 0x5454, 0x5438, 0x1000, 0x3C60, 0x407C, 0x4060, 0x3C00, 
	0x386C, 0x4444, 0x4444, 0x4400, 0x007C, 0x007C, 0x007C, 0x0000, 
	0x0010, 0x107C, 0x1010, 0x007C, 0x0020, 0x1008, 0x1020, 0x007C, 
	0x0008, 0x1020, 0x1008, 0x007C, 0x0609, 0x0808, 0x0808, 0x0808, 
	0x0808, 0x0808, 0x0808, 0x4830, 0x0010, 0x007C, 0x0010, 0x0000, 
	0x0032, 0x4C00, 0x324C, 0x0000, 0x3048, 0x4830, 0x0000, 0x0000, 
	0x3078, 0x7830, 0x0000, 0x0000, 0x0000, 0x0000, 0x3030, 0x0000, 
	0x0006, 0x0808, 0x6828, 0x1000, 0x3048, 0x4848, 0x0000, 0x0000, 
	0x3048, 0x1020, 0x7800, 0x0000, 0x7008, 0x3008, 0x7000, 0x0000, 
	0xFE00, 0x0000, 0x0000, 0x0000
};


void draw_text( int16 vdih, int16 xf, int16 yf, int16 color, const char *str)
{
	MFDB pic;
	short fx_mem[16];
	short sys_pxy[8];
	short char_width;
	short tcolor[2];
	unsigned short c;
	
	pic.fd_addr = fx_mem;
	pic.fd_w = 16;
	pic.fd_h = 8;
	pic.fd_wdwidth = 1;
	pic.fd_stand = 0;
	pic.fd_nplanes = 1;
	tcolor[0] = color;
	tcolor[1] = G_WHITE;

	sys_pxy[0] = 0;
	sys_pxy[1] = 0;
	sys_pxy[2] = 0;
	sys_pxy[3] = 7;
	sys_pxy[4] = xf;
	sys_pxy[5] = yf;
	sys_pxy[6] = 0;
	sys_pxy[7] = yf + 7;
	
	while ( (c = (unsigned char)*str++) != 0) {
		const char *model = (const char*) &my_font_8[(c - 1) << 2] ;
		char *dest = (char*)fx_mem;
		
		char_width = ofwf[c];
		
		memset(fx_mem,0,20);
		*dest = *model++; dest+=2;
		*dest = *model++; dest+=2;
		*dest = *model++; dest+=2;
		*dest = *model++; dest+=2;
		*dest = *model++; dest+=2;
		*dest = *model++; dest+=2;
		*dest = *model++; dest+=2;
		*dest = *model;
		
		sys_pxy[2] = char_width - 1;
		sys_pxy[6] = sys_pxy[4] + sys_pxy[2];
		
		/*vdi*/
		vrt_cpyfm( vdih, 2, sys_pxy, &pic, &screen, tcolor);
		
		sys_pxy[4] += char_width;
	}
}	


int16 get_text_width( const char *str)
{
	int16 c, text_width = 0;
	
	while ( (c = *str++) != 0) 
	{
		text_width += ofwf[c];
	}

	return text_width;
}	


int16 name_shorter( int16 max_size, char *str)
{
	int16 len, current_len;

	len = ( int16) strlen( str) - 1;
	
	do
	{
		str[len - 3] 	= '.';
		str[len - 2] 	= '.';
		str[len - 1] 	= '.';
		str[len] 		= '\0';

		len--;		

		current_len = get_text_width( str);
		
	} while ( ( current_len >= max_size) && ( len > 0));

	return current_len;
}
