#define _USE_MATH_DEFINES
#include<cmath>
#include <fstream>
#include <iostream>
#include "Exception.hh"
#include "DetectorGeometry.hh"

fc::DetectorGeometry::DetectorGeometry(){
  initSensorLimits();
  initDetectorGeometry();
}

fc::DetectorGeometry::DetectorGeometry(std::ifstream & detectorgeometryfile){
  initSensorLimits();
  initDetectorGeometry();
  initDetectorGeometryFromFile(detectorgeometryfile);
}

void fc::DetectorGeometry::initSensorLimits( void ) {

  // Minimum and maximum sensor limits
  // Confirms that users and entering a reasonable geometry

  _sensorMinLimits._nStrips = 1;
  _sensorMinLimits._stripPitch = 0.000005;
  _sensorMinLimits._center[1] = 0.0;
  _sensorMinLimits._intrinsicResolution = 0.000002;
  _sensorMinLimits._hitResolution = 0.000002;

  _sensorMaxLimits._nStrips = 10000;
  _sensorMaxLimits._stripPitch = 0.000200;
  _sensorMaxLimits._center[1] = 100;
  _sensorMaxLimits._intrinsicResolution = 0.000100;
  _sensorMaxLimits._hitResolution = 0.000100;
}


void fc::DetectorGeometry::initDetectorGeometry( void ) {

  _detectorGeometryVersion = 1;

  // Only Z oriented B field allowed
  _bField.SetXYZ(0.0,0.0,1.0);

  // Details of curvature calculation
  //using p = BqR, 
  //multiply by c to get energy in J  pc = BqRc, 
  //divide by e to get energy in eV pc = BqR3x108/1.6x10-19
  // q = Q1.6x10-19 where Q = 1, -1...
  //divide by 109 pc (in GeV) = BQR3x108/(1x10^9)
  // Also defines units  in meters
  _curvatureC = _bField.Z()*2.99792458e8/1.0e9; // ~ 0.3;  

  // MIP charge
  _MIP = 32.0;


  // Sensor postion and normal

  for (int ii_layer = 0; ii_layer < _nSensors; ++ii_layer){
    for (int ii = 0; ii < 3; ++ii){
      _sensor[ii_layer]._center.SetXYZ(0.0,0.0+(ii+1)*0.02,0.0);
      _sensor[ii_layer]._normal.SetXYZ(0.0,1.0,0.0);
      _sensor[ii_layer]._measurementVector.SetXYZ(1.0,0.0,0.0);
    }
  }

  _sensor[0]._nStrips = 2048;
  _sensor[0]._stripPitch = 0.000025;
  _sensor[0]._intrinsicResolution = 0.000007; 
  _sensor[0]._hitResolution = 0.000007; 
  _sensor[0]._threshold = 0; 


  _sensor[1]._nStrips = 2048;
  _sensor[1]._stripPitch = 0.000025;
  _sensor[1]._intrinsicResolution = 0.000007;
  _sensor[1]._hitResolution = 0.000007; 
  _sensor[1]._threshold = 0; 


  _sensor[2]._nStrips = 2048;
  _sensor[2]._stripPitch = 0.000050;
  _sensor[2]._intrinsicResolution = 0.000012;
  _sensor[2]._hitResolution = 0.000012;
  _sensor[2]._threshold = 0; 


  _sensor[3]._nStrips = 2048;
  _sensor[3]._stripPitch = 0.000050;
  _sensor[3]._intrinsicResolution = 0.000012;
  _sensor[3]._hitResolution = 0.000012;
  _sensor[3]._threshold = 0; 


  _sensor[4]._nStrips = 2048;
  _sensor[4]._stripPitch = 0.000050;
  _sensor[4]._intrinsicResolution = 0.000012;
  _sensor[4]._hitResolution = 0.000012;
  _sensor[4]._threshold = 0; 

  _defaultGeometry = true;

}

