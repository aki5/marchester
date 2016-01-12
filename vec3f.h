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

static void
copy3f(float *dst, float *src)
{
	dst[0] = src[0];
	dst[1] = src[1];
	dst[2] = src[2];
}

static void
lerp3f(float *dst, float *a, float *b, float bfact)
{
	float afact = 1.0 - bfact;
	dst[0] = a[0]*afact + b[0]*bfact;
	dst[1] = a[1]*afact + b[1]*bfact;
	dst[2] = a[2]*afact + b[2]*bfact;
}

static void
add3f(float *dst, float *a, float *b)
{
	dst[0] = a[0] + b[0];
	dst[1] = a[1] + b[1];
	dst[2] = a[2] + b[2];
}

static void
sub3f(float *dst, float *a, float *b)
{
	dst[0] = a[0] - b[0];
	dst[1] = a[1] - b[1];
	dst[2] = a[2] - b[2];
}

static void
cross3f(float *dst, float *a, float *b)
{
	dst[0] = a[1]*b[2] - a[2]*b[1];
	dst[1] = a[2]*b[0] - a[0]*b[2];
	dst[2] = a[0]*b[1] - a[1]*b[0];
}

static float
dot3f(float *a, float *b)
{
	return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

static void
scale3f(float *a, float *b, float scale)
{
	a[0] = b[0] * scale;
	a[1] = b[1] * scale;
	a[2] = b[2] * scale;
}

static float
len3f(float *a)
{
	return sqrtf(dot3f(a, a));
}

static void
abs3f(float *dst, float *a)
{
	dst[0] = fabsf(a[0]);
	dst[1] = fabsf(a[1]);
	dst[2] = fabsf(a[2]);
}

static void
max3f(float *dst, float *a, float *b)
{
	dst[0] = a[0] > b[0] ? a[0] : b[0];
	dst[1] = a[1] > b[1] ? a[1] : b[1];
	dst[2] = a[2] > b[2] ? a[2] : b[2];
}

static void
min3f(float *dst, float *a, float *b)
{
	dst[0] = a[0] < b[0] ? a[0] : b[0];
	dst[1] = a[1] < b[1] ? a[1] : b[1];
	dst[2] = a[2] < b[2] ? a[2] : b[2];
}

static void
normalize3f(float *a)
{
	float ilen = 1.0 / len3f(a);
	a[0] *= ilen;
	a[1] *= ilen;
	a[2] *= ilen;
}
