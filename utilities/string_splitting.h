//
// Created by madina on 20/12/17.
//

#pragma once

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

/* Skip over whitespace, stop if *p is whitespace or a NUL ('\0', a
   byte containing a zero which marks the end of C strings). */

#define SKIP(p) while (*p && isspace (*p)) { p++; }

/* Test whether *p is a NUL, and not whitespace. */

#define WANT(p) *p && ! isspace (*p)

/* Some error conditions. */

/* Everything seems to be under control. */
#define OK 0
/* A call to "malloc" returned failure. */
#define MALLOC_FAILED -1
/* We found a different number of substrings than we were told there
   were. */
#define MISCOUNT -2
/* There was nothing at all in the input. */
#define NO_INPUT -3

/* This counts the number of arguments in "input". */

static int count_args (const char * input)
{
    const char * p;
    /* "argc" is the count of arguments which we return. */
    int argc = 0;
    /* Point "p" to the start of "input". */
    p = input;
    /* Continue doing this while "*p" is not a NUL. */
    while (*p) {
        /* Point "p" to after any whitespace at its start. */
        SKIP (p);
        /* Check "*p" is not a NUL (we know it isn't whitespace from
           SKIP above). */
        if (WANT (p)) {
            argc++;
            /* Point "p" at the end of the non-whitespace by
               incrementing it until the test WANT fails. */
            while (WANT (p)) {
                p++;
            }
        }
    }
    return argc;
}

/* This is a helper routine for "copy_args" which frees any memory
   used so far in case of failure. */

static void free_argv (char ** argv, int i)
{
    int j;
    for (j = 0; j < i; j++) {
        free (argv[j]);
    }
}

/* Copy each non-whitespace argument into its own allocated space. */

static int copy_args (const char * input, int argc, char ** argv)
{
    /* "i" counts the number of strings we have seen so far. */
    int i = 0;
    /* "p" points to how far we have parsed "input". */
    const char *p;

    p = input;
    /* Continue doing this while "*p" is not a NUL. */
    while (*p) {
        /* Point "p" to after any whitespace at the start of "input". */
        SKIP (p);
        if (WANT (p)) {
            /* If "p" is the start of our string, "end" is going to be
               where the first whitespace in the string is, or where
               the end of the string is. */
            const char * end = p;
            /* "copy" is going to be the place we copy the string
               to. */
            char * copy;
            /* Find the end of "p" by searching for a NUL or
               whitespace. */
            while (WANT (end)) {
                end++;
            }
            /* We have now found the end of the string, so "p" is the
               start of the substring and "end" is the end of it, and
               the length of the string in bytes is "end - p"
               (so-called 'pointer arithmetic'). We now get enough
               memory to copy "end - p" bytes of the string, plus one
               extra byte for the NUL which marks the end of the
               string. */
            copy = argv[i] = malloc (end - p + 1);
            /* Check that malloc didn't fail. */
            if (! argv[i]) {
                free_argv (argv, i);
                /* Return a failure status. */
                return MALLOC_FAILED;
            }
            /* Copy the bytes. */
            while (WANT (p)) {
                *copy = *p;
                copy++;
                p++;
            }
            /* Add a NUL to the end of the string. */
            *copy = '\0';
            /* Increase the number of strings we have found. */
            i++;
        }
    }
    if (i != argc) {
        free_argv (argv, i);
        /* Return a failure status. */
        return MISCOUNT;
    }
    return OK;
}

/* Delete the macros. */

#undef SKIP
#undef WANT

/* This is the main routine. It splits "input" into pieces at
   whitespace, and returns the number of pieces in "argc_ptr", and the
   pieces themselves in "argv_ptr". "argv_ptr" is allocated within
   this function. The return value is a status, 0 or OK if nothing
   went wrong. */

static int argc_argv (const char * input, int * argc_ptr, char *** argv_ptr)
{
    int argc;
    char ** argv;
    int status;

    argc = count_args (input);
    if (argc == 0) {
        return NO_INPUT;
    }
    argv = malloc (sizeof (char *) * argc);
    if (! argv) {
        return MALLOC_FAILED;
    }
    status = copy_args (input, argc, argv);

    switch (status) {
        case OK:
            *argc_ptr = argc;
            *argv_ptr = argv;
            return OK;

        case MALLOC_FAILED:
            free (argv);
            return MALLOC_FAILED;

        default:
            fprintf (stderr, "copy_args failed with status %d for '%s'.\n",
                     status, input);
            exit (EXIT_FAILURE);
    }
}