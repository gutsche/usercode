#include "CMS1/Base/interface/BaseStreamer.h"
int* cms1::BaseStreamer::addInt( const std::string& name, const std::string& title, int default_value )
{
   intNames_.push_back(VarName(name,title));
   ints_.push_back(new int(default_value));
   intDefs_.push_back(default_value);
   return ints_.back();
}

float* cms1::BaseStreamer::addFloat( const std::string& name, const std::string& title, float default_value )
{
   floatNames_.push_back(VarName(name,title));
   floats_.push_back(new float(default_value));
   floatDefs_.push_back(default_value);
   return floats_.back();
}

cms1::LorentzVector* cms1::BaseStreamer::addP4( const std::string& name, const std::string& title, const LorentzVector& default_value )
{
   p4Names_.push_back(VarName(name,title));
   p4s_.push_back(new LorentzVector(default_value));
   p4Defs_.push_back(default_value);
   return p4s_.back();
}

void cms1::BaseStreamer::setDefaults()
{  
   for(unsigned int i=0; i<p4s_.size(); ++i) *p4s_[i] = p4Defs_[i];
   for(unsigned int i=0; i<floats_.size(); ++i) *floats_[i] = floatDefs_[i];
   for(unsigned int i=0; i<ints_.size(); ++i) *ints_[i] = intDefs_[i];
}

cms1::BaseStreamer::~BaseStreamer()
{
   for(unsigned int i=0; i<p4s_.size(); ++i)  delete p4s_[i];
   for(unsigned int i=0; i<floats_.size(); ++i) delete floats_[i];
   for(unsigned int i=0; i<ints_.size(); ++i) delete ints_[i];
}
