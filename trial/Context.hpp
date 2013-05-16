#pragma once

class LotusFeature
{
public:
  short int u1, v1, u2, v2;

  LotusFeature():u1(0),v1(0),u2(0),v2(0){}

  LotusFeature(short x1, short y1, short x2, short y2)
    :u1(x1),v1(y1),u2(x2),v2(y2){}
};

template class<RNG, F>
class LotusFeatureFactory
{
public:

  LotusFeatureFactory(RNG rg, short Max_Range)
    :max_range(Max_Range), rng(rg)
  {}
  
  F GetRandomFeature();

private:
  short int max_range:
  RNG rng;
  float mix_rate;
  float stationary;
};

template <RNG, F>
class Context
{
private:
  RNG rng_;
  LotusFeatureFactory lf_;

public:
  Context(RNG rng, short max_range)
    :rng_(rng), lf_(rng, max_range){}

  F GetRandomFeature()

  bool ShouldTerminate();

  void GetRandomThreshold(std::vector<float>& response
			  std::vector<float>& thresholds)
  {
    thresholds.resize(tp_.NumThresholdsPerFeature+1);
    std::vector<float>& quantiles = thresholds;

    int nThresholds;
    // If there are enough responses:
    if (reponse.size() > tp_.NumThresholdsPerFeature)
      {
	// ... make random draw of NumThresholdsPerFeature response values:
	nThresholds = tp_.NumThresholdsPerFeature;
	for(int i=0; i< nThresholds; i++)
	  quantiles[i] = responses[];
	// FIX: linear random draw from 0~reponse.size();
      }
    else
      {
	// ... then use all response values
	nThresholds = response.size() - 1;
	std::copy(responses.begin(), responses.end(), quantiles.begin());
      }

    // sort the response values to form approximate quantiles
    std::sort(quantiles.begin(), quantiles.end());

    if (quantiles[0] == quantiles[nThresholds])
      return 0;// all responses are the same

    // compute n candidate thresholds by sampling between n+1 intervals
    for (int i=0; i< nThresholds; ++i)
      {
	thresholds[i] = quantiles[i] + (float)
	  (/*rng.float* */(quantiles[i+1]-quantiles[i]));
      }
    return nThresholds;
  }
}
