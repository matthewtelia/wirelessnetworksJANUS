/*
 * AP.cc
 *
 *  Created on: Apr 9, 2018
 *      Author: matt
 */
#include "AP.h"

Define_Module(AP);

void AP::initialize()
{
    cModule* c = getModuleByPath("BaseNetwork");

    //Message Types
    initialProbeRequest = new cMessage("initializeProbeRequest", INIT);
    probeRequest = new cMessage("probeRequest", PROBE_REQUEST);
    requestInfo = new cMessage("requestInfo",REQUEST_INFO);
    scheduler = new cMessage("scheduler",SCHEDULER);
    requestAck = new cMessage("requestAck",REQUEST_ACK);

    round = 0;


    //Tshare[0] = .003;
    //Tshare[1] = 0.006;
    //Tshare[2] = 0.006;

    //slotOrder[1] = 1;in
    //slotOrder[2] = 2;

    //timeIncrement = par("timeIncrement");
    timeIncrement = 0.1;
    signalStrength = par("signalStrength");

    //numNodes = par("numNodes");
    numNodes = 2;
    receivedDataPackets = 0;
    expectedDataPackets = 1;
    signalStrength = 1;

    nodeID = 1;
    //initilize conflict map matrix numNodes x numNodes

    conflictMap[numNodes][numNodes] = {0};
    //initialize rate matrix numNodes x numnodes
    rateMatrix[numNodes][numNodes];


    scheduleAt(simTime(), initialProbeRequest);

}

void AP::handleMessage(cMessage *msg)
{
    cModule* c = getModuleByPath("BaseNetwork");
    EV << "Message Kind: " << msg -> getKind() << endl;

    //cMessage *ptr = check_and_cast<cMessage *>(msg);
    switch(msg -> getKind())
    {//kind=0
        case INIT:
        {
            cMessage *probeRequest1 = new cMessage("probeRequest1", PROBE_REQUEST);
            cMessage *probeRequest2 = new cMessage("probeRequest2", PROBE_REQUEST);
            EV<<"GOT TO CASE INIT"<< endl;

            //sends probe request showing which request flag slots are used
            //cMessage *probeRequest = new cMessage("probeRequest", PROBE_REQUEST);
            //registration();

            probeRequest1 -> addPar("slotOrder");
            probeRequest1 -> addPar("Tshare");
            //probeRequest -> par("slotOrder") = slotOrder;
            probeRequest1 -> par("slotOrder") = 1;
            probeRequest1 -> par("Tshare") = Tshare;
            probeRequest2 -> addPar("slotOrder");
            probeRequest2 -> addPar("Tshare");
            //probeRequest -> par("slotOrder") = slotOrder;
            probeRequest2 -> par("slotOrder") = 1;
            probeRequest2 -> par("Tshare") = Tshare;
            round = round + 1;
            if (round >= 3)
            {
                finish();
            }
            send(probeRequest1, "out", 0);
            //send(probeRequest2, "out", 1);
            //send(probeRequest, "out2", 0);

            break;

        }

        case REQUEST_FLAG:
        {
            cMessage *requestInfo1 = new cMessage("requestInfo1", REQUEST_INFO);
            cMessage *requestInfo2 = new cMessage("requestInfo2", REQUEST_INFO);

            //recieves flag from sensors that want to transmit data
            bool willSend = msg -> par("willSend");
            int nodeID = msg -> par("nodeID");
            transmitPoll(willSend, nodeID);
            //cMessage *txRequestInfo = new cMessage("txRequestInfo", REQUEST_INFO);
            requestInfo1 -> addPar("transmitOrder");
            requestInfo1 -> addPar("signalStrength");
            requestInfo1 -> par("transmitOrder") = transmitOrder[1];
            requestInfo1 -> par("signalStrength") = signalStrength;
            double timeToSend = numNodes * timeIncrement;
            requestInfo2 -> addPar("transmitOrder");
            requestInfo2 -> addPar("signalStrength");
            requestInfo2 -> par("transmitOrder") = transmitOrder[2];
            requestInfo2 -> par("signalStrength") = signalStrength;
            //double timeToSend = numNodes * timeIncrement;
            for(int i=0; i<5; i++)
            {
                packetLengths[i] = generateDataPacket();
            }
            //packetLengths[5] = generateDataPacket();
            Tqueue = measureQueue(packetLengths);
            Tdeficit = updatedeficit(Tdeficit,Tshare,Tqueue);
            //EV << "Time Delayed" << timeToSend << endl;
            sendDelayed(requestInfo1,simTime() + timeToSend, "out",0);


            EV << "Message Kind: " << msg -> getKind() << endl; //DEBUG
            //sendDelayed(requestInfo2,simTime() + timeToSend, "out",1);
            delete(msg);
            break;
        }

        case RRI:
        {
            //recieves intereference and packet length data from sensor node
            //get NodeID object
            //check if all NodeIDs accounted for, then schedulei
            EV<<"GOT HERE IN AP RRI"<<endl;
            //int nodeID = msg -> par("nodeID");
            //ouble signalStrength = msg -> par("signalStrength");
            //double signalStrength = 1;
            //packetLengths[0] = msg -> par("packetLengths0");
            //packetLengths[1] = msg -> par("packetLengths1");

            //interference[0] = msg -> par("interferenceArray0");
            //interference[1] = msg -> par("interferenceArray1");
            //packetLengths[0] = 1;
            //packetLengths[1] = 1;
            /*
            double SIR[5] = {0};
            for (int i = 0; i < 2; i++)
            {
                SIR[i] = signalStrength / interference[i];
            }

            for (int j = 0; j < 2; j++)
            {
                conflictMap[nodeID][j] = SIR[j];
            }
            */
            //schedule(packetLengths, interference, nodeID);
            //cMessage *scheduler = new cMessage("scheduler");
            //scheduler -> addPar("schedule0");
            //scheduler -> par("schedule0")= schedulerArray[0];
            //scheduler -> addPar("schedule1");
            //scheduler -> par("schedule1")= schedulerArray[1];
            //scheduler -> addPar("schedule2");
            //scheduler -> par("schedule2")= schedulerArray[2];
            //scheduler -> par("schedule") = scheduleSendTimes;

            double timeToSend = numNodes * timeIncrement;
            sendDelayed(scheduler, simTime() + timeToSend, "out",0);
            //sendDelayed(scheduler, simTime() + timeToSend, "out",1);
            //scheduleAt(simTime() + timeToSend, scheduler);
            break;
        }

        case SCHEDULER:
        {
            //recieves scheduler packet with data transmit schedule
            int schedule[numNodes] = {0};

            //scheduler -> par("schedule0") = schedule[0];
            //scheduler -> par("schedule1") = schedule[1];
            //scheduler -> par("schedule2") = schedule[2];

            int sendTime1 = schedule[1];
            //int sendTime2 = schedule[2];

            EV << "Time Delayed" << sendTime1 << endl;
            cMessage *dataPacket = new cMessage("dataPacket", DATA_PACKET);
            sendDelayed(dataPacket,simTime() + sendTime1,"out", 0);
            //sendDelayed(dataPacket,simTime() + sendTime2,"out2");
            break;
            
        }

        case DATA_PACKET:
        {
            //recieves data packet, records information
            receivedDataPackets = receivedDataPackets + 1;
            EV<<"AP has recieved: "<<receivedDataPackets<<endl;
            if (receivedDataPackets == expectedDataPackets)
            {
                send(requestAck, "out", 0);
                //send(requestAck, "out", 1);
            }
            break;
        }
        
        case ACK_FLAG:
        {
            //recieves Ack flag, records information
            EV<<"Received ACK"<<endl;
            //cancelAndDelete(probeRequest);
            //cancelAndDelete(requestInfo);
            //cancelAndDelete(scheduler);
            //cancelAndDelete(requestAck);


            //scheduleAt(simTime(), initialProbeRequest);

            break;
        
        }

        default:
        {
            break;
        }

    }
}

