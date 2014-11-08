#ifndef MANAK_UTIL_TEMPLATE_UTILS_HPP_INCLUDED
#define MANAK_UTIL_TEMPLATE_UTILS_HPP_INCLUDED

namespace manak
{
namespace utils
{
namespace helper
{

template<int>
struct template_placeholder;

}
}
}

namespace std
{
  template<int N>
  struct is_placeholder<manak::utils::helper::template_placeholder<N>>
    : integral_constant<int, N> // the one is important
  {};
}

namespace manak
{
namespace utils
{

namespace helper
{

template<int... Args>
struct Hold
{
};

template<int N, int... Args>
Hold<Args..., N> Combine(Hold<Args...>, Hold<N>)
{
  return Hold<Args..., N>();
}

template<int N>
struct SGenerate
{
  typedef decltype(Combine(typename SGenerate<N - 1>::type(), Hold<N>())) type;
};

template<>
struct SGenerate<1>
{
  typedef Hold<1> type;
};

template<typename RType, typename Fun, typename P, int... S>
RType Caller_2(Fun fun, P params, Hold<S...>)
{
  return fun(std::get<S - 1>(params)...);
}


template<int>
struct template_placeholder
{

};

template<typename RType, typename F, typename O, int... S>
RType BindToObject2(F fun, const O* o, Hold<S...>)
{
  return std::bind(fun, o, template_placeholder<S>()...);
}

template<typename RType, typename F, typename O, int... S>
RType BindToObject2(F fun, O* o, Hold<S...>)
{
  return std::bind(fun, o, template_placeholder<S>()...);
}

template<typename RType, typename C, typename... Args>
std::function<RType(Args...)> BindToObject(RType (C::*fun)(Args...) const, const C* c)
{
  return BindToObject2<std::function<RType(Args...)>>(fun, c, typename SGenerate<sizeof...(Args)>::type());
}

}; // namespace helpers

template<typename RType, typename... Args>
RType Caller(std::function<RType(Args...)> fun, std::tuple<Args...> params)
{
  return helper::Caller_2<RType>(fun, params, typename helper::SGenerate<sizeof...(Args)>::type());
}

template<typename RType, typename... Args>
RType Caller(RType (fun)(Args...), std::tuple<Args...> params)
{
  return helper::Caller_2<RType>(fun, params, typename helper::SGenerate<sizeof...(Args)>::type());
}

template<typename RType, typename C, typename... Args>
std::function<RType(Args...)> BindToObject(RType (C::*fun)(Args...), C* c)
{
  return helper::BindToObject2<std::function<RType(Args...)>>(fun, c, typename helper::SGenerate<sizeof...(Args)>::type());
}

}; // namespace utils
}; // namespace manak


#endif // MANAK_UTIL_TEMPLATE_UTILS_HPP_INCLUDED
