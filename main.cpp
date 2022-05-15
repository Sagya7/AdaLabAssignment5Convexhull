#include <iostream>
#include <stack>
#include <stdlib.h>

struct Point
{
	int x;
	int y;
};

Point pfirst;

Point nextToTop(std::stack<Point> &S)
{
	Point p = S.top();
	S.pop();
	Point res = S.top();
	S.push(p);
	return res;
}

void swap(Point &p1, Point &p2)
{
	Point temp = p1;
	p1 = p2;
	p2 = temp;
}

int magnitude(Point p1, Point p2)
{
	return (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y);
}

int angle(Point p, Point q, Point r)
{
	int value = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

	if (value == 0) return 0;  // points are collinear
	return (value > 0) ? 1 : 2; // angle between the points is clockwise or anticlockwise
}

int compare(const void *vp1, const void *vp2)
{
	Point *p1 = (Point *)vp1;
	Point *p2 = (Point *)vp2;

	// Find orientation
	int o = angle(pfirst, *p1, *p2);
	if (o == 0)
		return (magnitude(pfirst, *p2) >= magnitude(pfirst, *p1)) ? -1 : 1;

	return (o == 2) ? -1 : 1;
}

void convexHull(Point* points, int n)
{
	// Pick least y value to start
	int ymin = points[0].y;
	int min = 0;
	for (int i = 1; i < n; i++)
	{
		int y = points[i].y;
		if ((y < ymin) || (ymin == y && points[i].x < points[min].x))
		{
			ymin = points[i].y;
			min = i;
		}
	}

	// keep ymin first.
	swap(points[0], points[min]);

	//sort remaining points
	pfirst = points[0];
	qsort(&points[1], n - 1, sizeof(Point), compare);

	int m = 1;
	for (int i = 1; i<n; i++)
	{
		// Remove the points if theey are collinear leaving the farthest point.
		while (i < n - 1 && angle(pfirst, points[i],points[i + 1]) == 0)
			i++;

		points[m] = points[i];
		m++;
	}

	if (m < 3) return;

	std::stack<Point> myStack;
	myStack.push(points[0]);
	myStack.push(points[1]);
	myStack.push(points[2]);

	for (int i = 3; i < m; i++)
	{
		//Remove points if angle is clockwise
		while (myStack.size() > 1 && angle(nextToTop(myStack), myStack.top(), points[i]) != 2)
		{
			myStack.pop();
		}
		myStack.push(points[i]);
	}

	std::cout << "The points that make convex polygon are\n";
	while (!myStack.empty())
	{
		Point p = myStack.top();
		std::cout << p.x << ", " << p.y << std::endl;
		myStack.pop();
	}
}

int main()
{
	while (true)
	{
		std::cout << "Enter number of points present ";
		int number_of_points;
		std::cin >> number_of_points;
		Point* points = (Point*)malloc(number_of_points * sizeof(Point));
		std::cout << "Enter " << number_of_points << " points\n";
		int x, y;
		for (int i = 0; i < number_of_points; i++)
		{
			std::cin >> x;
			std::cin >> y;
			points[i].x = x;
			points[i].y = y;
		}
		convexHull(points, number_of_points);
		
		std::cout << "Prss q to exit or any other key to repeat...";
		char next;
		std::cin >> next;
		if (next == 'q')
			break;
	}
	return 0;
}
