#ifndef _MANAK_RUN_TREE_HPP_INCLUDED
#define _MANAK_RUN_TREE_HPP_INCLUDED

#include <map>
#include <string>
#include <list>
#include <tuple>

#include "pmeasure.hpp"
#include "benchmark_case.hpp"

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

  ~RNode()
  {
    for(auto it : nexts)
    {
      delete it.second;
    }

    for(auto it : results)
    {
      for(auto it2 : it.second)
      {
        std::string* name = (std::string*)it2.Get("name");
        delete name;
        double* tol = (double*)it2.Get("tolerance");
        delete tol;
        PMeasure* pm = (PMeasure*)it2.Get("pmeasure");
        delete pm;
        double* com = (double*)it2.Get("compare");
        delete com;
      }
    }
  }

  bool AddNext(const std::string& name, RNode*& n);

  void AddCase(BenchmarkCase* bc, size_t l_id)
  {
    children[l_id] = bc;
  }

  std::string GetPMRep(const utils::ObjectStore& entry);

  void Run();

  void PrintTXT(std::ostream& stream, size_t l_ids);

  void PrintHTML(std::ostream& stream,
                 size_t l_ids);

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
    current_l_id(0)
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
};

}

//! include implementation
#include "run_tree_impl.hpp"

#endif // _MANAK_RUN_TREE_HPP_INCLUDED
