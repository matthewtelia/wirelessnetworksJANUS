/*
 * SensorNode.cc
 *
 *  Created on: Apr 9, 2018
 *      Author: matt
 */
#include "SensorNode.h"

Define_Module(SensorNode);

void SensorNode::initialize()
{
    cModule *c = getModuleByPath("BaseNetwork");
    
    prob = par("prob");		//probability that sensor will transmit data
    distance = par("distance");
    nodeID = par("nodeID");
    timeIncrement = par("timeIncrement");
    signalStrength = 0; //par("signalStrength");
    slot = 0;
    Tdeficit = 0;
    prevTdeficit = 0;
    prevI = 1;



    //Message Types
    registerNode = new cMessage("registerNode", REGISTER_NODE);
    requestFlag = new cMessage("requestFlag", REQUEST_FLAG);
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
            double Tshare = msg -> par("Tshare");

            int slotOrder[5] = {0};
            //slotOrder[5] = msg -> par("slotOrder");
            //slot = slotOrder[nodeID];
            slot = msg -> par("slotOrder");
            EV<<"SLOT ORDER: "<< slot<<endl;
            double timeToSend = slot * timeIncrement;

            //Probe request asks sensor if wants to send information. sensor either replies with request flag or stays silent
            willSend = randomDataTransmit();
            //EV<<"willSend = "<<willSend<<endl;
            //nodeID = //create nodeID object
            //cMessage *requestFlag = new cMessage("requestFlag", REQUEST_FLAG);
            requestFlag -> addPar("willSend");
            requestFlag -> addPar("nodeID");
            requestFlag -> par("nodeID") = nodeID;
            requestFlag -> par("willSend") = willSend;
            sendDelayed(requestFlag, simTime() + timeToSend, "out", 0);
            break;
        }

        case REQUEST_INFO:
        { 
            //cMessage *replyRequestInfo1 = new cMessage("replyRequestInfo1", RRI);
            //cMessage *replyRequestInfo2 = new cMessage("replyRequestInfo2", RRI);

            int transmitOrder;
            transmitOrder = msg -> par("transmitOrder");
            //double APSignalStrength = msg -> par(signalStrength);
            //packetLength = randomPacketLength();	//creates packetLength object
            //interferenceInfo = getInterference();	//creates interferenceInfo oject

            for(int i=0; i<2; i++)
            {
                packetLengths[i] = generateDataPacket();
            }

            Tqueue = measureQueue(packetLengths);
            Tdeficit = updatedeficit(Tdeficit,Tshare,Tqueue);

            replyRequestInfo -> addPar("nodeID") = nodeID;
            replyRequestInfo -> addPar("signalStrength") = signalStrength;
            replyRequestInfo -> addPar("packetLengths0") = packetLengths[0];
            replyRequestInfo -> addPar("packetLengths1") = packetLengths[1];

            replyRequestInfo -> addPar("interferenceArray0") = interferenceArray[0];
            replyRequestInfo -> addPar("interferenceArray1") = interferenceArray[1];
            //replyRequestInfo2 -> addPar("nodeID") = nodeID;
            //replyRequestInfo2 -> addPar("signalStrength") = signalStrength;
            //replyRequestInfo2 -> addPar("packetLengths0") = packetLengths[0];
            //replyRequestInfo2 -> addPar("packetLengths1") = packetLengths[1];

            //replyRequestInfo2 -> addPar("interferenceArray0") = interferenceArray[0];
            //replyRequestInfo2 -> addPar("interferenceArray1") = interferenceArray[1];

            double timeToSend = timeIncrement * slot;
            sendDelayed(replyRequestInfo, simTime() + timeToSend, "out", 0);
            //sendDelayed(replyRequestInfo2, simTime() + timeToSend + e,"out", 1);

            EV << "Time Delayed" << timeToSend << endl;
            EV<<"GOT HERE"<<endl;
            //EV << "Message Kind: " << msg -> getKind() << endl; //DEBUG
            break;
        }
        
        case RRI:
        {
            EV<<"GOT HERE DEBUG SENSOR"<<endl;
            int otherNodeID = msg -> par("nodeID");
            double otherSensorSignalStrength = msg -> par("signalStrength");
            interferenceArray[otherNodeID] = otherSensorSignalStrength;
            break;
        }

        case SCHEDULER:
        {
            //getTransmitTime(msgInfo);
            int schedule[numNodes] = {0};
            schedule[0] = msg -> par("schedule0");
            schedule[1] = msg -> par("schedule1");
            schedule[2] = msg -> par("schedule2");
            
            slotTime = schedule[nodeID];

            cMessage *dataPacket = new cMessage("dataPacket", DATA_PACKET);
            sendDelayed(dataPacket, simTime() + slotTime, "out", 0);
            break;

        }
        case REQUEST_ACK:
        {
            //getAck(msgInfo);
            if (willSendAck == true)
                {
                    cMessage *ackFlag = new cMessage("ackFlag", ACK_FLAG);
                    //ackFlag -> addObject(nodeID);
                    send(ackFlag, "out",0);
                    break;
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
    std::bernoulli_distribution random_bool_generator(prob);
    
    return random_bool_generator(rand_engine);

}

bool SensorNode::getAck(int *requestAckNodes)
{
    //determines if node will send ack flag based on request ack info
    
    int length = sizeof(requestAckNodes)/sizeof(*requestAckNodes);
    for (int i = 0; 0<i<length; i++)
        {
            if (requestAckNodes[i] == nodeID)
                {
                    willSendAck = true;
                    break;
                }
        }

    willSendAck = false;
    return willSendAck;
}

//void SensorNode::getTransmitTime(int *schedule)
//{
    //reads schedule array from shceduler packets and determines what time node transmits
    //(rand()%10)+1;
//    slotTime = 0;

//}

int SensorNode::randomPacketLength()
{
    //randomly assigns packet length
    //return (rand()%10)+1;
    return 4;

}

void SensorNode::determineInterference()
{
    //determine the signal to noise ratio of the node
    distance = pow(sqrt(xAP - xNode),2) + pow((yAP - yNode),2); // determining distance between nodes and AP
    Psignal = distance * originalSignal; //multiplying original signal
    SNR = Psignal/Pnoise;
}

//void SensorNode::determinePacketLength()
//{
//
//}

int SensorNode::getInterference()
{
    return 0;
}

double SensorNode::updatedeficit(double Tdeficit, double Tshare , double Tqueue)
{
    Tdeficit = Tshare + prevTdeficit * prevI;
    prevTdeficit = Tdeficit;
    if(Tqueue = 0)
    {
        prevI = 0;
    }
    else
    {
        prevI = 1;
    }

 return Tdeficit;
}

int SensorNode::generateDataPacket()
{

    int numPackets = (rand()%2)+1;
    EV<<"Number of packets = "<<numPackets<<endl;
    return numPackets;
    /*
    packetLengths[numPackets] = {0};
    for (int i = 0; i <= numPackets; i++)
    {
        packetLengths[i] = randomPacketLength();
    }

    return packetLengths[5];
    */
}

int SensorNode::measureQueue(int packetLengths[])
{
    int numBytes = 0;
    int numPackets;
    numPackets = sizeof(packetLengths)/sizeof(*packetLengths);

    for (int i = 0; i < numPackets; i++)
    {
        numBytes = numBytes + packetLengths[i];
    }

    return numBytes;
}

void SensorNode::finish()
{

}


