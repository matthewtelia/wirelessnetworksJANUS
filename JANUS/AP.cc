/*
 * AP.cc
 *
 *  Created on: Apr 9, 2018
 *      Author: matt
 */
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
        case()
        {

        }
    }
}



