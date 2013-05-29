#pragma once

// This file implements IFeatureResponse used by the tree trainer

// forward declaration?
// class Random;
//#include <random>

//#include "DataCollection.hpp"

class OffsetFeature
{
public:
  COORDTYPE u1, v1, u2, v2;

  //  static short int box_range;
  //  static float const2ndrate;
  // global distribution generating function, needs initialization before main
  static std::uniform_int_distribution<short int> uniform_dist;
  static std::bernoulli_distribution bernoulli_dist;

public:
  OffsetFeature():u1(0),v1(0),u2(0),v2(0){}

  OffsetFeature(short int U1, short int V1, short int U2, short int V2)
    :u1(U1),v1(V1),u2(U2),v2(V2){}

  // <summary>
  // Create an OffsetFeature instance with random offsets within a range.

  //  static OffsetFeature CreateRandom(Random& random, short int range);

  // IFeatureResponse implementation
  /*
  inline float GetResponse(const IDataPointCollection& data, unsigned int sampleIdx)
  {// It's better to call this from Data's point of view
    return data.GetResponse(sampleIdx, *this);
  }
  */

  // Create random feature
  template<typename URNG>
  static OffsetFeature GetRandFeature(URNG& g)
  {
    OffsetFeature of;
    // first feature is always somewhere else:
    of.u1 = uniform_dist(g);
    of.v1 = uniform_dist(g);

    // second feature might be zero
    if (!bernoulli_dist(g))
      {
	of.u2 = uniform_dist(g);
	of.v2 = uniform_dist(g);
      }
    return of;
  }
};
/*
template<class R>
class OffsetFeatureFactory
{
public:
  // mix ratio? base =0 ratio? box range
  OffsetFeatureFacotory(R& RandNumGen, short int box_range, float const2ndrate)
    :rng(RandNumGen), un_int(-box_range, box_range), bern(const2ndrate)
  {
  }

  inline R& GetRNG() { return rng;}

private:
  R& rng;
  std::uniform_int_distribution<short int> un_int;
  std::bernoulli_distribution bern;
};
*/
