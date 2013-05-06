#progma once

#include <assert.h>

#include <vector>
#include <string>
#include <algorithm>

#include "ProgressStream.hpp"
#include "TrainingParameters.hpp"

#include "Interfaces.hpp"
#include "Tree.hpp"

template<class F, class S>
class TreeTrainOp
{
private:
  typedef typename std::vector<Node<F,S> >::size_type NodeIndex;
  typedef typename std::vector<unsigned int>::size_type SampleIndex;

  // Random& random_;

  const IDataCollection& data_;

  ITrainContex<F, S>& trainContext_;

  TrainingParameters params_;

  std::vector<unsigned int> indices_;

  std::vector<float> responses_;

  S pStats, lStats, rStats_;
  std::vector<S> partitionStats_;

  ProgressStream progress_;

public:
  TreeTrainOp(//Random& random,
	      ITrainContex<F,S>& trainContext,
	      const TrainingPrameters params,
	      const IDataCollection& data,
	      ProgressStream& progress)
    :data_(data),trainContext_(trainContext),
     params_(params),progress_(progress)
  {
    // TODO: why doing this??
    indices_.resize(data.Count());
    for(SampleIndex i=0; i<indices_.size(); i++)
      indices_[i] = i;

    responses_.resize(data.Count());
    
    pStats_ = trainContext_.CreateStatsAggregator();
    lStats_ = trainContext_.CreateStatsAggregator();
    rStats_ = trainContext_.CreateStatsAggregator();

    partitionStats_.resize(params.NumThresholdsPerFeature + 1);
    // TODO: check c++0x syntax
    for (S x&: partitionStats_)
      x = trainContext_.CreateStatsAggregator();
  }

  void TrainNodesRecurse(std::vector<Node<F,S> >& nodes,
			 NodeIndex nodeIndex,SampleIndex i0,
			 SampleIndex i1, int recurseDepth)
  {
    assert(nodeIndex < nodes.size());

    progress_[Verbose] << Tree<F,S>::GetPrettyPrintPrefix(nodeIndex) << i1-i0<<": ";

    // First aggregate statistics over the samples at the parent node
    // TODO: this is very in-efficient way of aggregating data!!
    pStats_.Clear();
    for(SampleIndex i= i0; i < i1; i++)
      pStats_.Aggregate(data_, indices_[i]);

    if (nodeIndex >= nodes.size() /2) // this is a leaft node, do nothing
      {
	nodes[nodeIndex].InitializeLeaf(pStats_);
	progress_[Verbose] << "Terminating at max depth."<< std::endl;
	return;
      }

    double maxGain = 0.0;
    F bestFeature;
    float bestThreshold =0.0f;

    // Iterate over candidate features
    std::vector<float> thresholds;
    for (int f=0; f < params_.NumFeaturesPerNode; f++)
      {
	// TODO: this function should take an RNG as input
	F feature = trainContext_.GetRandFeature();

	for(unsigned int b=0; b< params_.NumThresholdPerFeature; b++)
	  partitionStats_[b].Clear();// reset statistics

	// compute feature respone per sample at this node
	for(SampleIndex i=i0; i< i1; i++)
	  response_[i] = feature.GetResponse(data_, indices_[i]);

	// TODO: what is this for??
	int nThresholds;
	if(0)
	  continue;

	// Aggregate statistics over sample partitions
	for(SampleIndex i=i0; i< i1; i++)
	  {
	    int b=0;
	    while (b< nThresholds && response_[i]>= thresholds[b])
	      b++;

	    partitionStats_[b].Aggregate(data_, indices_[i]);
	  }

	for(int t=0; t< nThresholds; t++)
	  {
	    lStats_.Clear();
	    rStats_.Clear();

	    for(int p=0; p < nThresholds +1 /*i.e. nBins*/; p++)
	      {
		if (p<=t)
		  lStats_.Aggregate(partitionStats_[p]);
		else
		  rStats_.Aggregate(partitionStats_[p]);
	      }

	    // compute gain over sample partitions
	    double gain trainContext_.ComputeInfoGain(pStats_, lStats_, rStats_);

	    if (gain >= maxGain)
	      {
		maxGain = gain;
		bestFeature = feature;
		bestThreshold = thresholds[t];
	      }
	  }
      }

    if (maxGain == 0.0)
      {
	nodes[nodeIndex].InitializeLeaf(pStats_);
	progress_[Verbose] << "Terminating with zero gain."<<std::endl;
	return;
      }

    // Now reorder the data point indices using the winning feature and threshods.
    // Also recompute child node statistics so the client can decide whether
    // to terminate training of this branch.
    lStats_.Clear();
    rStats_.Clear();

    for(SampleIndex i=i0; i< i1; i++)
      {
	response_[i] = bestFeature.GetResponse(data_, indices_[i]);
	if (responses_[i] < bestThreshold)
	  lStats_.Aggregate(data_, indices_[i]);
	else
	  rStats_.Aggregate(data_, indices_[i]);
      }

    if (trainContext_.ShouldTerminate(pStats_, lStats_, rStats_, maxGain))
      {
	nodes[nodeIndex].InitializeLeaf(pStats_);
	progress_[Verbose] << "Terminating with no split." << std::endl;
	return;
      }

    // otherwise this is a new decision node, recurse for children.
    nodes[nodeIndex].InitializeSplit(bestFeature, bestThreshold, pStats_);

    // Now do partition sort - any sample with response greater goes left
    SampleIndex ii = Tree<F,S>::Partition(responses_, indices_, i0, i1, bestThreshold);
    
    assert(ii >=i0 && i1 >=ii);
    
    progress_[Verbose] <<" (threshold = " << bestThreshold << ", gain = " << maxGain<< ")."<<std::endl;

    TrainNodesRecurse(nodes, nodeIndex*2+1, i0, ii, recurseDepth + 1);
    TrainNodesRecurse(nodes, nodeIndex*2+2, ii, i1, recurseDepth + 1);
  }

private:
  int ChooseThreshold()
  {}
};

