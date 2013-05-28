
//#include <boost/scoped_ptr.hpp>
#include <memory>

#include "Sherwood.h"

#include "StatsAggregator.hpp"
#include "FeatureResponse.hpp"
#include "DataCollection.hpp"

template<class F, class fF, class R>
class BodyClassContext: public ITrainingContex<F, HistAggregator>
{
private:

  //  boost::shared_ptr<IFeatureReponseFactory<F> > pFactory_;
  std::shared_ptr<fF> pFactory_;

public:
  const int nClasses;

  BodyClassContext(int nClass, std::shared_ptr<fF>& pFactory)
    :nClasses(nClass), pFactory_(pFactory){}

  // Implementation of ITrainingContex
  F GetRandomFeature()
  {
    return pFactory_->CreateRandom();
  }

  inline R& GetRNG()
  {
    return pFactory->GetRNG();
  }

  HistAggregator GetStatsAggregator()
  {
    return HistAggregator(nClasses);
  }

  double ComputeInfoGain(const HistAggregator& allStats,
			 const HistAggregator& leftStats, 
			 const HistAggregator& rightStats)
  {
    double H_before = allStats.GetEntropy();

    unsigned int nTotalSamples = leftStats.SampleCount() + rightStats.SampleCount();

    if (nTotalSamples <= 1)
      return 0.0;

    double H_after = ( leftStats.SampleCount() * leftStats.GetEntropy() 
		       + rightStats.SampleCount() * rightStats.GetEntropy() ) / nTotalSamples;

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
