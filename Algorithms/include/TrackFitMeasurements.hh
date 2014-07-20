#ifndef TrackFitMeasurements_hh
#define TrackFitMeasurements_hh
//============================================================================
// TrackFitMeasurements.hh
// header with funciton definitions for Measurements and Measurement derivatives
// in local detector coordinates
// See <A HREF="doc/notes/Track.pdf">TrackF.pdf</A> for more information  !!!!! not done yet
//
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-07-14
//============================================================================

#include "TMatrixD.h"

#include "DataObjects/include/Helix.hh"

class TVector3;
class Helix;
class DetectorGeometry;

namespace fc {

  TMatrixD expectedMeasurementVectorXZ(const fc::Helix& helix, int layer, const fc::DetectorGeometry & detectorGeometry);
  TMatrixD expectedMeasurementDerivativedXZdHC(const fc::Helix& helix,int layer, const fc::DetectorGeometry & detectorGeometry);
  TMatrixD measurementVectorXZ(const TVector3 & hitPosition);

  TMatrixD expectedMeasurementVectorX(const fc::Helix& helix, int layer, const fc::DetectorGeometry & detectorGeometry);
  TMatrixD expectedMeasurementDerivativedXdHC(const fc::Helix& helix,int layer, const fc::DetectorGeometry & detectorGeometry);
  TMatrixD measurementVector1D(const TVector3 & hitPosition,int layer, const fc::DetectorGeometry & detectorGeometry);

  double expectedMeasurementUncertianty1D(const fc::Helix & helix, const TMatrixD& covMatrix, int layer,const fc::DetectorGeometry & detectorGeometry);  


} // end namescape fc

#endif // TrackFitMeasurements_hh

