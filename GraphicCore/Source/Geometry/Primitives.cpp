/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Geometry/Primitives.h"

namespace TG::Geometry
{
    template<> Mesh CreatePrimitive<PrimitiveType::Quad>()
    {
        Mesh mesh;
        mesh.vertices.assign({
            {  0.5f, -0.5f, 0.0f },
            {  0.5f,  0.5f, 0.0f },
            { -0.5f,  0.5f, 0.0f },
            { -0.5f, -0.5f, 0.0f },
        });
        mesh.indices.assign({
            0, 1, 2,
            2, 3, 0,
        });
        mesh.uv.assign({
            { 0.0f, 0.0f },
            { 1.0f, 0.0f },
            { 1.0f, 1.0f },
            { 0.0f, 1.0f },
        });
        mesh.normals.assign({
            { 0.0f, 0.0f, 1.0f },
            { 0.0f, 0.0f, 1.0f },
            { 0.0f, 0.0f, 1.0f },
            { 0.0f, 0.0f, 1.0f },
        });
        mesh.tangents.assign({
            { 1.0f, 0.0f, 0.0f },
            { 1.0f, 0.0f, 0.0f },
            { 1.0f, 0.0f, 0.0f },
            { 1.0f, 0.0f, 0.0f },
        });
        return mesh;
    }

    template<> Mesh CreatePrimitive<PrimitiveType::Cube>()
    {
        Mesh mesh;
        mesh.vertices.assign({
            // 前
            {  0.5f, -0.5f, -0.5f },
            {  0.5f, -0.5f,  0.5f },
            { -0.5f, -0.5f,  0.5f },
            { -0.5f, -0.5f, -0.5f },
            // 后
            { -0.5f,  0.5f, -0.5f },
            { -0.5f,  0.5f,  0.5f },
            {  0.5f,  0.5f,  0.5f },
            {  0.5f,  0.5f, -0.5f },
            // 左
            {  0.5f,  0.5f, -0.5f },
            {  0.5f,  0.5f,  0.5f },
            {  0.5f, -0.5f,  0.5f },
            {  0.5f, -0.5f, -0.5f },
            // 右
            { -0.5f, -0.5f, -0.5f },
            { -0.5f, -0.5f,  0.5f },
            { -0.5f,  0.5f,  0.5f },
            { -0.5f,  0.5f, -0.5f },
            // 上
            {  0.5f, -0.5f,  0.5f },
            {  0.5f,  0.5f,  0.5f },
            { -0.5f,  0.5f,  0.5f },
            { -0.5f, -0.5f,  0.5f },
            // 下
            {  0.5f,  0.5f, -0.5f },
            {  0.5f, -0.5f, -0.5f },
            { -0.5f, -0.5f, -0.5f },
            { -0.5f,  0.5f, -0.5f },
        });
        mesh.indices.assign({
            // 前
            0, 1, 2, 2, 3, 0,
            // 后
            4, 5, 6, 6, 7, 4,
            // 左
            8, 9, 10, 10, 11, 8,
            // 右
            12, 13, 14, 14, 15, 12,
            // 上
            16, 17, 18, 18, 19, 16,
            // 下
            20, 21, 22, 22, 23, 20,
        });
        mesh.uv.assign({
            { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f },
            { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f },
            { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f },
            { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f },
            { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f },
            { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f },
        });
        mesh.normals.assign({
            // 前
            {  0.0f, -1.0f,  0.0f },
            {  0.0f, -1.0f,  0.0f },
            {  0.0f, -1.0f,  0.0f },
            {  0.0f, -1.0f,  0.0f },
            // 后
            {  0.0f,  1.0f,  0.0f },
            {  0.0f,  1.0f,  0.0f },
            {  0.0f,  1.0f,  0.0f },
            {  0.0f,  1.0f,  0.0f },
            // 左
            {  1.0f,  0.0f,  0.0f },
            {  1.0f,  0.0f,  0.0f },
            {  1.0f,  0.0f,  0.0f },
            {  1.0f,  0.0f,  0.0f },
            // 右
            { -1.0f,  0.0f,  0.0f },
            { -1.0f,  0.0f,  0.0f },
            { -1.0f,  0.0f,  0.0f },
            { -1.0f,  0.0f,  0.0f },
            // 上
            {  0.0f,  0.0f,  1.0f },
            {  0.0f,  0.0f,  1.0f },
            {  0.0f,  0.0f,  1.0f },
            {  0.0f,  0.0f,  1.0f },
            // 下
            {  0.0f,  0.0f, -1.0f },
            {  0.0f,  0.0f, -1.0f },
            {  0.0f,  0.0f, -1.0f },
            {  0.0f,  0.0f, -1.0f },
        });
        mesh.tangents.assign({
            // 前
            {  1.0f,  0.0f,  0.0f },
            {  1.0f,  0.0f,  0.0f },
            {  1.0f,  0.0f,  0.0f },
            {  1.0f,  0.0f,  0.0f },
            // 后
            { -1.0f,  0.0f,  0.0f },
            { -1.0f,  0.0f,  0.0f },
            { -1.0f,  0.0f,  0.0f },
            { -1.0f,  0.0f,  0.0f },
            // 左
            {  0.0f,  1.0f,  0.0f },
            {  0.0f,  1.0f,  0.0f },
            {  0.0f,  1.0f,  0.0f },
            {  0.0f,  1.0f,  0.0f },
            // 右
            {  0.0f, -1.0f,  0.0f },
            {  0.0f, -1.0f,  0.0f },
            {  0.0f, -1.0f,  0.0f },
            {  0.0f, -1.0f,  0.0f },
            // 上
            {  1.0f,  0.0f,  0.0f },
            {  1.0f,  0.0f,  0.0f },
            {  1.0f,  0.0f,  0.0f },
            {  1.0f,  0.0f,  0.0f },
            // 下
            {  1.0f,  0.0f,  0.0f },
            {  1.0f,  0.0f,  0.0f },
            {  1.0f,  0.0f,  0.0f },
            {  1.0f,  0.0f,  0.0f },
        });
        return mesh;
    }
}
