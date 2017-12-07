
#include "RecoCTPPS/PixelLocal/interface/CTPPSPixelTrackAnalyzer.h"

#include "Geometry/VeryForwardGeometryBuilder/interface/CTPPSGeometry.h"
#include "Geometry/Records/interface/VeryForwardMisalignedGeometryRecord.h"

#include "DataFormats/GeometryVector/interface/LocalPoint.h"
#include "DataFormats/GeometryVector/interface/LocalVector.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"

#include <iostream>
#include <string>


using namespace std;

CTPPSPixelTrackAnalyzer:: CTPPSPixelTrackAnalyzer(const edm::ParameterSet& pset)
// : theRPixDetTopology_(pset)
{

  _outFile = new TFile("myFile.root","RECREATE");
  _verbosity = pset.getUntrackedParameter<unsigned int> ("Verbosity");
  if(_outFile->IsOpen()) cout<<"file open!"<<endl;
  else cout<<"*** Error in opening file ***"<<endl;
  
  auto tagPixelDigi = pset.getParameter<edm::InputTag>("tagPixelDigi"); 
  pixelDigiToken_ = consumes<edm::DetSetVector<CTPPSPixelDigi> >( tagPixelDigi);

  auto tagPixelRecHit = pset.getParameter<edm::InputTag>("tagPixelRecHit");
  tokenCTPPSPixelRecHit_ = consumes<edm::DetSetVector<CTPPSPixelRecHit> >(tagPixelRecHit);

  auto tagPixelCluster = pset.getParameter<edm::InputTag>("tagPixelCluster");
  tokenCTPPSPixelCluster_ = consumes<edm::DetSetVector<CTPPSPixelCluster> >(tagPixelCluster);

  auto tagPixelTrack = pset.getParameter<edm::InputTag>("tagPixelTrack"); 
  if (not tagPixelTrack.label().empty()){
    pixelTrackToken_   = consumes< edm::DetSetVector<CTPPSPixelLocalTrack> >  (tagPixelTrack);
  }
 		    

 }

CTPPSPixelTrackAnalyzer::~CTPPSPixelTrackAnalyzer(){
}

void CTPPSPixelTrackAnalyzer::fillDescriptions( edm::ConfigurationDescriptions& descriptions )
{
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>( "tagPixelTrack"  , edm::InputTag( "ctppsPixelLocalTracks"   ) );
  desc.add<edm::InputTag>( "tagPixelDigi"  , edm::InputTag( "ctppsPixelDigis"   ) );
  desc.add<edm::InputTag>( "tagPixelRecHit"  , edm::InputTag( "ctppsPixelRecHits"   ) );
  desc.add<edm::InputTag>( "tagPixelCluster"  , edm::InputTag( "ctppsPixelClusters"   ) );
  desc.addUntracked<unsigned int>("Verbosity",0);
  descriptions.add( "ctppsPixelTrackAnalyzer", desc );
}

void CTPPSPixelTrackAnalyzer::beginJob(){
  for(unsigned int i=0; i<10000; i++){
    tracks45_per_ls[i]=0;
    tracks56_per_ls[i]=0;
    events_per_ls[i]=0;
  }
  ls_max=10000;
  _h_45 = new TH2F("Tracks45","Tracks45",300,-0.5,299.5,200,-0.5,199.5);
  _h_56 = new TH2F("Tracks56","Tracks56",300,-0.5,299.5,200,-0.5,199.5);
  _h_D_45 = new TH2F("Digis45","Digis45",300,-0.5,299.5,200,-0.5,199.5);
  _h_D_56 = new TH2F("Digis56","Digis56",300,-0.5,299.5,200,-0.5,199.5);
  _h_D_ADC = new TH1F("DigisADC","DigisADC",257,-0.5,256.5);
  _h_RH_45 = new TH2F("RH45","RH45",100,-15,15,100,-15,15);
  _h_RH_56 = new TH2F("RH56","RH56",100,-15,15,100,-15,15);

  _h_CL_size = new TH1F("CLsize","CLsize",10,-0.5,9.5);
  _h_ELE_size1 = new TH1F("ELEsize1","ELEsize1",200,-0.5,100000);
  _h_ELE_size2 = new TH1F("ELEsize2","ELEsize2",200,-0.5,100000);
  _h_ELEsum_size2 = new TH1F("ELEsumsize2","ELEsumsize2",200,-0.5,100000);
  _h_ELEmin_size2 = new TH1F("ELEminsize2","ELEminsize2",200,-0.5,100000);
  _h_ELEmax_size2 = new TH1F("ELEmaxsize2","ELEmaxsize2",200,-0.5,100000);


}

