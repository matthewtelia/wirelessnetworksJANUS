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
    enum{
        REGISTER_NODE = 7
        ,REQUEST_FLAG = 8
        ,RRI = 9
        ,DATA_PACKET = 6
        ,ACK_FLAG = 5
        ,PROBE_REQUEST = 1
        ,REQUEST_ACK = 4
        ,REQUEST_INFO = 2
        ,SCHEDULER = 3
        ,INIT = 0
        };

    cMessage *registerNode;
    cMessage *requestFlag;
    cMessage *replyRequestInfo;
    cMessage *dataPacket;
    cMessage *ackFlag;

    cObject *msgInfo;

    bool willSend;  //determines if node will send data that round
    double prob;    //probability node will send data
    int slotTime;   //time data is transmitted after schedule packet recieved
    int packetLengths[];    //number of packets to send
    //int interferenceInfo;
    //int interference [];
    bool willSendAck;
    //double deficit;
    int originalSignal;
    //int length;
    int nodeID;
    int numNodes = 2;
    double e = 0.000001;
    double timeIncrement;
    double signalStrength;
    double interferenceArray[5] = {1};
    int slot;
    double Tdeficit;
    double Tqueue;
    double Tshare;
    double prevTdeficit;
    int prevI;

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg) override;
    virtual void determineInterference();
    //virtual void determinePacketLength();
    virtual bool randomDataTransmit();
    //virtual void getTransmitTime(int *schedule);
    virtual int getInterference();
    //virtual void determineInterference(double distance,double xNode, double yNode);
    virtual double updatedeficit(double Tdeficit, double Tshare, double Tqueue);
    virtual int generateDataPacket();
    virtual int measureQueue(int packetLengths[]);
    // void determinePacketLength();
    virtual int randomPacketLength();
    virtual bool getAck(int *requestAckNodes);
    virtual void finish();
};


#endif /* SENSORNODE_H_ */
