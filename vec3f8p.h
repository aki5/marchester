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

static void
copy3f8p(float *fdst, float *fsrc)
{
	float8p *dst = (float8p *)fdst;
	float8p *src = (float8p *)fsrc;

	dst[0] = src[0];
	dst[1] = src[1];
	dst[2] = src[2];
}

static void
lerp3f8p(float *fdst, float *fa, float *fb, float *fbfact)
{
	float8p *dst = (float8p *)fdst;
	float8p *a = (float8p *)fa;
	float8p *b = (float8p *)fb;
	float8p bfact = *(float8p *)fbfact;
	float8p afact = (float8p){1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f} - bfact;

	dst[0] = a[0]*afact + b[0]*bfact;
	dst[1] = a[1]*afact + b[1]*bfact;
	dst[2] = a[2]*afact + b[2]*bfact;
}

static void
add3f8p(float *fdst, float *fa, float *fb)
{
	float8p *dst = (float8p *)fdst;
	float8p *a = (float8p *)fa;
	float8p *b = (float8p *)fb;

	dst[0] = a[0] + b[0];
	dst[1] = a[1] + b[1];
	dst[2] = a[2] + b[2];
}

static void
sub3f8p(float *fdst, float *fa, float *fb)
{
	float8p *dst = (float8p *)fdst;
	float8p *a = (float8p *)fa;
	float8p *b = (float8p *)fb;

	dst[0] = a[0] - b[0];
	dst[1] = a[1] - b[1];
	dst[2] = a[2] - b[2];
}

static void
mul3f8p(float *fdst, float *fa, float *fb)
{
	float8p *dst = (float8p *)fdst;
	float8p *a = (float8p *)fa;
	float8p *b = (float8p *)fb;

	dst[0] = a[0] * b[0];
	dst[1] = a[1] * b[1];
	dst[2] = a[2] * b[2];
}

static void
cross3f8p(float *fdst, float *fa, float *fb)
{
	float8p *dst = (float8p *)fdst;
	float8p *a = (float8p *)fa;
	float8p *b = (float8p *)fb;

	dst[0] = a[1]*b[2] - a[2]*b[1];
	dst[1] = a[2]*b[0] - a[0]*b[2];
	dst[2] = a[0]*b[1] - a[1]*b[0];
}

static void
dot3f8p(float *fdst, float *fa, float *fb)
{
	float8p *dst = (float8p *)fdst;
	float8p *a = (float8p *)fa;
	float8p *b = (float8p *)fb;

	*dst = a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

static void
scale3f8p(float *fa, float *fb, float *fscale)
{
	float8p *a = (float8p *)fa;
	float8p *b = (float8p *)fb;
	float8p scale = *(float8p *)fscale;

	a[0] = b[0] * scale;
	a[1] = b[1] * scale;
	a[2] = b[2] * scale;
}

static void
len3f8p(float *fdst, float *fa)
{
	float8p *dst = (float8p *)fdst;
	float8p *a = (float8p *)fa;

	dot3f8p(fdst, (float*)a, (float*)a);
	sqrtf8p(dst, *dst);
}

static void
abs3f8p(float *fdst, float *fa)
{
	float8p *dst = (float8p *)fdst;
	float8p *a = (float8p *)fa;

	absf8p(dst+0, a[0]);
	absf8p(dst+1, a[1]);
	absf8p(dst+2, a[2]);
}

static void
max3f8p(float *fdst, float *fa, float *fb)
{
	float8p *dst = (float8p *)fdst;
	float8p *a = (float8p *)fa;
	float8p *b = (float8p *)fb;

	maxf8p(dst+0, a[0], b[0]);
	maxf8p(dst+1, a[1], b[1]);
	maxf8p(dst+2, a[2], b[2]);
}

static void
min3f8p(float *fdst, float *fa, float *fb)
{
	float8p *dst = (float8p *)fdst;
	float8p *a = (float8p *)fa;
	float8p *b = (float8p *)fb;

	minf8p(dst+0, a[0], b[0]);
	minf8p(dst+1, a[1], b[1]);
	minf8p(dst+2, a[2], b[2]);
}