void CTPPSPixelTrackAnalyzer::endJob(){

  float avg_tks_45[10000];
  float avg_tks_56[10000];
  float ls[10000];
  
  for(unsigned int i=0; i<10000; i++){
    avg_tks_45[i]=0;
    avg_tks_56[i]=0;
    ls[i]=float(i);
    if(events_per_ls[i]>0){
      avg_tks_45[i]=float(tracks45_per_ls[i])/float(events_per_ls[i]);
      avg_tks_56[i]=float(tracks56_per_ls[i])/float(events_per_ls[i]);
    }

    

  }
  _gr_avg_45 = new TGraph(10000,ls,avg_tks_45);
  _gr_avg_56 = new TGraph(10000,ls,avg_tks_56);




  _outFile->cd();



   _gr_avg_45->SetName("AvgPxlTrk45");
   _gr_avg_45->SetTitle("Avg pxl trk number 45");
   _gr_avg_45->SetMarkerColor(2); 
   _gr_avg_45->SetMarkerStyle(3);
   _gr_avg_45->GetXaxis()->SetLimits(0, ls_max+10);
   _gr_avg_45->GetXaxis()->SetTitle("LS");
//   _gr_avg_45->GetYaxis()->SetLimits(ylo,yhi);
//   _gr_avg_45->GetYaxis()->SetTitle(axis);
//   _gr_avg_45->Draw("A*");

   _gr_avg_56->SetName("AvgPxlTrk56");
   _gr_avg_56->SetTitle("Avg pxl trk number 56");
   _gr_avg_56->SetMarkerColor(4); 
   _gr_avg_56->SetMarkerStyle(3);
   _gr_avg_56->GetXaxis()->SetLimits(0, ls_max+10);
   _gr_avg_56->GetXaxis()->SetTitle("LS");

   _h_45->SetName("PxlTrk45");
   _h_45->SetTitle("pxl trk number 45");
   _h_45->GetXaxis()->SetTitle("LS");
   _h_45->GetYaxis()->SetTitle("N tracks");

   _h_56->SetName("PxlTrk56");
   _h_56->SetTitle("pxl trk number 56");
   _h_56->GetXaxis()->SetTitle("LS");
   _h_56->GetYaxis()->SetTitle("N tracks");

   _h_CL_size->SetName("CLsize");

  _gr_avg_45->Write();
  _gr_avg_56->Write();
  _h_45->Write();
  _h_56->Write();

  _h_D_45->Write();
  _h_D_56->Write();
  _h_D_ADC->Write();

  _h_RH_45->Write();
  _h_RH_56->Write();

  _h_CL_size->Write();
  _h_ELE_size1->Write();
  _h_ELE_size2->Write();
  _h_ELEsum_size2->Write();
  _h_ELEmin_size2->Write();
  _h_ELEmax_size2->Write();

 _outFile->Close();
  delete _outFile;
}

