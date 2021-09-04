/*
 Copyright 2011 Mario Mulansky
 Copyright 2012 Karsten Ahnert

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
 */

// See: https://www.boost.org/doc/libs/1_60_0/boost/numeric/odeint/algebra/detail/for_each.hpp

/* nested range algebra */

#ifndef NESTED_RANGE_ALGEBRA
#define NESTED_RANGE_ALGEBRA

namespace detail {

    template< class Iterator1,
              class Operation, class Algebra >
    void for_each( Iterator1 first1, Iterator1 last1,
                    Operation op, Algebra &algebra ) {
        for( ; first1 != last1 ; )
            algebra.for_each( *first1++, op );
    }

    template< class Iterator1, class Iterator2,
              class Operation, class Algebra >
    void for_each2( Iterator1 first1, Iterator1 last1,
                    Iterator2 first2,
                    Operation op, Algebra &algebra ) {
        for( ; first1 != last1 ; )
            algebra.for_each2( *first1++, *first2++, op );
    }



    template< class Iterator1, class Iterator2, class Iterator3,
              class Operation, class Algebra >
    void for_each3( Iterator1 first1, Iterator1 last1,
                    Iterator2 first2, Iterator3 first3,
                    Operation op, Algebra &algebra ) {
        for( ; first1 != last1 ; )
            algebra.for_each3( *first1++, *first2++, *first3++, op );
    }



    template< class Iterator1, class Iterator2, class Iterator3, class Iterator4,
              class Operation, class Algebra >
    void for_each4( Iterator1 first1, Iterator1 last1,
                    Iterator2 first2, Iterator3 first3, Iterator4 first4,
                    Operation op, Algebra &algebra ) {
        for( ; first1 != last1 ; )
            algebra.for_each4( *first1++, *first2++, *first3++, *first4++, op );
    }



    template< class Iterator1, class Iterator2, class Iterator3, class Iterator4, class Iterator5,
              class Operation, class Algebra >
    void for_each5( Iterator1 first1, Iterator1 last1,
                    Iterator2 first2, Iterator3 first3, Iterator4 first4, Iterator5 first5,
                    Operation op, Algebra &algebra ) {
        for( ; first1 != last1 ; )
            algebra.for_each5( *first1++, *first2++, *first3++, *first4++, *first5++, op );
    }



    template< class Iterator1, class Iterator2, class Iterator3, class Iterator4, class Iterator5, class Iterator6,
              class Operation, class Algebra >
    void for_each6( Iterator1 first1, Iterator1 last1,
                    Iterator2 first2, Iterator3 first3, Iterator4 first4, Iterator5 first5, Iterator6 first6,
                    Operation op, Algebra &algebra ) {
        for( ; first1 != last1 ; )
            algebra.for_each6( *first1++, *first2++, *first3++, *first4++, *first5++, *first6++, op );
    }



    template< class Iterator1, class Iterator2, class Iterator3, class Iterator4, class Iterator5, class Iterator6, class Iterator7,
              class Operation, class Algebra >
    void for_each7( Iterator1 first1, Iterator1 last1,
                    Iterator2 first2, Iterator3 first3, Iterator4 first4, Iterator5 first5, Iterator6 first6, Iterator7 first7,
                    Operation op, Algebra &algebra ) {
        for( ; first1 != last1 ; )
            algebra.for_each7( *first1++, *first2++, *first3++, *first4++, *first5++, *first6++, *first7++, op );
    }



    template< class Iterator1, class Iterator2, class Iterator3, class Iterator4, class Iterator5, class Iterator6, class Iterator7, class Iterator8,
              class Operation, class Algebra >
    void for_each8( Iterator1 first1, Iterator1 last1,
                    Iterator2 first2, Iterator3 first3, Iterator4 first4, Iterator5 first5, Iterator6 first6, Iterator7 first7, Iterator8 first8,
                    Operation op, Algebra &algebra ) {
        for( ; first1 != last1 ; )
            algebra.for_each8( *first1++, *first2++, *first3++, *first4++, *first5++, *first6++, *first7++, *first8++, op );
    }



