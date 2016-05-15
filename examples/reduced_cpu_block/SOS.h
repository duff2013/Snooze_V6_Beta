//
//  SOS.h
//  Teensyduino_128_rc1
//
//  Created by Colin on 5/13/16.
//  Copyright (c) 2016 Colin Duffy. All rights reserved.
//

#ifndef SOS_h
#define SOS_h

#include "Arduino.h"
#include "SnoozeBlock.h"

class SOS : public SnoozeBlock {
private:
    virtual void disableDriver( void );
    virtual void enableDriver( void );
public:
    SOS(void) {
        isDriver = true;
    }

};

#endif /* defined(SOS_h) */
