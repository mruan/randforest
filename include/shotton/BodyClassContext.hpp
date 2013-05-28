
//#include <boost/scoped_ptr.hpp>
#include <memory>

#include "Sherwood.h"

#include "StatsAggregator.hpp"
#include "FeatureResponse.hpp"
#include "DataCollection.hpp"

template<class F>
class BodyClassContext: public ITrainingContex<F, HistAggregator>
{
private:
  //  boost::shared_ptr<IFeatureReponseFactory<F> > pFactory_;
  //  std::shared_ptr<fF> pFactory_;

public:
  const int nClasses;

  BodyClassContext(int nClass)
    :nClasses(nClass){}

  // Implementation of ITrainingContex
  F GetRandomFeature()
  {
    return pFactory_->CreateRandom();
  }

  /*
  HistAggregator GetStatsAggregator()
  {
    return HistAggregator(nClasses);
  }
  */
  double ComputeInfoGain(const S& allStats,
			 const S& leftStats, 
			 const S& rightStats)
  {
    double H_before = allStats.GetEntropy();

    unsigned int nTotalSamples = leftStats.SampleCount() + rightStats.SampleCount();

    if (nTotalSamples <= 1)
      return 0.0;

    double H_after = ( leftStats.SampleCount() * leftStats.GetEntropy() 
		       + rightStats.SampleCount() * rightStats.GetEntropy() ) / nTotalSamples;

    return H_before - H_after;
  }

  bool ShouldTerminate(const S& parent, const S& leftChild,
		       const S& rightChild, gain)
  {
    // TODO:
    // 1. Stop when gain is too small or 
    // 2. Too few samples falling in each leaves??
    return gain <0.01;
  }
}
