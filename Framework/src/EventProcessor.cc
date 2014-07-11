#include "EventProcessor.hh"
#include "Event.hh"
#include "Module.hh"
#include "Source.hh"
#include "Exception.hh"
#include <iostream>

void fc::EventProcessor::addModule(Module* iModule) {
  _modules.emplace_back(iModule);
}

void fc::EventProcessor::processEvents() {
  try {
    int ii_event=0;
    while(true) {
      fc::Event event(ii_event);
      ++ii_event;
      if(not _source->fillNextEvent(event)) {
	break;
      }
      for(auto& module : _modules) {
	module->processEvent(event);
      }
    }
  } catch( const Exception& iException) {
    std::cerr<<"caught exception: "<<iException.what();
  } catch( const std::exception& iException) {
    std::cerr<<"caught std::exception: "<<iException.what();
  }

}