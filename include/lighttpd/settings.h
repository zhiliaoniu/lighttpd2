#ifndef _LIGHTTPD_SETTINGS_H_
#define _LIGHTTPD_SETTINGS_H_

/* check which OS we are compiling for */
#if defined(__BEOS__)
# define LIGHTY_OS_BEOS
#elif (defined(__APPLE__) && defined(__GNUC__)) || defined(__MACOSX__)
# define LIGHTY_OS_MACOSX
#elif defined(__FreeBSD__)
# define LIGHTY_OS_FREEBSD
#elif defined(__NetBSD__)
# define LIGHTY_OS_NETBSD
#elif defined(__OpenBSD__)
# define LIGHTY_OS_OPENBSD
#elif defined(__sgi)
# define LIGHTY_OS_IRIX
#elif defined(__AIX)
# define LIGHTY_OS_AIX
#elif defined(__LINUX__) || defined(__linux__) || defined(__linux)
# define LIGHTY_OS_LINUX
#elif defined(__SUN__) || defined(__sun) || defined(sun)
# define LIGHTY_OS_SOLARIS
#elif defined(__hpux__) || defined(__hpux)
# define LIGHTY_OS_HPUX
#elif defined(WIN64) || defined(_WIN64) || defined(__WIN64__)
# define LIGHTY_OS_WINDOWS
# define LIGHTY_OS_WIN64
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
# define LIGHTY_OS_WINDOWS
# define LIGHTY_OS_WIN32
#else
# warning "unknown OS, please report this"
#endif


#ifdef HAVE_CONFIG_H
#include <lighttpd/config.h>
#endif

#if defined HAVE_LIBSSL && defined HAVE_OPENSSL_SSL_H
# define USE_OPENSSL
# include <openssl/ssl.h>
#endif

#include <ev.h>
#include <glib.h>
#include <gmodule.h>

#define L_GOFFSET_FORMAT G_GINT64_FORMAT
#define L_GOFFSET_MODIFIER G_GINT64_MODIFIER

#define CONST_STR_LEN(x) (x), (x) ? sizeof(x) - 1 : 0

#define GSTR_LEN(x) (x) ? (x)->str : "", (x) ? (x)->len : 0
#define GSTR_SAFE_STR(x) ((x && x->str) ? x->str : "(null)")

#include <assert.h>

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif

#ifdef HAVE_STDINT_H
# include <stdint.h>
#endif

#ifdef HAVE_STDDEF_H
# include <stddef.h>
#endif

#ifdef HAVE_INTTYPES_H
# include <inttypes.h>
#endif

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#ifdef HAVE_PCRE_H
#include <pcre.h>
#endif

#include <errno.h>
#include <string.h>


/* on linux 2.4.x you get either sendfile or LFS */
#if defined(LIGHTY_OS_LINUX) && defined(HAVE_SYS_SENDFILE_H) && defined(HAVE_SENDFILE) && (!defined(_LARGEFILE_SOURCE) || defined(HAVE_SENDFILE64)) && defined(HAVE_WRITEV) && !defined(HAVE_SENDFILE_BROKEN)
# define USE_LINUX_SENDFILE
# include <sys/sendfile.h>
# include <sys/uio.h>
#endif

#if (defined(LIGHTY_OS_FREEBSD) || defined(__DragonFly__)) && defined(HAVE_SYS_UIO_H) && defined(HAVE_SENDFILE) && defined(HAVE_WRITEV)
# define USE_FREEBSD_SENDFILE
# include <sys/uio.h>
#endif

#if defined(LIGHT_OS_SOLARIS) && defined(HAVE_SYS_SENDFILE_H) && defined(HAVE_SENDFILEV) && defined(HAVE_WRITEV)
# define USE_SOLARIS_SENDFILEV
# include <sys/sendfile.h>
# include <sys/uio.h>
#endif

#if defined(LIGHTY_OS_MACOSX) && defined(HAVE_SYS_UIO_H) && defined(HAVE_SENDFILE)
# define USE_OSX_SENDFILE
# include <sys/uio.h>
#endif

#if defined(HAVE_SYS_UIO_H) && defined(HAVE_WRITEV)
# define USE_WRITEV
# include <sys/uio.h>
#endif

#if defined(HAVE_SYS_MMAN_H) && defined(HAVE_MMAP)
# define USE_MMAP
# include <sys/mman.h>
/* NetBSD 1.3.x needs it */
# ifndef MAP_FAILED
#  define MAP_FAILED -1
# endif

#if defined(MAP_ANON)
# define HAVE_MEM_MMAP_ANON
#else
/* let's try /dev/zero */
# define HAVE_MEM_MMAP_ZERO
#endif

#endif

#if defined(LIGHTY_OS_AIX) && defined(HAVE_SYS_UIO_H) && defined(HAVE_WRITEV) && defined(HAVE_SEND_FILE)
# define USE_AIX_SENDFILE
#endif


/**
* unix can use read/write or recv/send on sockets
* win32 only recv/send
*/
#ifdef _WIN32

# define WIN32_LEAN_AND_MEAN
# define NOGDI
# define USE_WIN32_SEND
/* wait for async-io support
# define USE_WIN32_TRANSMITFILE
*/
#else
# define USE_WRITE
#endif

/* Shared library support */
#ifdef _WIN32
  #define LI_IMPORT __declspec(dllimport)
  #define LI_EXPORT __declspec(dllexport)
  #define LI_DLLLOCAL
  #define LI_DLLPUBLIC
#else
  #define LI_IMPORT
  #ifdef GCC_HASCLASSVISIBILITY
    #define LI_EXPORT __attribute__ ((visibility("default")))
    #define LI_DLLLOCAL __attribute__ ((visibility("hidden")))
    #define LI_DLLPUBLIC __attribute__ ((visibility("default")))
  #else
    #define LI_EXPORT
    #define LI_DLLLOCAL
    #define LI_DLLPUBLIC
  #endif
#endif

#ifdef LI_DECLARE_EXPORTS
#define LI_API LI_EXPORT
#else
#define LI_API LI_IMPORT
#endif

/* Throwable classes must always be visible on GCC in all binaries */
#ifdef _WIN32
  #define LI_EXCEPTIONAPI(api) api
#elif defined(GCC_HASCLASSVISIBILITY)
  #define LI_EXCEPTIONAPI(api) LI_EXPORT
#else
  #define LI_EXCEPTIONAPI(api)
#endif

#ifdef UNUSED_PARAM
#elif defined(__GNUC__)
# define UNUSED_PARAM(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED_PARAM(x) /*@unused@*/ x
#else
# define UNUSED_PARAM(x) x
#endif

#define UNUSED(x) ( (void)(x) )

#if __GNUC__
#define INLINE static inline
/* # define INLINE extern inline */
#else
# define INLINE static
#endif

#include <lighttpd/sys-files.h>
#include <lighttpd/sys-mmap.h>
#include <lighttpd/sys-process.h>
#include <lighttpd/sys-socket.h>
#include <lighttpd/sys-strings.h>

#endif
