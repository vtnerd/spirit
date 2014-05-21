/*=============================================================================
    Copyright (c) 2011 Aaron Graham
    Copyright (c) 2014 Lee Clagett

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_SPIRIT_X3_ADVANCE_MAY_11_2014_1928PM)
#define BOOST_SPIRIT_X3_ADVANCE_MAY_11_2014_1928PM

#include <algorithm>
#include <iterator>

#include <boost/phoenix/core/reference.hpp>
#include <boost/phoenix/operator/arthimetic.hpp>
#include <boost/spirit/home/x3/core/parser.hpp>

namespace boost { namespace spirit { namespace x3
{
    template<typename Int>
    struct advance_parser : parser<advance_parser<Int>>
    {
        typedef parser<advance_parser<Int>> base_type;

        advance_parser(Int const& distance) :
            distance_(distance)
        {
        }

        template<typename Iterator, typename Context, typename Attribute>
        bool parse(Iterator& first, Iterator const& last
                   , Context const&, Attribute&) const
        {
            BOOST_ASSERT(0 <= std::distance(first, last));

            if (distance_ == 0)
            {
                return true;
            }

            typedef typename std::iterator_traits<Iterator>::iterator_category;
            return safe_advance(first, last, iterator_category());
        }          

        Int const distance_;

    private:

        template<typename Iterator, typename IteratorCategory>
        bool safe_advance(Iterator& first, Iterator const& last, IteratorCategory) const
        {
            Int count = requested_distance_;
            Iterator const new_first = 
                std::find_it_not(first, last, phoenix::ref(count)--);

            if (count == 0)
            {
                first = new_first;
                return true;
            }

            return false;
        }

        template<typename Iterator>
        bool safe_advance(Iterator& first, Iterator const& last
                          , std::random_access_iterator_tag) const
        {
            if (distance_ <= std::distance(first, last))
            {
                std::advance(first, distance_);
                return true;
            }

            return false;
        }
    };

    template<typename Int>
    inline advance_parser<Int> advance(Int const& distance)
    {
        return {distance};
    }
}

#endif
