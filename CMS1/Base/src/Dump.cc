
#include "CMS1/Base/interface/Dump.h"


namespace cms1 {


 void Dump::event() {
	
	std::cout << "Event Dump" << std::endl; 
  
	
}

 	void Dump::candidates(std::string name, std::vector<const reco::Candidate*> & c) {
	
		std::cout << "Dump of " << name << std::endl;
	
		for ( std::vector<const reco::Candidate*>::iterator i = c.begin(), ie = c.end();
			i != ie;
			++i ) {
				reco::Candidate* cp = *i;
  			std::cout << cp->pt() << cp->eta() << cp->phi() << std::endl; 
			}
		}
	
	}

