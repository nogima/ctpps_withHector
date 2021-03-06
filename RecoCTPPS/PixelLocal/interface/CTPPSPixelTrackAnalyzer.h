#ifndef CTPPS_RPIX_Track_Analyzer_h
#define CTPPS_RPIX_Track_Analyzer_h

#include <vector>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "DataFormats/CTPPSDigi/interface/CTPPSPixelDigi.h"
#include "DataFormats/CTPPSDigi/interface/CTPPSPixelDigiCollection.h"
#include "DataFormats/CTPPSReco/interface/CTPPSPixelRecHit.h"
#include "DataFormats/CTPPSReco/interface/CTPPSPixelCluster.h"
#include "SimDataFormats/CrossingFrame/interface/CrossingFrame.h"
#include "SimDataFormats/CrossingFrame/interface/MixCollection.h"

#include "DataFormats/Common/interface/DetSetVector.h"

#include "FWCore/Utilities/interface/InputTag.h"


#include "DataFormats/CTPPSReco/interface/CTPPSPixelLocalTrack.h"

//#include "Geometry/VeryForwardGeometry/interface/CTPPSPixelSimTopology.h"
#include "TGraph.h"
#include "TH2F.h"
#include "TFile.h"

class TGraph;

//namespace edm {
//  class ParameterSet; class Event; class EventSetup;}

class CTPPSPixelTrackAnalyzer : public edm::EDAnalyzer{
  
 public:
  explicit CTPPSPixelTrackAnalyzer(const edm::ParameterSet& pset);
  virtual ~CTPPSPixelTrackAnalyzer();
  void endJob();
  void beginJob();
  void analyze(const edm::Event & event, const edm::EventSetup& eventSetup);
  static void fillDescriptions( edm::ConfigurationDescriptions& descriptions );

  TGraph *_gr_avg_45;
  TGraph *_gr_avg_56;
  TH2F *_h_45;
  TH2F *_h_56;

  TH2F *_h_D_45;
  TH2F *_h_D_56;
  TH1F *_h_D_ADC;

  TH2F *_h_RH_45;
  TH2F *_h_RH_56;

  TH1F *_h_CL_size;
  TH1F *_h_ELE_size1;
  TH1F *_h_ELE_size2;
  TH1F *_h_ELEsum_size2;
  TH1F *_h_ELEmin_size2;
  TH1F *_h_ELEmax_size2;

 private:
  std::string _outFileName;

  edm::EDGetTokenT< edm::DetSetVector<CTPPSPixelLocalTrack> > pixelTrackToken_;
  edm::EDGetTokenT< edm::DetSetVector<CTPPSPixelDigi> > pixelDigiToken_;
  edm::EDGetTokenT<edm::DetSetVector<CTPPSPixelRecHit>> tokenCTPPSPixelRecHit_;
  edm::EDGetTokenT<edm::DetSetVector<CTPPSPixelCluster>> tokenCTPPSPixelCluster_;

  TFile* _outFile;
  unsigned int _verbosity;
  int _bunchCrossing;
  int _ls;

  unsigned int tracks45_per_ls[10000];
  unsigned int tracks56_per_ls[10000];
  unsigned int events_per_ls[10000];
  
  unsigned int ls_max;
  

};

#endif    
