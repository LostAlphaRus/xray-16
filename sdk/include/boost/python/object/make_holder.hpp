#if !defined(BOOST_PP_IS_ITERATING)

// Copyright David Abrahams 2001. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

# ifndef MAKE_HOLDER_DWA20011215_HPP
#  define MAKE_HOLDER_DWA20011215_HPP

#  include <boost/python/object/instance.hpp>

#  include <boost/python/object/forward.hpp>
#  include <boost/python/detail/wrap_python.hpp>
#  include <boost/python/detail/preprocessor.hpp>

#  include <boost/mpl/next.hpp>
#  include <boost/mpl/begin_end.hpp>
#  include <boost/mpl/apply.hpp>

#  include <boost/preprocessor/iterate.hpp>
#  include <boost/preprocessor/iteration/local.hpp>
#  include <boost/preprocessor/repeat.hpp>
#  include <boost/preprocessor/debug/line.hpp>
#  include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>

#  include <cstddef>

namespace boost { namespace python { namespace objects {

template <int nargs> struct make_holder;

#  define BOOST_PYTHON_DO_FORWARD_ARG(z, index, _) , f##index(a##index)

// specializations...
#  define BOOST_PP_ITERATION_PARAMS_1 (3, (0, BOOST_PYTHON_MAX_ARITY, <boost/python/object/make_holder.hpp>))
#  include BOOST_PP_ITERATE()

#  undef BOOST_PYTHON_DO_FORWARD_ARG

}}} // namespace boost::python::objects

# endif // MAKE_HOLDER_DWA20011215_HPP

#elif BOOST_PP_ITERATION_DEPTH() == 1
# line BOOST_PP_LINE(__LINE__, make_holder.hpp)

# define N BOOST_PP_ITERATION()

template <>
struct make_holder<N>
{
    template <class Holder, class ArgList>
    struct apply
    {
# if N
        // Unrolled iteration through each argument type in ArgList,
        // choosing the type that will be forwarded on to the holder's
        // templated constructor.
        typedef typename mpl::begin<ArgList>::type iter0;
        
#  define BOOST_PP_LOCAL_MACRO(n)               \
    typedef typename mpl::apply0<iter##n>::type t##n;        \
    typedef typename forward<t##n>::type f##n;  \
    typedef typename mpl::next<iter##n>::type   \
        BOOST_PP_CAT(iter,BOOST_PP_INC(n)); // Next iterator type
        
#  define BOOST_PP_LOCAL_LIMITS (0, N-1)
#  include BOOST_PP_LOCAL_ITERATE()
# endif 
        
        static void execute(
            PyObject* p
            BOOST_PP_ENUM_TRAILING_BINARY_PARAMS_Z(1, N, t, a))
        {
            typedef instance<Holder> instance_t;
            
            void* memory = Holder::allocate(p, offsetof(instance_t, storage), sizeof(Holder));
            try {
                (new (memory) Holder(
                    p BOOST_PP_REPEAT_1ST(N, BOOST_PYTHON_DO_FORWARD_ARG, nil)))->install(p);
            }
            catch(...) {
                Holder::deallocate(p, memory);
                throw;
            }
        }
    };
};

# undef N

#endif