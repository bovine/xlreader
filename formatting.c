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
#include "cell.h"
#include "book.h"
#include "sheet.h"
#include "interger.h"

static xfrecord formatter[] = {
{ 0xe , DATEFORMAT, "%m/%d/%y"},
{ 0xf , DATEFORMAT, "%d-%b-%y"},
{ 0x10, DATEFORMAT, "%d-%b"},
{ 0x11, DATEFORMAT, "%b-%y"},
{ 0x12, DATEFORMAT, "%I:%M %p"},
{ 0x13, DATEFORMAT, "%I:%M:%S %p"},
{ 0x14, DATEFORMAT, "%H:%M"},
{ 0x15, DATEFORMAT, "%H:%M:%S"},
{ 0x16, DATEFORMAT, "%m/%d/%y %H:%M"},
{ 0x2d, DATEFORMAT, "%M:%S"},
{ 0x2e, DATEFORMAT, "%H:%M:%S"},
{ 0x2f, DATEFORMAT, "%M:%S"},
{ 0xa5, DATEFORMAT, "%m/%d/%y %I:%M %p"},
{ 0x1 , NUMBERFORMAT, "%.0f"},
{ 0x2 , NUMBERFORMAT, "%.2f"},
{ 0x3 , NUMBERFORMAT, "#,##%.0f"},
{ 0x4 , NUMBERFORMAT, "#,##%.2f"},
{ 0x5 , NUMBERFORMAT, "$#,##%.0f"},
{ 0x6 , NUMBERFORMAT, "$#,##%.0f"},
{ 0x7 , NUMBERFORMAT, "$#,##%.2f"},
{ 0x8 , NUMBERFORMAT, "$#,##%.2f"},
{ 0x9 , NUMBERFORMAT, "%.0f%%"},
{ 0xa , NUMBERFORMAT, "%.2f%%"},
{ 0xb , NUMBERFORMAT, "%e"},
{ 0x25, NUMBERFORMAT, "#,##%.0f;(#,##0)"},
{ 0x26, NUMBERFORMAT, "#,##%.0f;(#,##0)"},
{ 0x27, NUMBERFORMAT, "#,##%.2f;(#,##0.00)"},
{ 0x28, NUMBERFORMAT, "#,##%.2f;(#,##0.00)"},
{ 0x29, NUMBERFORMAT, "#,##%.0f;(#,##0)"},
{ 0x2a, NUMBERFORMAT, "$#,##%.0f;($#,##0)"},
{ 0x2b, NUMBERFORMAT, "#,##%.2f;(#,##0.00)"},
{ 0x2c, NUMBERFORMAT, "$#,##%.2f;($#,##0.00)"},
{ 0x30, NUMBERFORMAT, "##0.0E0"},
{ 0, 0, NULL}
};

xfrecord *
formatting_get(int xf) {
	int i;
	xfrecord *rec;
	for (i = 0; formatter[i].format != NULL; i++) {
		if (xf == formatter[i].code) break;
	}
	if (formatter[i].format == NULL) return NULL;
	rec = mymalloc(sizeof(*rec));
	rec->code   = xf;
	rec->type   = formatter[i].type;
	rec->format = formatter[i].format;
	return rec;
}
