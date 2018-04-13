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
        case PROBE_REQUEST:
        {
            cMessage *txProbe = new cMessage("txProbe");
            send(txProbe, "out");
        }

        case REQUEST_INFO:
        {
            cMessage *txRequestInfo = new cMessage("txRequestInfo");
            send(txRequestInfo, "out");
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

        default:
        {
            break;
        }

    }
}

void AP::schedule(cMessage *msg)
{
//performs scheduling function. takes info from RRI to create schedule
    cModule* c = getModuleByPath("BaseNetwork");

    double interference;
    double packetLength;
    
    interference = //get interefernce data from RRI msg
    packetLength = //get packet length data from RRI msg


}
