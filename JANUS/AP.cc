/*
 * AP.cc
 *
 *  Created on: Apr 9, 2018
 *      Author: matt
 */
#include "AP.h"

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

    Tshare[0] = .003;
    Tshare[1] = 0.006;
    Tshare[2] = 0.006;

    slotOrder[1] = 1;
    slotOrder[2] = 2;
    timeIncrement = par("timeIncrement");
    signalStrength = par("signalStrength");

    numNodes = par("numNodes");
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

    cMessage *ptr = check_and_cast<cMessage *>(msg);
    switch(msg -> getKind())
    {
        case INIT:
        {

            //sends probe request showing which request flag slots are used
            //cMessage *probeRequest = new cMessage("probeRequest", PROBE_REQUEST);
            //registration();

            probeRequest -> addPar("slotOrder");
            probeRequest -> addPar("Tshare");
            probeRequest -> par("slotOrder") = slotOrder;
            probeRequest -> par("Tshare") = Tshare[round];
            send(probeRequest, "out");
            round = round + 1;
        }

        case REQUEST_FLAG:
        {
            //recieves flag from sensors that want to transmit data
            bool willSend = msg -> par("willSend");
            int nodeID = msg -> par("nodeID");
            transmitPoll(willSend, nodeID);
            //cMessage *txRequestInfo = new cMessage("txRequestInfo", REQUEST_INFO);
            requestInfo -> par("transmitOrder") = transmitOrder;
            requestInfo -> par("signalStrength") = signalStrength;
            double timeToSend = numNodes * timeIncrement;
            sendDelayed(requestInfo,simTime() + timeToSend, "out1");
            sendDelayed(requestInfo,simTime() + timeToSend, "out2");
        }

        case RRI:
        {
            //recieves intereference and packet length data from sensor node
            //get NodeID object
            //check if all NodeIDs accounted for, then schedule
            int nodeID = msg -> par("nodeID");
            double signalStrength = msg -> par("signalStrength");
            int packetLength = msg -> par("packetLength");
            double interference[5] = {0};
            interference[5] = msg -> par("interferenceArray");

            double SIR[5] = {0};
            for (int i = 0; i < 6; i++)
            {
                SIR[i] = signalStrength / interference[i];
            }

            for (int j = 0; j < 6; j++)
            {
                conflictMap[nodeID][j] = SIR[j];
            }
            schedule(msg);
            //cMessage *scheduler = new cMessage("scheduler");
            scheduler -> addPar("schedule");
            scheduler -> par("schedule") = scheduleSendTimes;

            double timeToSend = numNodes * timeIncrement;
            sendDelayed(scheduler, simTime() + timeToSend, "out1");
            sendDelayed(scheduler, simTime() + timeToSend, "out2");
            scheduleAt(simTime() + timeToSend, scheduler);
        }

        case SCHEDULER:
        {
            //recieves scheduler packet with data transmit schedule

            
        }

        case DATA_PACKET:
        {
            //recieves data packet, records information
        }
        
        case ACK_FLAG:
        {
            //recieves Ack flag, records information
        
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

void AP::schedulePackets()
{
    //sends out data packets and ACK requests
    sendDelayed(dataPackets,simTime() + time0,"out");
    sendDelayed(requestAck, simTime() + time1,"out");

}

void AP::schedule(cMessage *msg)
{
    //performs scheduling function. takes info from RRI to create schedule
    //schedule determines how long each node has to transmit
    cModule* c = getModuleByPath("BaseNetwork");

    double interference;
    double packetLength;
    
    interference =  0;//get interefernce data from RRI msg
    packetLength =  0;//get packet length data from RRI msg
    
    //update conflict map
  

    //update rate matrix

    
    //Load Control Unit (LCU) uses Deficit Round Robin DRR    
    schedulePackets();

}
