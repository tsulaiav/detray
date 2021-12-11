/** Detray library, part of the ACTS project (R&D line)
 *
 * (c) 2021 CERN for the benefit of the ACTS project
 *
 * Mozilla Public License Version 2.0
 */

#pragma once

#include <array>
#include <detray/grids/grid2.hpp>
#include <vecmem/containers/static_array.hpp>
#include <vecmem/memory/memory_resource.hpp>

namespace detray {

/**
 * detailed implementation of make_grid_data_array with index_sequence
 */
template <typename grid_type, std::size_t N, std::size_t... ints>
auto make_grid_data_array(vecmem::static_array<grid_type, N>& arr,
                          vecmem::memory_resource& resource,
                          std::index_sequence<ints...> seq) {

    return vecmem::static_array<grid2_data<grid_type>, N>{
        get_data(arr[ints], resource)...};
}

/**
 * Make the array of grid2_data from the array of grid2
 * @param arr is the input array of grid2
 * @param resource is the memory_resource
 * @return is the output array of grid2_data
 */
template <typename grid_type, std::size_t N>
auto make_grid_data_array(vecmem::static_array<grid_type, N>& arr,
                          vecmem::memory_resource& resource) {
    auto seq = std::make_index_sequence<N>{};
    return make_grid_data_array(arr, resource, seq);
}

/**
 * detailed implementation of make_grid_view_array with index_sequence
 */
template <typename grid_type, std::size_t N, std::size_t... ints>
auto make_grid_view_array(vecmem::static_array<grid2_data<grid_type>, N>& arr,
                          std::index_sequence<ints...> seq) {
    return vecmem::static_array<grid2_view<grid_type>, N>{arr[ints]...};
}

/**
 * Make the array of grid2_data from the array of grid2
 * @param arr is the input array of grid2_data
 * @param resource is the memory_resource
 * @return is the output array of grid2_view
 */
template <typename grid_type, std::size_t N>
auto make_grid_view_array(vecmem::static_array<grid2_data<grid_type>, N>& arr) {
    auto seq = std::make_index_sequence<N>{};
    return make_grid_view_array(arr, seq);
}

}  // namespace detray