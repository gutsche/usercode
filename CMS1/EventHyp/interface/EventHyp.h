#ifndef CMS1EventHyp_h
#define CMS1EventHyp_h
/*
 * Package:         CMS1/EventHyp
 * Class:           EventHyp
 *
 *  Description:     EventHyp Black Box to return a list of Event Hypotheses
 *
 *  Created by LATBauerdick on 3/22/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */
 
#include "CMS1/EventHyp/interface/DiLeptonCandidate.h"

namespace cms1 {
	class EventHyp: public BlackBox {
	public:
		EventHyp(): BlackBox(){}
      
		enum EventHypType { DiLeptonCandidate };
     
      // This is the function that does all the work
		std::vector<const cms1::DiLeptonCandidate*> getEventHyp (const EventHypType);
	
		void dump(ostream&, std::vector<const cms1::DiLeptonCandidate*>);
		
		std::vector<const cms1::DiLeptonCandidate*> getEventHyp (
			std::vector<const reco::Candidate*> tightElectrons,
			std::vector<const reco::Candidate*> looseElectrons,
			std::vector<const reco::Candidate*> tightMuons,
			std::vector<const reco::Candidate*> looseMuons,
			std::vector<const reco::Candidate*> jets,
			double met,
			Cuts metCut,
			Cuts metCutAroundZ);

protected:
	cms1::DiLeptonCandidate* returnDiLeptonCandidate(reco::Candidate* lt,reco::Candidate* ll,std::vector<const reco::Candidate*> jets, double met);
  };
}

#endif
