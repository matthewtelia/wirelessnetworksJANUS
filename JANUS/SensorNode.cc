/*
 * SensorNode.cc
 *
 *  Created on: Apr 9, 2018
 *      Author: matt
 */
#include "SensorNode.h"

void SensorNode::initialize()
{
    cModule* c = getModuleByPath("BaseNetwork");

    //Message Types
    registerNode = new cMessage("registerNode", REGISTER_NODE);
    requestFlag = new cMessage("requestFlag", REQUEST_FLAGS);
    dataPacket = new cMessage("dataPacket", DATA_PACKET);
    ackFlag = new cMessage("ackFlag", ACK_FLAG);

}

void SensorNode::handleMessage(cMessage *msg)
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
            cMessage *requestFlags = new cMessage("requestFlags");
            send(requestFlags, "out");
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

        default:
        {
            break;
        }
    }
}



