#program once

#include <memory> // for unique_ptr

template<class F>
class IDataCollection
{
public:
  virtual ~IDataCollection() {};
  virtual unsigned int Count() const=0;

  virtual float GetResponse(unsigned int index, F feature);
};

// Compute (single precision) response for data sample.
// A 'weak learner' comprises a feature and a threshold
class IFeatureResponse
{
public:
  // Computes the reponse for ONE given data point
  virtual float GetResponse(const IDataCollection& data, unsigned int index) const=0;

  // Compute reponse for all given data given in a list
  //  virtual unique_ptr<float> GetReponse(const IDataCollection& data, 
  //				       std::vector<unsigned int>& indices) const=0;
};

template<class S> // S is a concrete type of Aggregator
class IStatsAggregator
{
public:
  virtual void Clear()=0;

  // Update statistics with one additional data point
  virtual void Aggregate(const IDataCollection& data);

  virtual double ComputeInfoGain(S& parent, S& left, S& right);

  virtual S DeepClone() const=0;
};

/// Sherwood:
/// An abstract representation of a decision forest training problem that
/// intended to be implemented within client code. Instances of this
/// interface are used by the training framework to instantiate new
/// IFeatureResponse and IStatisticsAggregator instances, to compute
/// information gain, and to decide when to terminate training of a
/// particular tree branch.
/*
template<class F, class S, class R>
// F: IFeatureResponse, S: IStatsAggregator. R: random number engine
class ITrainContext
{
public:
  // Random number generator
  virtual R rng;

  // 
  virtual R& GetRng() {return rng;}

  // Called by the training framework to generate a new random feature.
  virtual F CreateRandFeature()=0;

  // Called by the framework to
  // create one StatsAggregator that is specific for this problem
  virtual S CreateStatsAggregator() = 0;

  // Called by the training framework to compute the gain over a partition
  virtual double ComputeInfoGain(const S& p, const S& l, const S& r)=0;

  // Called by the training framework to determin whether it should stop
  // TODO: absorb into the previous function?
  virtual double ShouldStop(const S& p, const S& l, const S& r, double gain)=0;
};
*/
