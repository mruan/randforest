
//#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "Sherwood.h"

#include "StatsAggregator.hpp"
#include "FeatureResponse.hpp"
#include "DataCollection.hpp"

template<class F>
class BodyClassContext: public ITrainingContex<F, HistAggregator>
{
private:
  int nClasses_;

  //  boost::shared_ptr<IFeatureReponseFactory<F> > pFactory_;
  IFeatureReponseFactory<F>* pFactory_;

public:
  BodyClassContext(int nClass, IFeatureReponseFactory<F>* pF)
    :nClasses_(nClass), pFactory_(pF){}

private:
  // Implementation of ITrainingContex
  F GetRandomFeature(Random& random)
  {
    return pFactory_->CreateRandom(random);
  }

  HistAggregator GetStatsAggregator()
  {
    return HistAggregator(nClasses_);
  }

  double ComputeInfoGain(const HistAggregator& allStats,
			 const HistAggregator& leftStats, 
			 const HistAggregator& rightStats)
  {
    double H_before = allStats.GetEntropy();

    unsigned int nTotalSamples = leftStats.SampleCount() + rightStats.SampleCount();

    if (nTotalSamples <= 1)
      return 0.0;

    double H_after = ( leftStats.SampleCount() * leftStats.Entropy() 
		       + rightStats.SampleCount * rightEntropy() ) / nTotalSamples;

    return H_before - H_after;
  }

  bool ShouldTerminate(const HistAggregator& parent, const HistAggregator& leftChild,
		       const HIstAggregator& rightChild, gain)
  {
    // TODO:
    // 1. Stop when gain is too small or 
    // 2. Too few samples falling in each leaves??
    return gain <0.01;
  }

}
