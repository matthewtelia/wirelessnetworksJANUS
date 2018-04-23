/*
 * AP.h
 *
 *  Created on: Apr 9, 2018
 *      Author: matt
 */

#ifndef AP_H_
#define AP_H_

#include <iostream>
#include <algorithm>
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
        ,REQUEST_INFO = 2
        ,SCHEDULER = 3
        ,REQUEST_ACK = 4
        ,ACK_FLAG = 5
        ,DATA_PACKET = 6
        ,REGISTER_NODE = 7
        ,REQUEST_FLAG = 8
        ,RRI = 9};

    cMessage *initialProbeRequest;
    cMessage *probeRequest;
    cMessage *requestInfo;
    cMessage *scheduler;
    cMessage *requestAck;
    cMessage *ackFlag;
    cMessage *dataPackets;


    double Tshare;
    int round; 
    int numberOfSlots=5;
    int numNodes;
    int nodeID;
    double time0;
    double time1;
    double conflictMap[][5];
    double rateMatrix[][5];
    double timeIncrement;
    double signalStrength;
    double scheduleSendTimes[5] = {0};
    int P[2][20];
    int packetLengths[5] = {0};
    double Tqueue;
    double Tdeficit;
    double interference[5] = {0};
    int prevI;
    double prevTdeficit;
    int numPacketsArray[2] = {0};
    int sendOrder[2] = {0};
    int schedulerArray[2] = {0};
    //int sendOrder[2] = {0};

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg) override;
    virtual void schedule(int packetLengths[], double interference[], int nodeID);
    virtual void registration();
    //virtual void schedulePackets();
    virtual void transmitPoll(bool willSend, int nodeID);
    virtual double updatedeficit(double Tdeficit, double Tshare , double Tqueue);
    virtual int generateDataPacket();
    virtual int randomPacketLength();
    virtual int measureQueue(int packetLengths[]);
    virtual void RTA(double interference[], int nodeID, int numNodes);
    virtual int LCU(int packetLengths[], int nodeID);
    virtual void finish();
};




#endif /* AP_H_ */
