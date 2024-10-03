/** Detray library, part of the ACTS project (R&D line)
 *
 * (c) 2024 CERN for the benefit of the ACTS project
 *
 * Mozilla Public License Version 2.0
 */

#pragma once

// Project include(s).
#include "detray/definitions/detail/qualifiers.hpp"
#include "detray/navigation/navigation_config.hpp"
#include "detray/propagator/stepping_config.hpp"
#include "detray/core/detail/data_context.hpp"

// System inlcudes
#include <ostream>

namespace detray::propagation {

/// Configuration of the propagation
struct config {
    navigation::config navigation{};
    stepping::config stepping{};
    geometry_context context{};
};

/// Print the propagation configuration
DETRAY_HOST
inline std::ostream& operator<<(std::ostream& out,
                                const detray::propagation::config& cfg) {
    out << "Navigation\n"
        << "----------------------------\n"
        << cfg.navigation << "\nParameter Transport\n"
        << "----------------------------\n"
        << cfg.stepping << "\nGeometry Context\n"
        << "----------------------------\n"
        << cfg.context.get() << "\n";

    return out;
}

}  // namespace detray::propagation