void  CTPPSPixelTrackAnalyzer::analyze(const edm::Event & event, const edm::EventSetup& eventSetup){
  if(_verbosity)cout << "--- Run: " << event.id().run()
       << " Event: " << event.id().event()
       << " Bunch crossing: " << event.bunchCrossing()
       << " Lumi block: " << event.luminosityBlock() << endl;
  

  if(event.luminosityBlock()>10000)throw cms::Exception("Track analyzer") << "lumi block > 10000";

  _bunchCrossing = event.bunchCrossing();
  _ls = event.luminosityBlock();
  edm::Handle<edm::DetSetVector<CTPPSPixelDigi> > digis;
  event.getByToken(pixelDigiToken_, digis);

  unsigned int digis45=0;
  unsigned int digis56=0;

 // Loop on digis
  edm::DetSetVector<CTPPSPixelDigi>::const_iterator digiDSViter = digis->begin();
  for (; digiDSViter != digis->end(); digiDSViter++) {
    CTPPSPixelDetId detIdObject(digiDSViter->detId());
    edm::DetSet<CTPPSPixelDigi>::const_iterator begin = (*digiDSViter).begin();
    edm::DetSet<CTPPSPixelDigi>::const_iterator end = (*digiDSViter).end();
    for (edm::DetSet<CTPPSPixelDigi>::const_iterator di = begin; di != end; di++) {
    // Detector ID
      _h_D_ADC->Fill((*di).adc());
      if(detIdObject.arm()==0){
	digis45++;
	_h_D_45->Fill((*di).row(),(*di).column());
      }else{
	digis56++;
	_h_D_56->Fill((*di).row(),(*di).column());
      }
    }
  }

//------------------------------------------------------
  edm::Handle<edm::DetSetVector<CTPPSPixelCluster> > clusters;
  event.getByToken(tokenCTPPSPixelCluster_, clusters);
  edm::DetSetVector<CTPPSPixelCluster>::const_iterator clDSViter = clusters->begin();

  for (; clDSViter != clusters->end(); clDSViter++) {
    CTPPSPixelDetId detIdObject(clDSViter->detId());
    edm::DetSet<CTPPSPixelCluster>::const_iterator begin = (*clDSViter).begin();
    edm::DetSet<CTPPSPixelCluster>::const_iterator end = (*clDSViter).end();
    for (edm::DetSet<CTPPSPixelCluster>::const_iterator cl = begin; cl != end; cl++) {
      _h_CL_size->Fill((*cl).size());

      if( (*cl).size()<=2){
//	cout << (*cl).pixelADC(0) << endl;
	if( (*cl).size()==1) _h_ELE_size1->Fill((*cl).pixelADC(0));
	else{
	  _h_ELE_size2->Fill((*cl).pixelADC(0));
	  _h_ELE_size2->Fill((*cl).pixelADC(1));
	  _h_ELEsum_size2->Fill((*cl).pixelADC(1)+(*cl).pixelADC(0));
	  _h_ELEmin_size2->Fill(fmin((*cl).pixelADC(1),(*cl).pixelADC(0)));
	  _h_ELEmax_size2->Fill(fmax((*cl).pixelADC(1),(*cl).pixelADC(0)));
	}
      }

    }
  }


//-------------------------------------------------------
  edm::Handle<edm::DetSetVector<CTPPSPixelRecHit> > recHits;
  event.getByToken(tokenCTPPSPixelRecHit_, recHits);
  edm::DetSetVector<CTPPSPixelRecHit>::const_iterator rhDSViter = recHits->begin();
  for (; rhDSViter != recHits->end(); rhDSViter++) {
    CTPPSPixelDetId detIdObject(rhDSViter->detId());
    edm::DetSet<CTPPSPixelRecHit>::const_iterator begin = (*rhDSViter).begin();
    edm::DetSet<CTPPSPixelRecHit>::const_iterator end = (*rhDSViter).end();
    for (edm::DetSet<CTPPSPixelRecHit>::const_iterator rh = begin; rh != end; rh++) {
    // Detector ID

      if(detIdObject.arm()==0){
	_h_RH_45->Fill((*rh).getPoint().x(),(*rh).getPoint().y());
      }else{
	_h_RH_56->Fill((*rh).getPoint().x(),(*rh).getPoint().y());
      }
    }
  }






  edm::Handle< edm::DetSetVector<CTPPSPixelLocalTrack> > inputPixelTracks;

  unsigned int tracks45=0;
  unsigned int tracks56=0;
  if (not pixelTrackToken_.isUninitialized()){
    event.getByToken( pixelTrackToken_, inputPixelTracks );
    
// process tracks from pixels
    
    for ( const auto& rpv : *inputPixelTracks ) {
      const uint32_t rpId = rpv.detId();
      for ( const auto& trk : rpv ) {
        if ( !trk.isValid() ) continue;
//        pOut->emplace_back( rpId, trk.getX0(), trk.getX0Sigma(), trk.getY0(), trk.getY0Sigma() );
	if(rpId & 0x1000000) tracks56++;
	else tracks45++;
      }
    }
  }
  
  if(_verbosity){
    cout << "           Tracks 45 - 56 : "<< tracks45 << " - " << tracks56 << endl;
    cout << "           Digis  45 - 56 : "<< digis45 << " - " << digis56 << endl;
  }

  if(tracks45==0 && digis45>20)  _h_45->Fill(_ls,20.);
  else if(tracks45>19)_h_45->Fill(_ls,19.);
  else _h_45->Fill(_ls,tracks45);
 
  if(tracks56==0 && digis56>20)  _h_56->Fill(_ls,20.);
  else if(tracks56>19)_h_56->Fill(_ls,19.);
  else _h_56->Fill(_ls,tracks56);
 


//incrementing total number of tracks per lumi
  tracks45_per_ls[_ls] += tracks45;
  tracks56_per_ls[_ls] += tracks56;
  
  events_per_ls[_ls]++;

/*

 // Loop on digis
  edm::DetSetVector<CTPPSPixelDigi>::const_iterator digiDSViter = digis->begin();
  for (; digiDSViter != digis->end(); digiDSViter++) {
    CTPPSPixelDetId detIdObject(digiDSViter->detId());
    edm::DetSet<CTPPSPixelDigi>::const_iterator begin = (*digiDSViter).begin();
    edm::DetSet<CTPPSPixelDigi>::const_iterator end = (*digiDSViter).end();
    for (edm::DetSet<CTPPSPixelDigi>::const_iterator di = begin; di != end; di++) {
      // Detector ID
      _arm_digi.push_back(detIdObject.arm());
      _station_digi.push_back(detIdObject.station());
      _rp_digi.push_back(detIdObject.rp());
      _plane_digi.push_back(detIdObject.plane());
      // Pixel data
      _row.push_back(di->row());
      _column.push_back(di->column());
      _adc.push_back(di->adc());
    }
  }

 */
}


#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(CTPPSPixelTrackAnalyzer);
