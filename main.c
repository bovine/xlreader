/*-
 * Copyright (c) 2001 David Giffin.
 * All rights reserved.
 *
 * Based on the the Java version: Andrew Khan Copyright (c) 2000. 
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by
 *     David Giffin <david@giffin.org>."
 *
 * 4. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by
 *     David Giffin <david@giffin.org>."
 *
 * THIS SOFTWARE IS PROVIDED BY DAVID GIFFIN ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL DAVID GIFFIN OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *      $Id$
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "biff.h"
#include "book.h"
#include "format.h"


#define CSV 1
#define TAB 2
#define SQL 3

#define ORACLE_QUOTES 1
#define PGSQL_QUOTES 2

void
help(FILE *errout,char *prog) {
	fprintf(errout, "Read Excel files without Microsoft!\n");
	fprintf(errout, "<xlreader> by David Giffin <david@giffin.org>\n");
	fprintf(errout, "based on Java Reader by Andrew Khan\n\n");

	fprintf(errout, "Usage: %s: [OTIONS] <FILE1> <FILE2> ...\n", prog);	
	fprintf(errout, "Example: %s --sql MyExcelFile.xls\n",prog);
	fprintf(errout, "\nOptions:\n");
	fprintf(errout, "  -t, --tab         Convert to TAB delimted text\n");
	fprintf(errout, "  -c, --csv         Convert to CSV text\n");
	fprintf(errout, "  -s, --sql         Convert to SQL inserts\n");
	fprintf(errout, "  -o, --oracle      Use Oracle style quoting [''] \n");
	fprintf(errout, "  -p, --postgres    Use PostgreSQL style quoting [\\\'] \n");
	fprintf(errout, "  -i, --isodate     Convert to ISO standard date [1975-12-10 03:34:59]\n");
	exit(1);
}

int
main (int argc, char **argv) {
	int i;
	int format;
	char *prog;
	book *book;
	char *dateformat;
	int quoteformat;

	prog = argv[0];
	quoteformat = ORACLE_QUOTES;
	dateformat = NULL;
	format = CSV;
	if (argc < 2) {
		help(stderr,prog);
	}
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i],"--isodate") == 0 ||
			strcmp(argv[i],"-i") == 0) {

			/* Y-m-d H:i:s */
			dateformat = "%Y-%m-%d %H:%M:%S";

		} else if (strcmp(argv[i],"-o") == 0 ||
			strcmp(argv[i],"--oracle") == 0) {

			quoteformat = ORACLE_QUOTES;

		} else if (strcmp(argv[i],"-p") == 0 ||
			strcmp(argv[i],"--postgres") == 0) {

			quoteformat = PGSQL_QUOTES;

		} else if (strcmp(argv[i],"-s") == 0 ||
			strcmp(argv[i],"--sql") == 0) {

			format = SQL;

		} else if (strcmp(argv[i],"-c") == 0 ||
			strcmp(argv[i],"--csv") == 0) {

			format = CSV;

		} else if (strcmp(argv[i],"--tab") == 0 ||
			strcmp(argv[i],"-t") == 0) {

			format = TAB;

		} else if (strcmp(argv[i],"--help") == 0 ||
			strcmp(argv[i],"-h") == 0) {

			help(stderr,prog);

		} else {
			break;
		}
	}
	for (; i < argc; i++) {
		book = book_parse(argv[i]);
		switch (format) {

			case TAB:
			book_format_tab(book, dateformat);
			break;

			case SQL:
			book_format_sql(book, dateformat, quoteformat);
			break;

			case CSV:
			default:
			book_format_csv(book, dateformat);
			break;
		}
		book_close(book);
		book = NULL;
	}
	return 1;
}

