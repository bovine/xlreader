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

#ifndef _HANDLER_H_
#define _HANDLER_H_

struct mulrk {
        int row;
        int first;
        int last;
        int numrks;
	double rkdbls[1024];
        int xfindices[1024];
};
  

double rkgetdouble(int rk);
void mulrk_read(struct mulrk *mulrk, char *data);
void handle_formula(book *, sheet *, record *);
void handle_number(book *, sheet *, record *);
void handle_rk(book *, sheet *, record *);
void handle_mulrk(book *, sheet *, record *);
void handle_sst(book *, sheet *, record *); 
void handle_xf(book *, sheet *, record *); 
void handle_format(book *, sheet *, record *); 
void handle_name(book *, sheet *, record *); 
void handle_labelsst(book *, sheet *, record *); 
void handle_boundsheet(book *, sheet *, record *); 
void handle_setsst(book *book, record *record, sstlist *cont, char *bytes);

#endif /* _HANDLER_H_ */
