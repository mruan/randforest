#program once

// This file implements IFeatureResponse used by the tree trainer

// forward declaration?
class Random;

class OffsetFeature
{
  short int u1, v1, u2, v2;

  static const short int box_range;

public:
  OffsetFeature():u1(0),v1(0),u2(0),v2(0){}

  OffsetFeature(short int U1, short int V1, short int U2, short int V2)
    :u1(U1),v1(V1),u2(U2),v2(V2){}

  // <summary>
  // Create an OffsetFeature instance with random offsets within a range.

  static OffsetFeature CreateRandom(Random& random, short int range);

  // IFeatureResponse implementation
  float GetResponse(const IDataPointCollection& data, unsigned int sampleIdx) const;
};
