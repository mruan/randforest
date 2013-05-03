#progma once

#include "Dataset.hpp"
#include <boost/shared_ptr.hpp>
// DataCollection should contain at least a pointer to all the images

struct CoordIdx
{
  unsigned char x, y;
  unsigned int idx;
};

class RangeDataCollection
{
  // pixel (x,y) of training image at 'idx'
  std::vector<CoordIdx> samples_;

  // label of the pixel
  std::vector<unsigned char> labels_;

  // pointer to all the pixel in the image dataset, often huge in size
  boost::shared_ptr<Dataset> pData_;
};
