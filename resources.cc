#include <gio/gio.h>

#if defined (__ELF__) && ( __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 6))
# define SECTION __attribute__ ((section (".gresource.resources"), aligned (sizeof(void *) > 8 ? sizeof(void *) : 8)))
#else
# define SECTION
#endif

static const SECTION union { const guint8 data[1368]; const double alignment; void * const ptr;}  resources_resource_data = {
  "\107\126\141\162\151\141\156\164\000\000\000\000\000\000\000\000"
  "\030\000\000\000\344\000\000\000\000\000\000\050\007\000\000\000"
  "\000\000\000\000\000\000\000\000\001\000\000\000\003\000\000\000"
  "\004\000\000\000\005\000\000\000\006\000\000\000\003\337\244\201"
  "\002\000\000\000\344\000\000\000\007\000\166\000\360\000\000\000"
  "\013\001\000\000\324\265\002\000\377\377\377\377\013\001\000\000"
  "\001\000\114\000\014\001\000\000\024\001\000\000\235\067\312\174"
  "\001\000\000\000\024\001\000\000\005\000\114\000\034\001\000\000"
  "\040\001\000\000\231\027\304\163\001\000\000\000\040\001\000\000"
  "\007\000\114\000\050\001\000\000\064\001\000\000\115\376\204\264"
  "\003\000\000\000\064\001\000\000\013\000\166\000\100\001\000\000"
  "\273\002\000\000\144\342\102\021\003\000\000\000\273\002\000\000"
  "\011\000\166\000\310\002\000\000\037\004\000\000\051\245\073\346"
  "\003\000\000\000\037\004\000\000\013\000\166\000\060\004\000\000"
  "\127\005\000\000\126\105\122\123\111\117\116\000\000\000\000\000"
  "\013\000\000\000\000\000\000\000\060\056\061\056\060\055\141\154"
  "\160\150\141\000\000\050\165\165\141\171\051\057\002\000\000\000"
  "\003\000\000\000\144\141\164\141\057\000\000\000\000\000\000\000"
  "\151\155\141\147\145\163\057\000\006\000\000\000\004\000\000\000"
  "\005\000\000\000\167\141\162\156\151\156\147\056\163\166\147\000"
  "\153\001\000\000\000\000\000\000\074\163\166\147\040\170\155\154"
  "\156\163\075\042\150\164\164\160\072\057\057\167\167\167\056\167"
  "\063\056\157\162\147\057\062\060\060\060\057\163\166\147\042\040"
  "\166\151\145\167\102\157\170\075\042\060\040\060\040\062\064\040"
  "\062\064\042\040\146\151\154\154\075\042\156\157\156\145\042\040"
  "\163\164\162\157\153\145\075\042\143\165\162\162\145\156\164\103"
  "\157\154\157\162\042\040\163\164\162\157\153\145\055\167\151\144"
  "\164\150\075\042\062\042\040\163\164\162\157\153\145\055\154\151"
  "\156\145\143\141\160\075\042\162\157\165\156\144\042\040\163\164"
  "\162\157\153\145\055\154\151\156\145\152\157\151\156\075\042\162"
  "\157\165\156\144\042\076\012\040\040\040\040\074\160\141\164\150"
  "\040\144\075\042\115\061\060\056\062\071\040\063\056\070\066\114"
  "\061\056\070\062\040\061\070\141\062\040\062\040\060\040\060\040"
  "\060\040\061\056\067\061\040\063\150\061\066\056\071\064\141\062"
  "\040\062\040\060\040\060\040\060\040\061\056\067\061\055\063\114"
  "\061\063\056\067\061\040\063\056\070\066\141\062\040\062\040\060"
  "\040\060\040\060\055\063\056\064\062\040\060\172\042\040\146\151"
  "\154\154\075\042\171\145\154\154\157\167\042\057\076\012\040\040"
  "\040\040\074\154\151\156\145\040\170\061\075\042\061\062\042\040"
  "\171\061\075\042\071\042\040\170\062\075\042\061\062\042\040\171"
  "\062\075\042\061\063\042\057\076\012\040\040\040\040\074\143\151"
  "\162\143\154\145\040\143\170\075\042\061\062\042\040\143\171\075"
  "\042\061\067\042\040\162\075\042\061\042\057\076\012\074\057\163"
  "\166\147\076\000\000\050\165\165\141\171\051\167\162\157\156\147"
  "\056\163\166\147\000\000\000\000\107\001\000\000\000\000\000\000"
  "\074\163\166\147\040\170\155\154\156\163\075\042\150\164\164\160"
  "\072\057\057\167\167\167\056\167\063\056\157\162\147\057\062\060"
  "\060\060\057\163\166\147\042\040\166\151\145\167\102\157\170\075"
  "\042\060\040\060\040\062\064\040\062\064\042\040\167\151\144\164"
  "\150\075\042\062\064\042\040\150\145\151\147\150\164\075\042\062"
  "\064\042\040\146\151\154\154\075\042\162\145\144\042\076\012\040"
  "\040\040\040\074\143\151\162\143\154\145\040\143\170\075\042\061"
  "\062\042\040\143\171\075\042\061\062\042\040\162\075\042\061\060"
  "\042\040\163\164\162\157\153\145\075\042\162\145\144\042\040\163"
  "\164\162\157\153\145\055\167\151\144\164\150\075\042\062\042\040"
  "\146\151\154\154\075\042\156\157\156\145\042\040\057\076\012\040"
  "\040\040\040\074\154\151\156\145\040\170\061\075\042\070\042\040"
  "\171\061\075\042\070\042\040\170\062\075\042\061\066\042\040\171"
  "\062\075\042\061\066\042\040\163\164\162\157\153\145\075\042\162"
  "\145\144\042\040\163\164\162\157\153\145\055\167\151\144\164\150"
  "\075\042\062\042\040\057\076\012\040\040\040\040\074\154\151\156"
  "\145\040\170\061\075\042\061\066\042\040\171\061\075\042\070\042"
  "\040\170\062\075\042\070\042\040\171\062\075\042\061\066\042\040"
  "\163\164\162\157\153\145\075\042\162\145\144\042\040\163\164\162"
  "\157\153\145\055\167\151\144\164\150\075\042\062\042\040\057\076"
  "\012\074\057\163\166\147\076\000\000\050\165\165\141\171\051\143"
  "\157\162\162\145\143\164\056\163\166\147\000\000\000\000\000\000"
  "\027\001\000\000\000\000\000\000\074\163\166\147\040\170\155\154"
  "\156\163\075\042\150\164\164\160\072\057\057\167\167\167\056\167"
  "\063\056\157\162\147\057\062\060\060\060\057\163\166\147\042\040"
  "\166\151\145\167\102\157\170\075\042\060\040\060\040\062\064\040"
  "\062\064\042\040\146\151\154\154\075\042\156\157\156\145\042\040"
  "\163\164\162\157\153\145\075\042\147\162\145\145\156\042\040\163"
  "\164\162\157\153\145\055\167\151\144\164\150\075\042\062\042\040"
  "\163\164\162\157\153\145\055\154\151\156\145\143\141\160\075\042"
  "\162\157\165\156\144\042\040\163\164\162\157\153\145\055\154\151"
  "\156\145\152\157\151\156\075\042\162\157\165\156\144\042\076\012"
  "\040\040\040\040\074\143\151\162\143\154\145\040\143\170\075\042"
  "\061\062\042\040\143\171\075\042\061\062\042\040\162\075\042\061"
  "\060\042\040\163\164\162\157\153\145\075\042\147\162\145\145\156"
  "\042\040\146\151\154\154\075\042\156\157\156\145\042\057\076\012"
  "\040\040\040\040\074\160\141\164\150\040\144\075\042\115\071\040"
  "\061\062\154\062\040\062\154\064\055\064\042\040\163\164\162\157"
  "\153\145\075\042\147\162\145\145\156\042\040\146\151\154\154\075"
  "\042\156\157\156\145\042\057\076\012\074\057\163\166\147\076\000"
  "\000\050\165\165\141\171\051" };

