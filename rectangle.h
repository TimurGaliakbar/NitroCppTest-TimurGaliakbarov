#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <optional>
#include <sstream>
#include <iostream>
#include <set>


using RectangleId = size_t;

struct Point {

    Point(int64_t x, int64_t y);
    Point(const Point & other);

    bool operator==(const Point & other) {
        if (m_x == other.m_x && m_y == other.m_y) return true;
        return false;
    }

    friend std::ostream & operator<< (std::ostream & out, const Point & obj) {
        out << '(' << obj.m_x << "," << obj.m_y << ')';
        return out;
    }
    int64_t m_x;
    int64_t m_y;
};

struct Rectangle
{
public:
    Rectangle(Point top_left, uint64_t w, uint64_t h, const std::set<RectangleId>  & rec_ids);
    Rectangle(const Rectangle & rec);


    bool isIntersectionOf(const Rectangle & other) const {
        for (auto id : other.m_rec_ids) {
            if (m_rec_ids.find(id) == m_rec_ids.end()) {
                return false;
            }
        }
        return true;
    }

    bool operator==(const Rectangle & other) const {
        return m_rec_ids == other.m_rec_ids;
    }


    // to order rectangles inside any set by lowest id
    bool operator<(const Rectangle & other) const {
        for (auto it = m_rec_ids.begin(), it2 =other.m_rec_ids.begin();
             it !=m_rec_ids.end() && it2 != other.m_rec_ids.end();
             ++it, ++it2) {
            if (*it < *it2) return true;
            if (*it2 < *it) return false;
        }
        return m_rec_ids.size() < other.m_rec_ids.size();
    }



    std::string toString() const;

    friend std::ostream & operator<< (std::ostream & out, const Rectangle & obj) {
        out << obj.toString();
        return out;
    }

    std::optional<Rectangle> intersect(const Rectangle & rec) const;


    Point m_top_left;
    uint64_t m_w;
    uint64_t m_h;
    std::set<RectangleId> m_rec_ids;

};


#endif // RECTANGLE_H
