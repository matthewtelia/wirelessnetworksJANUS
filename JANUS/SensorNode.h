/*
 * SensorNode.h
 *
 *  Created on: Apr 9, 2018
 *      Author: matt
 */

#ifndef SENSORNODE_H_
#define SENSORNODE_H_

#include <omnetpp.h>
#include <random>
#include <cstdlib>

using namespace omnetpp;

class SensorNode : public cSimpleModule
{
public:
    double distance;
    double SNR;
    double Pnoise;
    double Psignal;
    double xNode;
    double yNode;
    double xAP;
    double yAP;

private:
    enum{REGISTER_NODE,REQUEST_FLAGS,RRI,DATA_PACKET,ACK_FLAG};

    cMessage *registerNode;
    cMessage *requestFlag;
    cMessage *replyRequestInfo;
    cMessage *dataPacket;
    cMessage *ackFlag;

    bool willSend;
    int prob;
    int slotTime;
    int packetLength;
    int interference []; 
    bool willSendAck;
    double deficit;   

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg) override;
    virtual void determineInterference();
    virtual void determinePacketLength();
    virtual bool randomDataTransmit();
    virtual void getTransmitTime(int schedule[]);
    virtual void determineInterference(double distance,double xNode, double yNode);
    virtual void determinePacketLength();
    virtual void getAck(int requestAckNodes[]);
    virtual void finish();
};


#endif /* SENSORNODE_H_ */
