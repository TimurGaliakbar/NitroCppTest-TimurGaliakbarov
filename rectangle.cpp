#include "rectangle.h"


Point::Point(int64_t x, int64_t y) : m_x(x), m_y(y){}

Point::Point(const Point &other) : m_x(other.m_x), m_y(other.m_y) {}

Rectangle::Rectangle(const Point top_left, uint64_t w, uint64_t h, const std::set<RectangleId> &rec_ids) : m_top_left(top_left),
    m_w(w),
    m_h(h),
    m_rec_ids(rec_ids) {}

Rectangle::Rectangle(const Rectangle &rec) : m_top_left(rec.m_top_left), m_w(rec.m_w), m_h(rec.m_h), m_rec_ids(rec.m_rec_ids) {}

std::string Rectangle::toString() const {
    std::stringstream ss;
    switch (m_rec_ids.size()) {
    case 0 : {
        ss << "Rectangle {NO ID}" << *(m_rec_ids.begin()) << ": ";
        break;
    }
    case 1: {
        ss << "Rectangle " << *(m_rec_ids.begin()) << ": ";
        break;
    }
    default: {
        ss << "Between rectangle ";
        for ( auto it = m_rec_ids.begin(); it != m_rec_ids.end(); ++it) {
            if (std::next(it) != m_rec_ids.end()) {
                if (std::next(std::next(it)) == m_rec_ids.end()) {
                    ss << *it << " and ";

                } else {
                    ss << *it << ", ";
                }
            } else {
                ss << *it << ' ';
            }
        }
        break;
    }
    }
    ss << "at " << m_top_left << ", w=" << m_w << ", h=" << m_h << '.'<< std::endl;
    return ss.str();
}

std::optional<Rectangle> Rectangle::intersect(const Rectangle &rec) const
{
    if (isIntersectionOf(rec) || rec.isIntersectionOf(*this)) {
        return std::nullopt;
    }

    // find x of a left side of possible intersection
    int64_t x = std::max(m_top_left.m_x, rec.m_top_left.m_x);
    // find y of top of possible intersection
    int64_t y = std::max(m_top_left.m_y, rec.m_top_left.m_y);

    // find x of a right side of possible intersection
    int64_t x2= std::min(m_top_left.m_x + m_w, rec.m_top_left.m_x + rec.m_w);

    // find y of a bottom  side of possible intersection
    int64_t y2 = std::min(m_top_left.m_y + m_h, rec.m_top_left.m_y + rec.m_h);

    // if result is not an intersection (h =0 and w =0 considered an intersection)
    if (x > x2 || y > y2) {
        return std::nullopt;
    }

    std::set<RectangleId> ids(m_rec_ids);
    ids.insert(rec.m_rec_ids.begin(), rec.m_rec_ids.end());

    return std::optional<Rectangle>(Rectangle(Point(x,y), x2-x, y2-y, ids));
}


