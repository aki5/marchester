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

typedef float simdfloat __attribute__((vector_size(32)));

static void
simdabsf(simdfloat *dst, simdfloat a)
{
}

static void
simdsqrtf(simdfloat *dst, simdfloat a)
{
}

static void
simdminf(simdfloat *dst, simdfloat a, simdfloat b)
{
}

static void
simdmaxf(simdfloat *dst, simdfloat a, simdfloat b)
{
}

static void
simdcopy3f(float *fdst, float *fsrc)
{
	simdfloat *dst = (simdfloat *)fdst;
	simdfloat *src = (simdfloat *)fsrc;

	dst[0] = src[0];
	dst[1] = src[1];
	dst[2] = src[2];
}

static void
simdlerp3f(float *fdst, float *fa, float *fb, float *fbfact)
{
	simdfloat *dst = (simdfloat *)fdst;
	simdfloat *a = (simdfloat *)fa;
	simdfloat *b = (simdfloat *)fb;
	simdfloat bfact = *(simdfloat *)fbfact;
	simdfloat afact = 1.0 - bfact;

	dst[0] = a[0]*afact + b[0]*bfact;
	dst[1] = a[1]*afact + b[1]*bfact;
	dst[2] = a[2]*afact + b[2]*bfact;
}

static void
simdadd3f(float *fdst, float *fa, float *fb)
{
	simdfloat *dst = (simdfloat *)fdst;
	simdfloat *a = (simdfloat *)fa;
	simdfloat *b = (simdfloat *)fb;

	dst[0] = a[0] + b[0];
	dst[1] = a[1] + b[1];
	dst[2] = a[2] + b[2];
}

static void
simdsub3f(float *fdst, float *fa, float *fb)
{
	simdfloat *dst = (simdfloat *)fdst;
	simdfloat *a = (simdfloat *)fa;
	simdfloat *b = (simdfloat *)fb;

	dst[0] = a[0] - b[0];
	dst[1] = a[1] - b[1];
	dst[2] = a[2] - b[2];
}

static void
simdmul3f(float *fdst, float *fa, float *fb)
{
	simdfloat *dst = (simdfloat *)fdst;
	simdfloat *a = (simdfloat *)fa;
	simdfloat *b = (simdfloat *)fb;

	dst[0] = a[0] * b[0];
	dst[1] = a[1] * b[1];
	dst[2] = a[2] * b[2];
}

static void
simdcross3f(float *fdst, float *fa, float *fb)
{
	simdfloat *dst = (simdfloat *)fdst;
	simdfloat *a = (simdfloat *)fa;
	simdfloat *b = (simdfloat *)fb;

	dst[0] = a[1]*b[2] - a[2]*b[1];
	dst[1] = a[2]*b[0] - a[0]*b[2];
	dst[2] = a[0]*b[1] - a[1]*b[0];
}

static void
simddot3f(float *fdst, float *fa, float *fb)
{
	simdfloat *dst = (simdfloat *)fdst;
	simdfloat *a = (simdfloat *)fa;
	simdfloat *b = (simdfloat *)fb;

	*dst = a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

static void
simdscale3f(float *fa, float *fb, float *fscale)
{
	simdfloat *a = (simdfloat *)fa;
	simdfloat *b = (simdfloat *)fb;
	simdfloat scale = *(simdfloat *)fscale;

	a[0] = b[0] * scale;
	a[1] = b[1] * scale;
	a[2] = b[2] * scale;
}

static void
simdlen3f(float *dst, float *fa)
{
	simdfloat *dst = (simdfloat *)fdst;

	simddot3f((float*)dst, a, a);
	simdsqrtf(dst, dst);
}

static void
simdabs3f(float *fdst, float *fa)
{
	simdfloat *dst = (simdfloat *)fdst;
	simdfloat *a = (simdfloat *)fa;

	simdabsf(dst+0, a[0]);
	simdabsf(dst+1, a[1]);
	simdabsf(dst+2, a[2]);
}

static void
simdmax3f(float *fdst, float *fa, float *fb)
{
	simdfloat *dst = (simdfloat *)fdst;
	simdfloat *a = (simdfloat *)fa;
	simdfloat *b = (simdfloat *)fb;

	simdmaxf(dst+0, a[0], b[0]);
	simdmaxf(dst+1, a[1], b[1]);
	simdmaxf(dst+2, a[2], b[2]);
}

static void
simdmin3f(float *fdst, float *fa, float *fb)
{
	simdfloat *dst = (simdfloat *)fdst;
	simdfloat *a = (simdfloat *)fa;
	simdfloat *b = (simdfloat *)fb;

	simdminf(a[0], b[0]);
	simdminf(a[1], b[1]);
	simdminf(a[2], b[2]);
}
