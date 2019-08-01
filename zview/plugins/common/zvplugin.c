#include "zview.h"
#include <mint/slb.h>
#include <mint/mintbind.h>
#include <errno.h>
#include "symbols.h"
#include "zvplugin.h"
#include "plugin.h"
#include "plugin_version.h"
#include "slbload.h"

static struct _zview_plugin_funcs zview_plugin_funcs;

#undef SLB_NWORDS
#define SLB_NWORDS(_nargs) ((((long)(_nargs) * 2 + 1l) << 16) | (long)(_nargs))
#undef SLB_NARGS
#define SLB_NARGS(_nargs) SLB_NWORDS(_nargs)

#ifdef __GNUC__

#define PUSH_FNQ(fn) \
		" moveq %[fn],%%d1\n" /* push function number */ \
		" move.l %%d1,-(%%a7)\n"

#define PUSH_FNL(fn) \
		" move.l %[fn],-(%%a7)\n" /* push function number */

#define LIBFUNC_(_fn, name, _nargs, pushfn) \
static void __attribute__((used)) __ ## name ## _entry(SLB *lib) \
{ \
	__asm__ __volatile__ ( \
		" .globl " C_SYMBOL_NAME(name) "\n" \
C_SYMBOL_NAME(name) ":\n"); \
 \
	__asm__ __volatile__ ( \
		" move.l (%%a7)+,%%d0\n" /* get return pc */ \
		" move.l (%%a7),%%a0\n" /* get SLB pointer */ \
		" move.l %[nargs],(%%a7)\n"  /* push number of args */ \
		pushfn /* push function number */ \
		" move.l (%%a0),-(%%a7)\n" /* push SLB handle */ \
		" move.l %%d0,-(%%a7)\n" /* push return pc */ \
		" move.l 4(%%a0),%%a0\n" /* get exec function */ \
		" jmp (%%a0)\n"          /* go for it */ \
	: \
	: [fn]"i"(_fn), [nargs]"i"(SLB_NARGS(_nargs)) \
	: "cc", "memory" \
	); \
	__builtin_unreachable(); \
}

#define LIBFUNC(_fn, name, _nargs) LIBFUNC_(_fn, name, _nargs, PUSH_FNQ(fn))
#define LIBFUNC2(_fn, name, _nargs) LIBFUNC_(_fn, name, _nargs, PUSH_FNL(fn))

#endif /* __GNUC__ */

#if defined(__AHCC__)

#elif defined(__PUREC__)

static void *jmp_a0(void *) 0x4ed0;
static long move_l_a7_d1(void *, long) 0x221f;
static long move_l_a7_a1(void *, long) 0x225f;
static long push_d0(long) 0x2f00;
static long push_d1(long, long) 0x2f01;
static void *push_a0(long) 0x2f10;
static void *push_a1(void *) 0x2f09;
static void *move_4_a0_1(void *) 0x0004;
static void *move_4_a0_2(void *) 0x2068;

#define LIBFUNC(_fn, name, _nargs) \
void name(SLB *slb) \
{ \
	jmp_a0(push_a1(move_4_a0_1(move_4_a0_2(push_a0(push_d0(push_d1(move_l_a7_a1(slb, _fn), SLB_NARGS(_nargs)))))))); \
}
#define LIBFUNC2(_fn, name, _nargs) LIBFUNC(_fn, name, _nargs)

#endif

#define NOFUNC

#include "imports.h"

#undef LIBFUNC
#undef LIBFUNC2
#undef NOFUNC

#undef errno

static zv_int_t _CDECL get_errno(void)
{
	return errno;
}

long plugin_open(const char *name, const char *path, SLB *slb)
{
	long ret;
	unsigned long flags;
	long cpu = 0;

	if (!slb)
		return -EBADF;
	if (slb->handle)
		return 0;

	zview_plugin_funcs.struct_size = sizeof(zview_plugin_funcs);
	zview_plugin_funcs.int_size = sizeof(zv_int_t);
	if (zview_plugin_funcs.int_size != sizeof(long))
		return -EINVAL;
	zview_plugin_funcs.plugin_version = PLUGIN_VERSION;

#define S(x) zview_plugin_funcs.p_##x = x

	S(memset);
	S(memcpy);
	S(memchr);
	S(strlen);
	S(strcpy);
	S(strcat);
	S(strcmp);
	S(malloc);
	S(free);
	S(get_errno);
	S(strerror);
	S(open);
	S(close);
	S(read);
	S(write);
	S(lseek);
	S(fopen);
	S(fclose);
	S(fseek);
	S(fseeko);
	S(ftell);
	S(ftello);
	S(sprintf);
	S(vsnprintf);
	S(fread);
	S(fwrite);
	S(ferror);
	S(rand);
	S(srand);
#undef S
	
	ret = slb_load(name, path, PLUGIN_VERSION, &slb->handle, &slb->exec);
	if (ret < 0)
		return ret;

	/*
	 * check compile flags; that function should be as simple as to just return a constant
	 * and we can hopefully call it even on mismatched configurations
	 */
	flags = plugin_compile_flags(slb);
	Getcookie(C__CPU, &cpu);
	if (cpu >= 20)
	{
		/* should be able to use a 000 library, anyways */
	} else
	{
		if (flags & (1L << 16))
		{
			/* cpu is not 020+, but library was compiled for it */
			plugin_close(slb);
			return -EINVAL;
		}
	}
#if defined(__mcoldfire__)
	/* if cpu is cf, but library was not compiled for it... */
	if (!(flags & (1L << 17)))
#else
	/* if cpu is not cf, but library was compiled for it... */
	if (flags & (1L << 17))
#endif
	{
		plugin_close(slb);
		return -EINVAL;
	}
	
	ret = plugin_set_imports(slb, &zview_plugin_funcs);
	if (ret < 0)
	{
		plugin_close(slb);
		return ret;
	}
	
	return ret;
}


void plugin_close(SLB *slb)
{
	if (!slb || !slb->handle)
		return;
	slb_unload(slb->handle);
	slb->handle = 0;
	slb->exec = 0;
}
