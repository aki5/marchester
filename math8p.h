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

typedef float float8p __attribute__((vector_size(32)));
typedef int32_t int8p __attribute__((vector_size(32)));

#define bcastf8p(val) ((float8p){val, val, val, val, val, val, val, val})

static void
absf8p(float8p *dst, float8p a)
{
	*dst =  (float8p)((int8p)a & ~(int8p)bcastf8p(-0.0f));
}

static void
sqrtf8p(float8p *dst, float8p a)
{
	*dst = __builtin_ia32_sqrtps256(a);
}

static void
minf8p(float8p *dst, float8p a, float8p b)
{
	*dst = __builtin_ia32_minps256(a, b);
}

static void
maxf8p(float8p *dst, float8p a, float8p b)
{
	*dst = __builtin_ia32_maxps256(a, b);
}
