#include "gtest/gtest.h"
#include "Math/Core.hpp"
#include <random>

namespace TG::Math
{
    static float gEpsilon = 0.001f;
    static float gMin = 0.1f;
    static float gMax = 100.0f;

    std::random_device gRd;
    std::mt19937 gGen(gRd());
    std::uniform_real_distribution gUrd(gMin, gMax);
    
    TEST(TestMatrix, Constructor)
    {
        Matrix4f mat;
        for (std::size_t i = 0; i < 16; ++i)
            EXPECT_NEAR(mat[i], 0, gEpsilon);
    }

    TEST(TestMatrix, CWiseBinaryOp)
    {
        Matrix4f mat1, mat2;
        for (std::size_t i = 0; i < 16; ++i)
        {
            mat1[i] = gUrd(gGen);
            mat2[i] = gUrd(gGen);
        }
        Matrix4f result = mat1 + mat2;
        for (std::size_t i = 0; i < 16; ++i)
            EXPECT_NEAR(result[i], mat1[i] + mat2[i], gEpsilon);

        result = mat1 - mat2;
        for (std::size_t i = 0; i < 16; ++i)
            EXPECT_NEAR(result[i], mat1[i] - mat2[i], gEpsilon);

        result = mat1.CWiseProduct(mat2);
        for (std::size_t i = 0; i < 16; ++i)
            EXPECT_NEAR(result[i], mat1[i] * mat2[i], gEpsilon);

        result = mat1 / mat2;
        for (std::size_t i = 0; i < 16; ++i)
            EXPECT_NEAR(result[i], mat1[i] / mat2[i], gEpsilon);
    }

    TEST(TestMatrix, Block)
    {
        Matrix4f mat4;
        for (std::size_t i = 0; i < 16; ++i)
            mat4[i] = gUrd(gGen);

        std::size_t startRow = 1, startCol = 1;
        Matrix3f mat3 = mat4.block<3, 3>(startRow, startCol);
        for (std::size_t i = 0; i < 3; ++i)
            for (std::size_t j = 0; j < 3; ++j)
                EXPECT_NEAR(mat3(i, j), mat4(startRow + i, startCol + j), gEpsilon);

        Matrix2f mat2;
        for (std::size_t i = 0; i < 4; ++i)
            mat2[i] = gUrd(gGen);
        mat4.block<2, 2>(0, 0) = mat2;
        for (std::size_t i = 0; i < 2; ++i)
            for (std::size_t j = 0; j < 2; ++j)
                EXPECT_NEAR(mat4(i, j), mat2(i, j), gEpsilon);
    }

    TEST(TestMatrix, Transpose)
    {
        Matrix4f mat4;
        for (std::size_t i = 0; i < 16; ++i)
            mat4[i] = gUrd(gGen);

        Matrix4f transpose = mat4.transpose();
        for (std::size_t i = 0; i < 4; ++i)
            for (std::size_t j = 0; j < 4; ++j)
                EXPECT_NEAR(transpose(i, j), mat4(j, i), gEpsilon);

        Matrix4f transpose1 = mat4.transpose().transpose();
        for (std::size_t i = 0; i < 4; ++i)
            for (std::size_t j = 0; j < 4; ++j)
                EXPECT_NEAR(transpose1(i, j), mat4(i, j), gEpsilon);
    }

    TEST(TestMatrix, Reduce)
    {
        Matrix4f mat4;
        for (std::size_t i = 0; i < 16; ++i)
            mat4[i] = gUrd(gGen);

        float temp = mat4.Sum();
        float sum = 0.0f;
        for (std::size_t i = 0; i < 16; ++i)
            sum += mat4[i];
        EXPECT_NEAR(sum, temp, gEpsilon);
    }

    TEST(TestMatrix, Dot)
    {
        Vector4f vec0, vec1;
        for (std::size_t i = 0; i < 4; ++i)
        {
            vec0[i] = gUrd(gGen);
            vec1[i] = gUrd(gGen);
        }

        float temp = vec0.Dot(vec1);
        float dot = 0;
        for (std::size_t i = 0; i < 4; ++i)
            dot += vec0[i] * vec1[i];
        EXPECT_NEAR(dot, temp, gEpsilon);
    }

    TEST(TestMatrix, MatrixMultiplication)
    {
        Matrix4f mat1, mat2;
        for (std::size_t i = 0; i < 16; ++i)
        {
            mat1[i] = gUrd(gGen);
            mat2[i] = gUrd(gGen);
        }

        Matrix4f mat3 = mat1 * mat2;
        Matrix4f mat4;
        for (std::size_t i = 0; i < 4; ++i)
        {
            for (std::size_t j = 0; j < 4; ++j)
            {
                mat4(i, j) = 0;
                for (std::size_t k = 0; k < 4; ++k)
                    mat4(i, j) += mat1(i, k) * mat2(k, j);
            }
        }
        for (std::size_t i = 0; i < 16; ++i)
            EXPECT_NEAR(mat3[i], mat4[i], gEpsilon);

        Matrix4f mat5 = mat1.LazyProduct(mat2);
        for (std::size_t i = 0; i < 16; ++i)
            EXPECT_NEAR(mat5[i], mat4[i], gEpsilon);
    }

