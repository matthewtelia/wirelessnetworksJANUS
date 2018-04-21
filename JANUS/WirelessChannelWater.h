/*
 * WirelessChannel.h
 *
 *  Created on: Apr 9, 2018
 *      Author: matt
 */

#ifndef WIRELESSCHANNELWATER_H_
#define WIRELESSCHANNELWATER_H_

#include <omnetpp.h>

using namespace omnetpp;

class WirelessChannelWater : public cSimpleModule
{
    private:
        enum{REGISTER_NODE,REQUEST_FLAGS,RRI,DATA_PACKET,ACK_FLAG, PROBE_REQUEST, REQUEST_INFO, SCHEDULER, REQUEST_ACK};

        cMessage *registerNode;
        cMessage *requestFlag;
        cMessage *dataPacket;
        cMessage *ackFlag;
        cMessage *probeRequest;
        cMessage *requestInfo;
        cMessage *scheduler;
        cMessage *requestAck;

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg) override;
        virtual void finish();
};



#endif /* WIRELESSCHANNELWATER_H_ */
