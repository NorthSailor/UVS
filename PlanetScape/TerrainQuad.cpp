#include "TerrainQuad.h"
using namespace PlanetScape;
using namespace FV;
using namespace std;
using namespace glm;

shared_ptr<Mesh<vec2>> TerrainQuad::s_quadMesh;

TerrainQuad::TerrainQuad(std::shared_ptr<Quad> parent) :
    Quad(parent)
{
}

void TerrainQuad::CreateTileMesh()
{
    const int hSize = TILE_SIZE / 2;
    vec2 vertices[2 * hSize + 1][2 * hSize + 1]; // [x][y]
    for (int i = -hSize; i <= hSize; i++) {
        for (int j = -hSize; j <= hSize; j++) {
            vertices[i + hSize][j + hSize] = vec2(i, j);
        }
    }

    unsigned int indices[TILE_SIZE][TILE_SIZE][6];
    for (int i = 0; i < TILE_SIZE; i++) {
        for (int j = 0; j < TILE_SIZE; j++) {
            unsigned int sw = i * (TILE_SIZE + 1) + j;
            unsigned int se = sw + (TILE_SIZE + 1);
            unsigned int nw = sw + 1;
            unsigned int ne = nw + (TILE_SIZE + 1);
            indices[i][j][0] = sw;
            indices[i][j][1] = se;
            indices[i][j][2] = ne;
            indices[i][j][3] = sw;
            indices[i][j][4] = ne;
            indices[i][j][5] = nw;
        }
    }

    s_quadMesh = make_shared<Mesh<vec2>>();
    s_quadMesh->VAO.Bind();
    s_quadMesh->vertexBuffer.Bind();
    s_quadMesh->vertexBuffer.SetData((vec2*)vertices, pow(TILE_SIZE + 1, 2));
    s_quadMesh->vertexBuffer.SetAttribPointer(0, 2);
    s_quadMesh->indexBuffer.Bind();
    s_quadMesh->indexCount = pow(TILE_SIZE, 2) * 6;
    s_quadMesh->indexBuffer.SetData((unsigned int *)indices,
                                    s_quadMesh->indexCount);
}

void TerrainQuad::RenderTile()
{
    s_quadMesh->VAO.Bind();
    glDrawElements(GL_TRIANGLES, s_quadMesh->indexCount,
                   GL_UNSIGNED_INT, (void*)0);
}

void TerrainQuad::Subdivide()
{
    float new_scale = m_scale * 0.5f;
    float f = TILE_SIZE * 0.25f * m_scale;

    auto ne = make_shared<TerrainQuad>(shared_from_this());
    ne->m_center = m_center + f * vec2(1.0f, 1.0f);
    ne->m_scale = new_scale;
    m_children[NE] = ne;

    auto nw = make_shared<TerrainQuad>(shared_from_this());
    nw->m_center = m_center + f * vec2(-1.0f, 1.0f);
    nw->m_scale = new_scale;
    m_children[NW] = nw;

    auto se = make_shared<TerrainQuad>(shared_from_this());
    se->m_center = m_center + f * vec2(1.0f, -1.0f);
    se->m_scale = new_scale;
    m_children[SE] = se;

    auto sw = make_shared<TerrainQuad>(shared_from_this());
    sw->m_center = m_center + f * vec2(-1.0f, -1.0f);
    sw->m_scale = new_scale;
    m_children[SW] = sw;
}

void TerrainQuad::Parse(TerrainCallback callback)
{
    if (!IsLeaf()) {
        for (auto tquad : m_children) {
            dynamic_pointer_cast<TerrainQuad>(tquad)->Parse(callback);
        }
    } else {
        callback(dynamic_pointer_cast<TerrainQuad>(shared_from_this()));
    }
}
