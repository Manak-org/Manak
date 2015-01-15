#ifndef _MANAK_RUN_TREE_HPP_INCLUDED
#define _MANAK_RUN_TREE_HPP_INCLUDED

#include <map>
#include <string>
#include <list>
#include <tuple>
#include <iostream>
#include <iomanip>

#include "pmeasure.hpp"
#include "benchmark_case.hpp"
#include "output_manager.hpp"

#include <manak/util/version.hpp>
#include <manak/util/macro_utils.hpp>

namespace manak
{

struct RNode
{
  RNode(RNode* parent)
    : parent(parent)
  {

  }

  ~RNode();

  bool AddNext(const std::string& name, RNode*& n);

  void AddCase(BenchmarkCase* bc, size_t l_id)
  {
    children[l_id] = bc;
  }

  std::string GetPMRep(const utils::ObjectStore& entry);

  void Run();

  void PrintTXT(std::ostream& stream, size_t l_ids);

  void PrintHTML(std::ostream& stream,
                 std::ostream& stream2,
                 size_t l_ids,
                 const std::map<std::string, size_t>& l_map);

  void SaveForComparison(std::ostream& stream, const std::string& uname);

  void LoadForComparison(const std::string& uname,
                         size_t l_id,
                         const std::list<double>& readings);

  RNode* parent;

  std::map<std::string, RNode*> nexts;

  std::map<size_t, BenchmarkCase*> children;
  //std::map<size_t, std::list<std::tuple<std::string, double, PMeasure, double>>> results;
  std::map<size_t, std::list<utils::ObjectStore>> results;
};

class RunTree
{
 public:
  RunTree()
    : root(new RNode(NULL)),
    current_node(root),
    total_nodes(0),
    current_l_id(0),
    isComp(false)
  {
  }

  ~RunTree()
  {
    delete root;
  }

  static RunTree& GlobalRunTree()
  {
    static RunTree singleton;
    return singleton;
  }

  void AddSuite(const std::string& name)
  {
    RNode* temp;
    current_node->AddNext(name, temp);
    current_node = temp;
  }

  void CloseSuite()
  {
    current_node = current_node->parent;
  }

  void AddCase(BenchmarkCase* bc);

  void Run();

  void PrintTXT(std::ostream& stream);

  void PrintHTML(std::ostream& stream);

  void SaveForComparison(std::ostream& stream);

  void LoadForComparison(std::istream& stream);

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

//! include implementation
#include "run_tree_impl.hpp"

#endif // _MANAK_RUN_TREE_HPP_INCLUDED
