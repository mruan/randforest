
#include "Node.hpp"

template class<D, FF, F, S> // D: data, FF: featureFactory, F: feature, S: stats
class TreeTrainer
{
private:
  D& data_;
  FF& fFactory_;
  TrainParams params_;

  S leftStats_, rightStats_;
  

public:
  TreeTrainer(D& data, FF& factory, TrainParams params)
    :data_(data),fFactory_(factory),params_(params)
  {
    
  }

  Node TrainNode(std::vector<int>& pIdx, std::vector<int>& lIdx, std::vector<int>& rIdx)
  {
    double best_gain = 0.0;
    F best_feature;
    float best_threshold = 0.0f;

    std::vector<float> reponse(index.size(), 0.0f);
    std::vector<float> thresholds;
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

	    partitionStats_[b].Aggregate(data_, pIdx[i]);
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
	progress_[Verbose] << " Terminating with zero gain." << std::endl;
	return;
      }

    // Reorder the data points indices using the winning feature and thresholds
    leftStats_.Clear();
    rightStats_.Clear();
    
    lIdx.reserve(pIdx.size()/2);
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
	progress_[Verbose] << "Terminating with no splits." << std::endl;
	return;
      }

    // Otherwise this is a new decision node, recurse for children.
    // nodes[nodeIndex].InitializeSplit(bestFeature, bestThreshold, parentStats);

    // FIX: recurse
  }
};
