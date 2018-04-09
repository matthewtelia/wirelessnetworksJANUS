/*
 * SensorNode.h
 *
 *  Created on: Apr 9, 2018
 *      Author: matt
 */

#ifndef SENSORNODE_H_
#define SENSORNODE_H_

#include <omnetpp.h>

using namespace omnetpp;

class SensorNode : public cSimpleModule
{
private:
    enum{REGISTER_NODE,REQUEST_FLAGS,RRI,DATA_PACKET,ACK_FLAG};

    cMessage *registerNode;
    cMessage *requestFlag;
    cMessage *dataPacket;
    cMessage *ackFlag;

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish();
};


#endif /* SENSORNODE_H_ */
