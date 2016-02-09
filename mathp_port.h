/*
 *	Copyright 2016 Aki Nyrhinen <anyrhine@gmail.com>
 *
 *	Permission is hereby granted, free of charge, to any person obtaining
 *	a copy of this software and associated documentation files (the
 *	"Software"), to deal in the Software without restriction, including
 *	without limitation the rights to use, copy, modify, merge, publish,
 *	distribute, sublicense, and/or sell copies of the Software, and to
 *	permit persons to whom the Software is furnished to do so, subject to
 *	the following conditions:
 *
 *	The above copyright notice and this permission notice shall be
 *	included in all copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *	LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *	OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *	WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <math.h>

typedef float floatp[8];

#define bcastfp(dstp, val) ((*dstp) = (floatp){(val),(val),(val),(val),(val),(val),(val),(val)})

static void
copyfp(floatp *dst, floatp a)
{
	int i;
	for(i = 0; i < 8; i++)
		(*dst)[i] = a[i];
}

static void
absfp(floatp *dst, floatp a)
{
	int i;
	for(i = 0; i < 8; i++)
		(*dst)[i] = fabsf(a[i]);
}

static void
sqrtfp(floatp *dst, floatp a)
{
	int i;
	for(i = 0; i < 8; i++)
		(*dst)[i] = sqrtf(a[i]);

}

static void
minfp(floatp *dst, floatp a, floatp b)
{
	int i;
	for(i = 0; i < 8; i++)
		(*dst)[i] = a[i] < b[i] ? a[i] : b[i];
}

static void
maxfp(floatp *dst, floatp a, floatp b)
{
	int i;
	for(i = 0; i < 8; i++)
		(*dst)[i] = a[i] > b[i] ? a[i] : b[i];
}

static void
addfp(floatp *dst, floatp a, floatp b)
{
	int i;
	for(i = 0; i < 8; i++)
		(*dst)[i] = a[i] + b[i];
}

static void
subfp(floatp *dst, floatp a, floatp b)
{
	int i;
	for(i = 0; i < 8; i++)
		(*dst)[i] = a[i] - b[i];
}

static void
mulfp(floatp *dst, floatp a, floatp b)
{
	int i;
	for(i = 0; i < 8; i++)
		(*dst)[i] = a[i] * b[i];
}

static void
divfp(floatp *dst, floatp a, floatp b)
{
	int i;
	for(i = 0; i < 8; i++)
		(*dst)[i] = a[i] / b[i];
}

static void
rcpfp(floatp *dst, floatp a)
{
	int i;
	for(i = 0; i < 8; i++)
		(*dst)[i] = 1.0f / a[i];
}

static void
rsqrtfp(floatp *dst, floatp a)
{
	int i;
	for(i = 0; i < 8; i++)
		(*dst)[i] = 1.0f / sqrtf(a[i]);
}

static void
maddfp(floatp *dst, floatp a, floatp afact, floatp b, floatp bfact)
{
	int i;
	for(i = 0; i < 8; i++)
		(*dst)[i] = a[i]*afact[i] + b[i]*bfact[i];
}

static void
msubfp(floatp *dst, floatp a, floatp afact, floatp b, floatp bfact)
{
	int i;
	for(i = 0; i < 8; i++)
		(*dst)[i] = a[i]*afact[i] - b[i]*bfact[i];
}

static void
lerpfp(floatp *dst, floatp a, floatp b, floatp bfact)
{
	int i;
	for(i = 0; i < 8; i++)
		(*dst)[i] = a[i]*(1.0-bfact[i]) + b[i]*bfact[i];
}
