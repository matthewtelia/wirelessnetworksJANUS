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
    int slotOrder[];
    int transmitOrder[];
private:
    enum{PROBE_REQUEST = 0
        ,REQUEST_INFO = 1
        ,SCHEDULER = 2
        ,REQUEST_ACK = 3
        ,ACK_FLAG = 4
        ,DATA_PACKET = 5
        ,REGISTER_NODE = 6
        ,REQUEST_FLAG = 7
        ,RRI = 8};

    cMessage *probeRequest;
    cMessage *requestInfo;
    cMessage *scheduler;
    cMessage *requestAck;
    cMessage *ackFlag;
    cMessage *dataPackets;


    double Tshare1;
    double Tshare2;
    int round; 
    int numNodes;
    double time0;
    double time1;
    double conflictMap[][5];
    double rateMatrix[][5];

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg) override;
    virtual void schedule(cMessage *msg);
    virtual void registration();
    virtual void schedulePackets();
    virtual void transmitPoll();
    virtual void finish();
};




#endif /* AP_H_ */
