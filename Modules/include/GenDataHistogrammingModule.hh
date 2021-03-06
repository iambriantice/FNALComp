#ifndef Modules_GenDataHistogrammingModule_hh
#define Modules_GenDataHistogrammingModule_hh
//============================================================================
// GenDataHistogrammingModule.hh
// Module used to fill histograms from generated Event data objects
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-08-01
//============================================================================

#include <string>
#include "Framework/include/Module.hh"

class TH1D;

///
/// Class GenDataHistogrammingModule: Module for filling histograms from generated Event data objects
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-08-01
///

namespace fc {

class DetectorGeometry;

class GenDataHistogrammingModule : public Module {

public:

    GenDataHistogrammingModule(const std::string&
                               inputGenTrackSetLabel, //label used to GenTrackSet
                               const std::string& inputHitSetLabel, //label used to get HitSet
                               const std::string& inputStripSetLabel, //label used to get StripSet,
                               const DetectorGeometry&);

    void processEvent(Event& ) override;

    void endJob() override;

private:

    const std::string _tracksLabel;
    const std::string _hitSetLabel;
    const std::string _stripSetLabel;


    const DetectorGeometry& _detectorGeometry;

  int _numberEvents;

    //ROOT will delete this when the TFile to which it is attached is deleted

    TH1D * _hDR;
    TH1D * _hPhi0;
    TH1D * _hKappa;
    TH1D * _hDZ;
    TH1D * _hTanL;
    TH1D * _hPT;
    TH1D * _hPZ;
    TH1D * _hRC;

    TH1D* _hNHitsLayer;
    TH1D* _hNHitsTrack;

    TH1D* _hHitPositionX;
    TH1D* _hHitPositionY;
    TH1D* _hHitPositionZ;


    TH1D* _hNStripsPerLayer;

    void initializeHistograms();


};

} // end namespace fc

#endif // Modules_GenDataHistogrammingModule_hh
