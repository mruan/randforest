#include <random>

#include "Tree.hpp"
#include "Params.hpp"

#include "DataCollection.hpp"
#include "FeatureResponse.hpp"
#include "StatsAggregator.hpp"

#include "ForestTrainer.hpp"

#define MAX_DEPTH 10

using namespace std;
typedef Tree<OffsetFeature, HistAggregator> TreeType;
typedef RangeDataCollection DataType;
typedef vector<unsigned char> ClassType;
//typedef OffsetFeatureFactory<mt19937> FfactoryType;
//typedef BodyClassContext<OffsetFeature, FfactoryType> ContType;
typedef TreeTrainer<OffsetFeature, HistAggregator, DataType> TreeTrainerType; 

const int box_range = 50;
OffsetFeature::uniform_dist(-box_range, box_range);
OffsetFeature::bernoulli_dist(0.5);

int main(int argc, char* argv)
{
  // Pre-program initialization

  // Load Data

  // Q: How to create a forest?
  // A: create a tree first
  std::mt19937 g1 (seed1);

  // Params: 1. rng; 2. box range 3. 2nd point stationary rate
  //  shared_ptr<FfactoryType > pFf(new FfactoryType(g1, 20, 0.5));
  // Params: 1. tree max depth
  shared_ptr<TreeType> pTree(new TreeType(MAX_DEPTH)); 
  // Params: 1. nPixels 2.nFrames
  shared_ptr<DataType> pData(new DataType(100, 10));
  // Params: 1. nPixels(numSamples)
  shared_ptr<ClassType> pLabels(new ClassType(100, 0));
  // Params: 1. nClasses 2. Feature factory
  //  shared_ptr<ContType> pContext(new ContType(32, pFf));
  
  TrainParams tp;
  TreeTrainerType tt(pTree, pData, pLabels, 32, tp); // 32 classes
  return 0;
}
