#pragma once

#include "Node.hpp"

template<class F, class S> class Tree
{
  
public:
  void Apply();

  void Serialize(std::ostream& o) const;

  void NodeCount() const;

  cost Note<F,S>& GetNode(int index) const;

  Node<F,S>& GetNode(int index);

  static DataPointIndex Partition();

  void CheckValid() const;

private:
  void CheckValidRecurse() const;

public:
  static std::string GetPrettyPrintPrefix(int nodeIndex);

private:
  void ApplyNode();
};
