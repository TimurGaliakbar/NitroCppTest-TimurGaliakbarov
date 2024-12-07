#include <iostream>
#include <stdexcept>
#include "rectangle_intersections.h"
#include "parser.h"


int main(int argc, const char *argv[])
{
    std::vector<Rectangle> input_vector;
    try {
        input_vector = Parser::parse(argc, argv);
    } catch (std::runtime_error &e) {
        std::cerr << "Exeption on parsig file: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch(...) {
        std::cerr << "Unexpected exeption on parse file" << std::endl;
        return EXIT_FAILURE;
    }

    if (input_vector.empty()) {
        return EXIT_FAILURE;
    }

    for (auto it = input_vector.begin() ; it != input_vector.end(); ++it) {
        std::cout << *it;
    }

    std::vector<std::set<Rectangle>> result = RectangleIntersections::calculateIntersections(input_vector);


    for (auto it = result.begin() + 1; it != result.end(); ++it) {
        for (auto it2 = it->begin() ; it2 != it->end(); ++it2) {
           std::cout << *it2;

        }
    }


    return EXIT_SUCCESS;
}
