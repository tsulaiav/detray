/** Detray library, part of the ACTS project (R&D line)
 *
 * (c) 2023 CERN for the benefit of the ACTS project
 *
 * Mozilla Public License Version 2.0
 */

#pragma once

// Project include(s)
#include "detray/definitions/containers.hpp"
#include "detray/definitions/indexing.hpp"
#include "detray/definitions/qualifiers.hpp"

// System include(s)
#include <array>
#include <limits>
#include <ostream>
#include <string>

namespace detray {

/// @brief Wraps any shape, but does not enforce boundaries
template <typename shape_t>
class unbounded {
    public:
    using shape = shape_t;
    using boundaries = typename shape::boundaries;

    /// The name for this shape
    inline static const std::string name = "unbounded " + shape::name;

    /// Local coordinate frame for boundary checks
    template <typename algebra_t>
    using local_frame_type =
        typename shape::template local_frame_type<algebra_t>;

    /// Underlying surface geometry
    template <typename intersection_t>
    using intersector_type =
        typename shape::template intersector_type<intersection_t>;

    /// @brief Check boundary values for a local point.
    ///
    /// @tparam bounds_t any type of boundary values
    ///
    /// @note the parameters are ignored
    ///
    /// @return always true
    template <typename bounds_t, typename point_t, typename scalar_t>
    DETRAY_HOST_DEVICE inline constexpr bool check_boundaries(
        const bounds_t& /*bounds*/, const point_t& /*loc_p*/,
        const scalar_t /*tol*/) const {
        return true;
    }

    /// @brief Lower and upper point for minimal axis aligned bounding box.
    ///
    /// Computes the min and max vertices in a local cartesian frame of the
    /// shape it wraps.
    ///
    /// @note The @c check_boundaries method will return 'inside' for points
    /// that are outside the local min bounds! This results in the bounding box
    /// to behave reasonably in a BVH, but still be always intersected
    /// successfully when queried directly.
    ///
    /// @param bounds the boundary values for this shape
    /// @param env dynamic envelope around the shape
    ///
    /// @returns and array of coordinates that contains the lower point (first
    /// three values) and the upper point (latter three values).
    template <
        typename algebra_t, template <typename, std::size_t> class bounds_t,
        typename scalar_t, std::size_t kDIM,
        typename std::enable_if_t<kDIM == boundaries::e_size, bool> = true>
    DETRAY_HOST_DEVICE constexpr darray<scalar_t, 6> local_min_bounds(
        const bounds_t<scalar_t, kDIM>& bounds,
        const scalar_t env = std::numeric_limits<scalar_t>::epsilon()) const {
        return shape{}.template local_min_bounds<algebra_t>(bounds, env);
    }

    /// @returns the shapes centroid in local cartesian coordinates
    template <
        typename algebra_t, template <typename, std::size_t> class bounds_t,
        typename scalar_t, std::size_t kDIM,
        typename std::enable_if_t<kDIM == boundaries::e_size, bool> = true>
    DETRAY_HOST_DEVICE auto centroid(
        const bounds_t<scalar_t, kDIM>& bounds) const {
        return shape{}.template centroid<algebra_t>(bounds);
    }

    /// Generate vertices in local cartesian frame
    ///
    /// @param bounds the boundary values for the underlying shape
    /// @param ls is the number of line segments
    ///
    /// @return a generated list of vertices
    template <
        typename point2_t, typename point3_t,
        template <typename, std::size_t> class bounds_t, typename scalar_t,
        std::size_t kDIM,
        typename std::enable_if_t<kDIM == boundaries::e_size, bool> = true>
    DETRAY_HOST dvector<point3_t> vertices(
        const bounds_t<scalar_t, kDIM>& bounds, dindex n_seg) const {
        return shape{}.template vertices<point2_t, point3_t>(bounds, n_seg);
    }

    /// @brief Check consistency of boundary values.
    ///
    /// @param bounds the boundary values for this shape
    /// @param os output stream for error messages
    ///
    /// @return true if the bounds are consistent.
    template <
        template <typename, std::size_t> class bounds_t, typename scalar_t,
        std::size_t kDIM,
        typename std::enable_if_t<kDIM == boundaries::e_size, bool> = true>
    DETRAY_HOST constexpr bool check_consistency(
        const bounds_t<scalar_t, kDIM>& /*bounds*/,
        std::ostream& /*os*/) const {
        return true;
    }
};

}  // namespace detray
