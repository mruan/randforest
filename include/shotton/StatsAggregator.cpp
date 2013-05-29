
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
Aggregate(const unsigned char classID)
{
  ++bins_[classID];
  ++sampleCount_;
}
/*
void HistAggregator::
Aggregate(std::vector<unsigned char>& labels, std::vector<int>& index)
{
  for(int &i : index)
      ++bins_[labels[i]];

  sampleCount += index.size();
}
*/
void HistAggregator::
Aggregate(const HistAggregator& aggr)
{
  for(unsigned int i=0; i< binCount_; i++)
    {
      bins_[i] += aggr.bins_[i];
    }
  sampleCount_ += aggr.sampleCount_;
}

// FIX:: deep clone

double HistAggregator::
ComputeInfoGain(const HistAggregator& Pstats,
		const HistAggregator& Lstats,
		const HistAggregator& Rstats)
{
  double H_before = Pstats.GetEntropy();
  unsigned int nTotalSamples = Lstats.SampleCount() + Rstats.SampleCount();
  if (nTotalSamples <= 1)
    return 0.0;

  double H_after = ( Lstats.SampleCount()*Lstats.GetEntropy()
		    +Rstats.SampleCount()*Rstats.GetEntropy() )/nTotalSamples;
  return H_before - H_after;
}
