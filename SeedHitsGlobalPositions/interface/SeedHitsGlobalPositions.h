#ifndef DATAFORMATS_SEEDHITSGLOBALPOSITIONS_H
#define DATAFORMATS_SEEDHITSGLOBALPOSITIONS_H

#include <vector>
#include "Geometry/Vector/interface/GlobalPoint.h"

class SeedHitsGlobalPositions {
public:

  SeedHitsGlobalPositions() {}
  SeedHitsGlobalPositions(GlobalPoint InnerSeedGlobalPoint, GlobalPoint OuterSeedGlobalPoint): innerSeedGlobalPoint_(InnerSeedGlobalPoint), outerSeedGlobalPoint_(OuterSeedGlobalPoint)  {}

  inline const GlobalPoint GetInnerSeedGlobalPoint() const { return innerSeedGlobalPoint_;}
  inline const GlobalPoint GetOuterSeedGlobalPoint() const { return outerSeedGlobalPoint_;}
  inline void SetInnerSeedGlobalPoint(GlobalPoint point) { innerSeedGlobalPoint_ = point; }
  inline void SetOuterSeedGlobalPoint(GlobalPoint point) { outerSeedGlobalPoint_ = point; }

private:

  GlobalPoint innerSeedGlobalPoint_;
  GlobalPoint outerSeedGlobalPoint_;
  
};

#endif // DATAFORMATS_SEEDHITSGLOBALPOSITIONS_H
