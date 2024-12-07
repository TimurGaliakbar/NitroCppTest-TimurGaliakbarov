#include <gtest/gtest.h>
#include <limits>
#include "3rd-party/json.hpp"

#ifdef GTEST

#define private public
#define protected public

#endif

#include "parser.h"
#include "rectangle.h"
#include <iostream>
#include "rectangle_intersections.h"

TEST(Rectangle, SameRectangleIntersect ) {
    Rectangle rec(Point(0,0), 0,0,{0});
    Rectangle rec2(Point(0,0), 0,0,{0});
    auto result = rec.intersect(rec2);
    EXPECT_EQ(result, std::nullopt);
}


TEST(Rectangle, OnePointIntersect ) {
    Rectangle rec(Point(0,0), 0,0,{0});
    Rectangle rec2(Point(0,0), 0,0,{1});
    auto result = rec.intersect(rec2);
    EXPECT_NE(result, std::nullopt);
    EXPECT_EQ(result.value().m_top_left.m_x, 0);
    EXPECT_EQ(result.value().m_top_left.m_y, 0);
    EXPECT_EQ(result.value().m_w, 0);
    EXPECT_EQ(result.value().m_h, 0);
}

TEST(Rectangle, OnePointIntersect2 ) {
    Rectangle rec(Point(0,0), 0,0,{0});
    Rectangle rec2(Point(-10,-10), 20,20,{1});
    auto result = rec.intersect(rec2);
    EXPECT_NE(result, std::nullopt);
    EXPECT_EQ(result.value().m_top_left.m_x, 0);
    EXPECT_EQ(result.value().m_top_left.m_y, 0);
    EXPECT_EQ(result.value().m_w, 0);
    EXPECT_EQ(result.value().m_h, 0);
}


TEST(Rectangle, SideIntersect) {
    Rectangle rec(Point(0,0), 10,10,{0});
    Rectangle rec2(Point(0,10), 20,20,{1});
    auto result = rec.intersect(rec2);
    EXPECT_NE(result, std::nullopt);
    EXPECT_EQ(result.value().m_top_left.m_x, 0);
    EXPECT_EQ(result.value().m_top_left.m_y, 10);
    EXPECT_EQ(result.value().m_w, 10);
    EXPECT_EQ(result.value().m_h, 0);
}


TEST(Rectangle, OverflowInt) {
    Rectangle rec(Point(std::numeric_limits<int>::max(),std::numeric_limits<int>::max()), std::numeric_limits<unsigned int>::max(),std::numeric_limits<unsigned int>::max(),{0});
    Rectangle rec2(Point(std::numeric_limits<int>::max(),std::numeric_limits<int>::max()), std::numeric_limits<unsigned int>::max(),std::numeric_limits<unsigned int>::max(),{1});
    auto result = rec.intersect(rec2);
    EXPECT_NE(result, std::nullopt);

    EXPECT_EQ(result.value().m_top_left.m_x, std::numeric_limits<int>::max());
    EXPECT_EQ(result.value().m_top_left.m_y, std::numeric_limits<int>::max());
    EXPECT_EQ(result.value().m_w, std::numeric_limits<unsigned int>::max());
    EXPECT_EQ(result.value().m_h, std::numeric_limits<unsigned int>::max());
}

TEST(RectangleIntersections, TotalNumOfIntersections) {
    std::vector<Rectangle> input;
    input.reserve(10);
    for (size_t i = 0; i < 10 ; ++i) {
        input.push_back(Rectangle(Point(-1000,1000),1000,1000,{i}));
    }
    auto result = RectangleIntersections::calculateIntersections(input);
    size_t count = 0;
    for (auto it = result.begin(); it != result.end(); ++it) {
        count += it->size();
    }
    EXPECT_EQ(count, 1023);
}


TEST(Parser, jsonWithoutParameters) {
    const nlohmann::json obj = nlohmann::json::parse(R"({"happy": true, "pi": 3.141})");
    EXPECT_THROW({
        try
        {
          auto result = Parser::rectanglefromJson(obj, 0);
        }
        catch( const std::runtime_error& e )
        {
            EXPECT_STREQ( "In rectangle 0 is not defined one of parameters", e.what() );
            throw;
        }
    }, std::runtime_error );
}

TEST(Parser, jsonWithFloat) {
    const nlohmann::json obj = nlohmann::json::parse(R"({"x": 10, "y": 3.141, "y": 100, "h": 100})");
    EXPECT_THROW({
        try
        {
          auto result = Parser::rectanglefromJson(obj, 2);
        }
        catch( const std::runtime_error& e )
        {
            EXPECT_STREQ( "In rectangle 2 is not defined one of parameters", e.what() );
            throw;
        }
    }, std::runtime_error );
}

TEST(Parser, jsonLargerThenInt64) {
    const nlohmann::json obj = nlohmann::json::parse(R"({"x": 10, "y": 3.141, "w": 100, "h": 100})");
    EXPECT_THROW({
        try
        {
          auto result = Parser::rectanglefromJson(obj, 0);
        }
        catch( const std::runtime_error& e )
        {
            EXPECT_STREQ( "In rectangle 0 one of parameters is not int64_t or uint64_t", e.what() );
            throw;
        }
    }, std::runtime_error );
}

TEST(Parser, jsonLargerThenInt32) {
    const nlohmann::json obj = nlohmann::json::parse(R"({"x": 10, "y": 100, "w": 100, "h": 4294967296})");
    EXPECT_THROW({
        try
        {
          auto result = Parser::rectanglefromJson(obj, 0);
        }
        catch( const std::runtime_error& e )
        {
            EXPECT_STREQ( "In rectangle 0 one of parameters is not int32_t or uint32_t", e.what() );
            throw;
        }
    }, std::runtime_error );
}
