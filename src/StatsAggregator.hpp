#pragma once

#include <limits>

class HistAggregator
{
 public:
  HistAggregator(int nClasses)
    :binCount_(nClasses),sampleCount_(0),bins_(nClasses, 0){};

  double GetEntropy() const;

  double GetClassProb(int classIndex) const;

  unsigned int BinCount() const {return binCount_;};

  unsigned int SampleCount() const {return sampleCount_;};

  unsigned int FindMaxBinIdx() const;

  // IStatsAggregator implementation
  void Clear();

  void Aggregate(std::vector<unsigned char>& labels, std::vector<int>& index);
  
  void Aggregate(const unsigned char classID);

  void Aggregate(const HistAggregator& aggr);

  HistAggregator DeepClone() const;

 private:
  const unsigned int binCount_;
  unsigned int sampleCount_;
  std::vector<unsigned int> bins_;
};
