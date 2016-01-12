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

/*
 *	# List of geometric vertices, with (x,y,z[,w]) coordinates, w is optional and defaults to 1.0.
 *	v 0.123 0.234 0.345 1.0
 *	v ...
 *	...
 *	# List of texture coordinates, in (u, v [,w]) coordinates, these will vary between 0 and 1, w is optional and defaults to 0.
 *	vt 0.500 1 [0]
 *	vt ...
 *	...
 *	# List of vertex normals in (x,y,z) form; normals might not be unit vectors.
 *	vn 0.707 0.000 0.707
 *	vn ...
 *	...
 *	# Parameter space vertices in ( u [,v] [,w] ) form; free form geometry statement ( see below )
 *	vp 0.310000 3.210000 2.100000
 *	vp ...
 *	...
 *	# Polygonal face element (see below)
 *	f 1 2 3
 *	f 3/1 4/2 5/3
 *	f 6/4/1 3/5/3 7/6/5
 *	f ...
 *	...
 */

size_t
objtext_begin(FILE *out, char *comment, unsigned char *color)
{
}

size_t
objtext_facet(FILE *out, float *vert0, float *vert1, float *vert2, float *norm, unsigned char *_color)
{
}

size_t
objtext_end(FILE *out)
{
}