// Used to train decision trees
template<class F, class S>
// F: feature, S: statistics 
class TreeTrainer
{
public:
  // Train a new decision tree given some training data and a training
  // problem described by an ITrainContext instance.

  // TODO: fill in the parameters
  static std::unique_ptr<Tree<F,S> TrainTree()
  {
    ProgressStream defaultProgress(std::cout, parameters.Verbose? Verbose:Interest);
    if(progress==0)
      progress= &defaultProgress;

    // TODO: fill in the parameters
    TreeTrainOp<F,S> trainOp();

    std::unique_ptr<Tree<F,S> > tree(new Tree<F,S>(params.MaxDepth));

    (*progress)[Verbose] << std::endl;

    trainOp.TrainNodesRecurse(tree->GetNodes(), 0, 0, data.Count(), 0); //will recurse until termination

    (*progress)[Verbose] << std::endl;

    return tree;
  }
};

template<class F, class S>
class ForestTrainer
{
public:
  // Train a new decision forest given some training data
  // and a problem described by an instance of ITrainContext interface

  // TODO: Fill in the arguments
  static std::unique_ptr<Forest<F,S> > TrainForest()
  {
    ProgressStream defualtProgress(std::cout, params.Verbose? Verbose:Interest);
    if(progress=0)
      progress=&defaultProgress;
    
    std::unique_ptr<Forest<F,S> >forest(new Forest<F,S>());

    for(int t=0; t < params.NumTrees; t++)
      {
	(*progress)[Interest] <<"\rTraining tree "<< t << "...";

	// TOOD: fill in the parameters:
	std::unique_ptr<Tree<F,S> > tree= TreeTrainer::TrainTree();
	forest->AddTree(tree);
      }

    (*progress)[Interest] << "\rTrained " << params.NumTrees << " trees."<<std::endl;
    return forest;
  }
};
