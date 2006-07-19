#ifndef DATAFORMATS_RECHITGLOBALPOSITION_H
#define DATAFORMATS_RECHITGLOBALPOSITION_H

#include <vector>
#include "Geometry/Vector/interface/GlobalPoint.h"

class RecHitGlobalPosition {
public:

  RecHitGlobalPosition() {}
  RecHitGlobalPosition(GlobalPoint point): globalPoint_(point) {}

  inline const GlobalPoint GetGlobalPoint() const { return globalPoint_;}
  inline void SetGlobalPoint(GlobalPoint point) { globalPoint_ = point; }

private:

  GlobalPoint globalPoint_;
  
};

#endif // DATAFORMATS_RECHITGLOBALPOSITION_H