void fc::DetectorGeometry::initDetectorGeometryFromFile(std::ifstream & detectorgeometryfile) {

  // Sensor geometry file format is rigid to make mistakes less likley

  bool badGeometry = false;

  if (!detectorgeometryfile){
    throw Exception("DetectorGeometry::_initDetectorGeometryFromFile: can't open sensorgeometry.txt file");
  }

  std::string detectorGeometryString;
  int sensorNumber;

  detectorgeometryfile >> detectorGeometryString;
  if (detectorGeometryString == "DetectorGeometryVersion"){
    detectorgeometryfile >> _detectorGeometryVersion;
  } else {
    throw Exception("DetectorGeometry::_initDetectorGeometryFromFile: Bad format in sensorgeometry.txt");
  }

  detectorgeometryfile >> detectorGeometryString;
  if (detectorGeometryString == "zBField"){
  // Only Z oriented B field allowed
    double zBField;
    _bField.SetX(0.0);
    _bField.SetY(0.0);
    detectorgeometryfile >> zBField;
    _bField.SetZ(zBField);
  } else {
    throw Exception("DetectorGeometry::_initDetectorGeometryFromFile: Bad format in sensorgeometry.txt");
  }


  detectorgeometryfile >> detectorGeometryString;
  if (detectorGeometryString == "MIP"){
  // Only Z oriented B field allowed
    detectorgeometryfile >> _MIP;
  } else {
    throw Exception("DetectorGeometry::_initDetectorGeometryFromFile: Bad format in sensorgeometry.txt");
  }


  _curvatureC = _bField[2]*2.99792458e8/1.0e9;  


  for (int ii_layer = 0; ii_layer < _nSensors; ++ii_layer){
    detectorgeometryfile >> detectorGeometryString;
    if (detectorGeometryString == "Sensor"){
      detectorgeometryfile >> sensorNumber;
      if (sensorNumber == ii_layer){
	detectorgeometryfile >> _sensor[ii_layer]._nStrips;
	detectorgeometryfile >> _sensor[ii_layer]._stripPitch;
	detectorgeometryfile >> _sensor[ii_layer]._intrinsicResolution;
	detectorgeometryfile >> _sensor[ii_layer]._hitResolution;
        detectorgeometryfile >> _sensor[ii_layer]._center[0];
        detectorgeometryfile >> _sensor[ii_layer]._center[1];
        detectorgeometryfile >> _sensor[ii_layer]._center[2];
        detectorgeometryfile >> _sensor[ii_layer]._normal[0];
        detectorgeometryfile >> _sensor[ii_layer]._normal[1];
        detectorgeometryfile >> _sensor[ii_layer]._normal[2];
        detectorgeometryfile >> _sensor[ii_layer]._measurementDirection[0];
        detectorgeometryfile >> _sensor[ii_layer]._measurementDirection[1];
        detectorgeometryfile >> _sensor[ii_layer]._measurementDirection[2];


	if ( _sensor[ii_layer]._nStrips < _sensorMinLimits._nStrips|| _sensor[ii_layer]._nStrips > _sensorMaxLimits._nStrips ) badGeometry = true;
	if ( _sensor[ii_layer]._stripPitch < _sensorMinLimits._stripPitch|| _sensor[ii_layer]._stripPitch > _sensorMaxLimits._stripPitch ) badGeometry = true;
	if ( _sensor[ii_layer]._center[1] < _sensorMinLimits._center[1]|| _sensor[ii_layer]._center[1] > _sensorMaxLimits._center[1] ) badGeometry = true;
	if ( _sensor[ii_layer]._intrinsicResolution < _sensorMinLimits._intrinsicResolution|| _sensor[ii_layer]._intrinsicResolution > _sensorMaxLimits._intrinsicResolution ) badGeometry = true;
	if ( _sensor[ii_layer]._hitResolution < _sensorMinLimits._hitResolution|| _sensor[ii_layer]._hitResolution > _sensorMaxLimits._hitResolution ) badGeometry = true;
	if (badGeometry){
	  printSensorLimits();
          throw Exception("DetectorGeometry::_initDetectorGeometryFromFile: Out of bounds sensor number specifications in sensorgeometry.txt");
	}
      } else {
	throw Exception("DetectorGeometry::_initDetectorGeometryFromFile: Bad format in sensorgeometry.txt");
     }
    } else {
	throw Exception("DetectorGeometry::_initDetectorGeometryFromFile: Bad format in sensorgeometry.txt");
    }
  }

  _defaultGeometry = false;


}

void fc::DetectorGeometry::printDetectorGeometry( void ) const {

  std::cout << "Detector Geometry information" << std::endl;
  std::cout << "Detector Geometry version: " << _detectorGeometryVersion << std::endl;
  std::cout << "Magnetic field, Bz " << _bField[2] << " Tesla" << std::endl;
  std::cout << "MIP charge ACD counts in silicon sensor " << _MIP << std::endl;
  if (_defaultGeometry)  std::cout << "Using default geometry" << std::endl;
  if (!_defaultGeometry) std::cout << "Using custom geometry" << std::endl;
  std::cout << "Number sensor layers " << _nSensors << std::endl;

  for (int ii_layer = 0; ii_layer < _nSensors; ++ii_layer){
    std::cout << std::endl;
    std::cout << "Sensor layer " << ii_layer << std::endl;
    std::cout << "N strips    "  << _sensor[ii_layer]._nStrips    << std::endl;
    std::cout << "Strip pitch "  << _sensor[ii_layer]._stripPitch << std::endl;
    std::cout << "Y position  "  << _sensor[ii_layer]._center.y()       << std::endl;
    std::cout << "Intrinsic Resolution  "  << _sensor[ii_layer]._intrinsicResolution << std::endl;
    std::cout << "Hit Resolution  "  << _sensor[ii_layer]._hitResolution << std::endl;
    // !!!!! add center, normal and measurement direction
  }
}

void fc::DetectorGeometry::printSensorLimits( void ) const {

   std::cout << "Detector Sencor Geometry limit information" << std::endl;
   std::cout << "Custom Geometry specified sensor information may be outside of allowed limits" << std::endl;
   std::cout << "This may be intentional in which case allowed limits in _initDetectorGeometry should be reprogramed" << std::endl;

   std::cout << "N strips    min - max "  << _sensorMinLimits._nStrips    << " - " <<  _sensorMaxLimits._nStrips << std::endl;
   std::cout << "Strip pitch min - max "  << _sensorMinLimits._stripPitch << " - " <<  _sensorMaxLimits._stripPitch << std::endl;
   std::cout << "Y position  min - max"  << _sensorMinLimits._center[1]       << " - " <<  _sensorMaxLimits._center[1]  << std::endl;
   std::cout << "Resolution  min - max"  << _sensorMinLimits._intrinsicResolution << " - " <<  _sensorMaxLimits._intrinsicResolution  << std::endl;
   std::cout << "Resolution  min - max"  << _sensorMinLimits._hitResolution << " - " <<  _sensorMaxLimits._hitResolution  << std::endl;
 }


const fc::sensorDescriptor& fc::DetectorGeometry::getSensor(int layer) const{

  if (layer >= 0 && layer < _nSensors) return _sensor[layer];
  throw Exception(" DetectorGeometry::getSensor: Out of bounds layer");  

}