void AP::registration()
{
    //modifies slotOrder array to determine which slots are taken by which nodes
    cModule* c = getModuleByPath("BaseNetwork");


}

void AP::transmitPoll(bool willSend, int nodeID)
{
    //modifies transmitOrder array to determine which nodes are tramsitting when
    cModule* c = getModuleByPath("BaseNetwork");
    //0 = node won't transmit, 1 = node will transmit
    transmitOrder[nodeID] = willSend;


}

/*void AP::schedulePackets()
{
    //sends out data packets and ACK requests
    sendDelayed(dataPackets,simTime() + time0,"out");
    sendDelayed(requestAck, simTime() + time1,"out");

}*/
int AP::LCU(int packetLengths[], int nodeID)
{
    for (int i=0; i<6; i++)
    {
        while (P[nodeID][i] != 0)
        {

        }
        P[nodeID][i] = packetLengths[i];
    }

    int size = 0;
    int num = 0;

    while (size < Tdeficit)
    {
        for(int i=0; i<6; i++)
        {
            if (P[nodeID][i] !=0)
            {
                if (size + P[nodeID][i] <= Tdeficit)
                {
                    size = size + P[nodeID][i];
                    P[nodeID][i] = 0;

                }
                else
                {
                    break;
                }

            }

        }
    }

    int numPacketsToSend = size/4;
    return numPacketsToSend;

    //update P to move packets up queue


}

void AP::RTA(double interference[], int nodeID, int numNodes)
{
    for(int i = 0; i <= numNodes; i++)
    {
        sendOrder[i] = i;int schedule[numNodes] = {0};
    }
}

void AP::schedule(int packetLengths[], double interference[], int nodeID)
{
    //performs scheduling function. takes info from RRI to create schedule
    //schedule determines how long each node has to transmit
    cModule* c = getModuleByPath("BaseNetwork");

    
    numPacketsArray[nodeID] = LCU(packetLengths,nodeID);
    //int numPacketsToSend = LCU(packetLengths, nodeID);
    RTA(interference, nodeID, numNodes);


    schedulerArray[nodeID] = numPacketsArray[nodeID] * packetLengths[nodeID] * 0.001;

    
    //update conflict map
  

    //update rate matrix

    
    //Load Control Unit (LCU) uses Deficit Round Robin DRR    
    //schedulePackets();

}

double AP::updatedeficit(double Tdeficit, double Tshare , double Tqueue)
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

int AP::measureQueue(int packetLengths[])
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

int AP::randomPacketLength()
{
    //randomly assigns packet length
    //return (rand()%10)+1;
    return 4;

}

int AP::generateDataPacket()
{

    int numPackets = (rand()%5)+1;
    return numPackets;
    /*
    packetLengths[numPackets] = {0};
    for (int i = 0; i <= numPackets; i++)
    {
        packetLengths[i] = randomPacketLength();
    }

    return packetLengths;
    */
}

void AP::finish()
{

}
