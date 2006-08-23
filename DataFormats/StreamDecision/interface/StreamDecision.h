#ifndef DATAFORMATS_STREAMDECISION_H
#define DATAFORMATS_STREAMDECISION_H

//
// Package:         DataFormats/StreamDecision
// Class:           StreamDecision
// 
// Description:     Stream Decision
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Tue Aug 22 21:33:57 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/03/28 22:43:33 $
// $Revision: 1.3 $
//

class StreamDecision {
public:

  StreamDecision() : fnal_(false), 
		     cnaf_(false),
		     ral_(false),
		     in2p3_(false),
		     asgc_(false),
		     fzk_(false),
		     pic_(false) {}
  StreamDecision(bool fnal, 
		 bool cnaf, 
		 bool ral,
		 bool in2p3,
		 bool asgc,
		 bool fzk,
		 bool pic): fnal_(fnal), 
			    cnaf_(cnaf),
			    ral_(ral),
			    in2p3_(in2p3),
			    asgc_(asgc),
			    fzk_(fzk),
			    pic_(pic) {}

  inline void SetFNAL(bool input)  { fnal_ = input; }
  inline bool GetFNAL() const      { return fnal_; }

  inline void SetCNAF(bool input)  { cnaf_ = input; }
  inline bool GetCNAF() const      { return cnaf_; }

  inline void SetRAL(bool input)   { ral_ = input; }
  inline bool GetRAL() const       { return ral_; }

  inline void SetIN2P3(bool input) { in2p3_ = input; }
  inline bool GetIN2P3() const    { return in2p3_; }

  inline void SetASGC(bool input)  { asgc_ = input; }
  inline bool GetASGC() const      { return asgc_; }

  inline void SetFZK(bool input)   { fzk_ = input; }
  inline bool GetFZK() const       { return fzk_; }

  inline void SetPIC(bool input)   { pic_ = input; }
  inline bool GetPIC() const       { return pic_; }


private:

  bool fnal_;
  bool cnaf_;
  bool ral_;
  bool in2p3_;
  bool asgc_;
  bool fzk_;
  bool pic_;

};

#endif // DATAFORMATS_STREAMDECISION_H
