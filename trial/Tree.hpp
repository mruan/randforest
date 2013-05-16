
#include "Node.hpp"

template class<D, FF, F, S, T> 
// D: data, FF: featureFactory, F: feature, S: stats, T: tree
class TreeTrainer
{
private:
  T& tree_;
  D& data_;
  std::vector<unsigned char> labels_;
  FF& fFactory_;
  TrainParams params_;

  // These are just global storage for memory efficiency
  S leftStats_, rightStats_, myStats_;
  std::vector<float> reponse(index.size(), 0.0f);
  std::vector<float> thresholds;

public:
  TreeTrainer(T& tree, D& data, std::vector<unsinged char>& labels,
	      FF& factory, TrainParams params)
    :tree_(tree),data_(data),labels_(labels), fFactory_(factory),params_(params)
  {
    
  }

  void TrainNodeRecurse(const int nodeIdx,
			std::vector<int>& dataIdx)
  {
    // Aggregate the stats at this node:
    myStats_.Clear();
    myStats_.Aggregate(labels_, dataIdx);
    
    // If this is a leaf node, nothing else to do
    if (tree_.IsLeaf(nodeIdx))
      {
        tree_[nodeIndex].InitLeaf(myStats_);
        progress_[Verbose] << "Terminating at max depth." << std::endl;
        return;
      }

    double best_gain = 0.0;
    F best_feature;
    float best_threshold = 0.0f;
    for(int f=0; f < tp.NumCandidateFeatures; ++f)
      {
	F this_feature = fFactory.GetRandomFeature();

	// Compute response
	this_feature.ComputeResponse(data_, pIdx, response);

	// Aggregate stats over sample partitions
	for(int i=0; i < pIdx.size(); ++i)
	  {
	    int b=0;
	    while (b < nThresholds && response[i] >= thresholds[b])
	      ++b;

	    partitionStats_[b].Aggregate(labels_, pIdx[i]);
	  }

	// Slightly more efficient way of looping over all samples and thresholds
	for(int t=0; t < nThresholds; t++)
	  {
	    leftStats_.Clear();
	    rightStats_.Clear();

	    for (int p=0; p < nThresholds + 1; ++p)
	      {
		if (p <= t)
		  leftStats_.Aggregate(partitionStats_[p]);
		else
		  rightStats_.Aggregate(partitionStats_[p]);
	      }

	    // Compute Information Gain
	    double gain;

	    if (gain >= maxGain)
	      {
		maxGain = gain;
		bestFeature = feature;
		bestThreshold = thresholds[t];
	      }
	  }
      }

    // Whether training should terminate?
    if (maxGain == 0.0)
      {
	// FIX: this node will be a leaf node, no split
	tree_[nodeIndex].InitLeaf(myStats_);
	progress_[Verbose] << " Terminating with zero gain." << std::endl;
	return;
      }

    // Reorder the data points indices using the winning feature and thresholds
    //    leftStats_.Clear();
    //    rightStats_.Clear();
    
    std::vector<int> lIdx, rIdx;
    lIdx.reserve(dataIdx.size()/2);
    rIdx.reserve(lIdx.capacity());

    float responseI;
    for(int i=0; i < pIdx.size(); i++)
      {
	responseI = bestFeature.GetResponse(data_, pIdx[i]);
	if (responseI < bestThreshold)
	  lIdx.push_back(i);
	else
	  rIdx.push_back(i);
      }

    // FIX: shouldTerminate()
    if(false)
      {
	// nodes[nodeIndex].MakeLeaf(myStats);
	tree_[nodeIndex].InitLeaf(myStats_);
	progress_[Verbose] << "Terminating with no splits." << std::endl;
	return;
      }

    // Otherwise this is a new decision node, recurse for children.
    // FIX: initilize a split node
    // nodes[nodeIndex].InitializeSplit(bestFeature, bestThreshold, parentStats);
    tree_[nodeIndex].InitSplit(bestFeature, bestThreshold);

    // FIX: recurse
    TrainNode(nodeIdx*2+1, lIdx);
    TrainNode(nodeIdx*2+2, rIdx);
  }
};
