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
#include "cell.h"
#include "book.h"
#include "sheet.h"
#include "record.h"
#include "handler.h"
#include "interger.h"
#include "sstlist.h"
#include "formatting.h"

void
handle_mulrk(book *book, sheet *sheet, record *record) {
	struct mulrk mulrk;
	char *data;
	cell *cell;
	int len;
	int i;

	len = record->length;
	data = record_getdata(book,record);

	mulrk.row   = interger2byte(data[0],data[1]);
	mulrk.first = interger2byte(data[2],data[3]);
	mulrk.last  = interger2byte(data[len - 2],data[len - 1]);
	mulrk.numrks = mulrk.last - mulrk.first + 1;
	mulrk_read(&mulrk, data);
	
	// printf("MulRK first: %d last: %d records: %d\n",mulrk.first,mulrk.last,mulrk.numrks);

	for (i = 0; i < mulrk.numrks; i++) {
		cell = cell_number(mulrk.row, mulrk.first + i, mulrk.xfindices[i], mulrk.rkdbls[i]);
		sheet->cells[mulrk.row][mulrk.first + i] = cell;
	}
	/*
	free(mulrk.xfindices);
	free(mulrk.rkdbls);
	*/
}

void
mulrk_read(struct mulrk *mulrk, char *data) {
	double d;
	int i;
	int pos;
	pos = 4;
	/*
	mulrk->xfindices = mymalloc(sizeof(*mulrk->xfindices) * mulrk->numrks);
	mulrk->rkdbls    = mymalloc(sizeof(*mulrk->rkdbls) * mulrk->numrks);
	*/
	for (i = 0; i < mulrk->numrks; i++) {
		mulrk->xfindices[i] = interger2byte(data[pos], data[pos+1]);
		double4byte(&d,data[pos+2], data[pos+3], data[pos+4], data[pos+5]);
		mulrk->rkdbls[i] = d;
		// printf("Double 4 byte: %f\n",d);
		pos += 6;
	}
}

void
handle_number(book *book, sheet *sheet, record *record) {
	int xfindex, row, col;
	char *data;
	double d;

	data = record_getdata(book,record);
	row = interger2byte(data[0], data[1]);
	col = interger2byte(data[2], data[3]);
	xfindex = interger2byte(data[4], data[5]);
	double8byte(&d, data[6], data[7], data[8], data[9],data[10], data[11], data[12], data[13]);
	sheet->cells[row][col] = cell_number(row, col, xfindex, d);

	// printf("Double 8 byte: %f\n",d);
}

void
handle_format(book *book, sheet *sheet, record *rec) {
	xfrecord *xfrec;
	char *data;
	int format;

	data   = record_getdata(book,rec);
	format = interger2byte(data[2],data[3]);
	xfrec  = formatting_get(format);
	/*
	// printf("FORMAT: Record: %d code: %0X ->",book->xfcount,format);
	book->xfrecords[book->xfcount++] = xfrec;
	if (book->xfrecords[book->xfcount - 1] != NULL) {
		// printf("Format: %s\n",book->xfrecords[book->xfcount - 1]->format);
	} else {
		// printf("Format: NONE!\n");
	}
	book->xfrecords[book->xfcount] = NULL;
	*/
}

void
handle_xf(book *book, sheet *sheet, record *rec) {
	xfrecord *xfrec;
	char *data;
	int format;

	data   = record_getdata(book,rec);
	format = interger2byte(data[2],data[3]);
	xfrec  = formatting_get(format);

	// printf("XF: Record: %d code: %0X ->",book->xfcount,format);
	book->xfrecords[book->xfcount++] = xfrec;
	if (book->xfrecords[book->xfcount - 1] != NULL) {
		// printf("Format: %s\n",book->xfrecords[book->xfcount - 1]->format);
	} else {
		// printf("Format: NONE!\n");
	}
	book->xfrecords[book->xfcount] = NULL;
	
}

void
handle_rk(book *book, sheet *sheet, record *record) {
	int xfindex, row, col;
	char *data;
	double d;

	data = record_getdata(book,record);
	row = interger2byte(data[0], data[1]);
	col = interger2byte(data[2], data[3]);
	xfindex = interger2byte(data[4], data[5]);
	double4byte(&d, data[6], data[7], data[8], data[9]);
	sheet->cells[row][col] = cell_number(row, col, xfindex, d);

	// printf("Double 4 byte: %f\n",d);	
}

void
handle_formula(book *book, sheet *sheet, record *record) {
	int xfindex, row, col;
	char *data;
	double d;

	data = record_getdata(book,record);
	row = interger2byte(data[0], data[1]);
	col = interger2byte(data[2], data[3]);

	if (data[6] == 0 && data[12] == -1 && data[13] == -1) {
		// we have a string
	} else if (data[6] == 1  && data[12] == -1 && data[13] == -1) {
		// We have a boolean formula
	} else if ( data[6] == 2 && data[12] == -1 && data[13] == -1) {
		// The cell is in error
	} else {
		// it is most assuredly a number
		xfindex = interger2byte(data[4], data[5]);
		double8byte(&d, data[6], data[7], data[8], data[9],data[10], data[11], data[12], data[13]);
		sheet->cells[row][col] = cell_number(row, col, xfindex, d);

		// printf("Double 8 byte: %f\n",d);
	}
}

