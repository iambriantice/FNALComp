#include<iostream>
#include <algorithm>
#include "Geometry/include/StripHitFunctions.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/Hit.hh"
#include "DataObjects/include/HitSet.hh"
#include "DataObjects/include/StripSet.hh"
#include "Modules/include/HitRecoModule.hh"

fc::HitRecoModule::HitRecoModule(int debugLevel,
    const std::string& iInputStripsLabel, const std::string& iOutputHitsLabel,
    const DetectorGeometry & detectorGeometry):
  _debugLevel(debugLevel),
  _inStripsLabel(iInputStripsLabel),
  _outHitsLabel(iOutputHitsLabel),
  _detectorGeometry(detectorGeometry) {
  }

void fc::HitRecoModule::processEvent(fc::Event& event)
{
  Handle<StripSet> genStripSet = event.get<StripSet>(_inStripsLabel);
  std::unique_ptr<HitSet> recoHitSet( new HitSet );

  recoHits(*genStripSet,*recoHitSet);

  if (_debugLevel >= 2) recoHitSet->print(std::cout);

  event.put(_outHitsLabel, std::move(recoHitSet));
}

void fc::HitRecoModule::recoHits(const StripSet& stripSet,
    HitSet & hitSet) const {

  const int initializeDummyValue = -99;

  //loop over layers
  for( int iLayer = 0; iLayer != _detectorGeometry.getNSensors(); ++iLayer ) {

    std::vector<int> adcs;
    //initialize these to dummy value that will not let our first strip seem adjacent
    int firstStrip = initializeDummyValue;
    int lastStrip = initializeDummyValue;
    

    //we will ignore low adc values
    const SensorDescriptor & sensDesc = _detectorGeometry.getSensor(iLayer);
    const double adcThresh = sensDesc._threshold;

    //loop over the strips for each layer
    for( auto const& iLayerStrip : stripSet.getLayerStripMap(iLayer) ) {
      const int thisStrip = iLayerStrip.first;
      const int adc       = iLayerStrip.second;

      if( adc < adcThresh )
        continue;

      //time to make a new Hit if this strip is not adjacent to previous strip
      if( thisStrip != lastStrip+1 )
      {
        //if this was the very first strip then adcs are empty and we do not want to make a Hit yet
        if( ! adcs.empty() )
          addHit( hitSet, iLayer, firstStrip, adcs );

        //get ready for the next one
        adcs.clear();
        firstStrip = thisStrip;
      }

      adcs.push_back(adc);
      lastStrip = thisStrip;

    }//end loop over stripmap

    //add last hit if necessary
    if( ! adcs.empty() )
      addHit( hitSet, iLayer, firstStrip, adcs );

  }//end loop over layers
}


void fc::HitRecoModule::addHit( HitSet & hitSet, int layer, int firstStrip, std::vector<int>& adcs) const 
{
  //calculate the charged-weighted position of of the strips
  const double stripHitPos = fcf::calculateStripHitPositionFromCluster( firstStrip, adcs );
  const double localHitPos = fcf::calculateLocalFromStripPosition( stripHitPos, layer, _detectorGeometry );
  const TVector3  globalPos = fcf::calculateGlobalFromLocalPosition( localHitPos, layer, _detectorGeometry );

  //if there are more than 2 hits, then the hit is bad (it seems)
  const bool goodHit = adcs.size()<3;

  //use detector stuff to get resolution
  const SensorDescriptor & sensDesc = _detectorGeometry.getSensor(layer);
  const double resolution = sensDesc._hitResolution; //todo: different resolution for good/bad.  Test version does not do this.

  //make the new hit and add it to HitSet
  Hit hit( globalPos, layer, adcs.size(), std::accumulate(adcs.begin(), adcs.end(), 0), goodHit, resolution );
  hitSet.insertHit(std::move(hit));
}

