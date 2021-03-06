#include<iostream>
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

    for (int iiLayer =  0; iiLayer < _detectorGeometry.getNSensors(); ++iiLayer) {

        if (_debugLevel >= 5) std::cout << "HitReco layer: " << iiLayer << std::endl;
        recoHitsLayer(stripSet,iiLayer,hitSet);

    } // end layer loop


}


void fc::HitRecoModule::recoHitsLayer(const StripSet & stripSet,int layer,
                                      HitSet& hitSet) const {


    std::vector<int> stripAdcs;
    int initialStrip;

    LayerStripMap layerStrips = stripSet.getLayerStripMap(layer);
    LayerStripMap::const_iterator  layerStripIter = layerStrips.begin();
    LayerStripMap::const_iterator  layerStripsEnd = layerStrips.end();

    while (layerStripIter != layerStripsEnd) {
        if (findCluster(layerStripIter,layerStripsEnd,stripSet,layer,initialStrip,
                        stripAdcs))
            hitSet.insertHit(std::move(buildHit(layer, initialStrip,stripAdcs)));
    }

}


bool fc::HitRecoModule::findCluster(LayerStripMap::const_iterator&
                                    layerStripIter,
                                    LayerStripMap::const_iterator& layerStripsEnd,const StripSet& stripSet,
                                    int layer, int& initialStrip, std::vector<int>& stripAdcs) const {


    stripAdcs.clear();

    if (_debugLevel >= 5) std::cout << "findCluster " << std::endl;

    while (layerStripIter != layerStripsEnd
            && stripSet.getStripAdc(layerStripIter) < _detectorGeometry.getSensor(
                layer)._threshold) {
        ++layerStripIter;
    }

    if (layerStripIter == layerStripsEnd) return false;

    initialStrip = stripSet.getStripNumber(layerStripIter);

    if (_debugLevel >= 5) std::cout << "Initial strip: " << initialStrip <<
                                        std::endl;

    int intermediateStrip = initialStrip;

    stripAdcs.push_back(stripSet.getStripAdc(layerStripIter));

    ++layerStripIter;


    while ( layerStripIter != layerStripsEnd &&
            (stripSet.getStripNumber(layerStripIter) == (intermediateStrip + 1)) &&
            (stripSet.getStripAdc(layerStripIter) >= _detectorGeometry.getSensor(
                 layer)._threshold  )) {

        intermediateStrip = stripSet.getStripNumber(layerStripIter);
        stripAdcs.push_back(stripSet.getStripAdc(layerStripIter));
        ++layerStripIter;
        if (_debugLevel >= 5) std::cout << "Intermediate strip: " << intermediateStrip
                                            << std::endl;

    }

    return (stripAdcs.size()>0);

}

fc::Hit fc::HitRecoModule::buildHit(int layer, int initialStrip,
                                    const std::vector<int> & stripAdcs) const {

    double stripHitPosition = fcf::calculateStripHitPositionFromCluster(
                                  initialStrip,stripAdcs);
    double localHitPosition = fcf::calculateLocalFromStripPosition(stripHitPosition,
                              layer,_detectorGeometry);
    TVector3 hitPosition = fcf::calculateGlobalFromLocalPosition(localHitPosition,
                           layer,_detectorGeometry);

    int numberStrips = stripAdcs.size();
    int charge = 0;
    for (auto const& adc :   stripAdcs) {
        charge += adc;
    }

    bool goodHit = true;
    if (numberStrips>2 || charge > _detectorGeometry.getMIP()) goodHit = false;

    double resolution;
    if (goodHit) {
        resolution = _detectorGeometry.getSensor(layer)._hitResolution;
    } else {
        resolution = _detectorGeometry.getSensor(layer)._badHitResolution;
    }


    return Hit(hitPosition,layer,numberStrips,charge,goodHit,resolution);

}



