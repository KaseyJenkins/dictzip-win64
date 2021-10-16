/* defs.h -- 
 * Created: Sat Mar 15 17:27:23 2003 by Aleksey Cheusov <vle@gmx.net>
 * Copyright 1994-2003 Rickard E. Faith (faith@dict.org)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 1, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DEFS_H_
#define _DEFS_H_

#include "zlib.h"
#include <stdio.h>

#define PRINTF(flag, arg)  if (dbg_test(flag)) { log_info arg; }

#include <string.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>

static const char *err_programName = "Dictzip";
#define err_program_name err_programName

#define USE_CACHE 1

#ifdef PRINTF
#undef PRINTF
#define PRINTF(...);


static void err_warning(const char *routine, const char *format, ...) {
    va_list ap;

    fflush(stdout);
    if (err_program_name) {
        if (routine)
            fprintf(stderr, "%s (%s): ", err_program_name, routine);
        else
            fprintf(stderr, "%s: ", err_program_name);
    } else {
        if (routine) fprintf(stderr, "%s: ", routine);
    }

    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);

    fflush(stderr);
    fflush(stdout);
}

static void err_fatal(const char *routine, const char *format, ...) {
    va_list ap;

    fflush(stdout);
    if (err_program_name) {
        if (routine)
            fprintf(stderr, "%s (%s): ", err_program_name, routine);
        else
            fprintf(stderr, "%s: ", err_program_name);
    } else {
        if (routine) fprintf(stderr, "%s: ", routine);
    }

    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);

    fflush(stderr);
    fflush(stdout);
    exit(1);
}

/* \doc |err_fatal_errno| flushes "stdout", prints a fatal error report on
   "stderr", prints the system error corresponding to |errno|, flushes
   "stderr" and "stdout", and calls |exit|.  |routine| is the name of the
   routine in which the error took place. */

static void err_fatal_errno(const char *routine, const char *format, ...) {
    va_list ap;
    int errorno = errno;

    fflush(stdout);
    if (err_program_name) {
        if (routine)
            fprintf(stderr, "%s (%s): ", err_program_name, routine);
        else
            fprintf(stderr, "%s: ", err_program_name);
    } else {
        if (routine) fprintf(stderr, "%s: ", routine);
    }

    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);

#if HAVE_STRERROR
    fprintf( stderr, "%s: %s\n", routine, strerror( errorno ) );
   log_error( routine, "%s: %s\n", routine, strerror( errorno ) );
#else
    errno = errorno;
    perror(routine);
#endif

    fflush(stderr);
    fflush(stdout);
    exit(1);
}

/* \doc |err_internal| flushes "stdout", prints the fatal error message,
   flushes "stderr" and "stdout", and calls |abort| so that a core dump is
   generated. */

static void err_internal(const char *routine, const char *format, ...) {
    va_list ap;

    fflush(stdout);
    if (err_program_name) {
        if (routine)
            fprintf(stderr, "%s (%s): Internal error\n     ",
                    err_program_name, routine);
        else
            fprintf(stderr, "%s: Internal error\n     ", err_program_name);
    } else {
        if (routine) fprintf(stderr, "%s: Internal error\n     ", routine);
        else fprintf(stderr, "Internal error\n     ");
    }

    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);

    if (err_program_name)
        fprintf(stderr, "Aborting %s...\n", err_program_name);
    else
        fprintf(stderr, "Aborting...\n");
    fflush(stderr);
    fflush(stdout);
    abort();
}

#ifndef __func__
# ifdef __FUNCTION__
#  define __func__  __FUNCTION__
# else
#  define __func__  __FILE__
# endif
#endif

#endif


#define BUFFERSIZE 10240

#define DBG_VERBOSE     (0<<30|1<< 0) /* Verbose                            */
#define DBG_UNZIP       (0<<30|1<< 2) /* Unzip                              */


#define DICT_UNKNOWN    0
#define DICT_TEXT       1
#define DICT_GZIP       2
#define DICT_DZIP       3

#define DICT_CACHE_SIZE 5

typedef struct dictCache {
    int           chunk;
    char          *inBuffer;
    int           stamp;
    int           count;
} dictCache;

typedef struct dictData {
    int           fd;		/* file descriptor */
    const char    *start;	/* start of mmap'd area */
    const char    *end;		/* end of mmap'd area */
    unsigned long size;		/* size of mmap */

    int           type;
    const char    *filename;
    z_stream      zStream;
    int           initialized;

    int           headerLength;
    int           method;
    int           flags;
    time_t        mtime;
    int           extraFlags;
    int           os;
    int           version;
    int           chunkLength;
    int           chunkCount;
    int           *chunks;
    unsigned long *offsets;	/* Sum-scan of chunks. */
    const char    *origFilename;
    const char    *comment;
    unsigned long crc;
    unsigned long length;
    unsigned long compressedLength;
    dictCache     cache[DICT_CACHE_SIZE];
} dictData;

#endif /* _DEFS_H_ */

