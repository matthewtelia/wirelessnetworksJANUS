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
    probeRequest = new cMessage("probeRequest", PROBE_REQUEST);
    requestInfo = new cMessage("requestInfo",REQUEST_INFO);
    scheduler = new cMessage("scheduler",SCHEDULER);
    requestAck = new cMessage("requestAck",REQUEST_ACK);


}

void AP::handleMessage(cMessage *msg)
{
    cModule* c = getModuleByPath("BaseNetwork");
    EV << "Message Kind: " << msg -> getKind() << endl;

    switch(msg -> getKind())
    {
        case REGISTER_NODE:
        {
            //sends probe request showing which request flag slots are used
            //cMessage *probeRequest = new cMessage("probeRequest", PROBE_REQUEST);
            //registration();
            probeRequest -> addObject(slotOrder);
            send(probeRequest, "out");
        }

        case REQUEST_FLAG:
        {
            //recieves flag from sensors that want to transmit data
            transmitPoll(); 
            //cMessage *requestInfo = new cMessage("requestInfo", REQUEST_INFO);
            requestInfo -> addObject(TransmitOrder);
            send(txRequestInfo, "out");
        }

        case RRI:
        {
            //recieves intereference and packet length data from sensor node
            schedule(msg);
            //cMessage *scheduler = new cMessage("scheduler");
            scheduler -> addObject(schedule);
            send(scheduler, "out");
        }

        case DATA_PACKET:
        {
            //recieves data packet, records information
        }
        
        case ACK_FLAG:
        {
            //
        
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

void AP::transmitPoll()
{
    //modifies transmitOrder array to determine which nodes are tramsitting when
    cModule* c = getModuleByPath("BaseNetwork");
}

void AP::schedulePackets()
{
    //sends out data packets and ACK requests
    send(dataPackets, "out", simTime() + time0);
    send(requestAck, "out", simTime() + time1);

}

void AP::schedule(cMessage *msg)
{
    //performs scheduling function. takes info from RRI to create schedule
    //schedule determines how long each node has to transmit
    cModule* c = getModuleByPath("BaseNetwork");

    double interference;
    double packetLength;
    
    interference = //get interefernce data from RRI msg
    packetLength = //get packet length data from RRI msg
    
    schedulePackets();

}
