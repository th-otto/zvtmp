#include "general.h"
boolean __CDECL reader_init( const char *name, IMGINFO info);
boolean __CDECL reader_read( IMGINFO info, uint8_t *buffer);
void __CDECL reader_get_txt( IMGINFO info, txt_data *txtdata);
void __CDECL reader_quit( IMGINFO info);
boolean __CDECL encoder_init(const char *name, IMGINFO info);
boolean __CDECL encoder_write(IMGINFO info, uint8_t *buffer);
void __CDECL encoder_quit(IMGINFO info);
#ifdef PLUGIN_SLB
#include "zvplugin.h"
long __CDECL get_option(zv_int_t which);
long __CDECL set_option(zv_int_t which, zv_int_t value);
#endif

void tiff_init(void);

/* Options*/
extern uint16_t encode_compression;
extern int quality;	
