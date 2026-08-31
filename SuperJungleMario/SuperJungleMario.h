#pragma once

// 1. Define the triangle vertices
struct FVertexSimple
{
	float x, y, z;		// position
	float r, g, b, a;	// color
};

// structure for a 3D vector
struct FVector
{
	float x, y, z;
	FVector(float _x = 0, float _y = 0, float _z = 0) : x(_x), y(_y), z(_z) {}
};

struct FPos
{
	float x, y;
	FPos() : x(0.0f), y(0.0f) {};
};

// 경계를 나타내는 const 변수
constexpr float leftBorder = -1.0f;
constexpr float rightBorder = 1.0f;
constexpr float topBorder = 1.0f;
constexpr float bottomBorder = -1.0f;

// 델타타임 추가
inline double deltaTime = 1.0f;

inline FVertexSimple line_vertices[]
{
	{1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f ,0.0f},
	{0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f ,0.0f}
};
