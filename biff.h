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


#ifndef _BIFF_H_
#define _BIFF_H_

struct _format {
	int code;
	int type;
	const char *format;
};

struct _cell {
	int type;
	int row;
	int col;
	int xfindex;
	union value {
		int i;
		double d;
		char *c;
	} value;
};

struct _brec {
        int code;
        int length;
	int position;
        char *data;
};

struct _sheet {
	char *name;
	struct _brec *bofrecord;
	int position;
	int type;
	int rows;
	int cols;

	/* cell memory management */
	int cellsize;
	int rowalloc;
	int cellalloc;
	struct _cell ***cells;
};

struct _bfile {
        FILE *file;
        int position;
	int stringcount;
	char **sharedstrings;
	int xfcount;
	struct _format *xfrecords[255];
	int sheetcount;
	struct _sheet *sheet[10];
	int namecount;
	char *names[10];
};

struct _sstlist {
	int size;
	int num;
	int numalloc;
	struct _brec **rec;
};

struct bifftype {
	char *type;
	int code;
};


typedef struct _cell cell;
typedef struct _brec record;
typedef struct _sheet sheet;
typedef struct _bfile book;
typedef struct _sstlist sstlist;
typedef struct _format xfrecord;


#define DATEFORMAT 0x1
#define NUMBERFORMAT 0x2

#define BIFF8 0x600
#define BIFF7 0x500
#define WBKBLOBAL 0x5
#define WRKSHEET 0x10

#define XL_BOF 0x809
#define XL_EOF 0x0a
#define XL_BOUNDSHEET 0x85
#define XL_DIMENSION 0x200
#define XL_ROW 0x208
#define XL_NOTE 0x1c
#define XL_TXO 0x1b6
#define XL_RK  0x7e
#define XL_RK2  0x27e
#define XL_MULRK  0xbd
#define XL_INDEX 0x20b
#define XL_SST 0xfc
#define XL_EXTSST 0xff
#define XL_CONTINUE 0x3c
#define XL_LABEL 0x204
#define XL_LABELSST 0xfd
#define XL_NUMBER 0x203
#define XL_NAME 0x18
#define XL_ARRAY 0x221
#define XL_STRING 0x207
#define XL_FORMULA 0x406
#define XL_FORMULA2 0x6
#define XL_FORMAT 0x41e
#define XL_XF 0xe0
#define XL_BOOLERR 0x205
#define XL_UNKNOWN 0xffff


#endif /* _BIFF_H_ */
