#progma once

// This file defines the Node data structure, which is used to represent one node
// in a DecisionTree.

template<class T>
void Serialize_(std::ostream& os, const T& t)
{
  os.write((const char*)(&t), sizeof(T));
}

template<class T>
void Deserialize_(std::istream& is, T& t)
{
  is.read((char*)(&t), sizeof(T));
}

// One node in a decision tree.

// Sherwood: We implement Nodes and their constituent IFeatureResponse and
// IStatisticsAggregrator implementations using value types so as to
// avoid the need for multiple objects to be allocated seperately on the
// garbage collected heap - instead all the data associated with a tree can
// be stored within a single, contiguous block of memory. This can
// increase performance by (i) increasing spatial locality of reference
// (and thus cache utilization) and (ii) decreasing the load on the .NET
// memory manager.

template<class F, class S>
struct Node // F: IFeatureResponse, S: IStatsAggregatgor<S>
{
  // Null nodes -> leafType ==0;
  // leaf nodes -> leafType ==1;
  // split nodes-> leafType ==2;
#define NULL_TYPE  0
#define LEAF_TYPE  1
#define SPLIT_TYPE 2
  unsigned char leafType;

  // The weak learner associated with a decision node is defined by a
  // feature and an associated threshold. These values are only valid
  // for split nodes.  
  F Feature;

  float Threshold;

  // NB We store training data statistics for all nodes, including
  // decision nodes - this way we can prune the tree subsequent to
  // training.
  S TrainDataStats;

  // TODO: make them unique_ptr
  void InitializeLeaf(S trainDataStats)
  {
    Feature = F();
    Threshold = 0.0f;
    leafType = 1;
    TrainDataStats = tranDataStats.DeepClone();
  }

  void InitializeSplit(F feature, float threshold, S trainDataStats)
  {
    leafType = 2;
    Feature = feature;
    Threshold = threshold;
    TrainDataStats = trainDataStats.DeepClone();
  }

  Node():leafType(0){} // by default a null node

  void Serialize(std::ostream& os) const
  {
    os.write((const char*)(&leafType), sizeof(unsigned char));
    Feature.Serialize(os);
    os.write((const float*)(&Threshold), sizeof(float));
    trainDataStats.Serialize(os);
  }

  void Deserialize(std::istream& is)
  {
    is.read((char*)(&leafType), sizeof(unsigned char));
    Feature.Deserialize(is);
    is.read((char*)(&Threshold), sizeof(float));
    trainDataStats.Serialize(is);
  }

  bool IsSplit() const {return leafType== SPLIT_TYPE}
  bool IsLeaf()  const {return leafType== LEAF_TYPE}
  bool IsNull()  const {return leafType== NULL_TYPE}
}
