#ifndef RECTANGLEINTERSECTIONS_H
#define RECTANGLEINTERSECTIONS_H
#include <set>
#include <vector>
#include "rectangle.h"

namespace RectangleIntersections {
    static std::vector<std::set<Rectangle>> calculateIntersections(const std::vector<Rectangle> & vector) {

        std::vector<std::set<Rectangle>> result(vector.size());

        auto result_it = result.begin();

        for (auto rec : vector) {
            result_it->insert(rec);
        }

        //intersect only with rectangles with higher minimum id
        for (; result_it != result.end() -1 ; ++result_it ) {
            for (auto it = vector.begin() ; it != std::prev(vector.end()) ; ++it) {
                for (auto it_2 = result_it->lower_bound(*std::next(it)) ; it_2 != result_it->end(); ++it_2) {
                    auto intersect = it->intersect(*it_2);
                    if (intersect.has_value()) {
                        std::next(result_it)->insert(intersect.value());
                    }
                }
            }
        }
        return result;
    }
}


#endif // RECTANGLEINTERSECTIONS_H
