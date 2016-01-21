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
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include "marchcube.h"
#include "stltext.h"
#include "stlbin.h"
#include "vec3f.h"

typedef struct Params Params;
struct Params {
	int istart, jstart, kstart;
	int iend, jend, kend;
	float xmin, ymin, zmin;
	float xstep, ystep, zstep;
	FILE *out;
	pthread_mutex_t *outlock;
};

float fieldfunc(float *ipos);

void
flushtris(FILE *out, float *tris, float *norms, int ntris)
{
	int i;
	for(i = 0; i < ntris; i++)
		stlbin_facet(out, tris+9*i+3, tris+9*i+0, tris+9*i+6, norms+3*i, NULL);
}

void *
marchloop(void *apar)
{
	enum {
		BufferTris = 1024,
	};
	float cube[8*3];
	float vals[8];
	float *valsx0;
	float *valsx1;
	float *valstmp;
	float *tris;
	float *norms;
	Params *par = (Params*)apar;
	int i, j, k, l;
	int y0i, y1i, ystride;
	int z0i, z1i;
	int n, ntris;

	ntris = 0;

	tris = malloc(9 * BufferTris * sizeof tris[0]);
	norms = malloc(3 * BufferTris * sizeof norms[0]);

	/*
	 *	the highest level loop moves the yz-plane along the x-axis, from minimum to maximum.
	 *	the following two arrays are used to cache computed values at cube vertices, so that
	 *	only one new value needs to be computed per inner cube, compared to 8 in a naive implementation.
	 *
	 *	valsx0 holds samples from the previous yz-plane, while valsx1 holds samples from the current yz-plane.
	 *	valsx1 is updated by the innermost loop as new values are computed. After the inner loops, the two
	 *	pointers are swapped before starting the next yz-plane.
	 */
	valsx0 = malloc((par->jend - par->jstart + 1) * (par->kend - par->kstart + 1) * sizeof valsx0[0]);
	valsx1 = malloc((par->jend - par->jstart + 1) * (par->kend - par->kstart + 1) * sizeof valsx1[0]);
	ystride = par->kend - par->kstart + 1;

	for(i = par->istart; i < par->iend; i++){ // x
		y0i = 0;
		for(j = par->jstart; j < par->jend; j++){ // y
			for(k = par->kstart; k < par->kend; k++){ // z
				marchunit(cube, par->xmin+i*par->xstep, par->ymin+j*par->ystep, par->zmin+k*par->zstep, par->xmin+(i+1)*par->xstep, par->ymin+(j+1)*par->ystep, par->zmin+(k+1)*par->zstep);
				z0i = k - par->kstart;
				z1i = z0i+1;
				y1i = y0i+ystride;

				if(i == par->istart || j == par->jstart || k == par->kstart){
					vals[0] = fieldfunc(cube + 3*0); // x0 y0 z0
					vals[1] = fieldfunc(cube + 3*1); // x1 y0 z0
					vals[2] = fieldfunc(cube + 3*2); // x1 y1 z0
					vals[3] = fieldfunc(cube + 3*3); // x0 y1 z0
					vals[4] = fieldfunc(cube + 3*4); // x0 y0 z1
					vals[5] = fieldfunc(cube + 3*5); // x1 y0 z1
					//vals[6] = fieldfunc(cube + 3*6); // x1 y1 z1
					vals[7] = fieldfunc(cube + 3*7); // x0 y1 z1
				} else {
					vals[0] = valsx0[y0i+z0i]; // x0 y0 z0
					vals[1] = valsx1[y0i+z0i]; // x1 y0 z0
					vals[2] = valsx1[y1i+z0i]; // x1 y1 z0
					vals[3] = valsx0[y1i+z0i]; // x0 y1 z0
					vals[4] = valsx0[y0i+z1i]; // x0 y0 z1
					vals[5] = valsx1[y0i+z1i]; // x1 y0 z1
					//vals[6] = fieldfunc(cube + 3*6); // x1 y1 z1
					vals[7] = valsx0[y1i+z1i]; // x0 y1 z1
				}

				valsx1[y1i+z1i] = vals[6] = fieldfunc(cube + 3*6); // x1 y1 z1

				n = marchcube(cube, vals, 0.0, tris + 9*ntris);
				if(n == -1){
					printf("marchcube: fail\n");
					exit(1);
				}

				for(l = 0; l < n; l++){
					float ab[3], ac[3];
					sub3f(ab, tris + 9*ntris+3, tris + 9*ntris+0);
					sub3f(ac, tris + 9*ntris+6, tris + 9*ntris+0);
					cross3f(norms+3*ntris, ab, ac);
					normalize3f(norms+3*ntris);
					ntris++;
				}

				if(ntris > BufferTris-5){
					pthread_mutex_lock(par->outlock);
					flushtris(par->out, tris, norms, ntris);
					pthread_mutex_unlock(par->outlock);
					ntris = 0;
				}
			}
			y0i += ystride;
		}
		// xstep: swap buffers.
		valstmp = valsx0;
		valsx0 = valsx1;
		valsx1 = valstmp;
	}
	if(ntris > 0){
		pthread_mutex_lock(par->outlock);
		flushtris(par->out, tris, norms, ntris);
		pthread_mutex_unlock(par->outlock);
		ntris = 0;
	}
	free(norms);
	free(tris);
	free(par);
	free(valsx0);
	free(valsx1);

	return NULL;
}

int
main(int argc, char *argv[])
{
	float xmin, ymin, zmin;
	float xmax, ymax, zmax;
	float xstep, ystep, zstep;
	int ndiv;

	if(argc != 8){
		fprintf(stderr, "usage: %s ndiv xmin ymin zmin xmax ymax zmax\n", argv[0]);
		exit(1);
	}

	ndiv = strtol(argv[1], NULL, 10);

	xmin = strtod(argv[2], NULL);
	ymin = strtod(argv[3], NULL);
	zmin = strtod(argv[4], NULL);

	xmax = strtod(argv[5], NULL);
	ymax = strtod(argv[6], NULL);
	zmax = strtod(argv[7], NULL);

	xstep = (xmax-xmin)/ndiv;
	ystep = (ymax-ymin)/ndiv;
	zstep = (zmax-zmin)/ndiv;

	stlbin_begin(stdout, "no comment", NULL);

	int i, nthreads = sysconf(_SC_NPROCESSORS_ONLN);;
	pthread_t thr[nthreads];
	pthread_mutex_t outlock;
	Params par = (Params){
		.istart = 0,
		.jstart = 0,
		.kstart = 0,
		.iend = ndiv,
		.jend = ndiv,
		.kend = ndiv,
		.xmin = xmin,
		.ymin = ymin,
		.zmin = zmin,
		.xstep = xstep,
		.ystep = ystep,
		.zstep = zstep,
		.out = stdout,
		.outlock = &outlock,
	};
	pthread_mutex_init(&outlock, NULL);
	for(i = 0; i < nthreads; i++){
		Params *parp;
		par.jstart = i*ndiv/nthreads;
		par.jend = (i+1)*ndiv/nthreads;
		parp = malloc(sizeof par);
		memcpy(parp, &par, sizeof par);
		pthread_create(&thr[i], NULL, marchloop, parp);
	}
	for(i = 0; i < nthreads; i++)
		pthread_join(thr[i], NULL);

	stlbin_end(stdout);

	return 0;
}