void
handle_labelsst(book *book, sheet *sheet, record *rec) {
	int index, row, col;
	char *data;
	char *string;

	data = record_getdata(book,rec);
	index = interger4byte(data[6], data[7], data[8], data[9]);
	row = interger2byte(data[0], data[1]);
	col = interger2byte(data[2], data[3]);
	string = book_getstring(book,index);
	sheet->cells[row][col] = cell_label(row, col, string);
	
	// printf("SST Label: row: %d col: %d  %s\n",row,col,string);
}

void
showrawdata(char *data, int len) {
	int i;
	printf("rawdata: ------------------ Start ------------------\n");
	for (i = 0; i < len; i++) {
		printf("rawdata: %c <- %x\n",data[i],data[i]);
	}
	printf("rawdata: ------------------  End  ------------------\n");
}

char *
sstmergebytes(book *book,record *rec,sstlist *cont) {
	int i, pos;
	int length;
	char *data;
	char *bytes;

	/* get the total length */
	length = rec->length;
	// printf("data is %d length\n",length);
	for (i = 0; i < cont->num; i++) {
		length += cont->rec[i]->length;	
	}
	bytes = record_getdata(book,rec);
	// showrawdata(bytes,rec->length);
	pos = rec->length;
	for (i = 0; i < cont->num; i++) {
		data = record_getdata(book,cont->rec[i]);
		*bytes += pos;
		bytes = data;
		pos += cont->rec[i]->length;
	}
	return bytes;
}

void
handle_sst(book *book, sheet *sheet, record *rec) {

	char *bytes;
	sstlist *cont;
	record *nr;

	// printf("WBK: SST Processing\n");
	cont = sstlist_init();
	nr = record_peek(book);
	while (nr->code == XL_CONTINUE) {
		// printf("WBK: XL_CONTINUE - Adding to SST\n");
		sstlist_push(cont, record_get(book));
		nr = record_peek(book);
	}
	bytes = sstmergebytes(book,rec,cont);
	// showrawdata(bytes,rec->length);
	handle_setsst(book,rec,cont,bytes);
	sstlist_free(cont);
}

int
getascii(char *bytes, int pos, int chars, char *str) {
	int i;

	bytes += pos;
	for (i = 0; i < chars; i++) {		
		*str++ = *bytes++;
	}
	*str = '\0';
	return chars;

}

int
getunicode(char *bytes, int pos, int chars, char *str) {
	return 4;
}

void
handle_setsst(book *book, record *rec, sstlist *cont, char *bytes) {
	char *str;
	char *data;
	char **strings;
	int chars, pos, options, i;
	int richstring, extendedstring, runlength;
	int totalstrings;
	int uniquestrings;

	data = record_getdata(book,rec);
	totalstrings  = interger4byte(data[0], data[1], data[2], data[3]);
	uniquestrings = interger4byte(data[4], data[5], data[6], data[7]);

	// printf("SST: total strings: %d unique: %d\n",totalstrings,uniquestrings);
	pos = 8;

	strings = mymalloc(sizeof(*strings) * (uniquestrings + 1));

	for (i = 0; i < uniquestrings; i++) {
		chars = interger2byte(data[pos], data[pos+1]);

		pos += 2;
		options = data[pos];
		pos++;

		extendedstring = ((options & 0x04) != 0);
		richstring     = ((options & 0x08) != 0);
		
		if (richstring) {
			runlength = interger2byte(data[pos], data[pos+1]);
			pos += 2;			
		} else if (extendedstring) {
			runlength = interger4byte(data[pos], data[pos+1], data[pos+2], data[pos+3]);
			pos += 4;
		}
		if ((options & 0x01) == 0) {
			/* ascii encoding */
			str = mymalloc(chars + 1); 
			pos += getascii(bytes,pos,chars,str);
		} else {
			/* unicode encoding */
			str = mymalloc(chars + 1);
			pos += getunicode(bytes,pos,chars,str);
		}
		// printf("WBK: SST: string %d: %s\n",i,str);
		strings[i] = str;

		if (richstring) {
			pos += 4 * runlength;
		} else if (extendedstring) {
			pos += runlength;
		}

	}
	book->stringcount = uniquestrings;
	book->sharedstrings = strings;	
}

void
handle_name(book *book, sheet *sheet, record *rec) {
	char *data;
	char *name;
	int length;
	int pos;

	pos = 15;
	data   = record_getdata(book,rec);
	length = data[3];
	name = mymalloc(length);
	getascii(data,pos,length,name);	
	sheet->name = name;
}

void
handle_boundsheet(book *book, sheet *sheet, record *rec) {
	char *data;
	char *name;
	int type;
	int length;
	int visibility;
	int pos;

	pos = 8;
	data   = record_getdata(book,rec);
	type = data[4];
	visibility = data[5];
	length = interger2byte(data[6], data[7]);
	name = mymalloc(length);
	getascii(data,pos,length,name);
	// printf("WBK: BOUNDSHEET: name %s\n",name);
	book->names[book->namecount++] = name;
}
