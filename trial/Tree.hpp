#pragma once

// This file defines the Tree class, which is used to represent decision trees.

#include <assert.h>

#include <iostream>
#include <vector>


template<class F, class S> class Tree
{

private:
  int maxLevels_;

  std::vector<Node<F,S> > nodes_;

public:
  Tree(int maxLevels):maxLevels_(maxLevels)
  {
    if(maxLevels <=0)
      throw std::runtime_error("Tree cannot have fewer than 1 levels.");

    if(maxLevels >20)
      throw std::runtime_error("Tree cannot handle more than 20 levels.");

    // this may waste some memory but it is more efficient to retrieve nodes
    nodes_.resize((1 << (maxLevels + 1)) -1);
  }

  // FIX: apply the decision tree to a collection of test data points
  void Apply()
  {
    // TODO: fill with something
  }

  void Serialize(std::ostream& os) const
  {
    // NB. We could allow IFeatureResponse and IStatisticsAggregrator to
    // write type information here for safer deserialization (and
    // friendlier exception descriptions in the event that the user
    // tries to deserialize a tree of the wrong type).
    os.write((const char*) &maxLevels_, sizeof(maxLevels_));

    for(int n=0; n < nodes_.size(); n++)
      nodes_[n].Serialize(os);
  }

  static std::shared_ptr<Tree<F,S> > Deserialize(std::istream& is)
  {
    int maxLevels;
    is.read((char*) &maxLevels, sizeof(maxLevels));

    if(maxLevels <=0)
      throw std::runtime_error("Invalid data.");

    std::shared_ptr<Tree<F,S> > ptree(new Tree<F,S>(maxLevels));

    for(int n=0; n< ptree->NumNodes(); n++)
      ptree->nodes_[n].Deserialize(is);

    ptree->CheckValid();

    return ptree;
  }

  inline int NumNodes() const
  {
    return nodes_.size();
  }

  inline const Node<F,S>& GetNode(int index) const
  {
    return nodes_.[index];
  }

  inline Node<F,S>& GetNode(int index)
  {
    return nodes_.[index];
  }

  void CheckValid() const
  {
    if( nodes_.size() ==0)
      throw std::runtime_error("ValidCheck failed: should have at least one node.");

    if( GetNode(0).IsNull() == true)
      throw std::runtime_error("ValidCheck failed: root node cannot be null.");

    CheckValidRecurse(0);
  }

private:
  // FIX: todo
  void CheckValidRecurse(int index, bool LeafFlag = false) const {}

  void ApplyNode();
};
