#include <stdio.h>
#include <math.h>

typedef struct point
{
	float x, y;
} point;

point third(point a, point b, float ratio)
{
	float dx = b.x - a.x;
	float dy = b.y - a.y;
	float c = sqrt(dx * dx + dy * dy);
	float angle = M_PI - 2 * atan(ratio);
	float h = sin(angle) * c / 2;
	float c_0 = (1 - cos(angle)) * c / 2;
	point result = {a.x + c_0 / c * dx - h / c * dy, a.y + c_0 / c * dy + h / c * dx};
	return result;
}

void process_line(FILE *out, point bl, point br, int depth, float ratio)
{
	point tl = {bl.x + bl.y - br.y, bl.y + br.x - bl.x};
	point tr = {br.x + bl.y - br.y, br.y + br.x - bl.x};

	fprintf(out, "<polygon points=\"%.2f,%.2f %.2f,%.2f %.2f,%.2f %.2f,%.2f\" fill=\"#301500\" stroke=\"#00FF00\" stroke-width=\"1\"/>\n",
		bl.x, 1000 - bl.y, br.x, 1000 - br.y, tr.x, 1000 - tr.y, tl.x, 1000 - tl.y);

	if (depth > 0)
	{
		point c = third(tl, tr, ratio);
		process_line(out, tl, c, depth - 1, ratio);
		process_line(out, c, tr, depth - 1, ratio);
	}
}

int main(int argc, char *argv[])
{
	if (argc < 3) return 1;
	float ratio;
	int count;
	sscanf(argv[1], "%f", &ratio);
	sscanf(argv[2], "%d", &count);

	FILE *out = fopen("out.svg", "w");
	fputs("<svg viewBox=\"0 0 1000 1000\" xmlns=\"http://www.w3.org/2000/svg\">\n", out);

	point bl = {400, 100}, br = {600, 100};
	process_line(out, bl, br, count, ratio);
	fputs("</svg>", out);
	fclose(out);
	return 0;
}
