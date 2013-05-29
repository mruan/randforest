#pragma once

#include <limits>

class HistAggregator
{
 public:
  HistAggregator(int nClasses)
    :binCount_(nClasses),sampleCount_(0),bins_(nClasses, 0){};

  double GetEntropy() const
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

  double GetClassProb(int classIndex) const
  {
    return (double)(bins_[classIdx]) /sampleCount_;
  }

  unsigned int BinCount() const {return binCount_;};

  unsigned int SampleCount() const {return sampleCount_;};

  unsigned int FindMaxBinIdx() const
  {
    unsigned int maxCount = bins_[0];
    unsigned int maxIdx = 0;

    for(unsigned int i=0; i < binCount_; ++i)
      {
	if (bins_[i] > maxCount)
	  {
	    maxCount = bins_[i];
	    maxIdx = i;
	  }	
      }
    return maxIdx;
  }
  
  // IStatsAggregator implementation
  void Clear()
  {
    memset(&bins_.front(), 0, binCount_ * sizeof(unsigned int));
    sampleCount_ = 0;   
  }

  //  void Aggregate(std::vector<unsigned char>& labels, std::vector<int>& index)
 
  void Aggregate(const unsigned char classID)
  {
    ++bins_[classID];
    ++sampleCount_;    
  }

  void Aggregate(const HistAggregator& aggr)
  {
    for(unsigned int i=0; i< binCount_; i++)
    {
      bins_[i] += aggr.bins_[i];
    }
    sampleCount_ += aggr.sampleCount_;
  }

  HistAggregator DeepClone() const;

  static double ComputeInfoGain(const HistAggregator& Pstats,
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
  
 private:
  const unsigned int binCount_;
  unsigned int sampleCount_;
  std::vector<unsigned int> bins_;
};
