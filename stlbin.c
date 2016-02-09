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
#include <stdint.h>
#include <string.h>

static void
put16(char *buf, uint16_t val)
{
	buf[0] = val & 0xff;
	buf[1] = (val >> 8) & 0xff;
}

static void
put32(char *buf, uint32_t val)
{
	buf[0] = val & 0xff;
	buf[1] = (val >> 8) & 0xff;
	buf[2] = (val >> 16) & 0xff;
	buf[3] = (val >> 24) & 0xff;
}

/* TODO: write a real one */
static void
putvert(char *buf, float *vert)
{
	memcpy(buf, vert, 3 * sizeof vert[0]);
}

size_t
stlbin_begin(FILE *out, char *comment, unsigned char *rgba)
{
	char buf[84];

	memset(buf, 0, sizeof buf);
	strncpy(buf, comment, 69);

	if(rgba != NULL){
		strncpy(buf+70, "COLOR=", 6);
		buf[76] = rgba[0];
		buf[77] = rgba[1];
		buf[78] = rgba[2];
		buf[79] = rgba[3];
	}
	return fwrite(buf, 84, 1, out);
}

size_t
stlbin_facet(FILE *out, float *vert0, float *vert1, float *vert2, float *norm, unsigned char *rgba)
{
	unsigned char fake[] = {128, 128, 128, 128};
	char buf[50];

	if(norm != NULL)
		putvert(buf+0, norm);
	else
		memset(buf+0, 0, 12);
	putvert(buf+12, vert0);
	putvert(buf+24, vert1);
	putvert(buf+36, vert2);

	if(rgba == NULL)
		rgba = fake;
	put16(buf+48, rgba[0]/8 + (rgba[1]/8<<5) + (rgba[2]/8<<10));

	return fwrite(buf, 50, 1, out);
}

size_t
stlbin_end(FILE *out)
{
	char buf[4];
	off_t off;
	uint32_t ntris;

	if((off = ftello(out)) == -1)
		return -1;
	ntris = (off - 84) / 50;
	put32(buf, ntris);
	if(fseek(out, 80, SEEK_SET) == -1)
		return -1;
	if(fwrite(buf, 4, 1, out) != 1)
		return -1;
	if(fseek(out, off, SEEK_SET) == -1)
		return -1;
	return 0;
}