static GStaticResource static_resource = { resources_resource_data.data, sizeof (resources_resource_data.data) - 1 /* nul terminator */, NULL, NULL, NULL };

G_MODULE_EXPORT
GResource *resources_get_resource (void);
GResource *resources_get_resource (void)
{
  return g_static_resource_get_resource (&static_resource);
}
/* GLIB - Library of useful routines for C programming
 * Copyright (C) 1995-1997  Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Modified by the GLib Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GLib Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GLib at ftp://ftp.gtk.org/pub/gtk/.
 */

#ifndef __G_CONSTRUCTOR_H__
#define __G_CONSTRUCTOR_H__

/*
  If G_HAS_CONSTRUCTORS is true then the compiler support *both* constructors and
  destructors, in a usable way, including e.g. on library unload. If not you're on
  your own.

  Some compilers need #pragma to handle this, which does not work with macros,
  so the way you need to use this is (for constructors):

  #ifdef G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA
  #pragma G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(my_constructor)
  #endif
  G_DEFINE_CONSTRUCTOR(my_constructor)
  static void my_constructor(void) {
   ...
  }

*/

#ifndef __GTK_DOC_IGNORE__

#if  __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 7)

#define G_HAS_CONSTRUCTORS 1

#define G_DEFINE_CONSTRUCTOR(_func) static void __attribute__((constructor)) _func (void);
#define G_DEFINE_DESTRUCTOR(_func) static void __attribute__((destructor)) _func (void);

