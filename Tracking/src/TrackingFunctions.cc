#include "DataObjects/include/Track.hh"
#include "DataObjects/include/TrackSet.hh"
#include "Tracking/include/TrackingFunctions.hh"

int fcf::numberMatchedHits(const fc::Track& track1, const fc::Track& track2 ) {

    int matchedHits = 0;
    for (auto const& hit : track1.getHits()) {

        for (auto const& hit2 :  track2.getHits()) {

            if (hit == hit2) ++matchedHits;
        }

    }

    return matchedHits;

}


bool fcf::betterOverlappingTrack(const fc::Track& track0,
                                 const fc::Track& track1) {

    if ((track0.getHits().size() == track1.getHits().size()
            && track0.getChi2()/track0.getNDof()> track1.getChi2()/track1.getNDof())||
            track0.getHits().size() < track1.getHits().size()) {
        return true;
    } else if ((track1.getHits().size() == track0.getHits().size()
                && track1.getChi2()/track1.getNDof()> track0.getChi2()/track0.getNDof())||
               track1.getHits().size() < track0.getHits().size()) {
        return false;
    }
    return false;
}
