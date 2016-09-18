#ifndef PS_QUAD_H
#define PS_QUAD_H

#include "SpacePosition.h"
#include <memory>

namespace PlanetScape {

//!
//! \brief A quad in a (not necessarily) terrain quadtree.
//!
class Quad : public std::enable_shared_from_this<Quad>
{
public:
    //! \brief Initializes a root quad.
    Quad();

    //! \brief Create a new quad with the given parent.
    //! \param parent The quad's parent.
    Quad(std::shared_ptr<Quad> parent);

    //! \brief Returns the NE quad.
    inline std::shared_ptr<Quad> GetNE() {
        return m_ne;
    }

    //! \brief Returns the NW quad.
    inline std::shared_ptr<Quad> GetNW() {
        return m_nw;
    }

    //! \brief Returns the SE quad.
    inline std::shared_ptr<Quad> GetSE() {
        return m_se;
    }

    //! \brief Returns the SW quad.
    inline std::shared_ptr<Quad> GetSW() {
        return m_sw;
    }

    //!
    //! \brief Subdivides the quad.
    //!
    //! This function should be overloaded by the derived classes to load the
    //! specific data for the children tiles.
    //!
    virtual void Subdivide();

    //! \brief Returns wether the quad is a leaf.
    inline bool IsLeaf() const {
        return !m_sw;
    }

protected:
    std::shared_ptr<Quad> m_ne;
    std::shared_ptr<Quad> m_nw;
    std::shared_ptr<Quad> m_se;
    std::shared_ptr<Quad> m_sw;
    std::weak_ptr<Quad> m_parent;
};

}

#endif // PS_QUAD_H
