#ifndef HiggsSelector_h
#define HiggsSelector_h
/** \class HiggsSelector
 *
 * Simple Higgs reconstruction analysis example
 *
 * \author Oliver Gutsche
 *
 * $Id: HiggsSelector.h,v 1.2 2006/07/07 15:55:38 chrjones Exp $
 */
#include <TH1.h>
#include "FWCore/TFWLiteSelector/interface/TFWLiteSelectorBasic.h"

class HiggsSelector : public TFWLiteSelectorBasic {
 public :
  HiggsSelector();
  ~HiggsSelector();
  void begin(TList*&);
  void preProcessing(const TList*, TList&);
  void process(const edm::Event&);
  void postProcessing(TList&);
  void terminate(TList&);

 private:
  /// histograms
  TH1F* h_chisqtr;
  TH1F* h_pttr;
  TH1F* h_mmumu;
  TH1F* h_noz;
  TH1F* h_mzz;
  TH1F* h_m4mu;

  HiggsSelector(HiggsSelector const&);
  HiggsSelector operator=(HiggsSelector const&);
};
#endif
