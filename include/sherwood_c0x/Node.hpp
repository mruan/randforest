#pragma once

#include <memory>
//#include <random>
//#include <vector>

//#include <types.hpp>
#define INVALID  0
#define ROOTNode 1
#define SPLIT    2
#define LEAF     3

template<class F, class S>
class Node
{
public:
  unsigned char nodeType;
  F feature;
  float threshold;
  
  std::unique_ptr<S> pStats;
  /*
  static Node& make_node(unsigned char type, F f, float th, std::unique_ptr<S> pS)
  {
    return Node(type, f, th, pS);
  }
  */

  Node():nodeType(INVALID){}

  Node(unsigned char type, F f, float th, std::unique_ptr<S> pS)
    :nodeType(type),feature(f), threshold(th), pStats(pS){}

  void Serialize(std::ostream& os) const
  {
    feature.Serialize(os);
    os.write((const char*)(&threshold), sizeof(float));
    pStats->Serialize(os);
  }

  void Deserialize(std::istream& is) const
  {
    feature.Deserialize(is);
    is.read((const char*)(&threshold), sizeof(float));
    pStats.reset(S::Deserialize(is));
  }

  inline bool IsSplit() const{ return nodeType == SPLIT; }

  inline bool IsLeaf() const { return nodeType == LEAF; }

  inline bool IsNull() const { return nodeType == INVALID;}

  void InitSplit(F& f, float th)
  {
    feature = f;
    threshold = th;
    nodeType = SPLIT;
  }

  void InitLeaf(const S& stats)
  {
    pStats.reset(new S(stats));
    nodeType = LEAF;
  }
};
