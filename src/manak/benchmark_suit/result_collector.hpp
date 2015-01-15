#ifndef MANAK_RESULT_COLLECTOR_HPP_INCLUDED
#define MANAK_RESULT_COLLECTOR_HPP_INCLUDED

#include <iostream>
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <list>

#include "output_manager.hpp"

#include <manak/util/version.hpp>

namespace manak
{

struct RNode
{
  RNode(RNode* parent)
    : parent(parent), count(0)
  {}

  virtual ~RNode() {}

  virtual RNode* AddSuite(BenchmarkSuite* suite) = 0;

  virtual RNode* EraseSuite(BenchmarkSuite* suite) = 0;

  virtual RNode* AddCase(BenchmarkCase* bc, size_t l_id) = 0;

  virtual void Run() = 0;

  virtual void Print() = 0;

  virtual void SaveForComparison(std::ostream& stream) = 0;

  virtual void LoadForComparison(const std::string& uname,
                                 size_t l_id,
                                 const std::list<double>& readings) = 0;

  virtual BenchmarkSuite* GetSuite()
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

  RNode* AddSuite(BenchmarkSuite* Suite) { return NULL; }

  RNode* AddCase(BenchmarkCase* bc, size_t l_id);

  RNode* EraseSuite(BenchmarkSuite* suite) { return NULL; }

  void Run();

  void Print();

  void SaveForComparison(std::ostream& stream);

  void LoadForComparison(const std::string& uname,
                         size_t l_id,
                         const std::list<double>& readings);

  std::map<size_t, std::list<utils::ObjectStore>> results;
  std::map<size_t, BenchmarkCase*> children;
};

struct RSuite : public RNode
{
  RSuite(RNode* parent, BenchmarkSuite* suite)
    :RNode(parent), suite(suite) {}

  ~RSuite();

  RNode* AddSuite(BenchmarkSuite* Suite);

  RNode* EraseSuite(BenchmarkSuite* suite);

  RNode* AddCase(BenchmarkCase* bc, size_t l_id);

  void Run();

  void Print();

  BenchmarkSuite* GetSuite()
  {
    return suite;
  }

  void SaveForComparison(std::ostream& stream);

  void LoadForComparison(const std::string& uname,
                         size_t l_id,
                         const std::list<double>& readings);

  std::map<std::string, RNode*> nexts;

  BenchmarkSuite* suite;
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

  void OpenSuite(BenchmarkSuite* suite);

  void AddCase(BenchmarkCase* bc);

  void CloseSuite();

  void Run();

  void SaveForComparison(std::ostream& stream);

  void LoadForComparison(std::istream& stream);

  void Print();

 private:
  RNode* root;
  RNode* current_node;

  std::map<std::string, size_t> l_map;

  size_t total_nodes;
  size_t current_l_id;

  bool isComp;
  std::string compare_time;
};

}

#include "result_collector_impl.hpp"


#endif // MANAK_RESULT_COLLECTOR_HPP_INCLUDED
