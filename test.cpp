#include "include/vector.hpp"
#include "gtest/gtest.h"

TEST(Constructors, Default) {
    my_containers::MyVector<int> v1;
    EXPECT_EQ(v1.size(), 0);
    EXPECT_EQ(v1.capacity(), 0);
}

TEST(Constructors, InitializerList) {
    my_containers::MyVector<int> v1{ 1, 2, 3, 4 };
    EXPECT_EQ(v1.size(), 4);
    EXPECT_EQ(v1[0], 1);
    EXPECT_EQ(v1[1], 2);
    EXPECT_EQ(v1[2], 3);
    EXPECT_EQ(v1[3], 4);
}

TEST(Constructors, Move) {
    my_containers::MyVector<int> v1{ 1, 2, 3, 4 };
    my_containers::MyVector<int> v2{ std::move(v1) };
    EXPECT_EQ(v1.size(), 0);
    EXPECT_EQ(v2.size(), 4);
    EXPECT_EQ(v2[0], 1);
    EXPECT_EQ(v2[1], 2);
    EXPECT_EQ(v2[2], 3);
    EXPECT_EQ(v2[3], 4);
}

TEST(Constructors, Copy) {
    my_containers::MyVector<int> v1{ 1, 2, 3, 4 };
    my_containers::MyVector<int> v2{ v1 };
    EXPECT_EQ(v2.size(), 4);
    EXPECT_EQ(v2[0], 1);
    EXPECT_EQ(v2[1], 2);
    EXPECT_EQ(v2[2], 3);
    EXPECT_EQ(v2[3], 4);
}

TEST(Operators, CopyAssignment) {
    my_containers::MyVector<int> v1{ 1, 2, 3, 4 };
    my_containers::MyVector<int> v2;
    v2 = v1;
    EXPECT_EQ(v2.size(), 4);
    EXPECT_EQ(v2[0], 1);
    EXPECT_EQ(v2[1], 2);
    EXPECT_EQ(v2[2], 3);
    EXPECT_EQ(v2[3], 4);
}

TEST(Operators, MoveAssignment) {
    my_containers::MyVector<int> v1{ 1, 2, 3, 4 };
    my_containers::MyVector<int> v2;
    v2 = std::move(v1);
    EXPECT_EQ(v1.size(), 0);
    EXPECT_EQ(v2.size(), 4);
    EXPECT_EQ(v2[0], 1);
    EXPECT_EQ(v2[1], 2);
    EXPECT_EQ(v2[2], 3);
    EXPECT_EQ(v2[3], 4);
}

TEST(Methods, PushBack) {
    my_containers::MyVector<int> v1{ 1, 2, 3, 4 };
    v1.push_back(5);
    EXPECT_EQ(v1.size(), 5);
    EXPECT_EQ(v1[4], 5);
}

TEST(Methods, EmplaceBack) {
    my_containers::MyVector<my_containers::MyVector<int>> v1;
    v1.emplace_back({ 1, 2, 3, 4 });
    EXPECT_EQ(v1.size(), 1);
    EXPECT_EQ(v1[0].size(), 4);
}

TEST(Methods, PopBack) {
    my_containers::MyVector<int> v1{ 1, 2, 3, 4 };
    v1.pop_back();
    EXPECT_EQ(v1.size(), 3);
    EXPECT_EQ(v1[3], 3);
}

TEST(Methods, DefaultResize) {
    my_containers::MyVector<int> v1{ 1, 2, 3, 4 };
    v1.resize(10);
    EXPECT_EQ(v1.size(), 10);
    EXPECT_EQ(v1[9], 0);
}

TEST(Methods, ValueResize) {
    my_containers::MyVector<int> v1{ 1, 2, 3, 4 };
    v1.resize(10, 5);
    EXPECT_EQ(v1.size(), 10);
    EXPECT_EQ(v1[9], 5);
}

TEST(Methods, Clear) {
    my_containers::MyVector<int> v1{ 1, 2, 3, 4 };
    v1.clear();
    EXPECT_EQ(v1.size(), 0);
}

TEST(Methods, Empty) {
    my_containers::MyVector<int> v1{ 1, 2, 3, 4 };
    EXPECT_FALSE(v1.empty());
    v1.clear();
    EXPECT_TRUE(v1.empty());
}

TEST(Methods, Reserve) {
    my_containers::MyVector<int> v1;
    v1.reserve(10);
    EXPECT_EQ(v1.capacity(), 10);
}

TEST(Methods, Front) {
    my_containers::MyVector<int> v1{ 1, 2, 3, 4 };
    EXPECT_EQ(v1.front(), 1);
    v1.front() = 5;
    EXPECT_EQ(v1.front(), 5);
}

TEST(Methods, Back) {
    my_containers::MyVector<int> v1{ 1, 2, 3, 4 };
    EXPECT_EQ(v1.back(), 4);
    v1.back() = 5;
    EXPECT_EQ(v1.back(), 5);
}

TEST(Iterators, RangeBasedFor) {
    my_containers::MyVector<int> v1{ 1, 2, 3, 4 };
    for (int& i : v1) {
        i = 2;
    }
    for (int i : v1) {
        EXPECT_EQ(i, 2);
    }
}

TEST(Iterators, Iterator) {
    my_containers::MyVector<int> v1{ 1, 2, 3, 4 };
    auto it = v1.begin();
    EXPECT_EQ(*it, 1);
    it++;
    EXPECT_EQ(*it, 2);
    it += 2;
    EXPECT_EQ(*it, 4);
    it -= 3;
    EXPECT_EQ(*it, 1);
    it += 3;
    EXPECT_EQ(it - v1.begin(), 3);
    EXPECT_EQ(it - it, 0);
    EXPECT_EQ(v1.end() - it, 1);
    it++;
    EXPECT_EQ(it, v1.end());
}

TEST(Methods, At) {
    my_containers::MyVector<int> v1{ 1, 2, 3, 4 };
    EXPECT_EQ(v1.at(0), 1);
    EXPECT_EQ(v1.at(1), 2);
    EXPECT_EQ(v1.at(2), 3);
    EXPECT_EQ(v1.at(3), 4);
    EXPECT_THROW(v1.at(4), std::out_of_range);
}