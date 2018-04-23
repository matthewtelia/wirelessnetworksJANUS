/*
ensorNode.cc
 *
 *  Created on: Apr 9, 2018
 *      Author: matt
 */
#include "WirelessChannelWater.h"

Define_Module(WirelessChannelWater);

void WirelessChannelWater::initialize()
{
    cModule* c = getModuleByPath("BaseNetwork");

    //Message Types
    probeRequest = new cMessage("probeRequest", PROBE_REQUEST);
    requestInfo = new cMessage("requestInfo", REQUEST_INFO);
    scheduler = new cMessage("scheduler", SCHEDULER);
    requestAck = new cMessage("requestAck", REQUEST_ACK);

    registerNode = new cMessage("registerNode", REGISTER_NODE);
    requestFlag = new cMessage("requestFlag", REQUEST_FLAGS);
    dataPacket = new cMessage("dataPacket", DATA_PACKET);
    ackFlag = new cMessage("ackFlag", ACK_FLAG);

}

void WirelessChannelWater::handleMessage(cMessage *msg)
{
    cModule* c = getModuleByPath("BaseNetwork");
    EV << "Message:" << msg -> getKind() << endl;

    switch(msg -> getKind())
    {
        case REGISTER_NODE:
        {
            cMessage *registerNode = new cMessage("registerNode");
            send(registerNode, "out");
        }

        case REQUEST_FLAGS:
        {
            cMessage *requestFlag = new cMessage("requestFlag");
            send(requestFlag, "out");
        }
        case DATA_PACKET:
        {
            cMessage *dataPacket = new cMessage("dataPacket");
            send(dataPacket, "out");
        }
        case ACK_FLAG:
        {
            cMessage *ackFlag = new cMessage("ackFlag");
            send(ackFlag, "out");
        }
        case PROBE_REQUEST:
        {
            cMessage *probeRequest = new cMessage("probeRequest");
            send(probeRequest, "out");
        }
        case REQUEST_INFO:
        {
            cMessage *requestInfo = new cMessage("requestInfo");
            send(requestInfo, "out");
        }
        case SCHEDULER:
        {
            cMessage *scheduler = new cMessage("scheduler");
            send(scheduler, "out");
        }
        case REQUEST_ACK:
        {
            cMessage *requestAck = new cMessage("requestAck");
            send(requestAck, "out");
        }

  }
}

void WirelessChannelWater::finish()
{

}




