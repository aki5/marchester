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

#include <immintrin.h>

typedef float floatp __attribute__((vector_size(32)));
typedef int32_t intp __attribute__((vector_size(32)));

#define xbcastip(val) ((intp){(val),(val),(val),(val),(val),(val),(val),(val)})
#define bcastfp(dstp, val) ((*dstp) = (floatp){(val),(val),(val),(val),(val),(val),(val),(val)})

static intp _absmaskp = xbcastip(0x7fffffff);

static void
copyfp(floatp *dst, floatp a)
{
	*dst = a;
}

static void
_andip(intp *dst, intp a, intp b)
{
	*dst = a & b;
}

static void
absfp(floatp *dst, floatp a)
{
	_andip((intp*)dst, (intp)a, _absmaskp);
}

static void
sqrtfp(floatp *dst, floatp a)
{
	*dst = __builtin_ia32_sqrtps256(a);
}

static void
minfp(floatp *dst, floatp a, floatp b)
{
	*dst = __builtin_ia32_minps256(a, b);
}

static void
maxfp(floatp *dst, floatp a, floatp b)
{
	*dst = __builtin_ia32_maxps256(a, b);
}

static void
addfp(floatp *dst, floatp a, floatp b)
{
	*dst = a + b;
}

static void
subfp(floatp *dst, floatp a, floatp b)
{
	*dst = a - b;
}

static void
mulfp(floatp *dst, floatp a, floatp b)
{
	*dst = a * b;
}

static void
divfp(floatp *dst, floatp a, floatp b)
{
	*dst = a / b;
}

static void
rcpfp(floatp *dst, floatp a)
{
	*dst = __builtin_ia32_rcpps256(a);
}

static void
rsqrtfp(floatp *dst, floatp a)
{
	*dst = __builtin_ia32_rsqrtps256(a);
}

static void
maddfp(floatp *dst, floatp a, floatp b, floatp afact, floatp bfact)
{
	floatp tmp;
	mulfp(dst, a, afact);
	mulfp(&tmp, b, bfact);
	addfp(dst, *dst, tmp);
}

static void
msubfp(floatp *dst, floatp a, floatp b, floatp afact, floatp bfact)
{
	floatp tmp;
	mulfp(dst, a, afact);
	mulfp(&tmp, b, bfact);
	subfp(dst, *dst, tmp);
}

static void
lerpfp(floatp *dst, floatp a, floatp b, floatp bfact)
{
	floatp tmp;
	bcastfp(&tmp, 1.0f);
	subfp(&tmp, tmp, bfact);
	mulfp(dst, a, tmp);
	mulfp(&tmp, b, bfact);
	addfp(dst, *dst, tmp);
}
