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
            double Tshare = msg -> par("Tshare"); //pulling Tshare value from probe request message

            // NOT USED ANYMORE
            //int slotOrder[5] = {0};
            //int slotOrder = msg -> par("slotOrder");
            //slot = slotOrder[nodeID];

            slot = msg -> par("slotOrder");

            EV<<"SLOT ORDER: "<< slot << endl; //print slot order

            double timeToSend = slot * timeIncrement; //send delay of next message

            //Probe request asks sensor if wants to send information. sensor either replies with request flag or stays silent
            willSend = randomDataTransmit();
            EV << "willSend = "<< willSend << endl;

            //adding parameters to request flag
            requestFlag -> addPar("willSend");
            requestFlag -> addPar("nodeID");
            requestFlag -> par("nodeID") = nodeID;
            requestFlag -> par("willSend") = willSend;

            //sending request flag
            sendDelayed(requestFlag, simTime() + timeToSend, "out", 0);


            break;
        }

        case REQUEST_INFO:
        { 

            EV << "RI Recieved" << endl;

            //Pulling transmitOrder data from messge
            int transmitOrder1;
            transmitOrder1 = msg -> par("transmitOrder1");
            int transmitOrder2;
            transmitOrder2 = msg -> par("transmitOrder2");

            double APSignalStrength;
            APSignalStrength = msg -> par(signalStrength);

            int packetLength;
            packetLength = generateDataPacket();	//creates number of packets to send

            int interferenceInfo1 = getInterference();	//creates interferenceInfo object
            int interferenceInfo2 = 2;

            //fills packet lengths into array
            for(int i=0; i < packetLength; i++)
            {
                packetLengths[i] = randomPacketLength();
            }


            Tqueue = measureQueue(packetLengths);               //adding up packet lengths
            Tdeficit = updatedeficit(Tdeficit,Tshare,Tqueue);   //updates Tdeficit

            //Add parameters to RRI

            replyRequestInfo -> addPar("nodeID");
            replyRequestInfo -> par("nodeID")= nodeID;

            replyRequestInfo -> addPar("signalStrength");
            replyRequestInfo -> par("signalStrength")= signalStrength;

            replyRequestInfo -> addPar("packetLengths0");
            replyRequestInfo -> par("packetLengths0")= packetLengths[0];

            replyRequestInfo -> addPar("packetLengths1");
            replyRequestInfo -> par("packetLengths1")= packetLengths[1];

            replyRequestInfo -> addPar("interferenceInfo1");
            replyRequestInfo-> par("interferenceInfo1")= interferenceInfo1;

            replyRequestInfo -> addPar("interferenceInfo2");
            replyRequestInfo-> par("interferenceInfo2")= interferenceInfo2;

            //calculate delay send time
            double timeToSend = timeIncrement * slot;
            EV << "Time Delayed" << timeToSend << endl;

            //Send replyRequestInfo Packet
            sendDelayed(replyRequestInfo, simTime() + timeToSend, "out", 0);


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
            cMessage *dataPacket = new cMessage("dataPacket", DATA_PACKET);

            dataPacket -> addPar("nodeID");
            dataPacket -> par("nodeID") = nodeID;

            int schedule[numNodes];
            schedule[0] = msg -> par("schedule0");
            schedule[1] = msg -> par("schedule1");
            
            //pulling correct slot time to send out of schedule array
            slotTime = schedule[nodeID - 1];

            EV << "Time from schedule: " << slotTime << endl;

            //Sending data packet back to AP
            sendDelayed(dataPacket, simTime() + slotTime, "out", 0);


            break;

        }
        case REQUEST_ACK:
        {
            //getAck(msgInfo);
            willSendAck = true;
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
    int i = (rand()%2);
    if(i == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
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

    return 4; // number of bytes

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
    return 1;
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
    //numPackets = sizeof(packetLengths)/sizeof(*packetLengths);

    for (int i = 0; i < 2; i++)
    {
        numBytes = numBytes + packetLengths[i];
    }

    return numBytes;
}

void SensorNode::finish()
{

}


