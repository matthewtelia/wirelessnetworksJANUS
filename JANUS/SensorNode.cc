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
    replyRequestInfo = new cMessage("replyRequestInfo", RRI);
    dataPacket = new cMessage("dataPacket", DATA_PACKET);
    ackFlag = new cMessage("ackFlag", ACK_FLAG);

}

void SensorNode::handleMessage(cMessage *msg)
{
    cModule* c = getModuleByPath("BaseNetwork");
    EV << "Message:" << msg -> getKind() << endl;

    switch(msg -> getKind())
    {
        case PROBE_REQUEST:
        {
            cMessage *registerNode = new cMessage("registerNode");
            send(registerNode, "out");
        }

        case REQUEST_INFO:
        {
            cMessage *requestFlags = new cMessage("requestFlags");
            send(requestFlags, "out");
        }
        
        case SCHEDULER:
        {
            cMessage *replyRequestInfo = new cMessage("replyRequestInfo");
            
            replyRequestInfo -> addObject(interferenceInfo);
            replyRequestInfor -> addObject(packetLength);
            send(replyRequestInfo, "out");
        }
        case REQUEST_ACK:
        {
            cMessage *dataPacket = new cMessage("dataPacket");
            send(dataPacket, "out");
        }


        default:
        {
            break;
        }
    }
}

void SensorNode::determineInterference()
{

}

void SensorNode::determinePacketLength()
{

}

