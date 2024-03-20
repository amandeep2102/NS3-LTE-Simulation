#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/mobility-module.h>
#include <ns3/lte-module.h>

using namespace ns3;

int main(int argc, char *argv[]){
    Ptr<LteHelper> lteHelper = CreateObject<LteHelper>();

    NodeContainer enbNodes;
    enbNodes.Create(4);
    NodeContainer ueNodes;
    ueNodes.Create(40);

    //placing enodebs in a square topology

    MobilityHelper mobility;
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");

    //enodeb 0
    Ptr<ConstantPositionMobilityModel> loc0 = CreateObject<ConstantPositionMobilityModel>();
    loc0 ->SetPosition(Vector(0,0,0));
    enbNodes.Get(0) ->AggregateObject(loc0);

    //enodeb 1
    Ptr<ConstantPositionMobilityModel> loc1 = CreateObject<ConstantPositionMobilityModel>();
    loc1 ->SetPosition(Vector(5000, 0, 0));
    enbNodes.Get(1)->AggregateObject(loc1);

    //enodeb 2
    Ptr<ConstantPositionMobilityModel> loc2 = CreateObject<ConstantPositionMobilityModel>();
    loc2 ->SetPosition(Vector(5000, 5000, 0));
    enbNodes.Get(2) ->AggregateObject(loc2);

    //enodeb 3
    Ptr<ConstantPositionMobilityModel> loc3 = CreateObject<ConstantPositionMobilityModel>();
    loc3->SetPosition(Vector(0,5000,0));
    enbNodes.Get(3)->AggregateObject(loc3);

    //for uenodes defining stationary mobility model
    mobility.Install(ueNodes);

    //installing lte protocol stack on eNBs
    NetDeviceContainer enbDevs;
    enbDevs = lteHelper->InstallEnbDevice(enbNodes);

    //installing lte protocol stack on UEs
    NetDeviceContainer ueDevs;
    ueDevs = lteHelper->InstallUeDevice(ueNodes);

    for (int j = 0; j < 10; j++)
    {
        lteHelper->Attach(ueDevs.Get(j), enbDevs.Get(0));
    }

    for (int j = 10; j < 20; j++)
    {
        lteHelper->Attach(ueDevs.Get(j), enbDevs.Get(1));
    }

    for (int j = 20; j < 30; j++)
    {
        lteHelper->Attach(ueDevs.Get(j), enbDevs.Get(2));
    }

    for (int j = 30; j < 40; j++)
    {
        lteHelper->Attach(ueDevs.Get(j), enbDevs.Get(3));
    }

    // configuring rem plot
    Ptr<RadioEnvironmentMapHelper> remHelper = CreateObject<RadioEnvironmentMapHelper>();
    remHelper->SetAttribute("Channel", PointerValue(lteHelper->GetDownlinkSpectrumChannel()));
    remHelper->SetAttribute("OutputFile", StringValue("rem.out"));
    remHelper->SetAttribute("XMin", DoubleValue(-400.0));
    remHelper->SetAttribute("XMax", DoubleValue(400.0));
    remHelper->SetAttribute("XRes", UintegerValue(100));
    remHelper->SetAttribute("YMin", DoubleValue(-300.0));
    remHelper->SetAttribute("YMax", DoubleValue(300.0));
    remHelper->SetAttribute("YRes", UintegerValue(75));
    remHelper->SetAttribute("Z", DoubleValue(0.0));
    remHelper->SetAttribute("UseDataChannel", BooleanValue(true));
    remHelper->SetAttribute("RbId", IntegerValue(10));
    remHelper->Install();

    Simulator::Stop(Seconds(0.005));
    Simulator::Run();
    Simulator::Destroy();
    return 0;
}