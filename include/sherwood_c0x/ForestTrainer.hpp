#pragma once

#include <random> // for random distributions
#include <memory> // for shared_ptr

//#include "Node.hpp"
#include "Tree.hpp"
#include "Params.hpp"

template <class F, class S, class D>
// D: data, F: feature, S: stats
class TreeTrainer
{
private:
  typedef std::vector<unsigned char> LabelType;

  std::shared_ptr<Tree<F,S> > pTree_;
  std::shared_ptr<D> pData_;
  std::shared_ptr<LabelType > pLabels_;
  //  std::shared_ptr<IContext> pContext_;
  TrainParams params_;

  // Local stuff: 
  std::uniform_int_distribution<DataIndex> un_int; // [0, max_int]
  std::uniform_real_distribution<float> un_real;   // [0.0, 1.0]

  // These are just global storage for memory efficiency
  S myStats_, LcStats_, RcStats_;
  std::vector<S> partitionStats_;
  std::vector<float> thresholds; // much smaller in size
  std::vector<unsigned int> dataIndices; // these are as big as Data (in term of size)
  std::vector<float> responses;

public:
  TreeTrainer(std::shared_ptr<Tree<F,S>>& pTree, std::shared_ptr<D>& pData,
	      std::shared_ptr<LabelType>& pLabels, int nClass,TrainParams params)
    :pTree_(pTree), pData_(pData), pLabels_(pLabels), params_(params),
     // Stats must be intialized with a known number of classes
     myStats_(nClass), LcStats_(nClass), RcStats_(nClass), 
     partitionStats_(params.NumThresholdsPerFeature + 1, S(nClass)),
     // Pre-allocate spaces
     thresholds(params.NumThresholdsPerFeature + 1, 0.0f),
     dataIndices(pData->Count()), responses(pData->Count(), 0.0f)
  {
    // initialize index list to all indices
    unsigned int i=0;
    for(unsigned int &idx: dataIndices)
      idx = i++;
  }

  void TrainNodeRecurse(const int nodeIdx, DataIndex i0, DataIndex i1, int curDepth)
  {
    // TODO: Aggregate the stats at this node:
    myStats_.Clear();
    myStats_.Aggregate(labels_, dataIndices);
    
    // If this is a leaf node, nothing else to do
    if (nodeIdx >= nodes.size() / 2)// FIX: figure out 
      {
        pTree_->GetNode(nodeIndex).InitLeaf(myStats_);
        progress_[Verbose] << "Terminating at max depth." << std::endl;
        return;
      }

    double best_gain = 0.0;
    F best_feature;
    float best_threshold = 0.0f;
    for(int f=0; f < params_.NumCandidateFeatures; ++f)
      {
	F this_feature = F::GetRandomFeature(rng_);

	// Compute response
	for(DataIndex i=i0; i<i1; i++)
	  responses[i] = pData_->GetResponse(dataIndices[i], this_feature);

	// FIX: get random thresholds
	int nThresholds = GetRandThresholds(i0, i1, thresholds);
	if (nThresholds == 0)
	  continue;

	// Aggregate stats over sample partitions
	for(DataIndex i=i0; i < i1; ++i)
	  {
	    int b=0;
	    while (b < nThresholds && responses[i] >= thresholds[b])
	      ++b;

	    partitionStats_[b].Aggregate(*pLabels_, dataIndices[i]);
	  }

	// Slightly more efficient way of looping over all samples and thresholds
	for(int t=0; t < nThresholds; t++)
	  {
	    LcStats_.Clear();
	    RcStats_.Clear();

	    for (int p=0; p < nThresholds + 1; ++p)
	      {
		if (p <= t)
		  LcStats_.Aggregate(partitionStats_[p]);
		else
		  RcStats_.Aggregate(partitionStats_[p]);
	      }

	    // Compute Information Gain
	    double gain = S::ComputeInfoGain(myStats_, LcStats_, RcStats_);
	    
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
	pTree_->GetNode(nodeIndex).InitLeaf(myStats_);
	progress_[Verbose] << " Terminating with zero gain." << std::endl;
	return;
      }

    // Reorder the data points indices using the winning feature and thresholds
    LcStats_.Clear();
    RcStats_.Clear();

    for(DataIndex i=i0; i < i1; i++)
      {
	responses[i] = bestFeature.GetResponse(data_, dataIndices[i]);
	if (response[i] < bestThreshold)
	  LcStats_.Aggregate(labels_, dataIndices[i]);
	else
	  RcStats_.Aggregate(labels_, dataIndices[i]);
      }

    if(pContext_->ShouldTerminate(myStats_, LcStats_, RcStats_))
      {
	pTree_->GetNode(nodeIndex).InitLeaf(myStats_);
	progress_[Verbose] << "Terminating with no splits." << std::endl;
	return;
      }

    // Otherwise this is a new decision node, recurse for children.
    // nodes[nodeIndex].InitializeSplit(bestFeature, bestThreshold, parentStats);
    pTree_->GetNode(nodeIndex).InitSplit(bestFeature, bestThreshold);

    DataIndex middle = Tree<F,S>::Partition(responses, dataIndices, i0, i1, bestThreshold);
    assert(middle>= i0 && middle <= i1);

    progress_[Verbose] << " (threshold = " << bestThreshold << ", gain = "<< maxGain << ")." << std::endl;

    TrainNodeRecurse(nodeIdx*2+1, i0, middle, curDepth+1);
    TrainNodeRecurse(nodeIdx*2+2, middle, i1, curDepth+1);
  }

private:
  // DataIndices and responses are implicitly passed as class member variables
  int GetRandThresholds(DataIndex i0, DataIndex i1, std::vector<float>& thresholds)
  {
    thresholds.resize(params_.NumThresholdsPerFeature + 1);
    // a shorthand, for code clarity and memory efficiency:
    std::vector<float>& quantiles = thresholds;

    int nThresholds;
    // If there are enough response values...
    int diffIdx = i1 - i0;
    if (diffIdx > params_.NumThresholdsPerFeature)
      {
	//... make a random draw of NumThresholdsPerFeature + 1 response values
	nThresholds = params_.NumThresholdsPerFeature;
	for (int i=0; i< nThresholds+1; i++)
	  {
	    // quantiles <- a random response within range [i0, i1]
	    quantiles[i] = responses[i0+un_int(rng_)%diffIdx];
	  }
      }
    else
      {
	//... otherwise use all response values.
	nThresholds = i1 - i0 - 1;
	std::copy(&responses[i0], &responses[i1], quantiles.begin());
      }

    // Sort the response values to form approximate quantiles.
    std::sort(quantiles.begin(), quantiles.end());

    if (quantiles[0] == quantiles[nThresholds])
      return 0; // all sampled responses are identical

    // compute N candidate thresholds by sampling in between N+1 apprixmate quantiles
    for (int i=0; i< nThresholds; i++)
      thresholds[i]=quantiles[i]+(un_real(rng_)*(quantiles[i+1]-quantiles[i]));

    return nThresholds;
  }

  
};
