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
#include <strings.h>
#include "biff.h"
#include "book.h"
#include "record.h"
#include "sheet.h"
#include "cell.h"
#include "interger.h"
#include "handler.h"

void
book_format_csv(book *bk, char *dateformat) {
	int i, x, y;
	sheet *s;
	char *str;
	char *delim;
	for (i = 0; i < bk->sheetcount; i++) {
		s = bk->sheet[i];
		for (x = 0; x < s->rows; x++) {
			delim = "";
			printf("\"");	
			for (y = 0; y < s->cols; y++) {
				printf("%s",delim);
				str = cell_data_string(bk,s,x,y);
				if (str != NULL) {
					printf("%s",str);
				}
				delim = "\",\"";
			}
			printf("\"\n");	
		}
	}
}

void
book_format_tab(book *bk, char *dateformat) {
	int i, x, y;
	sheet *s;
	char *str;
	char *delim;
	for (i = 0; i < bk->sheetcount; i++) {
		s = bk->sheet[i];
		for (x = 0; x < s->rows; x++) {
			delim = "";
			for (y = 0; y < s->cols; y++) {
				printf("%s",delim);
				str = cell_data_string(bk,s,x,y);
				if (str != NULL) {
					printf("%s",str);
				}
				delim = "\t";
			}
			printf("\n");
		}
	}
}

static int
quote_count(char *str) {
	int quotes, len, i;

	len = strlen(str);
	/* count number for single quotes */
	quotes = 0;
	for (i = 0; i < len; i++) {
		if (str[i] == '\'') quotes++;
	}
	return quotes;
}

static void
oracle_quote(char *str, char *quotedstr) {	
	while (*str != '\0') {
		if (*str == '\'') *quotedstr++ = '\'';
		*quotedstr++ = *str++;
	}
}

static void
postgres_quote(char *str, char *quotedstr) {	
	while (*str != '\0') {
		if (*str == '\'') *quotedstr++ = '\\';
		*quotedstr++ = *str++;
	}
}

void
book_format_sql(book *bk, char *dateformat, int quoteformat) {
	int i, x, y, quotes;
	sheet *s;
	char *str;
	char *quotedstr;
	char *delim;
	char insert_start[1024 * 4];

	cell_setdateformat(dateformat);
	for (i = 0; i < bk->sheetcount; i++) {
		delim = "";
		s = bk->sheet[i];
		if (s->name != NULL) {
			sprintf(insert_start,"INSERT INTO %s (",s->name);
		} else {
			sprintf(insert_start,"INSERT INTO ?TABLE? (");
		}
		for (y = 0; y < s->cols; y++) {
			str = cell_data_string(bk,s,0,y);
			if (str != NULL) {
				strcat(insert_start,delim);
				strcat(insert_start,str);
			} else {
				strcat(insert_start,delim);
			}
			delim = ",";
		}
		for (x = 1; x < s->rows; x++) {
			delim = "";
			printf("%s) values (",insert_start);
			for (y = 0; y < s->cols; y++) {
				printf("%s",delim);
				str = cell_data_string(bk,s,x,y);
				if (str != NULL) {
					if ((quotes = quote_count(str)) > 0) {
						quotedstr = malloc(quotes + strlen(str));
						if (quoteformat == 2) {
							postgres_quote(str,quotedstr);
						} else {
							oracle_quote(str,quotedstr);
						}
						printf("'%s'",quotedstr);
						free(quotedstr);
					} else {
						printf("'%s'",str);
					}
				} else {
					printf("NULL");
				}
				delim = ",";
			}
			printf(");\n");	
		}
	}
}


