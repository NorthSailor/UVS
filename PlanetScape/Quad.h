#ifndef PS_QUAD_H
#define PS_QUAD_H

#include "SpacePosition.h"
#include <memory>
#include <vector>

namespace PlanetScape {

//!
//! \brief A quad in a (not necessarily) terrain quadtree.
//!
class Quad : public std::enable_shared_from_this<Quad>
{
public:
    //! Child indexing enumeration.
    //! The NSWE designation don't necessarily correspond to their
    //! compass counterparts. They are rather shorthands for up/down/left/right.
    enum ChildIndex {
        //! North east tile.
        NE = 0,
        //! North west tile.
        NW = 1,
        //! South east tile.
        SE = 2,
        //! South west tile.
        SW = 3
    };

    //! \brief Initializes a root quad.
    Quad();

    //! \brief Create a new quad with the given parent.
    //! \param parent The quad's parent.
    Quad(std::shared_ptr<Quad> parent);

    //! \brief Returns the NE quad.
    inline std::shared_ptr<Quad> GetNE() {
        return m_children.at(NE);
    }

    //! \brief Returns the NW quad.
    inline std::shared_ptr<Quad> GetNW() {
        return m_children.at(NW);
    }

    //! \brief Returns the SE quad.
    inline std::shared_ptr<Quad> GetSE() {
        return m_children.at(SE);
    }

    //! \brief Returns the SW quad.
    inline std::shared_ptr<Quad> GetSW() {
        return m_children.at(SW);
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
        return !m_children[0];
    }

protected:
    std::vector<std::shared_ptr<Quad>> m_children;
    std::weak_ptr<Quad> m_parent;
};

}

#endif // PS_QUAD_H
