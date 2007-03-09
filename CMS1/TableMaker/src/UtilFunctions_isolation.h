#ifndef CMS1UtilFunctions_isolation_h
#define CMS1UtilFunctions_isolation_h

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/Math/interface/Vector.h"
#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/Math/interface/Point3D.h"
#include <Math/GenVector/VectorUtil.h>
#include <vector>

namespace cms1 {
  double trackRelIsolation(const math::XYZVector momentum, 
			   const math::XYZPoint vertex, 
			   const  std::vector<reco::Track>* tracks, 
			   double dRConeMax, double dRConeMin, 
			   double tkVtxDMax,
			   double vtxDiffDMax, double vtxDiffZMax, double ptMin, bool debug = false);

/*   double trackRelIsolation(const reco::Track& tk, const  std::vector<reco::Track>* tracks,  */
/* 			   double dRConeMax, double dRConeMin,  */
/* 			   double tkVtxDMax, */
/* 			   double vtxDiffDMax, double vtxDiffZMax){ */
/*     return trackRelIsolation(tk.momentum(), tk.vertex(), tracks, */
/* 			     dRConeMax, dRConeMin, tkVtxDMax, */
/* 			     vtxDiffDMax, vtxDiffZMax); */
/*   } */

}

#endif
