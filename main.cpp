#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include "mylibraries/geometry.h"
#include "mylibraries/colors.h"

using point=geometry::coordinates::Cartesian<double>;
using line=geometry::Line<point, double>;
using color=colors::RGB;

std::string to_string(color col)
{
	constexpr static char alphabet[17]="0123456789ABCDEF";
	char result[7]={'#', alphabet[col.r/16], alphabet[col.r%16], alphabet[col.g/16], alphabet[col.g%16], alphabet[col.b/16], alphabet[col.b%16]};
	return std::string(result, 7);
}
constexpr point third(point p1, point p2, double ab)
{
	point dp=p2-p1;
	double c=dp.Distance();
	double angle=geometry::pi<double>-2*std::atan(ab);
	double vyska=sin(angle)*c/2;
	double c_0=(1-cos(angle))*c/2;
	double y=p1.y+c_0/c*dp.y+vyska/c*dp.x;
	double x=p1.x+c_0/c*dp.x-vyska/c*dp.y;
	return point(x, y);
}
line shifted(line ln)
{
	point shift=~(ln.end-ln.begin);
	shift.x*=-1;
	return line(ln.begin+shift, ln.end+shift);
}
void svg_open(std::ostream& out, int w, int h)
{
	out<<"<svg viewBox=\"0 0 "<<w<<" "<<h<<"\" xmlns=\"http://www.w3.org/2000/svg\">";
}
void svg_close(std::ostream& out)
{
	out<<"</svg>";
}
template<typename Cont>
void svg_polygon(std::ostream& out, const Cont& cont, const std::string& fill, const std::string& stroke, double offset, double width)
{
	out<<"<polygon points=\"";
	for(const auto& it:cont)
	{
		out<<it.x<<","<<offset-it.y<<" ";
	}
	out<<"\" fill=\""<<fill<<"\" stroke=\""<<stroke<<"\" stroke-width=\""<<width<<"\" />";
}
template<typename T, typename... Types>
std::vector<T> vec(const T& arg, const Types&... args)
{
	return std::move(std::vector({arg, args...}));
}
void svg_quadrilateral(std::ostream& out, line a, line b, const std::string& fill, const std::string& stroke, double offset, double width)
{
	svg_polygon(out, vec(a.begin, a.end, b.end, b.begin), fill, stroke, offset, width);
}
constexpr bool IsPowerOf2(int value)
{
	return value>0&&(value&(value-1))==0;
}
int main()
{
	double ab=0.0;
	int count=0;
	std::cin>>ab>>count;
	std::ofstream ofile("out.svg");
	std::vector<line> lines={line{point{400,100}, point{600,100}}};
	svg_open(ofile, 1000, 1000);
	int line_i=0;
	for(size_t i=0; i<lines.size() && i<(1u<<count)-1; ++i)
	{
		if(IsPowerOf2(i+1))
		{
			++line_i;
		}
		line moved=shifted(lines[i]);
		svg_quadrilateral(ofile, lines[i], moved, "#301500", "#00FF00", 1000, 1.0);
		point c=third(moved.begin, moved.end, ab);
		lines.push_back(line(moved.begin, c));
		lines.push_back(line(c, moved.end));
	}
	svg_close(ofile);
    return 0;
}