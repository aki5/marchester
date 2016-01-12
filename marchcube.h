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
 *
 *	Based on the wonderful code and explanation by Paul Bourke, available at
 *	http://paulbourke.net/geometry/polygonise/
 */

/*
 *	supplied cube is an array of 8 vertices, 3 floats (xyz) each,
 *	representing a cube of the following kind
 *
 *	x-axis grows to the right,
 *	y-axis grows toward you and
 *	z-axis grows up.
 *
 *		  4 ---- 5
 *		 /|     /|
 *		7 ---- 6 |
 *		| 0 ---| 1
 *		|/     |/
 *		3 ---- 2
 *
 *	hence, the unit cube could be as follows
 *
 *	float marchunit[] = {
 *		0,0,0, 1,0,0, 1,1,0, 0,1,0,
 *		0,0,1, 1,0,1, 1,1,1, 0,1,1
 *	};
 *
 */
void marchunit(float *cube, float x0, float y0, float z0, float x1, float y1, float z1);

/*
 *	Marchcube puts the surface triangles based on the supplied cube to the tris array and
 *	returns the number of triangles.
 *
 *	Each triangle is represented by 9 floats, and the maximum number of triangles returned
 *	is 5, so the supplied array needs to be float[45] or longer.
 */
int marchcube(float *cube, float *vals, float thrval, float *tris);
