#pragma once

//#include "RangeImage.hpp"

// DataCollection
#include <iostream>
#include <exception>
#include <cfloat>

typedef short int COORDTYPE;
struct CoordIdx
{
  COORDTYPE x, y;
  unsigned int idx;
};

class RangeImage
{
  static const int W = 174;
  static const int H = 144;
public:
  RangeImage(int i=0)//const int width, const int height)
  //    :W(width), H(height)
  {
    try{
      pData.reset(new float[W*H]);
    }
    catch(std::exception& e){
      std::cout<< "Exception at" << i<< " : " << e.what() << std::endl;
    }
  }
  
  ~RangeImage()
  {
    pData.release();
  }

  inline float GetDepth(unsigned int w, unsigned int h)
  {
    if(w <=0 || w>=W)
      return FLT_MAX;

    if(h <=0 || h>=H)
      return FLT_MAX;

    // row major matrix
    return pData.get()[h*W+w];
  }

private:
  std::unique_ptr<float[]> pData;
};

//typedef typename std::vector<Node<F,S> >::size_type NodeIndex;
typedef typename std::vector<RangeImage>::size_type DataIndex;

class RangeDataCollection
{
public:
  RangeDataCollection(const int nPixels, const int nFrames)
  :samples_(nPixels), frames_(nFrames)
  {}

public:
  // pixel (x,y) of training image at 'idx'
  std::vector<CoordIdx> samples_;

  // label of the pixel
  // std::vector<unsigned char> labels_;

  // const int numPixels;
  // const int numFrames;
  // pointer to all the depth images in the dataset, huge in size
  std::vector<RangeImage> frames_;

  // There are more "datum" than RangeImages, since each RangeImages
  // will contribute quite a few "datum"
  inline int Count() {return samples_.size();}

  inline float GetValue(unsigned int index)
  {
    CoordIdx& coord = samples_[index];
    return frames_[coord.idx].GetDepth(coord.x, coord.y);
  }

  /******************************************************************
   This function implements the awesome feature in Shotton paper 2010
   *****************************************************************/
  template<class F>
  inline float GetResponse(unsigned int index, F& feature)
  {
    CoordIdx& coord = samples_[index];
    RangeImage& ri = frames_[coord.idx];
    float dOrigin = ri.GetDepth(coord.x, coord.y);

    // TODO: how to deal with rounding error?
    COORDTYPE X1 = coord.x + feature.u1 / dOrigin;
    COORDTYPE Y1 = coord.y + feature.v1 / dOrigin;

    COORDTYPE X2 = coord.x + feature.u2 / dOrigin;
    COORDTYPE Y2 = coord.y + feature.v2 / dOrigin;
    return ri.GetDepth(X1, Y1) - ri.GetDepth(X2, Y2);
  }

  // FIX: TODO: how to serialize/deserialize these data from a script!
};
