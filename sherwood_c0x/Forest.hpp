// Based on Sherwood's implementation of random forest
// but use C++0x

#include <memory>
#include <stdexcept>
#include <fstream>
#include <istream>
#include <iostream>
#include <vector>

#include "ProgressStream.hpp"

#include "Interfaces.hpp"
#include "Tree.hpp"

#define BINARY_FILE_HEADER "Sherwood.Forest"

template<class F, class S>
class Forest // where F:IFeatureResponse and S:IStatsAggregator<S>
{
  // static const char* binaryFileHeader_;
  std::vector<std::shared_ptr<Tree<F,S> > > trees_;

public:
  typedef typename std::vector<std::unique_ptr<Tree<F,S> > >::size_type TreeIndex;

  // ~Forest() // Not needed with shared_ptr

  // Add another tree to the forest
  void AddTree(std::unique_ptr<Tree<F,S> > tree){tree_.push_back(move(tree));}

  // Deserialize a forest from a file
  static std::unique_ptr<Forest<F, S> > Deserialize(const std::string& path)
  {
    std::ifstream is(path.c_str(), std::ios_base::binary);
    return Forest<F,S>::Deserialize(is);
  }

  static std::unique_ptr<Forest<F, S> > Deserialize(std::istream& i)
  {
    // TODO: Forest.h Line 70-98
  }

  // Serialize the forest to file
  void Serialize(const std::string& path)
  {
    std::ofstream os(path.c_str(), std::ios_base::binary);
    Serialize(os);
  }

  void Serialize(std::ostream& stream)
  {
    const int majorVersion=0, miniorVersion =0;

    stream.write(BINARY_FILE_HEADER, strlen(BINARY_FILE_HEADER));
    stream.write((const char*)(&majorVersion), sizeof(majorVersion));
    stream.write((const char*)(&minorVersion), sizeof(minorVersion));

    // From Forest.h: We could allow IFeatureResponse and IStatisticsAggregrator
    // to write type information here for safer deserialization (and
    // friendlier exception descriptions in the event that the user
    // tries to deserialize a tree of the wrong type).

    int treeCount = TreeGetCount();
    stream.write((const char*)(&treeCount), sizeof(treeCount));

    for(int t=0; t< treeCount; t++)
      trees_[t]->Serialize(stream);

    if (stream.bad())
      throw std::runtime_error("Forest serialization failed.");
  }

  // Access the specified tree.
  const Tree<F, S>& GetTree(int idx) const {return *trees_[idx];}
  Tree<F, S>&       GetTree(int idx) const {return *trees_[idx];}
  
  // How many trees in the forest?
  int GetTreeCount() const {return trees_.size()}

  // Apply a forest of trees to a set of data points.
  void Apply(const IDataPointCollection& data,
	     std::vector<std::vector<int> >& leafNodeIndices,
	     ProgressStream* progress=0) const
  {
    ProgressStream defaultStream(std::cout, Interest);
    progress = (progress==0)? &defaultStream : progress;

    leafNodeIndices.resize(GetTreeCount);

    for (int t=0; t< trees_.size(); t++)
      {
	leafNodeIndices[t].resize(data.Count());

	(*progress)[Interest] <<"\rApplying tree " << t<< "...";
	trees_[t]->Apply(data, leafNodeIndices[t]);
      }

    (*progress)[Interest] << "\rApplied " << trees_.size() << " trees."<<std::endl;
  }
};
