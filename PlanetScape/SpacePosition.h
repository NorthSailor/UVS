#ifndef PS_SPACEPOSITION_H
#define PS_SPACEPOSITION_H

#include <glm/vec3.hpp>

namespace PlanetScape {

//! The number (or class) big enough to describe accurately a distance in our
//! huge universe. This one is in millimeters.
typedef signed long long UniverseComponent;

//!
//! \brief Represents a position in the universe.
//!
//! This structure can represent any position with millimeter accuracy about
//! a thousand times outside Pluto's orbit. It is imperative that it is
//! only used for storage and simple operations like addition and subtraction.
//! Physics calculations will most probably fail unless specifically tailored
//! for fixed-point arithmetic.
//! Although generally, the position is relative to the Sun, subsquent
//! operations may yield vectors relative to e.g. a planet's center.
//!
class SpacePosition : public glm::tvec3<UniverseComponent>
{
public:
    //!
    //! Creates a new space position object.
    //! \param x The x coordinate in millimeters.
    //! \param y The y coordinate in millimeters.
    //! \param z The z coordinate in millimeters.
    //!
    SpacePosition(UniverseComponent x,
                  UniverseComponent y,
                  UniverseComponent z);

    //!
    //! Creates a new space position object from a given GLM vector.
    //! \param v The vector to use.
    //!
    template <typename T>
    inline SpacePosition(const glm::tvec3<T> &v) :
        SpacePosition((v.x * static_cast<T>(1000)),
                      (v.y * static_cast<T>(1000)),
                      (v.z * static_cast<T>(1000))) { }

    //!
    //! Converts to a GLM vector. The components of the vector are in meters.
    //! Generally, to avoid loss of precision, the components should be less
    //! than 1km when converting to glm::vec3, and less than approximately
    //! Saturn's orbit when converting to glm::dvec3.
    //!
    template <typename T>
    inline operator glm::tvec3<T>() const {
        return glm::tvec3<T>(*this) * static_cast<T>(0.001);
    }
};

}

#endif // PS_SPACEPOSITION_H
