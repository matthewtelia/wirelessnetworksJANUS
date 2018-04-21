/*
 * AP.h
 *
 *  Created on: Apr 9, 2018
 *      Author: matt
 */

#ifndef AP_H_
#define AP_H_

#include <omnetpp.h>

using namespace omnetpp;

class AP : public cSimpleModule
{
public:
    int slotOrder[5]={0};
    int transmitOrder[5];
private:
    enum{INIT=0
        ,PROBE_REQUEST = 1
        ,REQUEST_INFO = 1
        ,SCHEDULER = 2
        ,REQUEST_ACK = 3
        ,ACK_FLAG = 4
        ,DATA_PACKET = 5
        ,REGISTER_NODE = 0
        ,REQUEST_FLAG = 7
        ,RRI = 8};

    cMessage *initialProbeRequest;
    cMessage *probeRequest;
    cMessage *requestInfo;
    cMessage *scheduler;
    cMessage *requestAck;
    cMessage *ackFlag;
    cMessage *dataPackets;


    double Tshare[2];
    int round; 
    int numberOfSlots=5;
    int numNodes;
    double time0;
    double time1;
    double conflictMap[][5];
    double rateMatrix[][5];
    double timeIncrement;
    double signalStrength;
    double scheduleSendTimes[5] = {0};

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg) override;
    virtual void schedule(cMessage *msg);
    virtual void registration();
    virtual void schedulePackets();
    virtual void transmitPoll(bool willSend, int nodeID);
    virtual void finish();
};




#endif /* AP_H_ */
