/*
 * WirelessChannel.h
 *
 *  Created on: Apr 9, 2018
 *      Author: matt
 */

#ifndef WIRELESSCHANNEL_H_
#define WIRELESSCHANNEL_H_

#include <omnetpp.h>

using namespace omnetpp;

class WirelessChannel : public cSimpleModule
{
    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg) override;
        virtual void finish();
};



#endif /* WIRELESSCHANNEL_H_ */
