/*  bgzeof.c -- check for bgzf EOF.

    Copyright (C) 2015 Genome Research Ltd.

    Author: Shane McCarthy <sm15@sanger.ac.uk>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.  */

/* 
    Quick check that the BGZF EOF marker exists.
    
    Author: Shane McCarthy, sm15@sanger.ac.uk
    
    gcc -g -Wall -O2 -o bgzeof bgzeof.c -I../htslib -L../htslib -lhts -lz
    
*/
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "htslib/bgzf.h"

static void usage(FILE *fp, int status)
{
    fprintf(fp,
"\nDescription: Detect truncation of BGZF files by checking the EOF marker exists.\n"
"Usage: bgzeof FILE [...]\n\n");
    exit(status);
}

int main(int argc, char **argv)
{
    BGZF *fp;
    
    if (argc == 1) usage(stderr, EXIT_FAILURE);;
    
    int status = EXIT_SUCCESS;
    for ( ; optind < argc; optind++) {
        fp = bgzf_open(argv[optind], "r");
        if (fp == NULL) {
            fprintf(stderr, "[bgzeof] Could not open file: \"%s\": %s\n", argv[optind], strerror(errno));
            status = EXIT_FAILURE;
            continue;
        }
        if (bgzf_check_EOF(fp) == 0) {
            fprintf(stderr, "[bgzeof] %s: BGZF EOF marker is absent. File may be truncated or not a BGZF compressed file.\n", argv[optind]);
            status = EXIT_FAILURE;
        }
        bgzf_close(fp);
    }
    
    return status;
}