#elif defined (_MSC_VER)

/*
 * Only try to include gslist.h if not already included via glib.h,
 * so that items using gconstructor.h outside of GLib (such as
 * GResources) continue to build properly.
 */
#ifndef __G_LIB_H__
#include "gslist.h"
#endif

#include <stdlib.h>

#define G_HAS_CONSTRUCTORS 1

/* We do some weird things to avoid the constructors being optimized
 * away on VS2015 if WholeProgramOptimization is enabled. First we
 * make a reference to the array from the wrapper to make sure its
 * references. Then we use a pragma to make sure the wrapper function
 * symbol is always included at the link stage. Also, the symbols
 * need to be extern (but not dllexport), even though they are not
 * really used from another object file.
 */

/* We need to account for differences between the mangling of symbols
 * for x86 and x64/ARM/ARM64 programs, as symbols on x86 are prefixed
 * with an underscore but symbols on x64/ARM/ARM64 are not.
 */
#ifdef _M_IX86
#define G_MSVC_SYMBOL_PREFIX "_"
#else
#define G_MSVC_SYMBOL_PREFIX ""
#endif

#define G_DEFINE_CONSTRUCTOR(_func) G_MSVC_CTOR (_func, G_MSVC_SYMBOL_PREFIX)
#define G_DEFINE_DESTRUCTOR(_func) G_MSVC_DTOR (_func, G_MSVC_SYMBOL_PREFIX)

#define G_MSVC_CTOR(_func,_sym_prefix) \
  static void _func(void); \
  extern int (* _array ## _func)(void);              \
  int _func ## _wrapper(void);              \
  int _func ## _wrapper(void) { _func(); g_slist_find (NULL,  _array ## _func); return 0; } \
  __pragma(comment(linker,"/include:" _sym_prefix # _func "_wrapper")) \
  __pragma(section(".CRT$XCU",read)) \
  __declspec(allocate(".CRT$XCU")) int (* _array ## _func)(void) = _func ## _wrapper;

#define G_MSVC_DTOR(_func,_sym_prefix) \
  static void _func(void); \
  extern int (* _array ## _func)(void);              \
  int _func ## _constructor(void);              \
  int _func ## _constructor(void) { atexit (_func); g_slist_find (NULL,  _array ## _func); return 0; } \
   __pragma(comment(linker,"/include:" _sym_prefix # _func "_constructor")) \
  __pragma(section(".CRT$XCU",read)) \
  __declspec(allocate(".CRT$XCU")) int (* _array ## _func)(void) = _func ## _constructor;

#elif defined(__SUNPRO_C)

/* This is not tested, but i believe it should work, based on:
 * http://opensource.apple.com/source/OpenSSL098/OpenSSL098-35/src/fips/fips_premain.c
 */

#define G_HAS_CONSTRUCTORS 1

#define G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA 1
#define G_DEFINE_DESTRUCTOR_NEEDS_PRAGMA 1

#define G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(_func) \
  init(_func)
#define G_DEFINE_CONSTRUCTOR(_func) \
  static void _func(void);

#define G_DEFINE_DESTRUCTOR_PRAGMA_ARGS(_func) \
  fini(_func)
#define G_DEFINE_DESTRUCTOR(_func) \
  static void _func(void);

#else

/* constructors not supported for this compiler */

#endif

#endif /* __GTK_DOC_IGNORE__ */
#endif /* __G_CONSTRUCTOR_H__ */

#ifdef G_HAS_CONSTRUCTORS

#ifdef G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA
#pragma G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(resourcesresource_constructor)
#endif
G_DEFINE_CONSTRUCTOR(resourcesresource_constructor)
#ifdef G_DEFINE_DESTRUCTOR_NEEDS_PRAGMA
#pragma G_DEFINE_DESTRUCTOR_PRAGMA_ARGS(resourcesresource_destructor)
#endif
G_DEFINE_DESTRUCTOR(resourcesresource_destructor)

#else
#warning "Constructor not supported on this compiler, linking in resources will not work"
#endif

static void resourcesresource_constructor (void)
{
  g_static_resource_init (&static_resource);
}

static void resourcesresource_destructor (void)
{
  g_static_resource_fini (&static_resource);
}
