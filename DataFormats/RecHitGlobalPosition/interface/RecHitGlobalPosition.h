#ifndef DATAFORMATS_RECHITGLOBALPOSITION_H
#define DATAFORMATS_RECHITGLOBALPOSITION_H

#include "Geometry/Vector/interface/GlobalPoint.h"

class RecHitGlobalPosition {
public:

  RecHitGlobalPosition() {}
  RecHitGlobalPosition(GlobalPoint point): globalPoint_(point) {}

  inline GlobalPoint GetGlobalPoint() { return globalPoint_;}
  inline void SetGlobalPoint(GlobalPoint input) { globalPoint_ = input; }

private:

  GlobalPoint globalPoint_;
  
};

#endif // DATAFORMATS_RECHITGLOBALPOSITION_H
