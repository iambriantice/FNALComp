#ifndef Tracking_TrackingSelectors_hh
#define Tracking_TrackingSelectors_hh
//============================================================================
// TrackingSelectors.hh
// header with helper function definitions for Track, TrackCandididate finding strategies
// and LayerFinder
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-07-30
//============================================================================

class Track;
class DetectorGeometry;
struct TrackingSelection;

namespace fcf {
struct TrackingSelector {
    double _minPTCut {0.0};
    double _nHitCut {0};
    double _maxChi2NDofCut {1000000.0};
    bool _useFiducialDRCut {false};
    bool _useFiducialDZCut {false};
};

// helper functions for track selection
bool goodCandidateHelix(const fc::Helix &,const fc::DetectorGeometry&,
                        const TrackingSelector&);
bool goodCandidateTrack(const fc::Track &,const fc::DetectorGeometry&,
                        const TrackingSelector&);


} // end namescape fcf

#endif // Tracking_TrackingSelectors_hh

