// Obsolete

#include "FeatureResponse.hpp"

#include "DataCollection.hpp"
#include "Random.h"

OffsetFeature OffsetFeature::CreateRandom(Random& random)
{
  return OffsetFeature();
}

std::string OffsetFeature::ToString() const
{
  std::stringstream s;
  s << "Offsetfeature(" << u1 << "," << v1 <<"), ("<< u2 <<", "<< v2<<")";
  return s.str();
}

float OffsetFeature::GetResponse(const IDataPointCollection& data, unsigned int index) const
{
  const RangeDataCollection& rd = (const RangeDataCollection&) (data);
  
  // Implement Jamie Shotton's depth invariant feature
  // TODO:
}

