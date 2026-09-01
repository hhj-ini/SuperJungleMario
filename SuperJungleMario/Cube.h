FVertex v0 = { -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,          0.0f, 1.0f };
FVertex v1 = { 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,           1.0f, 1.0f };
FVertex v2 = { -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,           0.0f, 0.0f };
FVertex v3 = { 0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,            1.0f, 0.0f };

FVertex cube_vertices[] =
{
    v0, v2, v1,
    v2, v3, v1
};