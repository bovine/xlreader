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
#include "interger.h"
#include "handler.h"
#include "sheet.h"

sheet *
sheet_get(book *book) {
	sheet *sh;
	int type;

	sh = NULL;
	type = book_seekbof(book);
	if (type) {
		sh = mymalloc(sizeof(*sh));
		sh->type = type;
		sh->position = book->position;
		sh->name = NULL;
	}
	if (type == 8 || type == 7) {
		sheet_read(book, sh);
	}
	return sh;
}

/*
int
sheet_type(sheet *sheet) {
	if (sheet->type) {
		return sheet->type;
	}
	return -1;
}
*/

int
sheet_read(book *book, sheet *sheet) {
	record *record;
	int oldpos;

	oldpos = book->position;
	// printf("Skipping to Sheet Position: %d\n",sheet->position); 
	book_position(book, sheet->position);

	while ((record = record_get(book)) != NULL) {
		if (!sheet_handle_record(book, sheet, record)) break;
		if (record->data != NULL) {
			free(record->data);
			record->data = NULL;
		}
		free(record);
		record = NULL;
	}
	book_position(book, oldpos);
	return 1;
}

void
sheet_initcells(sheet *sh) {
	int x, y;
	sh->cells = mymalloc(sizeof(cell) * (sh->rows + 1));
	if (sh->cells == NULL) {
		printf("Out of memory creating cell rows: %d",sh->rows);
		exit(1);
	}
	for (x = 0; x < sh->rows; x++) {
		sh->cells[x] = mymalloc(sizeof(cell) * (sh->cols + 1));
		if (sh->cells[x] == NULL) {
			printf("Out of memory creating cell cols: %d",sh->cols);
			exit(1);
		}
		for (y = 0; y < sh->cols; y++) {
			sh->cells[x][y] = NULL;
		}
	}
}

int
sheet_handle_record(book *book, sheet *sheet, record *record) {
	char *data;

	data = NULL;
	switch (record->code) {
		case XL_DIMENSION:
			// printf("XL_DIMENSION\n");
			data = record_getdata(book, record);
			if (record->length == 10) {
				sheet->rows = interger2byte(data[2], data[3]);
				sheet->cols = interger2byte(data[6], data[7]);
			} else {
				sheet->rows = interger4byte(data[4], data[5], data[6], data[7]);
				sheet->cols = interger2byte(data[10], data[11]);
			}
			sheet_initcells(sheet);
			// printf("Dimension cols: %d rows: %d\n",sheet->cols,sheet->rows);
			break;

		case XL_LABELSST:
			// printf("XL_LABELSST\n");
			handle_labelsst(book, sheet, record);
			break;

		case XL_RK:
		case XL_RK2:
			// printf("XL_RK\n");
			handle_rk(book, sheet, record);
			break;

		case XL_MULRK:
			// printf("XL_MULRK\n");
			handle_mulrk(book, sheet, record);
			break;

		case XL_ROW:
			// printf("XL_ROW\n");
			break;

		case XL_NUMBER:
			// printf("XL_NUMBER\n");
			handle_number(book, sheet, record);
			break;

		case XL_BOOLERR:
			// printf("XL_BOOLERR\n");
			break;

		case XL_CONTINUE:
			// printf("XL_CONTINUE\n");
			break;

		case XL_FORMULA:
		case XL_FORMULA2:
			// printf("XL_FORMULA\n");
			handle_formula(book, sheet, record);
			break;

		case XL_LABEL:
			// printf("XL_LABEL\n");
			break;

		case XL_NAME:
			handle_name(book, sheet, record);
			// printf("XL_NAME\n");
			break;

		case XL_BOF:
			// printf("XL_BOF\n");
			break;

		case XL_EOF:
			// printf("XL_EOF\n");
			return 0;
		default:
			break;
	}
	return 1;
}
