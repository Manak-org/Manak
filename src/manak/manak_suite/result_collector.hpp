#ifndef MANAK_RESULT_COLLECTOR_HPP_INCLUDED
#define MANAK_RESULT_COLLECTOR_HPP_INCLUDED

#include <iostream>
#include <string>
#include <map>
#include <list>

#include <manak/output_manager/output_manager.hpp>

#include <manak/util/version.hpp>
#include <manak/util/macro_utils.hpp>

namespace manak
{

struct RNode
{
  RNode(RNode* parent)
    : parent(parent), count(0)
  {}

  virtual ~RNode() {}

  virtual RNode* AddSuite(ManakSuite* suite) = 0;

  virtual RNode* EraseSuite(ManakSuite* suite) = 0;

  virtual RNode* AddCase(ManakCase* bc) = 0;

  virtual void Run() = 0;

  virtual void Print() = 0;

  virtual void SaveForComparison(std::ostream& stream) = 0;

  virtual void LoadForComparison(const std::string& uname,
                                 std::string library,
                                 const std::list<double>& readings) = 0;

  virtual ManakSuite* GetSuite()
  {
    return NULL;
  }

  RNode* parent;
  size_t count;
};

struct RCase : public RNode
{
  RCase(RNode* parent)
    : RNode(parent) {}

  ~RCase();

  RNode* AddSuite(ManakSuite* Suite)
  {
    (void)Suite;
    return NULL;
  }

  MANAK_INLINE RNode* AddCase(ManakCase* bc);

  RNode* EraseSuite(ManakSuite* suite)
  {
    (void)suite;
    return NULL;
  }

  MANAK_INLINE void Run();

  MANAK_INLINE void Print();

  MANAK_INLINE void SaveForComparison(std::ostream& stream);

  MANAK_INLINE void LoadForComparison(const std::string& uname,
                                      std::string library,
                                      const std::list<double>& readings);

  std::map<std::string, std::list<utils::ObjectStore>> results;
  std::map<std::string, ManakCase*> children;
};

struct RSuite : public RNode
{
  RSuite(RNode* parent, ManakSuite* suite)
    :RNode(parent), suite(suite) {}

  MANAK_INLINE ~RSuite();

  MANAK_INLINE RNode* AddSuite(ManakSuite* Suite);

  MANAK_INLINE RNode* EraseSuite(ManakSuite* suite);

  MANAK_INLINE RNode* AddCase(ManakCase* bc);

  MANAK_INLINE void Run();

  MANAK_INLINE void Print();

  ManakSuite* GetSuite()
  {
    return suite;
  }

  MANAK_INLINE void SaveForComparison(std::ostream& stream);

  MANAK_INLINE void LoadForComparison(const std::string& uname,
                                      std::string library,
                                      const std::list<double>& readings);

  std::map<std::string, RNode*> next_suites;
  std::map<std::string, RNode*> next_cases;

  ManakSuite* suite;
};

class ResultCollector
{
 public:
  ResultCollector()
    : root(new RSuite(NULL, NULL)), current_node(root) {}

  ~ResultCollector()
  {
    delete root;
  }

  static ResultCollector& GlobalResultCollector()
  {
    static ResultCollector singleton;
    return singleton;
  }

  MANAK_INLINE void OpenSuite(ManakSuite* suite);

  MANAK_INLINE void AddCase(ManakCase* bc);

  MANAK_INLINE void CloseSuite();

  MANAK_INLINE void Run();

  MANAK_INLINE void SaveForComparison(std::ostream& stream);

  MANAK_INLINE void LoadForComparison(std::istream& stream);

  MANAK_INLINE void Print();

 private:
  RNode* root;
  RNode* current_node;

  bool isComp;
  std::string compare_time;
};

}

#ifndef MANAK_USE_DYN_LINK
#include "result_collector.cpp"
#endif // MANAK_USE_DYN_LINK

#endif // MANAK_RESULT_COLLECTOR_HPP_INCLUDED
