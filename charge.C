{
  TFile *f=new TFile("simevent_CTPPS_CLU__REC_TRA_DB_mem_ALL_1900_fullCH_135.root");
  Events->Draw("CTPPSPixelClusteredmDetSetVector_ctppsPixelClusters__CTPPS2.obj._sets.data.charge()","CTPPSPixelClusteredmDetSetVector_ctppsPixelClusters__CTPPS2.obj._sets.data.charge()<100000");

  TFile *g=new TFile("simevent_CTPPS_CLU__REC_TRA_DB_mem_ALL_1900_missingCH_135.root");
  Events->Draw("CTPPSPixelClusteredmDetSetVector_ctppsPixelClusters__CTPPS2.obj._sets.data.charge()","CTPPSPixelClusteredmDetSetVector_ctppsPixelClusters__CTPPS2.obj._sets.data.charge()<100000","SAME");

  TFile *h=new TFile("/eos/user/f/ferro/fabferro/_304292/CTPPSPixelReco_304292_zb_6.root");

  Events->Draw("CTPPSPixelClusteredmDetSetVector_ctppsPixelClusters__RECO.obj._sets.data.charge()","61000/2884000*(CTPPSPixelClusteredmDetSetVector_ctppsPixelClusters__RECO.obj._sets.data.charge()<100000)","SAME");

}
