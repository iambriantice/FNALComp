#ifndef TrackRecoStrategy_hh
#define TrackRecoStrategy_hh
//============================================================================
// TrackRecoStrategy.hh
// header with function definitions of the TrackRecoStrategy base class for tracking stratgies
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-07-22
//============================================================================

#include<vector>
#include "DataObjects/include/TrackSet.hh"
#include "DataObjects/include/Track.hh"

namespace fc {

  class DetectorGeometry;
  class HitSet;

  class TrackRecoStrategy {

  private:

    int _debugLevel;

    // Detector information
    const DetectorGeometry & _detectorGeometry;

 

  protected:
    TrackRecoStrategy(int debugLevel,const DetectorGeometry & detectorGeometry):_debugLevel(debugLevel),_detectorGeometry(detectorGeometry) {};
  public: 
   TrackRecoStrategy( const TrackRecoStrategy&) = delete;
    ~TrackRecoStrategy() = default;

    void recoTracks(trackSet & trackCandidateSet, const HitSet& hitSet);


    // Iterative track reconstruction
    virtual void findTracks(trackSet& trackCandidateSet,const HitSet & recoHitSet) = 0;

    void finalTrackFilter(fc::trackSet & trackCandidateSet);

    // Helper functions for final track filter
    int numberMatchedHits(const Track&, const Track&) const;
    bool betterOverlappingTrack(const Track&, const Track&) const;

    // get functions
    const DetectorGeometry & getDetectorGeometry() const {return _detectorGeometry;}
    int getDebugLevel() const {return _debugLevel;}

  };


} // end namescape fc

#endif // TrackRecoStrategy_hh

