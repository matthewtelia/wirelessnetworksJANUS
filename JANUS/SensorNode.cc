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
    
    prob = par("prob");		//probability that sensor will transmit data    

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
    //msgInfo = msg -> getObject();

    switch(msg -> getKind())
    {
        case PROBE_REQUEST:
        {
            //Probe request asks sensor if wants to send information. sensor either replies with request flag or stays silent
            willSend = randomDataTransmit();
            //nodeID = //create nodeID object
            //cMessage *requestFlag = new cMessage("requestFlag", REQUEST_FLAG);
            //requestFlag -> addObject(nodeID);
            send(requestFlag, "out");
        }

        case REQUEST_INFO:
        { 
            packetLength = randomPacketLength();	//creates packetLength object
            interferenceInfo = getInterference();	//creates interferenceInfo oject 
            cMessage *replyRequestInfo = new cMessage("replyRequestInfo", RRI);
            //replyRequestInfo -> addObject(packetLength);
            //replyRequestInfo -> addObject(interferenceInfo);
            send(replyRequestInfo, "out");
        }
        
        case SCHEDULER:
        {
            
            getTransmitTime(msgInfo);
            cMessage *dataPacket = new cMessage("dataPacket", DATA_PACKET);
            sendDelayed(dataPacket, simTime() + slotTime, "out");

        }
        case REQUEST_ACK:
        {
            getAck(msgInfo);
            if (willSendAck == true)
                {
                    cMessage *ackFlag = new cMessage("ackFlag", ACK_FLAG);
                    //ackFlag -> addObject(nodeID);
                    send(ackFlag, "out");
                }
            else
                {
                    break;
                }
        }


        default:
        {
            break;
        }
    }
}

bool SensorNode::randomDataTransmit()
{
    //randomly determines whether or not sensor will transmit data with probability prob
    std::knuth_b rand_engine;
    std::bernoulli_distribution random_bool_generator(0.5);
    
    return random_bool_generator(rand_engine);

}

bool SensorNode::getAck(int requestAckNodes)
{
    //determines if node will send ack flag based on request ack info
    
    length = sizeof(requestAckNodes)/sizeof(*requestAckNodes);
    for (int i = 0; 0<i<length; i++)
        {
            if (requestAckNodes[i] == nodeID)
                {
                    willSendAck = true;
                    break;
                }
        }

    willSendAck = false;
 
}

void SensorNode::getTransmitTime(int schedule)
{
    //reads schedule array from shceduler packets and determines what time node transmits
    
    slotTime = 0;

}

int SensorNode::randomPacketLength()
{
    //randomly assigns packet length
    return (rand()%10)+1;

}

void SensorNode::determineInterference()
{
    //determine the signal to noise ratio of the node
    distance = sqrt((xAP - xNode)^2 + (yAP - yNode)^2); // determining distance between nodes and AP
    Psignal = distance * orignalSignal; //multiplying original signal
    SNR = Psignal/Pnoise;
}

void SensorNode::determinePacketLength()
{

}

