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
#include <math.h>
#include <time.h>
#include <sys/types.h>
#include "biff.h"
#include "cell.h"
#include "book.h"
#include "sheet.h"
#include "interger.h"

static char *dateformat = NULL;

void
cell_setdateformat(char *d) {
	dateformat = d;
}

int
cell_getprecision(double d) {
	double t;
	int i,x;
	int count;

	
	// printf("precision: for number: %f\n",d);
	if (d < 0) d *= -1;
	i = (int)d;
	t = d - (double)i;
	if (t <= 0) {
		return 0;
	}
	count = 0;
	for (x = 6; x > 1; x--) {
		i = (int)d;
		t = d - (double)i;
		t *= pow(10,x - 2);
		i = (int)t;
		t = t - (double)i;
		t *= 10;
		i = (int)t;
		if (i > 0) break;
		count++;
	}
	return (5 - count);
}

char *
cell_data_string(book *book, sheet *sh, int row, int col) {
	time_t date;
	struct tm *tmptr;
	cell *c;
	char *str = NULL;
	char *format;
	int precision;
	int utcOffsetDays = 25569;
	int sInADay = 24 * 60 * 60;

	c = sh->cells[row][col];
	if (c == NULL) return NULL;

	switch (c->type) {
		case CELL_LABEL:
			return c->value.c;

		case CELL_NUMBER:
			str = mymalloc(50);
			// printf("\nUsing format: %d\n",c->xfindex);
			if (book->xfrecords[c->xfindex] != NULL &&
				book->xfrecords[c->xfindex]->type == DATEFORMAT) {

				format = (char *) book->xfrecords[c->xfindex]->format;
				// printf("format is: %s\n",format);
				date = (time_t) ((c->value.d - utcOffsetDays) * sInADay);
				tmptr = gmtime(&date);
				if (dateformat) {
					strftime(str,50,dateformat,tmptr);
				} else {
					strftime(str,50,format,tmptr);
				}
			} else if (book->xfrecords[c->xfindex] != NULL &&
				book->xfrecords[c->xfindex]->type == NUMBERFORMAT) {

				format = (char *) book->xfrecords[c->xfindex]->format;
				// printf("format is: %s\n",format);
				snprintf(str,50,format,c->value.d);
			} else {
				precision = cell_getprecision(c->value.d);
				snprintf(str,50,"%.*f",precision,c->value.d);
			}
			break;

		case CELL_DATE:
			//c->value.i = (int) val;
			break;

		case CELL_BOOLEAN:
			//c->value.i = (int) val;
			break;

		case CELL_ERROR:
			break;

	}
	return str;

}

cell *
cell_number(int col, int row, int index, double d) {
	cell *c;
	c = mymalloc(sizeof(*c));
	c->row = row;
	c->col = col;
	c->xfindex = index;
	c->type = CELL_NUMBER;
	c->value.d = d;
	return c;
}

cell *
cell_label(int col, int row, char *str) {
	cell *c;
	c = mymalloc(sizeof(*c));
	c->row = row;
	c->col = col;
	// c->xfindex = index;
	c->type = CELL_LABEL;
	c->value.c = str;
	return c;
}

cell *
cell_set(int row, int col, int type, void *val) {
	cell *c;
	c = mymalloc(sizeof(*c));
	c->row  = row;
	c->col  = col;
	c->type = type;
	switch (type) {
		case CELL_LABEL:
			printf("Setting Cell %d,%d: %s\n",c->row,c->col, (char *) val); 
			c->value.c = (char *) val;
			break;

		/*
		case CELL_NUMBER:
			printf("Setting Cell %d,%d: %f\n",c->row,c->col, (double *) val); 
			c->value.d = (double *) val;
			break;
		*/
		case CELL_DATE:
			c->value.i = (int) val;
			break;

		case CELL_BOOLEAN:
			c->value.i = (int) val;
			break;

		case CELL_ERROR:
			break;

	}
	return c;
}

void *
cell_get(sheet *sheet, int row, int col) {
	cell *c;
	c = sheet->cells[row][col];

	switch (c->type) {
		case CELL_LABEL:
			return ((void *) c->value.c);
		/*
		case CELL_NUMBER:
			return ((void *) c->value.d);
		*/
		case CELL_DATE:
			return ((void *) c->value.i);

		case CELL_BOOLEAN:
			return ((void *) c->value.i);

		case CELL_ERROR:
			return ((void *) c->value.i);
	}
	return NULL;
}
