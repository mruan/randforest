#pragma once

#include <memory>
#include <stdexcept>
#include <fstream>
#include <istream>
#include <iostream>
#include <vector>

#include "Interfaces.hpp"
#include "Tree.hpp"

template<class F, class S> class Forest
{
private:
  
  std::vector<Tree<F,S>* > trees_;

public:
  typedef typename std::vector<Tree<F,S> >::size_type TreeIndex;

  ~Forest()
  {
    for(int i=0; i< trees_.size(); i++)
      delete trees_[i];
  }

  void AddTree(std::auto_ptr<Tree<F,S> > tree)
  {
    tree->CheckValid();

    trees_.push_back(tree.get());
    tree.release();
  }
  
  static std::shared_ptr<Forest<F, S> > Deserialize(const std::string& path)
  {
    std::ifstream is(path.c_str(), std::ios_base::binary);

    return Forest<F,S>::Deserialize(is);
  }

  static std::shared_ptr<Forest<F, S> > Deserialize(std::istream& is)
  {
    std::shared_ptr<Forest<F,S> > forest(new Forest<F,S>);

    int treeCount;
    is.read((char*) (&treeCount), sizeof(treeCount));

    for(int t=0; t< treeCount; t++)
      {
	std::shared_ptr<Tree<F,S> > tree = Tree<F,S>::Deserialize(i);
	forest->trees_.push_back(tree.release());
      }

    return forest;
  }

  void Serialize(const std::string& path)
  {
    std::ofstream os(path.c_str(), std::ios_base::binary);
    Serialize(os);
  }

  void Serialize(std::ostream& os)
  {
    int treeCount = trees_.size();
    os.write((const char*) &treeCount, sizeof(treeCount));

    for(int t=0; t< treeCount; t++)
      GetTree(t).Serialize(os);

    if (os.bad())
      throw std::runtime_error("Forest serialization failed.");
  }

  const Tree<F,S>& GetTree(int index) const
  {
    return *trees_[index];
  }

  Tree<F,S>& GetTree(int index)
  {
    return *trees_[index];
  }

  int NumTrees() const
  {
    return trees_.size();
  }

  // TODO:
  void Apply()
};
