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
#include "record.h"
#include "sheet.h"
#include "interger.h"
#include "handler.h"

book *
book_parse(char *file) {
	book *bk;
	detect_endian();
	bk = mymalloc(sizeof(*bk));
	book_file(bk,file);
	book_seekbof(bk);
	book_parse_sheets(bk);
	book_getsheets(bk);
	return bk;
}

void
book_file(book *book, char *filename) {
	book->file = fopen(filename, "r");

	mymallocinit();

	book->position    = 0;
	book->xfcount     = 0;
	book->stringcount = 0;
	book->namecount   = 0;
	book->sheetcount  = 0;

	if (book->file == NULL) {
		fprintf(stderr,"Could not open file for parsing: %s\n",filename);
		exit(1);
	}
}

void
book_getsheets(book *book) {
	sheet *sheet;

	book->sheetcount = 0;
	while ((sheet = sheet_get(book)) != NULL) {
		sheet->name = book->names[book->sheetcount];
		book->sheet[book->sheetcount] = sheet;
		book->sheetcount++;
	}
	book->sheet[book->sheetcount] = NULL;
}

char *
book_read(book *book, int pos, int length) {
	int i;
	char *data;
	data = mymalloc(length);
	// printf("position: %d going to: %d\n",book->position, pos);
	book_position(book, pos);
	for (i = 0; i < length; i++) {
		data[i] = fgetc(book->file);
	}
	book->position += length;
	return data;
}

int
book_get2bytes(book *book) {
	FILE *f;
	int i1, i2;
	i1 = i2 = 0;
	f = book->file;
	if (!feof(f)) {
		i1 = fgetc(f);
		book->position++;
	}
	if (!feof(f)) {
		i2 = fgetc(f);
		book->position++;
	}
	return interger2byte(i1, i2);
}

int
book_feof(book *book) {
	return feof(book->file);
}

void
book_skip(book *book, int pos) {
	book->position += pos;
	fseek(book->file, book->position, SEEK_SET);	
}

void
book_position(book *book, int pos) {
	book->position = pos;
	fseek(book->file, pos, SEEK_SET);	
}

void
book_parse_sheets(book *book) {
	int bofs;
	record *r;

	bofs = 1;
	book_position(book,512);
	r = record_get(book);

	while (bofs) {
		r = record_get(book);
		switch(r->code) {
			case XL_SST:
				// printf("WBK: XL_SST\n");
				handle_sst(book, NULL, r);				
				break;

			case XL_TXO:
				// printf("WBK: XL_TXO\n");
				break;

			case XL_NAME:
				// printf("WBK: XL_NAME\n");
				break;

			case XL_ROW:
				// printf("WBK: XL_ROW\n");
				break;

			case XL_FORMAT:
				// printf("WBK: XL_FORMAT\n");
				handle_format(book, NULL, r);
				break;

			case XL_XF:
				// printf("WBK: XL_XF\n");
				handle_xf(book, NULL, r);
				break;

			case XL_BOUNDSHEET:
				// printf("WBK: XL_BOUNDSHEET\n");
				handle_boundsheet(book, NULL, r);
				break;

			case XL_EXTSST:
				// printf("WBK: XL_EXTSST\n");
				break;

			case XL_CONTINUE:
				// printf("WBK: XL_CONTINUE\n");
				break;

			case XL_EOF:
				// printf("WBK: XL_EOF\n");
				bofs--;
				break;

			default:
				break;

		}
	}
}

char *
book_getstring(book *book, int num) {
	if (num >= 0 && num <= book->stringcount) {
		return book->sharedstrings[num];
	}
	return NULL;
}

void
book_close(book *bk) {
	int i;
	for (i = 0; i < bk->xfcount; i++) {
		free(bk->xfrecords[i]);
		bk->xfrecords[i] = NULL;
	}
	for (i = 0; i < bk->namecount; i++) {
		free(bk->names[i]);
		bk->names[i] = NULL;
	}
	for (i = 0; i < bk->sheetcount; i++) {
		free(bk->sheet[i]);
		bk->sheet[i] = NULL;
	}
	for (i = 0; i < bk->stringcount; i++) {
		free(bk->sharedstrings[i]);
		bk->sharedstrings[i] = NULL;
	}
	free(bk->sharedstrings);
	bk->sharedstrings = NULL;
	fclose(bk->file);
	bk->file = NULL;
	free(bk);
	bk = NULL;
}

int
book_seekbof(book *book) {
	int opcode, version, streamtype, length, ret;
	char *data;

	while (!feof(book->file)) {
		opcode = book_get2bytes(book);
		if (opcode == XL_BOF) {
			length     = book_get2bytes(book);

			data = book_read(book, book->position, length);
			version    = interger2byte( data[0], data[1]);
			streamtype = interger2byte( data[2], data[3]);
			free(data); data = NULL;

			if (version == BIFF8) {
				// printf("Version: BIFF8");
				ret = 8;
			} else if (version == BIFF7) {
				// printf("Version: BIFF7");
				ret = 7;
			}
			if (streamtype == WBKBLOBAL) {
				// printf("Stream: WorkBook Globals: %d\n",book->position);	
				return ret * 2;
			} else if (streamtype == WRKSHEET) {
				// printf("Stream: WorkSheet Data: %d\n",book->position);
				return ret * 1;
			}
			return 1;
		}
	}
	return 0;
	
}
