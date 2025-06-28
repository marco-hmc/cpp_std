#include <iostream>
#include <memory>
#include <vector>

struct Point {
    double x, y;
    Point(double x, double y) : x(x), y(y) {}
};

struct Rect {
    double x, y, width, height;
    Rect(double x, double y, double width, double height)
        : x(x), y(y), width(width), height(height) {}

    bool contains(const Point& point) const {
        return point.x >= x && point.x <= x + width && point.y >= y &&
               point.y <= y + height;
    }

    bool intersects(const Rect& other) const {
        return other.x <= x + width && other.x + other.width >= x &&
               other.y <= y + height && other.y + other.height >= y;
    }
};

class Quadtree {
  public:
    Quadtree(const Rect& boundary, int capacity)
        : boundary(boundary), capacity(capacity) {}

    bool insert(const Point& point) {
        if (!boundary.contains(point)) {
            return false;
        }

        if (points.size() < capacity) {
            points.push_back(point);
            return true;
        }
        if (!divided) {
            subdivide();
        }
        if (northwest->insert(point)) {
            return true;
        }
        if (northeast->insert(point)) {
            return true;
        }
        if (southwest->insert(point)) {
            return true;
        }
        if (southeast->insert(point)) {
            return true;
        }

        return false;
    }

    void query(const Rect& range, std::vector<Point>& found) const {
        if (!boundary.intersects(range)) {
            return;
        }

        for (const auto& point : points) {
            if (range.contains(point)) {
                found.push_back(point);
            }
        }

        if (divided) {
            northwest->query(range, found);
            northeast->query(range, found);
            southwest->query(range, found);
            southeast->query(range, found);
        }
    }

  private:
    Rect boundary;
    int capacity;
    std::vector<Point> points;
    bool divided = false;
    std::unique_ptr<Quadtree> northwest, northeast, southwest, southeast;

    void subdivide() {
        double x = boundary.x;
        double y = boundary.y;
        double w = boundary.width / 2;
        double h = boundary.height / 2;

        northwest = std::make_unique<Quadtree>(Rect(x, y, w, h), capacity);
        northeast = std::make_unique<Quadtree>(Rect(x + w, y, w, h), capacity);
        southwest = std::make_unique<Quadtree>(Rect(x, y + h, w, h), capacity);
        southeast =
            std::make_unique<Quadtree>(Rect(x + w, y + h, w, h), capacity);

        divided = true;
    }
};

int main() {
    Rect boundary(0, 0, 200, 200);
    Quadtree qt(boundary, 4);

    qt.insert(Point(50, 50));
    qt.insert(Point(150, 150));
    qt.insert(Point(100, 100));
    qt.insert(Point(75, 75));
    qt.insert(Point(125, 125));

    Rect range(50, 50, 100, 100);
    std::vector<Point> found;
    qt.query(range, found);

    std::cout << "Points found: " << found.size() << std::endl;
    for (const auto& point : found) {
        std::cout << "(" << point.x << ", " << point.y << ")" << std::endl;
    }

    return 0;
}