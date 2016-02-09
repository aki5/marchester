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
#include "mathp.h"
#include "vec3f.h"

static float clampf(float a, float min, float max) {return a < min ? min : a > max ? max : a; }
static float lerpf(float a, float b, float bfact) { return (1.0f-bfact)*a + bfact*b; }
static float absf(float a) { return a < 0.0 ? -a : a; }

static float minf(float a, float b) { return a < b ? a : b; }
static float maxf(float a, float b) { return a > b ? a : b; }

#if 0
float
sminf(float a, float b)
{
	float k = 32.0f;
	float res = expf(-k*a) + expf(-k*b);
	return -log(res)/k;
}
#else
static float
sminf(float a, float b)
{
	float k = 0.1;
	float h = clampf(0.5+0.5*(b-a)/k, 0.0, 1.0);
	return lerpf(b, a, h) - k*h*(1.0-h);
}
#endif

static float
smaxf(float a, float b)
{
	return -sminf(-a, -b);
}

static float square(float a) { return a*a; }

static float
halfspace(float *pos, float *pl)
{
	return dot3f(pos, pl) - pl[3];
}

static float
sphere(float *pos, float r)
{
	return sqrtf(square(pos[0])+square(pos[1])+square(pos[2]))-r;
}

static float
cylinder(float *pos, int axis, float r)
{
	int ax0, ax1;
	ax0 = (axis+1)%3; // 1 2 0
	ax1 = (axis+2)%3; // 2 0 1
	return sqrtf(square(pos[ax0])+square(pos[ax1]))-r;
}

static float
cube(float *pos, float *size)
{
	float d[3];
	float dst;
	abs3f(d, pos);
	sub3f(d, d, size);
	dst = minf(maxf(d[0], maxf(d[1],d[2])),0.0);	
	max3f(d, d,(float[]){0,0,0});
	return dst + len3f(d);
}

static float
roundcube(float *pos, float *size, float r)
{
	float d[3];
	abs3f(d, pos);
	sub3f(d, d, size);
	max3f(d, d, (float[]){0,0,0});
	return len3f(d)-r;
}

static float
torus(float *pos, float a, float b)
{
	float tmp;
	tmp = sqrtf(square(pos[0])+square(pos[2]))-a;
	return sqrtf(square(tmp)+square(pos[1]))-b;
}

static float
capsule(float *pos, float *a, float *b, float r)
{
	float pa[3], ba[3], v[3];
	float h;

	sub3f(pa, pos, a);
	sub3f(ba, b, a);
	h = clampf(dot3f(pa, ba)/dot3f(ba, ba), 0.0, 1.0);
	scale3f(ba, ba, h);
	sub3f(v, pa, ba);
	return len3f(v) - r;
}

static float
boolisect(float a, float b)
{
	return maxf(a, b);
}

static float
boolsub(float a, float b)
{
	return maxf(a, -b);
}

static float
booladd(float a, float b)
{
	return minf(a, b);
}

static float
sboolisect(float a, float b)
{
	return smaxf(a, b);
}

static float
sboolsub(float a, float b)
{
	return smaxf(a, -b);
}

static float
sbooladd(float a, float b)
{
	return sminf(a, b);
}

static void
twist3f(float *pos, float angvel)
{
	float c = cosf(angvel*pos[2]);
	float s = sinf(angvel*pos[2]);
	float x = c*pos[0] - s*pos[1];
	float y = s*pos[0] + c*pos[1];
	pos[0] = x;
	pos[1] = y;
}

static void
rep3f(float *pos, float c)
{
	pos[0] = fmodf(fabsf(pos[0]), c)-0.5*c;
	pos[1] = fmodf(fabsf(pos[1]), c)-0.5*c;
	pos[2] = fmodf(fabsf(pos[2]), c)-0.5*c;
}

void
field(float *r, float *ipos)
{
	float pos[3];
	float rep[3];
	float dst = 1.75f;
	float boxes;
	float tor;

/*
	float caps;
	caps = capsule(ipos, (float[]){-1,-1,-1}, (float[]){1,1,1}, 0.5);
*/

	copy3f(pos, ipos);
//	twist3f(pos, 1.5f);

	copy3f(rep, ipos);
	rep3f(rep, 0.375);
	boxes = roundcube(rep, (float[]){0.1, 0.1, 0.1f}, 0.1);

/*
	float box;
	box = sphere(pos, 1.0);
*/
	tor = torus(pos, 1.0, 0.6);



/*
	float cyl1 = cylinder(pos, 2, 0.5);
	float cyl2 = cylinder(pos, 0, 0.5);
	float cyl3 = cylinder(pos, 1, 0.5);
	float sect = halfspace(pos, (float[]){0.00,1.00,0.00,0.1});
*/

	float res;

/*
	res = box;
	res = sbooladd(res, cyl1);
	res = sbooladd(res, cyl2);
	res = sboolsub(res, cyl3);
*/
	res = tor;
	res = boolsub(res, boxes);

	float tor2;
	tor2 = fabsf(torus(pos, 1.0, 0.6))-0.05;
	tor2 = boolisect(tor2, halfspace(pos, (float[]){0,1,0,0.5}));
	tor2 = boolisect(tor2, halfspace(pos, (float[]){0,-1,0,0.5}));
	res = booladd(res, tor2);


	res = boolisect(res, halfspace(pos, (float[]){0,1,0,0.5}));
	res = boolisect(res, halfspace(pos, (float[]){0,-1,0,0.5}));

	//res = sboolisect(res, sect);

	/* clip to bounding box  */
	float negx = halfspace(ipos, (float[]){-1,0,0,dst});
	float negy = halfspace(ipos, (float[]){0,-1,0,dst});
	float negz = halfspace(ipos, (float[]){0,0,-1,dst});
	float posx = halfspace(ipos, (float[]){1,0,0,dst});
	float posy = halfspace(ipos, (float[]){0,1,0,dst});
	float posz = halfspace(ipos, (float[]){0,0,1,dst});

	res = boolisect(res, negx);
	res = boolisect(res, negy);
	res = boolisect(res, negz);
	res = boolisect(res, posx);
	res = boolisect(res, posy);
	res = boolisect(res, posz);

	*r = res;
}
