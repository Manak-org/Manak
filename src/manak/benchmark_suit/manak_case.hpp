#ifndef MANAK_MANAK_CASE_HPP_INCLUDED
#define MANAK_MANAK_CASE_HPP_INCLUDED

#include <string>
#include <functional>
#include <limits>
#include <list>

#include "pmeasure.hpp"

#include <manak/util/timer.hpp>
#include <manak/util/macro_utils.hpp>
#include <manak/util/template_utils.hpp>
#include <manak/util/object_store.hpp>

namespace manak
{

class ManakCase
{
 public:
  ManakCase(const std::string& name,
            const std::string& library_name,
            double tolerance = MANAK_DEFAULT_TOLERANCE,
            const size_t iterations = MANAK_DEFAULT_ITERATIONS,
            double success_p = MANAK_DEFAULT_SP)
    : name(name),
    library_name(library_name),
    tolerance(tolerance),
    iterations(iterations),
    success_p(success_p)
  {}

  template<typename T>
  ManakCase(const std::string& name,
            const std::string& library_name,
            std::function<T()> run_function,
            double tolerance = MANAK_DEFAULT_TOLERANCE,
            const size_t iterations = MANAK_DEFAULT_ITERATIONS,
            const double success_p = MANAK_DEFAULT_SP)
    : name(name),
    library_name(library_name),
    tolerance(tolerance),
    iterations(iterations),
    success_p(success_p)
  {
    run_functions.emplace_back([=](){run_function();});
  }

  template<typename T>
  ManakCase(const std::string& name,
            const std::string& library_name,
            T (run_function)(),
            double tolerance = MANAK_DEFAULT_TOLERANCE,
            const size_t iterations = MANAK_DEFAULT_ITERATIONS,
            double success_p = MANAK_DEFAULT_SP)
    : name(name),
    library_name(library_name),
    tolerance(tolerance),
    iterations(iterations),
    success_p(success_p)
  {
    std::function<T()> fun(run_function);
    run_functions.emplace_back("", [=](){fun();});
  }

  virtual ~ManakCase() {}

  virtual std::list<utils::ObjectStore> Run() = 0;

  void AddComparisonEntry(double d)
  {
    to_c.push_back(d);
  }

  const std::string& Name() const
  {
    return name;
  }

  const std::string& LibraryName() const
  {
    return library_name;
  }
  const std::string& UName() const
  {
    return uname;
  }
  std::string& UName()
  {
    return uname;
  }

 protected:
  std::string name;
  std::string uname;
  std::string library_name;
  std::list<std::pair<std::string, std::function<void()>>> run_functions;
  double tolerance;
  size_t iterations;
  double success_p;

  std::vector<double> to_c;
};

}

#endif // MANAK_MANAK_CASE_HPP_INCLUDED