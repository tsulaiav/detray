/** Detray library, part of the ACTS project (R&D line)
 * 
 * (c) 2020 CERN for the benefit of the ACTS project
 * 
 * Mozilla Public License Version 2.0
 */
#pragma once

#include "core/intersection.hpp"
#include "utils/containers.hpp"
#include "tools/planar_intersector.hpp"

#include <cmath>
#include <climits>
#include <iostream>

namespace detray
{
    /** This is a simple 2-dimensional mask for a closed ring
     * 
     * It is defined by the two radii _values[0] and  _values[1], 
     * and can be checked with a tolerance in t0 and t1.
     **/
    template <typename scalar_type,
              typename intersector_type = planar_intersector,
              typename links_type = bool,
              unsigned int kMaskIdentifier = 2>
    struct ring2
    {
        using mask_values = darray<scalar_type, 2>;

        mask_values _values = {0., std::numeric_limits<scalar_type>::infinity()};

        links_type _links;

        static constexpr unsigned int mask_identifier = kMaskIdentifier;

        /** Assignment operator from an array, convenience function
         * 
         * @param rhs is the right hand side object
         **/
        ring2<scalar_type, intersector_type, links_type, kMaskIdentifier> &
        operator=(const darray<scalar_type, 2> &rhs)
        {
            _values = rhs;
            return (*this);
        }

        /** Mask operation 
         * 
         * @tparam point2_type is the type of the point to be checked w.r.t. to
         * the mask bounds
         * 
         * @param p the point to be checked
         * @param t0 is the tolerance in local 0
         * @param t1 is the tolerance in local 1 and is ignored in polar coord.
         * 
         * @return an intersection status e_inside / e_outside
         **/

        template <typename local_type = __plugin::polar2>
        intersection_status operator()(const typename local_type::point2 &p,
                                       scalar_type t0 = std::numeric_limits<scalar_type>::epsilon(),
                                       scalar_type t1 = std::numeric_limits<scalar_type>::epsilon()) const
        {
            if (typeid(local_type) == typeid(__plugin::cartesian2)) {
               std::cout << "Using cartesian coord." << std::endl;
               scalar_type r = getter::perp(p);
               return (r + t0 >= _values[0] and r <= _values[1] + t0) ? e_inside : e_outside;
            }
            std::cout << "Using non. cartesian coord." << std::endl;
            return (p[0] + t0 >= _values[0] and p[0] <= _values[1] + t0) ? e_inside : e_outside;
        }

        /** Mask operation 
         * 
         * @tparam point2_type is the type of the point to be checked w.r.t. to
         * the mask bounds
         * 
         * @param p the point to be checked
         * @param t0 is the tolerance in local 0
         * @param t1 is the tolerance in local 1 and is ignored
         * 
         * @return an intersection status e_inside / e_outside
         **/
        /*template <typename local_type>
        intersection_status operator()(const typename local_type::point2 &p,
                                       scalar_type t0 = std::numeric_limits<scalar_type>::epsilon(),
                                       scalar_type t1 = std::numeric_limits<scalar_type>::epsilon()) const
        {
            std::cout << "Using cartesian coord." << std::endl;
            //scalar_type r = getter::perp(p - decltype(p){t0, t1});
            scalar_type r = getter::perp(p);
            return (r + t0 >= _values[0] and r <= _values[1] + t0) ? e_inside : e_outside;
        }*/


        /** Equality operator from an array, convenience function
         * 
         * @param rhs is the rectangle to be compared with
         * 
         * checks identity within epsilon and @return s a boolean*
         **/
        bool operator==(const darray<scalar_type, 2> &rhs)
        {
            return (_values == rhs);
        }

        /** Equality operator 
         * 
         * @param rhs is the rectangle to be compared with
         * 
         * checks identity within epsilon and @return s a boolean*
         **/
        bool operator==(const ring2<scalar_type> &rhs)
        {
            return operator==(rhs._values);
        }

        /** Access operator - non-const
         * @return the reference to the member variable
         */
        scalar_type &operator[](unsigned int value_index)
        {
            return _values[value_index];
        }

        /** Access operator - non-const
         * @return a copy of the member variable
         */
        scalar_type operator[](unsigned int value_index) const
        {
            return _values[value_index];
        }

        /** Return an associated intersector type */
        intersector_type intersector() const { return intersector_type{}; };

        /** Return the volume link */
        const links_type &links() const { return _links; }
    };

} // namespace detray
