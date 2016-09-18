#include "Quad.h"
using namespace PlanetScape;
using namespace std;

Quad::Quad() { }

Quad::Quad(const shared_ptr<Quad> parent) : m_parent(parent)
{
}

void Quad::Subdivide()
{
    m_ne = make_shared<Quad>(shared_from_this());
    m_nw = make_shared<Quad>(shared_from_this());
    m_se = make_shared<Quad>(shared_from_this());
    m_sw = make_shared<Quad>(shared_from_this());
}
