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
#include <cmath>
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
    enum{REGISTER_NODE = 0
        ,REQUEST_FLAGS = 1
        ,RRI = 2
        ,DATA_PACKET = 3
        ,ACK_FLAG = 4
        ,PROBE_REQUEST = 1
        ,REQUEST_ACK = 6
        ,REQUEST_INFO = 7
        ,SCHEDULER = 8
        };

    cMessage *registerNode;
    cMessage *requestFlag;
    cMessage *replyRequestInfo;
    cMessage *dataPacket;
    cMessage *ackFlag;

    cObject *msgInfo;

    bool willSend;
    double prob;
    int slotTime;
    int packetLengths[];
    int interferenceInfo;
    int interference []; 
    bool willSendAck;
    double deficit;
    int originalSignal;
    int length;
    int nodeID;
    double timeIncrement;
    double signalStrength;
    double interferenceArray[5] = {1};
    int slot;
    double Tdeficit;
    double prevTdeficit;
    int prevI;

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg) override;
    virtual void determineInterference();
    virtual void determinePacketLength();
    virtual bool randomDataTransmit();
    virtual void getTransmitTime(int *schedule);
    virtual int getInterference();
    virtual void determineInterference(double distance,double xNode, double yNode);
    virtual double updateTdeficit(double Tdeficit, double Tshare, double Tqueue);
    virtual int generateDataPacket();
    virtual int measureQueue(int packetLengths[5]);
    // void determinePacketLength();
    virtual int randomPacketLength();
    virtual bool getAck(int *requestAckNodes);
    virtual void finish();
};


#endif /* SENSORNODE_H_ */