    template< class Iterator1, class Iterator2, class Iterator3, class Iterator4, class Iterator5, class Iterator6, class Iterator7, class Iterator8, class Iterator9,
              class Operation, class Algebra >
    void for_each9( Iterator1 first1, Iterator1 last1,
                    Iterator2 first2, Iterator3 first3, Iterator4 first4, Iterator5 first5, Iterator6 first6, Iterator7 first7, Iterator8 first8, Iterator9 first9,
                    Operation op, Algebra &algebra ) {
        for( ; first1 != last1 ; )
            algebra.for_each9( *first1++, *first2++, *first3++, *first4++, *first5++, *first6++, *first7++, *first8++, *first9++, op );
    }



    template< class Iterator1, class Iterator2, class Iterator3, class Iterator4, class Iterator5, class Iterator6, class Iterator7, class Iterator8, class Iterator9, class Iterator10,
              class Operation, class Algebra >
    void for_each10( Iterator1 first1, Iterator1 last1,
                    Iterator2 first2, Iterator3 first3, Iterator4 first4, Iterator5 first5, Iterator6 first6, Iterator7 first7, Iterator8 first8, Iterator9 first9, Iterator10 first10,
                    Operation op, Algebra &algebra ) {
        for( ; first1 != last1 ; )
            algebra.for_each10( *first1++, *first2++, *first3++, *first4++, *first5++, *first6++, *first7++, *first8++, *first9++, *first10++, op );
    }



    template< class Iterator1, class Iterator2, class Iterator3, class Iterator4, class Iterator5, class Iterator6, class Iterator7, class Iterator8, class Iterator9, class Iterator10, class Iterator11,
              class Operation, class Algebra >
    void for_each11( Iterator1 first1, Iterator1 last1,
                    Iterator2 first2, Iterator3 first3, Iterator4 first4, Iterator5 first5, Iterator6 first6, Iterator7 first7, Iterator8 first8, Iterator9 first9, Iterator10 first10, Iterator11 first11,
                    Operation op, Algebra &algebra ) {
        for( ; first1 != last1 ; )
            algebra.for_each11( *first1++, *first2++, *first3++, *first4++, *first5++, *first6++, *first7++, *first8++, *first9++, *first10++, *first11++, op );
    }



    template< class Iterator1, class Iterator2, class Iterator3, class Iterator4, class Iterator5, class Iterator6, class Iterator7, class Iterator8, class Iterator9, class Iterator10, class Iterator11, class Iterator12,
              class Operation, class Algebra >
    void for_each12( Iterator1 first1, Iterator1 last1,
                    Iterator2 first2, Iterator3 first3, Iterator4 first4, Iterator5 first5, Iterator6 first6, Iterator7 first7, Iterator8 first8, Iterator9 first9, Iterator10 first10, Iterator11 first11, Iterator12 first12,
                    Operation op, Algebra &algebra ) {
        for( ; first1 != last1 ; )
            algebra.for_each12( *first1++, *first2++, *first3++, *first4++, *first5++, *first6++, *first7++, *first8++, *first9++, *first10++, *first11++, *first12++, op );
    }



    template< class Iterator1, class Iterator2, class Iterator3, class Iterator4, class Iterator5, class Iterator6, class Iterator7, class Iterator8, class Iterator9, class Iterator10, class Iterator11, class Iterator12, class Iterator13,
              class Operation, class Algebra >
    void for_each13( Iterator1 first1, Iterator1 last1,
                    Iterator2 first2, Iterator3 first3, Iterator4 first4, Iterator5 first5, Iterator6 first6, Iterator7 first7, Iterator8 first8, Iterator9 first9, Iterator10 first10, Iterator11 first11, Iterator12 first12, Iterator13 first13,
                    Operation op, Algebra &algebra ) {
        for( ; first1 != last1 ; )
            algebra.for_each13( *first1++, *first2++, *first3++, *first4++, *first5++, *first6++, *first7++, *first8++, *first9++, *first10++, *first11++, *first12++, *first13++, op );
    }



