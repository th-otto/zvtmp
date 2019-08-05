/*
 * tiff/exports.h - internal header with definitions of all exported functions
 *
 * Copyright (C) 2019 Thorsten Otto
 *
 * For conditions of distribution and use, see copyright file.
 */

#ifndef LIBFUNC
# error "LIBFUNC must be defined before including this file"
#endif
#ifndef LIBFUNC2
#define LIBFUNC2(_fn, name, _nargs) LIBFUNC(_fn, name, _nargs)
#endif

/*   0 */ LIBFUNC(0, slb_control, 2)
/*   1 */ LIBFUNC(1, reader_init, 2)
/*   2 */ LIBFUNC(2, reader_read, 2)
/*   3 */ LIBFUNC(3, reader_get_txt, 2)
/*   4 */ LIBFUNC(4, reader_quit, 1)
/*   5 */ LIBFUNC(5, encoder_init, 2)
/*   6 */ LIBFUNC(6, encoder_write, 2)
/*   7 */ LIBFUNC(7, encoder_quit, 1)
/*   8 */ LIBFUNC(8, get_option, 1)
/*   9 */ LIBFUNC(9, set_option, 2)

#undef LIBFUNC
#undef LIBFUNC2
#undef NOFUNC
