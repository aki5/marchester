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

static void
bcast3fp(float *fdst, float *src)
{
	floatp *dst = (floatp *)fdst;
	bcastfp(&dst[0], src[0]);
	bcastfp(&dst[1], src[1]);
	bcastfp(&dst[2], src[2]);
}

static void
copy3fp(float *fdst, float *fsrc)
{
	floatp *dst = (floatp *)fdst;
	floatp *src = (floatp *)fsrc;

	copyfp(&dst[0], src[0]);
	copyfp(&dst[1], src[1]);
	copyfp(&dst[2], src[2]);
}

static void
lerp3fp(float *fdst, float *fa, float *fb, float *fbfact)
{
	floatp *dst = (floatp *)fdst;
	floatp *a = (floatp *)fa;
	floatp *b = (floatp *)fb;
	floatp bfact = *(floatp *)fbfact;
	floatp afact;
	bcastfp(&afact, 1.0f);
	subfp(&afact, afact, bfact);
	maddfp(&dst[0], a[0], afact, b[0], bfact);
	maddfp(&dst[1], a[1], afact, b[1], bfact);
	maddfp(&dst[2], a[2], afact, b[2], bfact);
}

static void
add3fp(float *fdst, float *fa, float *fb)
{
	floatp *dst = (floatp *)fdst;
	floatp *a = (floatp *)fa;
	floatp *b = (floatp *)fb;

	addfp(&dst[0], a[0], b[0]);
	addfp(&dst[1], a[1], b[1]);
	addfp(&dst[2], a[2], b[2]);
}

static void
sub3fp(float *fdst, float *fa, float *fb)
{
	floatp *dst = (floatp *)fdst;
	floatp *a = (floatp *)fa;
	floatp *b = (floatp *)fb;

	subfp(&dst[0], a[0], b[0]);
	subfp(&dst[1], a[1], b[1]);
	subfp(&dst[2], a[2], b[2]);
}

static void
mul3fp(float *fdst, float *fa, float *fb)
{
	floatp *dst = (floatp *)fdst;
	floatp *a = (floatp *)fa;
	floatp *b = (floatp *)fb;

	mulfp(&dst[0], a[0], b[0]);
	mulfp(&dst[1], a[1], b[1]);
	mulfp(&dst[2], a[2], b[2]);
}

static void
cross3fp(float *fdst, float *fa, float *fb)
{
	floatp *dst = (floatp *)fdst;
	floatp *a = (floatp *)fa;
	floatp *b = (floatp *)fb;

	msubfp(&dst[0], a[1],b[2], a[2],b[1]);
	msubfp(&dst[1], a[2],b[0], a[0],b[2]);
	msubfp(&dst[2], a[0],b[1], a[1],b[0]);
}

static void
dot3fp(float *fdst, float *fa, float *fb)
{
	floatp *dst = (floatp *)fdst;
	floatp *a = (floatp *)fa;
	floatp *b = (floatp *)fb;
	floatp tmp0, tmp1;

	mulfp(dst, a[0], b[0]);
	mulfp(&tmp0, a[1], b[1]);
	mulfp(&tmp1, a[2], b[2]);
	addfp(dst, *dst, tmp0);
	addfp(dst, *dst, tmp1);
}

static void
scale3fp(float *fa, float *fb, float *fscale)
{
	floatp *a = (floatp *)fa;
	floatp *b = (floatp *)fb;
	floatp scale = *(floatp *)fscale;

	a[0] = b[0] * scale;
	a[1] = b[1] * scale;
	a[2] = b[2] * scale;
}

static void
len3fp(float *fdst, float *fa)
{
	floatp *dst = (floatp *)fdst;
	floatp *a = (floatp *)fa;

	dot3fp(fdst, (float*)a, (float*)a);
	sqrtfp(dst, *dst);
}

static void
abs3fp(float *fdst, float *fa)
{
	floatp *dst = (floatp *)fdst;
	floatp *a = (floatp *)fa;

	absfp(dst+0, a[0]);
	absfp(dst+1, a[1]);
	absfp(dst+2, a[2]);
}

static void
max3fp(float *fdst, float *fa, float *fb)
{
	floatp *dst = (floatp *)fdst;
	floatp *a = (floatp *)fa;
	floatp *b = (floatp *)fb;

	maxfp(dst+0, a[0], b[0]);
	maxfp(dst+1, a[1], b[1]);
	maxfp(dst+2, a[2], b[2]);
}

static void
min3fp(float *fdst, float *fa, float *fb)
{
	floatp *dst = (floatp *)fdst;
	floatp *a = (floatp *)fa;
	floatp *b = (floatp *)fb;

	minfp(dst+0, a[0], b[0]);
	minfp(dst+1, a[1], b[1]);
	minfp(dst+2, a[2], b[2]);
}