    TEST(TestMatrix, NullaryOp)
    {
        Matrix4f mat1 = Matrix4f::Identity();
        for (std::size_t i = 0; i < 4; ++i)
        {
            for (std::size_t j = 0; j < 4; ++j)
            {
                if (i == j)
                    EXPECT_NEAR(mat1(i, j), 1.0f, gEpsilon);
                else
                    EXPECT_NEAR(mat1(i, j), 0.0f, gEpsilon);
            }
        }

        mat1 = Matrix4f::Zero();
        for (std::size_t i = 0; i < 16; ++i)
            EXPECT_NEAR(mat1[i], 0.0f, gEpsilon);

        float constant = 2.0f;
        mat1 = Matrix4f::Constant(constant);
        for (std::size_t i = 0; i < 16; ++i)
            EXPECT_NEAR(mat1[i], constant, gEpsilon);

        constant = 3.0f;
        mat1 = constant * Matrix4f::Identity();
        for (std::size_t i = 0; i < 16; ++i)
            EXPECT_NEAR(mat1[i], constant, gEpsilon);

        constant = 4.0f;
        mat1 = Matrix4f::Identity() * constant;
        for (std::size_t i = 0; i < 16; ++i)
            EXPECT_NEAR(mat1[i], constant, gEpsilon);
    }

    TEST(TestMatrix, Determinant)
    {
        std::array<std::size_t, 4> arr{4, 2, 3, 1};
        EXPECT_EQ(InversionNumber(arr), 5);

        Matrix2f mat2;
        for (std::size_t i = 0; i < 4; ++i)
            mat2[i] = gUrd(gGen);
        EXPECT_NEAR(mat2.Determinant(), mat2(0, 0) * mat2(1, 1) - mat2(0, 1) * mat2(1, 0), gEpsilon);

        Matrix3f mat3;
        for (std::size_t i = 0; i < 9; ++i)
            mat3[i] = 3.0f;
        EXPECT_NEAR(mat3.Determinant(), 0, gEpsilon);
        mat3 = Matrix3f::Identity();
        EXPECT_NEAR(mat3.Determinant(), 1.0f, gEpsilon);

        Matrix4f mat4;
        for (std::size_t i = 0; i < 16; ++i)
            mat4[i] = 4.0f;
        EXPECT_NEAR(mat4.Determinant(), 0, gEpsilon);
        mat4 = Matrix4f::Identity();
        EXPECT_NEAR(mat4.Determinant(), 1.0f, gEpsilon);
    }
}

// #include <format>
// #include <iostream>
// #include <iterator>
// #include <memory_resource>
// #include <string>
//
// std::pmr::string PMRStr(std::pmr::monotonic_buffer_resource *mbr)
// {
//     std::pmr::string buffer{mbr};
//
//     std::format_to(
//         std::back_inserter(buffer), //< OutputIt
//         "Hello, C++{}!\n",          //< fmt
//         "20");                      //< arg
//     std::cout << buffer;
//     return buffer;
// }
//
// #include <type_traits>
// #include <memory>
// #include <forward_list>
//
// std::unique_ptr<int> TestUnique()
// {
//     std::unique_ptr<int> pp = std::make_unique<int>();
//     *pp = 3;
//     return pp;
// }
//
// struct base
// {
//     virtual void print() { std::cout << "base class" << std::endl; }
// };
//
// struct derived : base
// {
//     void print() override { std::cout << "derived class" << num << std::endl; }
//     int num = 0;
// };
//
// struct derived1 : base
// {
//     void print() override { std::cout << "derived1 class" << num << std::endl; }
//     int num = 0;
// };
//
// struct Base
// {
//     // virtual void Print() = 0;
//     virtual ~Base() { std::cout << "base" << std::endl; };
// };
// struct Derived : Base
// {
//     // void Print() override { std::cout << "Derived\n"; }
//     int i{ 2 };
// };
// struct Derived1 : Base
// {
//     // ~Derived1() override { std::cout << "derived1" << std::endl; }
//     // void Print() override { std::cout << "Derived1\n"; }
//     int j{ 3 };
// };
// void TypeIdTest(const Base& b)
// {
//     std::cout << std::boolalpha << (typeid(b) != typeid(Derived1)) << std::endl;
// }
//
// int main()
// {
//     Derived1 d;
//     TypeIdTest(d);
//
//     int* inum = new int(2);
//     std::reference_wrapper<int> iwrap(*inum);
//     delete inum;
//     // std::unique_ptr<int> p2;
//     // p2 = TestUnique();
//     // std::pmr::monotonic_buffer_resource mbr;
//     // std::pmr::string buffer1 = PMRStr(&mbr);;
//     // std::cout << buffer1;
//     // // buffer.clear();
//     //
//     // derived d;
//     // d.num = 2;
//     // derived1 d1;
//     // const derived1& d2 = d1;
//     // std::vector<std::reference_wrapper<base>> vb;
//     // vb.emplace_back(d);
//     // vb.emplace_back(d1);
//     // for (auto b : vb)
//     //     b.get().print();
//     // std::erase_if(vb, [&d2](const std::reference_wrapper<base> b) { return std::addressof(b.get()) == std::addressof(d2); });
//     // for (auto b : vb)
//     //     b.get().print();
//     // std::format_to(
//     //     std::back_inserter(buffer), //< OutputIt
//     //     "Hello, {0}::{1}!{2}",      //< fmt
//     //     "std",                      //< arg {0}
//     //     "format_to()",              //< arg {1}
//     //     "\n",                       //< arg {2}
//     //     "extra param(s)...");       //< unused
//     // std::cout << buffer;
//     //
//     // std::wstring wbuffer;
//     // std::format_to(
//     //     std::back_inserter(wbuffer),//< OutputIt
//     //     L"Hello, {2}::{1}!{0}",     //< fmt
//     //     L"\n",                      //< arg {0}
//     //     L"format_to()",             //< arg {1}
//     //     L"std",                     //< arg {2}
//     //     L"...is not..."             //< unused
//     //     L"...an error!");           //< unused
//     // std::wcout << wbuffer;
// }