    template< class Iterator1, class Iterator2, class Iterator3, class Iterator4, class Iterator5, class Iterator6, class Iterator7, class Iterator8, class Iterator9, class Iterator10, class Iterator11, class Iterator12, class Iterator13, class Iterator14,
              class Operation, class Algebra >
    void for_each14( Iterator1 first1, Iterator1 last1,
                    Iterator2 first2, Iterator3 first3, Iterator4 first4, Iterator5 first5, Iterator6 first6, Iterator7 first7, Iterator8 first8, Iterator9 first9, Iterator10 first10, Iterator11 first11, Iterator12 first12, Iterator13 first13, Iterator14 first14,
                    Operation op, Algebra &algebra ) {
        for( ; first1 != last1 ; )
            algebra.for_each14( *first1++, *first2++, *first3++, *first4++, *first5++, *first6++, *first7++, *first8++, *first9++, *first10++, *first11++, *first12++, *first13++, *first14++, op );
    }



    template< class Iterator1, class Iterator2, class Iterator3, class Iterator4, class Iterator5, class Iterator6, class Iterator7, class Iterator8, class Iterator9, class Iterator10, class Iterator11, class Iterator12, class Iterator13, class Iterator14, class Iterator15,
              class Operation, class Algebra >
    void for_each15( Iterator1 first1, Iterator1 last1,
                    Iterator2 first2, Iterator3 first3, Iterator4 first4, Iterator5 first5, Iterator6 first6, Iterator7 first7, Iterator8 first8, Iterator9 first9, Iterator10 first10, Iterator11 first11, Iterator12 first12, Iterator13 first13, Iterator14 first14, Iterator15 first15,
                    Operation op, Algebra &algebra ) {
        for( ; first1 != last1 ; )
            algebra.for_each15( *first1++, *first2++, *first3++, *first4++, *first5++, *first6++, *first7++, *first8++, *first9++, *first10++, *first11++, *first12++, *first13++, *first14++, *first15++, op );
    }

}


template< class InnerAlgebra >
struct nested_range_algebra
{

    nested_range_algebra()
        : m_inner_algebra()
    { }

    template< class S1, class Op >
    void for_each( S1 &s1, Op op ) {
        detail::for_each( boost::begin( s1 ),
                           boost::end( s1 ),
                           op, m_inner_algebra );
    }

    template< class S1, class S2, class Op >
    void for_each2( S1 &s1, S2 &s2, Op op ) {
        detail::for_each2( boost::begin( s1 ),
                           boost::end( s1 ),
                           boost::begin( s2 ),
                           op, m_inner_algebra );
    }


    template< class S1, class S2, class S3, class Op >
    void for_each3( S1 &s1, S2 &s2, S3 &s3, Op op ) {
        detail::for_each3( boost::begin( s1 ),
                           boost::end( s1 ),
                           boost::begin( s2 ), boost::begin( s3 ),
                           op, m_inner_algebra );
    }


    template< class S1, class S2, class S3, class S4, class Op >
    void for_each4( S1 &s1, S2 &s2, S3 &s3, S4 &s4, Op op ) {
        detail::for_each4( boost::begin( s1 ),
                           boost::end( s1 ),
                           boost::begin( s2 ), boost::begin( s3 ), boost::begin( s4 ),
                           op, m_inner_algebra );
    }


