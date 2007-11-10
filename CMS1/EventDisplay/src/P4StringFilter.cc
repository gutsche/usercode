/* 
 *
 * Selects events with a configurable string-based cut on 
 * a collection of vector<LorentzVector>
 * 
 * usage:
 *
 * module filter = P4StringFilter {
 *   src = edmNtupleMaker:genps_id
 *   string cut = "== 13"
 * }
 *
 */

#include "FWCore/Framework/interface/MakerMacros.h"
#include "PhysicsTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "PhysicsTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "DataFormats/Math/interface/LorentzVector.h"

typedef SingleObjectSelector<std::vector<math::XYZTLorentzVector>, StringCutObjectSelector<math::XYZTLorentzVector> > P4StringFilter;

DEFINE_FWK_MODULE( P4StringFilter );
