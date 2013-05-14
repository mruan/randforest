#pragma once

#include <memory>
#include <random>
#include <vector>

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
  
  unique_ptr<S> pStats;

  static Node& make_node(unsigned char type, F f, float th, unique_ptr<S> pS)
  {
    return Node(type, f, th, pS);
  }

  Node(unsigned char type, F f, float th, unique_ptr<S> pS)
    :nodeType(type),feature(f), threshold(th), pStats(pS){}

  void Serialize(std::ostream& os) const
  {
    feature.Serialize(os);
    os.write((const char*)(&threshold), sizeof(float));
    //    pStats->Serialize(os);
  }

  void Deserialize(std::istream& is) const
  {
    feature.Deserialize(is);
    is.read((const char*)(&threshold), sizeof(float));
    //    pStats->Deserialize(is);
  }
};