    template< class S1, class S2, class S3, class S4, class S5, class Op >
    void for_each5( S1 &s1, S2 &s2, S3 &s3, S4 &s4, S5 &s5, Op op ) {
        detail::for_each5( boost::begin( s1 ),
                           boost::end( s1 ),
                           boost::begin( s2 ), boost::begin( s3 ), boost::begin( s4 ), boost::begin( s5 ),
                           op, m_inner_algebra );
    }


    template< class S1, class S2, class S3, class S4, class S5, class S6, class Op >
    void for_each6( S1 &s1, S2 &s2, S3 &s3, S4 &s4, S5 &s5, S6 &s6, Op op ) {
        detail::for_each6( boost::begin( s1 ),
                           boost::end( s1 ),
                           boost::begin( s2 ), boost::begin( s3 ), boost::begin( s4 ), boost::begin( s5 ), boost::begin( s6 ),
                           op, m_inner_algebra );
    }


    template< class S1, class S2, class S3, class S4, class S5, class S6, class S7, class Op >
    void for_each7( S1 &s1, S2 &s2, S3 &s3, S4 &s4, S5 &s5, S6 &s6, S7 &s7, Op op ) {
        detail::for_each7( boost::begin( s1 ),
                           boost::end( s1 ),
                           boost::begin( s2 ), boost::begin( s3 ), boost::begin( s4 ), boost::begin( s5 ), boost::begin( s6 ), boost::begin( s7 ),
                           op, m_inner_algebra );
    }


    template< class S1, class S2, class S3, class S4, class S5, class S6, class S7, class S8, class Op >
    void for_each8( S1 &s1, S2 &s2, S3 &s3, S4 &s4, S5 &s5, S6 &s6, S7 &s7, S8 &s8, Op op ) {
        detail::for_each8( boost::begin( s1 ),
                           boost::end( s1 ),
                           boost::begin( s2 ), boost::begin( s3 ), boost::begin( s4 ), boost::begin( s5 ), boost::begin( s6 ), boost::begin( s7 ), boost::begin( s8 ),
                           op, m_inner_algebra );
    }


    template< class S1, class S2, class S3, class S4, class S5, class S6, class S7, class S8, class S9, class Op >
    void for_each9( S1 &s1, S2 &s2, S3 &s3, S4 &s4, S5 &s5, S6 &s6, S7 &s7, S8 &s8, S9 &s9, Op op ) {
        detail::for_each9( boost::begin( s1 ),
                           boost::end( s1 ),
                           boost::begin( s2 ), boost::begin( s3 ), boost::begin( s4 ), boost::begin( s5 ), boost::begin( s6 ), boost::begin( s7 ), boost::begin( s8 ), boost::begin( s9 ),
                           op, m_inner_algebra );
    }


    template< class S1, class S2, class S3, class S4, class S5, class S6, class S7, class S8, class S9, class S10, class Op >
    void for_each10( S1 &s1, S2 &s2, S3 &s3, S4 &s4, S5 &s5, S6 &s6, S7 &s7, S8 &s8, S9 &s9, S10 &s10, Op op ) {
        detail::for_each10( boost::begin( s1 ),
                           boost::end( s1 ),
                           boost::begin( s2 ), boost::begin( s3 ), boost::begin( s4 ), boost::begin( s5 ), boost::begin( s6 ), boost::begin( s7 ), boost::begin( s8 ), boost::begin( s9 ), boost::begin( s10 ),
                           op, m_inner_algebra );
    }


    template< class S1, class S2, class S3, class S4, class S5, class S6, class S7, class S8, class S9, class S10, class S11, class Op >
    void for_each11( S1 &s1, S2 &s2, S3 &s3, S4 &s4, S5 &s5, S6 &s6, S7 &s7, S8 &s8, S9 &s9, S10 &s10, S11 &s11, Op op ) {
        detail::for_each11( boost::begin( s1 ),
                           boost::end( s1 ),
                           boost::begin( s2 ), boost::begin( s3 ), boost::begin( s4 ), boost::begin( s5 ), boost::begin( s6 ), boost::begin( s7 ), boost::begin( s8 ), boost::begin( s9 ), boost::begin( s10 ), boost::begin( s11 ),
                           op, m_inner_algebra );
    }


