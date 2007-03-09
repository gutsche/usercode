#include "UtilFunctions_isolation.h"

namespace cms1 {
  double trackRelIsolation(const math::XYZVector momentum, 
			   const math::XYZPoint vertex,
			   const  std::vector<reco::Track>* tracks, 
			   double dRConeMax, double dRConeMin, 
			   double tkVtxDMax,
			   double vtxDiffDMax, double vtxDiffZMax, double ptMin, bool debug){

    double isoResult = -10.;

    double sumPt = 0;

    std::vector<reco::Track>::const_iterator iTk = tracks->begin();
    for (; iTk != tracks->end(); ++iTk){
      double dR = ROOT::Math::VectorUtil::DeltaR(momentum, iTk->momentum());
      //exclude tks in veto cone (set it to small number to 
      //exclude this track
      double dZ = fabs(vertex.z() - iTk->vz());
      double d0 = sqrt(iTk->vertex().perp2());
      double dD0 = sqrt((iTk->vertex() - vertex).perp2());
      if (debug) std::cout<<"Track: "
	       <<" pt: "<< iTk->pt()
	       <<", dZ: "<<dZ
	       <<", d0: "<<d0
	       <<", dD0: "<< dD0
	       <<std::endl;
      if (dR < dRConeMin) continue;
      if ( dR < dRConeMax 
	   && dZ < vtxDiffZMax
	   && d0 < tkVtxDMax
	   && dD0 < vtxDiffDMax ){
	sumPt += iTk->pt();
      }
    }

    isoResult = sumPt/sqrt(momentum.perp2());
    return isoResult;

  }

}
