#ifndef CMS1DUMP_h
#define CMS1DUMP_h

#include <vector>
#include <string>
#include "DataFormats/Candidate/interface/Candidate.h"

namespace cms1 {
   class Dump {
      public:
	static void event();
	static void candidates(std::string name, std::vector<const reco::Candidate*>& );

	};
}

#endif
