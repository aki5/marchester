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

size_t
stltext_begin(FILE *out, char *comment, unsigned char *color)
{
	return fprintf(out, "solid %s\n", comment);
}

size_t
stltext_facet(FILE *out, float *vert0, float *vert1, float *vert2, float *norm, unsigned char *_color)
{
	float fake[] = { 0.0f, 0.0f, 0.0f };
	size_t off = 0;
	if(norm == NULL)
		norm = fake;
	off += fprintf(out, "facet normal %f %f %f\n", norm[0], norm[1], norm[2]);
	off += fprintf(out, "	outer loop\n");
	off += fprintf(out, "		vertex %f %f %f\n", vert0[0], vert0[1], vert0[2]);
	off += fprintf(out, "		vertex %f %f %f\n", vert1[0], vert1[1], vert1[2]);
	off += fprintf(out, "		vertex %f %f %f\n", vert2[0], vert2[1], vert2[2]);
	off += fprintf(out, "	endloop\n");
	off += fprintf(out, "endfacet\n");
	if(off < 30) // decide we got a short write.
		return -1;
	return off;
}

size_t
stltext_end(FILE *out)
{
	return fprintf(out, "endsolid\n");
}
