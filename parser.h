#ifndef PARSER_H
#define PARSER_H
#include "3rd-party/json.hpp"
#include "rectangle.h"
#include <fstream>
#include <limits>

namespace Parser
{
    static std::string getFileName(int argc, const char *argv[]) {
        if (argc <2) {
            throw std::runtime_error("File name is not defined" );
        }
        return std::string(argv[1]);
    }


    static nlohmann::json getJsonFromFile(const std::string & file_name){

        std::ifstream f(file_name);
        if (!f.good()) {
            throw std::runtime_error("Can not open file: " + file_name );
        }
        nlohmann::json data;
        try {
            data = nlohmann::json::parse(f);
        } catch(...) {
            throw std::runtime_error("Can not parse json file: " + file_name );
        }

        if (!data.is_object()) {
            throw std::runtime_error("Can not parse json file: " + file_name );
        }

        auto rects = data["rects"];

        if (!rects.is_array()) {
            throw std::runtime_error("Can not parse array file: " + file_name );
        }
        return rects;
    }

    static Rectangle rectanglefromJson(const nlohmann::json & obj, RectangleId id) {
        if (!obj.is_object()) {
            throw std::runtime_error("In rectangle " + std::to_string(id) + " bad json object" );
        }
        if (!obj.contains("x") || !obj.contains("y") || !obj.contains("h") || !obj.contains("w")) {
            throw std::runtime_error("In rectangle " + std::to_string(id) + " is not defined one of parameters" );
        }
        if (!obj["x"].is_number_integer() || !obj["y"].is_number_integer() || !obj["h"].is_number_unsigned() || !obj["w"].is_number_unsigned()) {
            throw std::runtime_error("In rectangle " + std::to_string(id) + " one of parameters is not int64_t or uint64_t" );
        }
        int64_t x(obj["x"].template get<int64_t>());
        int64_t y(obj["y"].template get<int64_t>());
        int64_t w(obj["w"].template get<uint64_t>());
        int64_t h(obj["h"].template get<uint64_t>());
        if (x > std::numeric_limits<int>::max() || y > std::numeric_limits<int>::max() || h > std::numeric_limits<unsigned int>::max() || w > std::numeric_limits<unsigned int>::max()) {
            throw std::runtime_error("In rectangle " + std::to_string(id) + " one of parameters is not int32_t or uint32_t" );
        }
        return Rectangle(Point(x, y), w, h, {id});
    }


    static std::vector<Rectangle> getInputVector(const nlohmann::json & rects)  {
        std::vector<Rectangle> result;
        result.reserve(20);
        RectangleId i = 0;

        for (; i < 20 && i < rects.size(); ++i) {
            try {
                auto rec = rectanglefromJson(rects[i],i+1);
                result.push_back(rec);
            }
            catch (std::runtime_error &e) {
                if (result.size() < 10) {
                    std::cerr << "Can not parse first 10 rectangles in array: Abort " << std::endl;
                    std::cerr << "Exception: " << e.what() << '\n';
                    throw e;
                } else {
                    std::cerr << "Can not parse rectangle: " << i  << ": Skipped"<< std::endl;
                    break;
                }
            }
            catch(...) {
                std::cerr << "Unexpected exeption on creating rectangle " << i+1 << std::endl;
                throw std::exception();
            }
        }
        return result;
    }

    static std::vector<Rectangle> parse(int argc, const char *argv[]){
        auto file_name = getFileName(argc, argv);

        auto rects = getJsonFromFile(file_name);

        return getInputVector(rects);
    }


};

#endif // PARSER_H
