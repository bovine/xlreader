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

#define BIG_ENDIAN    2
#define LITTLE_ENDIAN 1

static size_t alloced;
static int endian;

void
detect_endian(void) {
	int end;
	long i = 0x44332211;
	unsigned char* a = (unsigned char*) &i;
	end = (*a != 0x11); 
	// printf("The endianess is %s!\n", ((end==1)?"big":"little"));
	if (end == 1) {
		endian = BIG_ENDIAN;
	} else {
		endian = LITTLE_ENDIAN;
	}
}


void
mymallocinit(void) {
	alloced = 0;
}

void *
mymalloc(size_t s) {
	void *t;

	alloced += s;
	t = malloc(s);
	if (t == NULL) {
		printf("Error malloc failed! Mallocing %d bytes\n",(int)s);
		exit(1);
	}
	return (void *)t;
}

int
interger2byte(int b1, int b2) {
	int i1 = b1 & 0xff;
	int i2 = b2 & 0xff;
	int val = i2 << 8 | i1;
	return val;
}

int
interger4byte(int b1, int b2, int b3, int b4) {

	int i1 = interger2byte(b1, b2);
	int i2 = interger2byte(b3, b4);

	int val = i2 << 16 | i1;
	return val;
}

void
double4byte(double *d, int b1, int b2, int b3, int b4) {
	int i;
	unsigned char *ieee;

	ieee = (unsigned char *) d;
	for (i = 0; i < 8; i++) ieee[i] = 0;

	if (endian == BIG_ENDIAN) {
		//printf("Big Endian!!!\n");
		ieee[0] = ((int)b4) & 0xff;
		ieee[1] = ((int)b3) & 0xff;
		ieee[2] = ((int)b2) & 0xff;
		ieee[3] = ((int)b1) & 0xff;
		if ((ieee[3] & 0x01) != 0)
			*d /= 100;
	} else {
		ieee[4] = ((int)b1) & 0xff;
		ieee[5] = ((int)b2) & 0xff;
		ieee[6] = ((int)b3) & 0xff;
		ieee[7] = ((int)b4) & 0xff;
		if ((ieee[4] & 0x01) != 0)
			*d /= 100;
	}
	/*
	for (i = 0; i < 8; i++) {
		printf("double char: %d -> 0x%X\n",i,ieee[i]);
	}
	*/
}

void
float4byte(float *f, int b1, int b2, int b3, int b4) {
	int i;
	unsigned char *ieee;

	ieee = (unsigned char *) f;
	for (i = 0; i < 4; i++) ieee[i] = 0;

	ieee[0] = ((int)b4) & 0xff;
	ieee[1] = ((int)b3) & 0xff;
	ieee[2] = ((int)b2) & 0xff;
	ieee[3] = ((int)b1) & 0xff;

}

void
double8byte(double *d, int b1, int b2, int b3, int b4, int b5, int b6, int b7, int b8) {
	int i;
	unsigned char *ieee;

	ieee = (unsigned char *)d;

	for (i = 0; i < 8; i++) ieee[i] = 0;

	if (endian == BIG_ENDIAN) {
		// printf("Big Endian!!!\n");
		ieee[0] = ((int)b8) & 0xff;
		ieee[1] = ((int)b7) & 0xff;
		ieee[2] = ((int)b6) & 0xff;
		ieee[3] = ((int)b5) & 0xff;

		ieee[4] = ((int)b4) & 0xff;
		ieee[5] = ((int)b3) & 0xff;
		ieee[6] = ((int)b2) & 0xff;
		ieee[7] = ((int)b1) & 0xff;
	} else {
		ieee[0] = ((int)b1) & 0xff;
		ieee[1] = ((int)b2) & 0xff;
		ieee[2] = ((int)b3) & 0xff;
		ieee[3] = ((int)b4) & 0xff;

		ieee[4] = ((int)b5) & 0xff;
		ieee[5] = ((int)b6) & 0xff;
		ieee[6] = ((int)b7) & 0xff;
		ieee[7] = ((int)b8) & 0xff;
	}
	/*
	for (i = 0; i < 8; i++) {
		printf("double char: %d -> 0x%X\n",i,ieee[i]);
	}
	*/
}


int
interger2bytefile(FILE *f) {
	int i1, i2;
	i1 = fgetc(f);
	i2 = fgetc(f);
	return interger2byte(i1,i2);
}

