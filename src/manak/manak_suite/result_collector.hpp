#ifndef MANAK_RESULT_COLLECTOR_HPP_INCLUDED
#define MANAK_RESULT_COLLECTOR_HPP_INCLUDED

#include <iostream>
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <list>

#include <manak/output_manager/output_manager.hpp>

#include <manak/util/version.hpp>

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

  RNode* AddSuite(ManakSuite* Suite) { return NULL; }

  RNode* AddCase(ManakCase* bc);

  RNode* EraseSuite(ManakSuite* suite) { return NULL; }

  void Run();

  void Print();

  void SaveForComparison(std::ostream& stream);

  void LoadForComparison(const std::string& uname,
                         std::string library,
                         const std::list<double>& readings);

  std::map<std::string, std::list<utils::ObjectStore>> results;
  std::map<std::string, ManakCase*> children;
};

struct RSuite : public RNode
{
  RSuite(RNode* parent, ManakSuite* suite)
    :RNode(parent), suite(suite) {}

  ~RSuite();

  RNode* AddSuite(ManakSuite* Suite);

  RNode* EraseSuite(ManakSuite* suite);

  RNode* AddCase(ManakCase* bc);

  void Run();

  void Print();

  ManakSuite* GetSuite()
  {
    return suite;
  }

  void SaveForComparison(std::ostream& stream);

  void LoadForComparison(const std::string& uname,
                         std::string library,
                         const std::list<double>& readings);

  std::map<std::string, RNode*> nexts;

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

  void OpenSuite(ManakSuite* suite);

  void AddCase(ManakCase* bc);

  void CloseSuite();

  void Run();

  void SaveForComparison(std::ostream& stream);

  void LoadForComparison(std::istream& stream);

  void Print();

 private:
  RNode* root;
  RNode* current_node;

  bool isComp;
  std::string compare_time;
};

}

#include "result_collector_impl.hpp"


#endif // MANAK_RESULT_COLLECTOR_HPP_INCLUDED
