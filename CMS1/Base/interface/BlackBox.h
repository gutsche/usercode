#ifndef CMS1BlackBox_h
#define CMS1BlackBox_h

//
// Package:         CMS1/Base
// Class:           BlackBox
// 
// Description:     common functions and data members for various Black Boxes
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/03/16 07:00:21 $
// $Revision: 1.1 $
//

#include "CMS1/Base/interface/EventData.h"

namespace cms1 {
   class BlackBox  {
    public:
      BlackBox():data_(0) {}
      
      // We need to have a pointer to the event data that we might want to use.
      // This is done by setting the data_ private member at the beginning of 
      // the analysis for each event.
      // The black box is allowed to add new information to the EventData.
      void setEventData( EventData* data ) { data_ = data; } 
      EventData* getEventData() { return data_; } 

    protected:
      EventData* data_;
  };
}
#endif
