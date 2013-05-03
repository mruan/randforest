
#include "StatsAggregator.hpp"

#include <math>
#include <string.h>

double HistAggregator::
GetEntropy() const
{
  if (sampleCount ==0)
    return 0.0;

  double result = 0.0;
  double p = 0.0;
  for (unsigned int b=0; b < binCount_; ++b)
    {
      p = (double) bins_[b] / (double) sampleCount_;
      result -= (p == 0.0 ? 0.0 : p*log(p)/log(2.0));
    }
  return result;
}

double HistAggregator::
GetClassProb(unsigned classIdx) const
{
  return (double)(bins_[classIdx]) /sampleCount_;
}

unsigned int HistAggregator::
FindMaxBinIdx() const
{
  unsigned int maxCount = bins_[0];
  unsigned int maxIdx = 0;

  for (unsigned int i=0; i < binCount_; ++i)
    {
      if (bins_[i] > maxCount)
	{
	  maxCount = bins_[i];
	  maxIdx = i;
	}
    }
  return maxIdx;
}

// IStatisAgg implementation
void HistAggregator::
Clear()
{
  memset(&bins_.front(), 0, binCount_ * sizeof(unsigned int));
  sampleCount_ = 0;
}

void HistAggregator::
Aggregate()
{}
