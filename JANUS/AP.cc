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

    //MOVED TO CASEs
    //requestAck = new cMessage("requestAck",REQUEST_ACK);

    //Sets round number will be iterated in INIT
    round = 0;

    //TSHARES FOR EACH ROUND
    Tshare = .003;
    //Tshare1 = 0.006;
    //Tshare2 = 0.006;

    timeIncrement = 0.1;
    signalStrength = par("signalStrength");

    numNodes = par("numNodes");

    receivedDataPackets = 0;
    expectedDataPackets = 2;

    APNodeID = 0; //setting NODEID for the AP
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


    switch(msg -> getKind())
    {
        case INIT:
        {
            cMessage *probeRequest1 = new cMessage("probeRequest1", PROBE_REQUEST);
            cMessage *probeRequest2 = new cMessage("probeRequest2", PROBE_REQUEST);


            //sends probe request showing which request flag slots are used

            //registration();

            //Probe Request 1 setup
            probeRequest1 -> addPar("slotOrder");
            probeRequest1 -> addPar("Tshare");
            probeRequest1 -> par("slotOrder") = 1;
            probeRequest1 -> par("Tshare") = Tshare;

            //Probe Request 2 setup
            probeRequest2 -> addPar("slotOrder");
            probeRequest2 -> addPar("Tshare");
            probeRequest2 -> par("slotOrder") = 2;
            probeRequest2 -> par("Tshare") = Tshare;

            round = round + 1; //increment round

            //ending simulation after getting through three rounds
            if (round >= 3)
            {
                finish();
            }

            //prob request to node 1 and 2
            send(probeRequest1, "out", 0);
            send(probeRequest2, "out", 1);


            break;

        }

        case REQUEST_FLAG:
        {
            //declaring requestInfo message
            cMessage *requestInfo = new cMessage("requestInfo", REQUEST_INFO);

            //recieves willSend(decision to send) and nodeID from msg
            bool willSend = msg -> par("willSend");
            int nodeID = msg -> par("nodeID");

            //makes array out of willSend data
            transmitPoll(willSend, nodeID);

            //Adding parameters to requestInfo message
            requestInfo -> addPar("transmitOrder1");
            requestInfo -> addPar("signalStrength");
            requestInfo -> addPar("transmitOrder2");
            requestInfo -> par("transmitOrder1") = transmitOrder[0];
            requestInfo -> par("transmitOrder2") = transmitOrder[1];
            requestInfo -> par("signalStrength") = signalStrength;


            double timeToSend = numNodes * timeIncrement; // delay for message based on number sending

            //FOR AP SENDING MECHANISIM

            //for(int i=0; i<5; i++)
            //{
            //    packetLengths[i] = generateDataPacket();
            //}

            //packetLengths[5] = generateDataPacket();

            Tqueue = measureQueue(packetLengths);
            Tdeficit = updatedeficit(Tdeficit,Tshare,Tqueue);

            //sending requestInfo
            sendDelayed(requestInfo,simTime() + timeToSend, "out",nodeID - 1);


            break;
        }

        case RRI:
        {
            //recieves intereference and packet length data from sensor node
            //get NodeID object
            //check if all NodeIDs accounted for, then schedule

            //make cMessage scheduler
            cMessage *scheduler = new cMessage("scheduler",SCHEDULER);

            //pull in parameters from RRI message

            int sensornodeID;
            sensornodeID = msg -> par("nodeID");

            double signalStrength;
            signalStrength = msg -> par("signalStrength");

            //packetLengths declared in the header
            packetLengths[0] = msg -> par("packetLengths0");
            packetLengths[1] = msg -> par("packetLengths1");

            //interference declared in the header
            interference[0] = msg -> par("interferenceInfo1");
            interference[1] = msg -> par("interferenceInfo2");


            double SIR[5] = {0};
            for (int i = 0; i < 2; i++)
            {
                SIR[i] = signalStrength / interference[i];
            }

            for (int j = 0; j < 2; j++)
            {
                conflictMap[sensornodeID][j] = SIR[j];
            }

            //Calls schedule function which includes both LCU and RTA function
            schedule(sensornodeID);

            //add parameters to message scheduler
            scheduler -> addPar("schedule0");
            scheduler -> par("schedule0")= schedulerArray[0];

            scheduler -> addPar("schedule1");
            scheduler -> par("schedule1")= schedulerArray[1];

            EV << "Schedule 0" << schedulerArray[0] << endl;
            EV << "Schedule 1" << schedulerArray[1] << endl;

            //calculate time delay to send
            double timeToSend = numNodes * timeIncrement;
            sendDelayed(scheduler, simTime() + timeToSend, "out",sensornodeID - 1);

            //FOR SENDING TO SCHEDULE AP MESSAGES
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
            cMessage *requestAck1 = new cMessage("requestAck",REQUEST_ACK);
            cMessage *requestAck2 = new cMessage("requestAck",REQUEST_ACK);

            //recieves data packet, records information
            receivedDataPackets = receivedDataPackets + 1;
            int nodeID = msg -> par("nodeID");
            EV<<"AP has recieved: "<<receivedDataPackets<<endl;
            if (receivedDataPackets == expectedDataPackets)
            {
                send(requestAck1, "out", 0);
                send(requestAck2, "out", 1);
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

void AP::LCU(int nodeID)
{
    /*for (int i=0; i<2; i++)
    {
        while (P[nodeID][i] != 0)
        {

        }
        P[nodeID][i] = packetLengths[i];
    }*/

    int size = 0;
    int num = 0;

    while (size < Tdeficit)
    {
        for(int i = 0; i < 2; i++)
        {
            if (P[nodeID][i] !=0)
            {
                EV << Tdeficit << endl;

                if ((size + P[nodeID][i]) <= Tdeficit)
                {
                    size = size + P[nodeID][i];
                    P[nodeID][i] = 0;

                }
                else
                {

                }

            }

        }
    }

    numPacketsToSend = size/4;

    //update P to move packets up queue
}

void AP::RTA(int nodeID, int numNodes)
{
    for(int i = 0; i <= numNodes; i++)
    {
        sendOrder[i] = i;int schedule[numNodes] = {0};
    }
}

void AP::schedule(int nodeID)
{
    //performs scheduling function. takes info from RRI to create schedule
    //schedule determines how long each node has to transmit
    //update conflict map, update rate matrix, Load Control Unit (LCU) uses Deficit Round Robin DRR

    cModule* c = getModuleByPath("BaseNetwork");
    
    //calls LCU
    //LCU(nodeID);
    numPacketsToSend = 2;

    //calls RTA
    RTA(nodeID, numNodes);

    //time offsett for each packet
    schedulerArray[nodeID] = numPacketsArray[nodeID] * packetLengths[nodeID] * 0.001;

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
    //FOR AP SENDING MECHANISIM

    //randomly assigns packet length
    //return (rand()%10)+1;
    return 4;
}

int AP::generateDataPacket()
{
    //FOR AP SENDING MECHANISIM

    int numPackets = (rand()%5) + 1;
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
