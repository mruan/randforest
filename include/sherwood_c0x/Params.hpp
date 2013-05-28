#pragma once

#include <assert.h>

struct TrainParams
{
  TrainParams()
  {
    NumTrees = 5;
    NumCandidateFeatures = 50;
    NumThresholdsPerFeature = 50;
    MaxDecisionLevels = 20;
    Verbose = false;
  }

  int NumTrees;
  int NumCandidateFeatures;
  unsigned int NumThresholdsPerFeature;
  unsigned int MaxDecisionLevels;
  bool Verbose;
};
