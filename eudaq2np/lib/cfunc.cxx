
#include <stdio.h>
#include "eudaq/FileReader.hh"
#include "eudaq/PluginManager.hh"
#include "eudaq/OptionParser.hh"
#include "eudaq/StandardEvent.hh"

#include <iostream>
#include <vector>
#ifndef WIN32
#include <inttypes.h> 
#endif

#include "cfunc.h"

using namespace eudaq;


void eudaq_data_vector(const std::string & filename, const std::string & type, std::vector<data_row> & data)
{
// Create a reader for this iframele
  eudaq::FileReader reader(filename);

  // Display the actual iframelename (argument could have been a run number)
  //std::cout << "Opened file: " << reader.Filename() << std::endl;

  while (reader.NextEvent()) {
      try {
        //const eudaq::RawDataEvent & rev = reader.GetDetectorEvent().GetRawSubEvent(type);
        //std::cout << rev << std::endl;

        eudaq::StandardEvent sev =  eudaq::PluginManager::ConvertToStandard(reader.GetDetectorEvent());
        //std::cout << sev << std::endl;
      
        for(unsigned int iplane = 0; iplane < sev.NumPlanes(); iplane++){
          const StandardPlane & plane = sev.GetPlane(iplane);
            
            for(unsigned int iframe = 0; iframe < plane.NumFrames(); iframe++){
                   
              const std::vector<double> & xv = plane.XVector(iframe);
              const std::vector<double> & yv = plane.YVector(iframe);
              const std::vector<double> & pix = plane.PixVector(iframe);
              
              for(int ipix = 0; ipix < pix.size(); ipix++){
                    //std::cout << plane.TLUEvent() << " " << iplane << " "<< iframe << " " << xv[ipix] << " " << yv[ipix] << " "<< pix[ipix] << "" << std::endl;
                    data_row data_pix = {plane.TLUEvent(), iplane, iframe, plane.ID(), xv[ipix],  yv[ipix], pix[ipix]};
                    data.push_back(data_pix);
              }                     
            }

        }
      } catch (const eudaq::Exception & e) {
        //std::cout << "No " << op.GetArg(0) << " subevent in event "  << reader.GetDetectorEvent().GetEventNumber() << std::endl;
      }    

    }
}
