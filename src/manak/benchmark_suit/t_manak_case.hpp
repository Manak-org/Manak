#ifndef MANAK_T_MANAK_CASE_HPP_INCLUDED
#define MANAK_T_MANAK_CASE_HPP_INCLUDED

namespace manak
{

template<class Case, typename RType, typename... Args>
class TManakCase : public Case
{
 public:
  TManakCase()
    : Case("", "", 0, 0, ""){}

  TManakCase(const std::string& name,
             const std::string& library_name,
             std::function<RType(Args...)> t_function,
             double tolerance = MANAK_DEFAULT_TOLERANCE,
             const size_t iterations = MANAK_DEFAULT_ITERATIONS,
             double success_p = MANAK_DEFAULT_SP)
    : Case(name, library_name, tolerance, iterations, success_p),
    t_function(t_function)
  {

  }

  TManakCase* AddArgs(Args... args)
  {
    std::tuple<Args...> temp(args...);
    this->run_functions.emplace_back("", [=](){utils::Caller(t_function, temp); });
    return this;
  }

  TManakCase* AddArgs_N(const std::string& name, Args... args)
  {
    std::tuple<Args...> temp(args...);
    this->run_functions.emplace_back(name, [=](){utils::Caller(t_function, temp); });
    return this;
  }

  template<typename... Args2>
  TManakCase* AddCustomArgs(std::function<std::list<std::tuple<Args...>>(Args2...)> fun, Args2... params)
  {
    std::list<std::tuple<Args...>> args = fun(params...);
    for(auto s_arg : args)
    {
      utils::Caller(utils::BindToObject(&TManakCase<Case, RType, Args...>::AddArgs, this), s_arg);
    }
    return this;
  }

  template<typename... Args2>
  TManakCase* AddCustomArgs(std::list<std::tuple<Args...>> (&fun)(Args2...), Args2... params)
  {
    std::list<std::tuple<Args...>> args = fun(params...);
    for(auto s_arg : args)
    {
      utils::Caller(utils::BindToObject(&TManakCase<Case, RType, Args...>::AddArgs, this), s_arg);
    }
    return this;
  }

  template<typename ST, typename... Args2>
  TManakCase* AddCustomArgs_N(std::list<std::tuple<ST, Args...>> (&fun)(Args2...), Args2... params)
  {
    std::list<std::tuple<ST, Args...>> args = fun(params...);
    for(auto s_arg : args)
    {
      utils::Caller(utils::BindToObject(&TManakCase<Case, RType, Args...>::AddArgs_N, this), s_arg);
    }
    return this;
  }

private:
  std::function<RType(Args...)> t_function;
  size_t iter;
};

class BenchmarkCase;

template<typename RType, typename... Args>
using T_Benchmark_Case = TManakCase<BenchmarkCase, RType, Args...>;

template<class Case, typename RType, typename... Args>
TManakCase<Case, RType, Args...>* CTManakCase(const std::string& name,
                                              const std::string& library_name,
                                              RType (*fun)(Args...),
                                              double tolerance,
                                              const size_t iterations,
                                              double success_p)
{
  return new TManakCase<Case, RType, Args...>(name, library_name, std::function<RType(Args...)>(fun), tolerance, iterations, success_p);
}

template<class Case, typename RType, typename... Args>
TManakCase<Case, RType, Args...>* CTManakCase(const std::string& name,
                                              const std::string& library_name,
                                              std::function<RType(Args...)> fun,
                                              double tolerance,
                                              const size_t iterations,
                                              double success_p)
{
  return new TManakCase<RType, Args...>(name, library_name, fun, tolerance, iterations, success_p);
}

}

#endif // MANAK_T_MANAK_CASE_HPP_INCLUDED