    template< class S1, class S2, class S3, class S4, class S5, class S6, class S7, class S8, class S9, class S10, class S11, class S12, class Op >
    void for_each12( S1 &s1, S2 &s2, S3 &s3, S4 &s4, S5 &s5, S6 &s6, S7 &s7, S8 &s8, S9 &s9, S10 &s10, S11 &s11, S12 &s12, Op op ) {
        detail::for_each12( boost::begin( s1 ),
                           boost::end( s1 ),
                           boost::begin( s2 ), boost::begin( s3 ), boost::begin( s4 ), boost::begin( s5 ), boost::begin( s6 ), boost::begin( s7 ), boost::begin( s8 ), boost::begin( s9 ), boost::begin( s10 ), boost::begin( s11 ), boost::begin( s12 ),
                           op, m_inner_algebra );
    }


    template< class S1, class S2, class S3, class S4, class S5, class S6, class S7, class S8, class S9, class S10, class S11, class S12, class S13, class Op >
    void for_each13( S1 &s1, S2 &s2, S3 &s3, S4 &s4, S5 &s5, S6 &s6, S7 &s7, S8 &s8, S9 &s9, S10 &s10, S11 &s11, S12 &s12, S13 &s13, Op op ) {
        detail::for_each13( boost::begin( s1 ),
                           boost::end( s1 ),
                           boost::begin( s2 ), boost::begin( s3 ), boost::begin( s4 ), boost::begin( s5 ), boost::begin( s6 ), boost::begin( s7 ), boost::begin( s8 ), boost::begin( s9 ), boost::begin( s10 ), boost::begin( s11 ), boost::begin( s12 ), boost::begin( s13 ),
                           op, m_inner_algebra );
    }


    template< class S1, class S2, class S3, class S4, class S5, class S6, class S7, class S8, class S9, class S10, class S11, class S12, class S13, class S14, class Op >
    void for_each14( S1 &s1, S2 &s2, S3 &s3, S4 &s4, S5 &s5, S6 &s6, S7 &s7, S8 &s8, S9 &s9, S10 &s10, S11 &s11, S12 &s12, S13 &s13, S14 &s14, Op op ) {
        detail::for_each14( boost::begin( s1 ),
                           boost::end( s1 ),
                           boost::begin( s2 ), boost::begin( s3 ), boost::begin( s4 ), boost::begin( s5 ), boost::begin( s6 ), boost::begin( s7 ), boost::begin( s8 ), boost::begin( s9 ), boost::begin( s10 ), boost::begin( s11 ), boost::begin( s12 ), boost::begin( s13 ), boost::begin( s14 ),
                           op, m_inner_algebra );
    }


    template< class S1, class S2, class S3, class S4, class S5, class S6, class S7, class S8, class S9, class S10, class S11, class S12, class S13, class S14, class S15, class Op >
    void for_each15( S1 &s1, S2 &s2, S3 &s3, S4 &s4, S5 &s5, S6 &s6, S7 &s7, S8 &s8, S9 &s9, S10 &s10, S11 &s11, S12 &s12, S13 &s13, S14 &s14, S15 &s15, Op op ) {
        detail::for_each15( boost::begin( s1 ),
                           boost::end( s1 ),
                           boost::begin( s2 ), boost::begin( s3 ), boost::begin( s4 ), boost::begin( s5 ), boost::begin( s6 ), boost::begin( s7 ), boost::begin( s8 ), boost::begin( s9 ), boost::begin( s10 ), boost::begin( s11 ), boost::begin( s12 ), boost::begin( s13 ), boost::begin( s14 ), boost::begin( s15 ),
                           op, m_inner_algebra );
    }


private:
    InnerAlgebra m_inner_algebra;
};

#endif
