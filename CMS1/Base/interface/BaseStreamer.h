#ifndef CMS1_BaseStreamer_h
#define CMS1_BaseStreamer_h
// Description:     streamer classes that break composite classes in basic types and know how to extract information
//
// Original Author: Dmytro Kovalskyi
//
// $Author: kalavase $
// $Date: 2007/06/08 19:23:01 $
// $Revision: 1.3 $
//
#include "CMS1/Base/interface/UserData.h"
#include "CMS1/Base/interface/EventData.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "CLHEP/HepMC/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"

#include <string>
namespace cms1 {
  // first is the branch name and the second is its title
  typedef std::pair<std::string,std::string> VarName;
  struct StreamerArguments
  {
    const reco::Candidate*    candidate;
    const HepMC::GenParticle* genParticle;
    const reco::GenJet*       genJet;
    const reco::Track*        track;
    float                     jetcorrection;
    
    StreamerArguments():
      candidate(0),genParticle(0),genJet(0),track(0),jetcorrection(0)
    {}
  };
   
  class BaseStreamer
  {
  public:
    const std::vector<VarName>&     getIntNames()     { return intNames_; }
    const std::vector<VarName>&     getFloatNames()   { return floatNames_; }
    const std::vector<VarName>&     getP4Names()      { return p4Names_; }
    int                             getInt( int i )   { return ints_.empty()? 0 : *(ints_[i]); }
    float                           getFloat( int i ) { return floats_.empty()? 0 : *(floats_[i]); }
    LorentzVector                   getP4( int i )    { return p4s_.empty()? LorentzVector(0,0,0,0) : *(p4s_[i]); }
	     
    float*                          addFloat( const std::string& name, const std::string& title, float default_value );
    int*                            addInt( const std::string& name, const std::string& title, int default_value );
    LorentzVector*                  addP4( const std::string& name, const std::string& title, const LorentzVector& default_value );
     
    virtual ~BaseStreamer();
     
  protected:
    void                           setDefaults();
    std::vector<VarName>           intNames_;
    std::vector<VarName>           floatNames_;
    std::vector<VarName>           p4Names_;
    std::vector<int*>              ints_;
    std::vector<float*>            floats_;
    std::vector<LorentzVector*>    p4s_;
    std::vector<int>               intDefs_;
    std::vector<float>             floatDefs_;
    std::vector<LorentzVector>     p4Defs_;
  };
}

#endif
