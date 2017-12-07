#ifndef SimCTPPS_CTPPSPixelDigiProducer_RPix_PILE_UP_SIGNALS_H
#define SimCTPPS_CTPPSPixelDigiProducer_RPix_PILE_UP_SIGNALS_H

#include <map>
#include "FWCore/ParameterSet/interface/ParameterSet.h"
//#include "SimTotem/RPDigiProducer/interface/RPSimTypes.h"



class RPixPileUpSignals
{
public:
  RPixPileUpSignals(const edm::ParameterSet & params, uint32_t det_id);
  void reset();
  void add(const std::map<unsigned short, double, std::less<unsigned short> > &charge_induced, int PSimHitIndex);
  inline const std::map<unsigned short, double, std::less<unsigned short> > &dumpSignal()
  {return the_pixel_charge_piled_up_map_;}
  inline std::map<unsigned short, std::vector< std::pair<int, double> > >  &dumpLinks() // for each pixel the indeces of PSimHit and amounts of charge generated by it is given
  {return the_pixel_charge_piled_up_map_links_;}
private:
  std::map<unsigned short, double, std::less<unsigned short> > the_pixel_charge_piled_up_map_;
  std::map<unsigned short, std::vector< std::pair<int, double> > >  the_pixel_charge_piled_up_map_links_;
  bool links_persistence_;
  uint32_t det_id_;
  bool verbosity_;
};

#endif  

