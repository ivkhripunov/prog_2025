//
// Created by ivankhripunov on 02.11.2025.
//

#include <gtest/gtest.h>
#include <vector>
#include <numeric>

struct Rectangle {
    int left_top_x;
    int left_top_y;

    int right_bottom_x;
    int right_bottom_y;

    [[nodiscard]] int area() const {
        return std::max(right_bottom_x - left_top_x, 0) * std::max(right_bottom_y - left_top_y, 0);
    }

    [[nodiscard]] bool operator==(const Rectangle &r) const {
        return left_top_x == r.left_top_x &&
               left_top_y == r.left_top_y &&
               right_bottom_x == r.right_bottom_x &&
               right_bottom_y == r.right_bottom_y;
    }
};

[[nodiscard]] int intersection_area(const std::vector<Rectangle> &rectangles) {
    const Rectangle rect = std::accumulate(rectangles.begin() + 1, rectangles.end(),
                                           rectangles[0],
                                           [](const Rectangle &first, const Rectangle &second) -> Rectangle {
                                               return Rectangle{
                                                       std::max(first.left_top_x, second.left_top_x),
                                                       std::max(first.left_top_y, second.left_top_y),
                                                       std::min(first.right_bottom_x, second.right_bottom_x),
                                                       std::min(first.right_bottom_y, second.right_bottom_y)
                                               };
                                           });

    return rect.area();
}

[[nodiscard]] Rectangle border_rect(const std::vector<Rectangle> &rectangles) {
    return std::accumulate(rectangles.begin() + 1, rectangles.end(),
                           rectangles[0],
                           [](const Rectangle &extremes, const Rectangle &rect) -> Rectangle {
                               return Rectangle{
                                       std::min(extremes.left_top_x, rect.left_top_x),
                                       std::min(extremes.left_top_y, rect.left_top_y),
                                       std::max(extremes.right_bottom_x, rect.right_bottom_x),
                                       std::max(extremes.right_bottom_y, rect.right_bottom_y)
                               };
                           });
}

TEST(TASK_03_01, INTERSECTION) {
    {
        const std::vector<Rectangle> rects = {{1, 1, 3, 3},
                                              {1, 1, 3, 3}};
        ASSERT_EQ(intersection_area(rects), 4);
    }

    {
        const std::vector<Rectangle> rects = {{1, 1, 3, 3},
                                              {2, 2, 3, 3}};
        ASSERT_EQ(intersection_area(rects), 1);
    }

    {
        const std::vector<Rectangle> rects = {{1, 1, 3, 3},
                                              {3, 3, 4, 4}};
        ASSERT_EQ(intersection_area(rects), 0);
    }

    {
        const std::vector<Rectangle> rects = {{1, 1, 3, 3},
                                              {4, 4, 5, 5}};
        ASSERT_EQ(intersection_area(rects), 0);
    }

    {
        const std::vector<Rectangle> rects = {{-1, -1, 3, 3},
                                              {0,  0,  1, 1}};
        ASSERT_EQ(intersection_area(rects), 1);
    }

    {
        const std::vector<Rectangle> rects = {{-1, -1, 3, 3},
                                              {0,  0,  1, 1},
                                              {4,  4,  5, 5}};
        ASSERT_EQ(intersection_area(rects), 0);
    }
}

TEST(TASK_03_01, BORDER) {
    {
        const std::vector<Rectangle> rects = {{1, 1, 3, 3},
                                              {1, 1, 3, 3}};
        const Rectangle border = {1, 1, 3, 3};
        ASSERT_EQ(border_rect(rects), border);
    }

    {
        const std::vector<Rectangle> rects = {{1, 1, 3, 3},
                                              {2, 2, 3, 3}};
        const Rectangle border = {1, 1, 3, 3};
        ASSERT_EQ(border_rect(rects), border);
    }

    {
        const std::vector<Rectangle> rects = {{1, 1, 3, 3},
                                              {3, 3, 4, 4}};
        const Rectangle border = {1, 1, 4, 4};
        ASSERT_EQ(border_rect(rects), border);
    }

    {
        const std::vector<Rectangle> rects = {{1, 1, 3, 3},
                                              {4, 4, 5, 5}};
        const Rectangle border = {1, 1, 5, 5};
        ASSERT_EQ(border_rect(rects), border);
    }

    {
        const std::vector<Rectangle> rects = {{-1, -1, 3, 3},
                                              {0,  0,  1, 1}};
        const Rectangle border = {-1, -1, 3, 3};
        ASSERT_EQ(border_rect(rects), border);
    }

    {
        const std::vector<Rectangle> rects = {{-1, -1, 3, 3},
                                              {0,  0,  1, 1},
                                              {4,  4,  5, 5}};
        const Rectangle border = {-1, -1, 5, 5};
        ASSERT_EQ(border_rect(rects), border);
    }
}