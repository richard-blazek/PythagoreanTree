#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>

struct Point
{
	double x, y;
};

struct Line
{
	Point begin, end;
};

constexpr Point third(Point p1, Point p2, double ab)
{
	double dx = p2.x - p1.x, dy = p2.y - p1.y;
	double c = std::sqrt(dx * dx + dy * dy);
	double angle = M_PI - 2 * std::atan(ab);
	double height = sin(angle) * c / 2;
	double c_0 = (1 - cos(angle)) * c / 2;
	double y = p1.y + c_0 / c * dy + height / c * dx;
	double x = p1.x + c_0 / c * dx - height / c * dy;
	return Point{x, y};
}

Line shifted(Line ln)
{
	double shift_y = ln.end.x - ln.begin.x;
	double shift_x = ln.begin.y - ln.end.y;
	return Line{Point{ln.begin.x + shift_x, ln.begin.y + shift_y}, Point{ln.end.x + shift_x, ln.end.y + shift_y}};
}

void svg_open(std::ostream &out, int w, int h)
{
	out << "<svg viewBox=\"0 0 " << w << " " << h << "\" xmlns=\"http://www.w3.org/2000/svg\">\n";
}

void svg_close(std::ostream &out)
{
	out << "</svg>";
}

void svg_quadrilateral(std::ostream &out, Line a, Line b, const std::string &fill, const std::string &stroke, double offset, double width)
{
	out << "<polygon points=\"";
	out << a.begin.x << "," << offset - a.begin.y << " ";
	out << a.end.x << "," << offset - a.end.y << " ";
	out << b.end.x << "," << offset - b.end.y << " ";
	out << b.begin.x << "," << offset - b.begin.y;
	out << "\" fill=\"" << fill << "\" stroke=\"" << stroke << "\" stroke-width=\"" << width << "\"/>\n";
}

constexpr bool IsPowerOf2(int value)
{
	return value > 0 && (value & (value - 1)) == 0;
}
int main()
{
	double ab = 0.0;
	int count = 0;
	std::cout << "Enter the a/b ratio: ";
	std::cin >> ab;
	std::cout << "Enter the precision: ";
	std::cin >> count;

	std::ofstream ofile("out.svg");
	std::vector<Line> lines = {Line{Point{400, 100}, Point{600, 100}}};
	svg_open(ofile, 1000, 1000);
	int line_i = 0;
	for (size_t i = 0; i < lines.size() && i < (1u << count) - 1; ++i)
	{
		if (IsPowerOf2(i + 1))
		{
			++line_i;
		}
		Line moved = shifted(lines[i]);
		svg_quadrilateral(ofile, lines[i], moved, "#301500", "#00FF00", 1000, 1.0);
		Point c = third(moved.begin, moved.end, ab);
		lines.push_back(Line{moved.begin, c});
		lines.push_back(Line{c, moved.end});
	}
	svg_close(ofile);
	return 0;
}