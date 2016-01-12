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
	float *tris;
	float *norms;
	Params *par = (Params*)apar;
	int i, j, k, l;
	int n, ntris;

	ntris = 0;
	tris = malloc(9 * BufferTris * sizeof tris[0]);
	norms = malloc(3 * BufferTris * sizeof norms[0]);
	for(i = par->istart; i < par->iend; i++){
		for(j = par->jstart; j < par->jend; j++){
			for(k = par->kstart; k < par->kend; k++){

				marchunit(cube, par->xmin+i*par->xstep, par->ymin+j*par->ystep, par->zmin+k*par->zstep, par->xmin+(i+1)*par->xstep, par->ymin+(j+1)*par->ystep, par->zmin+(k+1)*par->zstep);

				for(l = 0; l < 8; l++)
					vals[l] = fieldfunc(cube + 3*l);
				
				n = marchcube(cube, vals, 0.0, tris + 9*ntris);

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
		}
	}
	if(ntris > 0){
		pthread_mutex_lock(par->outlock);
		flushtris(par->out, tris, norms, ntris);
		pthread_mutex_unlock(par->outlock);
		ntris = 0;
	}
	free(norms);
	free(tris);

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

	int i, nthreads = 2;
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
		par.istart = i*ndiv/nthreads;
		par.iend = (i+1)*ndiv/nthreads;
		parp = malloc(sizeof par);
		memcpy(parp, &par, sizeof par);
		pthread_create(&thr[i], NULL, marchloop, parp);
	}
	for(i = 0; i < nthreads; i++)
		pthread_join(thr[i], NULL);

	stlbin_end(stdout);

	return 0;
}