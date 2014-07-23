#ifndef TrackFit_hh
#define TrackFit_hh
//============================================================================
// TrackFit.hh
// header with class definition of the TrackFit for reconstructed tracks
// See <A HREF="doc/notes/Track.pdf">TrackF.pdf</A> for more information  !!!!! not done yet
//
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-07-08
//============================================================================

#include "TVector3.h"
#include "DataObjects/include/Track.hh"
#include <map>

class DetectorGeometry;
class HitSet;
class Helix;


namespace fc {



  Helix FitToHelix(const Helix& initialHelix, const HitSet& hitSet, const trackHitSet & trackHitSet, const DetectorGeometry& detectorGeometry, TMatrixD &, double&, int&, int _debugLevel);
  Helix FitToHelixWithPV(const Helix& initialHelix, const HitSet& hitSet, const trackHitSet&  trackHitSet, const DetectorGeometry& detectorGeometry, TMatrixD& finalCovMatrix, double& finalChi2, int& finalNDof, int fitType, int _debugLevel);

} // end namescape fc

#endif // TrackFit_hh

