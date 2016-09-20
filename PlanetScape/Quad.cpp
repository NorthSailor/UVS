#include "Quad.h"
using namespace PlanetScape;
using namespace std;

Quad::Quad() { }

Quad::Quad(const shared_ptr<Quad> parent) : m_parent(parent)
{
    m_children.resize(4);
}

void Quad::Subdivide()
{
    m_children[NE] = make_shared<Quad>(shared_from_this());
    m_children[NW] = make_shared<Quad>(shared_from_this());
    m_children[SE] = make_shared<Quad>(shared_from_this());
    m_children[SW] = make_shared<Quad>(shared_from_this());
}
