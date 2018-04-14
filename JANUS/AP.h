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
    enum{PROBE_REQUEST,REQUEST_INFO,SCHEDULER,REQUEST_ACK};
    cMessage *probeRequest;
    cMessage *requestInfo;
    cMessage *scheduler;
    cMessage *requestAck;
    

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg) override;
    virtual void schedule(cMessage *msg);
    virtual void registration();
    virtual void schedulePackets();
    virtual void finish();
};




#endif /* AP_H_ */
