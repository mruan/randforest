#program once

// This file implements IFeatureResponse used by the tree trainer

// forward declaration?
// class Random;
#include <random>
#include "DataCollection.hpp"

class OffsetFeature
{
  COORDTYPE u1, v1, u2, v2;

  //  static const short int box_range;

public:
  OffsetFeature():u1(0),v1(0),u2(0),v2(0){}

  OffsetFeature(short int U1, short int V1, short int U2, short int V2)
    :u1(U1),v1(V1),u2(U2),v2(V2){}

  // <summary>
  // Create an OffsetFeature instance with random offsets within a range.

  //  static OffsetFeature CreateRandom(Random& random, short int range);

  // IFeatureResponse implementation
  inline float GetResponse(const IDataPointCollection& data, unsigned int sampleIdx)
  {// It's better to call this from Data's point of view
    return data.GetResponse(sampleIdx, *this);
  }
};

template<class R>
class OffsetFeatureFactory
{
public:
  // mix ratio? base =0 ratio? box range
  OffsetFeatureFacotory(R& RandNumGen, short int box_range, float const2ndrate)
    :rng(RandNumGen), un_int(-box_range, box_range), bern(const2ndrate)
  {
  }

  OffsetFeature GetRandFeature()
  {
    OffsetFeature of;
    // first feature is always somewhere else:
    of.u1 = un_int(rng);
    of.v1 = un_int(rng);

    // second feature might be zero
    if (!bern(rng))
      {
	of.u2 = un_int(rng);
	of.v2 = un_int(rng);
      }
    return of;
  }

  inline R& GetRNG() { return rng;}

private:
  R& rng;
  std::uniform_int_distribution<short int> un_int;
  std::bernoulli_distribution bern;
};
