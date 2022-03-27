/**
 * @file ExposedTerminal.cc
 * @author Abhishek Guleri
 * @brief Exposed Terminal Problem code using ns-3 and NetAnim to animate
 * @date 2022-03-27
 */
#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/propagation-module.h"
#include "ns3/netanim-module.h"
#include <iostream>

using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE("Third");

void exposedTerminal(bool enableRtsCts)
{
    // Enable or disable RTS/CTS
    UintegerValue ctsThr = (enableRtsCts ? UintegerValue(2200) : UintegerValue(100));
    Config::SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold", ctsThr);

    uint32_t nWifi = 3, nWifi2 = 3, nWifi3 = 3, nWifi4 = 3;

    NodeContainer adhoc;
    adhoc.Create(4);
    NodeContainer network1, network2, network3;
    network1.Add(adhoc.Get(0));
    network1.Add(adhoc.Get(1));
    network2.Add(adhoc.Get(1));
    network2.Add(adhoc.Get(2));
    network3.Add(adhoc.Get(2));
    network3.Add(adhoc.Get(3));
    MobilityHelper mobility;
    Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();
    positionAlloc->Add(Vector(0.0, 0.0, 0.0));
    positionAlloc->Add(Vector(5.0, 0.0, 0.0));
    positionAlloc->Add(Vector(10.0, 0.0, 0.0));
    positionAlloc->Add(Vector(15.0, 0.0, 0.0));
    mobility.SetPositionAllocator(positionAlloc);
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(adhoc);

    // Propagation Loss Matrix with 200 dB default loss and a set symmetric loss 0 <-> 1 to 50 dB
    Ptr<MatrixPropagationLossModel> lossModel = CreateObject<MatrixPropagationLossModel>();
    lossModel->SetDefaultLoss(200);
    lossModel->SetLoss(network1.Get(1)->GetObject<MobilityModel>(), network1.Get(0)->GetObject<MobilityModel>(), 50);
    Ptr<MatrixPropagationLossModel> lossModel2 = CreateObject<MatrixPropagationLossModel>();
    lossModel2->SetDefaultLoss(200);
    lossModel2->SetLoss(network2.Get(0)->GetObject<MobilityModel>(), network2.Get(1)->GetObject<MobilityModel>(), 50);
    Ptr<MatrixPropagationLossModel> lossModel3 = CreateObject<MatrixPropagationLossModel>();
    lossModel3->SetDefaultLoss(200);
    lossModel3->SetLoss(network3.Get(0)->GetObject<MobilityModel>(), network3.Get(1)->GetObject<MobilityModel>(), 50);

    // WiFi Communication channel setup
    Ptr<YansWifiChannel> wifiChannel = CreateObject<YansWifiChannel>();
    wifiChannel->SetPropagationLossModel(lossModel);
    wifiChannel->SetPropagationDelayModel(CreateObject<ConstantSpeedPropagationDelayModel>());

    Ptr<YansWifiChannel> wifiChannel2 = CreateObject<YansWifiChannel>();
    wifiChannel2->SetPropagationLossModel(lossModel2);
    wifiChannel2->SetPropagationDelayModel(CreateObject<ConstantSpeedPropagationDelayModel>());

    Ptr<YansWifiChannel> wifiChannel3 = CreateObject<YansWifiChannel>();
    wifiChannel3->SetPropagationLossModel(lossModel3);
    wifiChannel3->SetPropagationDelayModel(CreateObject<ConstantSpeedPropagationDelayModel>());

    // Wireless devices installation
    WifiHelper wifi2;
    wifi2.SetStandard(WIFI_STANDARD_80211b);
    wifi2.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                                  "DataMode", StringValue("DsssRate2Mbps"),
                                  "ControlMode", StringValue("DsssRate1Mbps"));
    YansWifiPhyHelper wifiPhy;
    wifiPhy.SetChannel(wifiChannel);
    YansWifiPhyHelper wifiPhy2;
    wifiPhy2.SetChannel(wifiChannel2);
    YansWifiPhyHelper wifiPhy3;
    wifiPhy3.SetChannel(wifiChannel3);
    YansWifiPhyHelper wifiPhy4;
    wifiPhy4.SetChannel(wifiChannel);
    YansWifiPhyHelper wifiPhy5;
    wifiPhy5.SetChannel(wifiChannel);

    WifiMacHelper wifiMac;
    wifiMac.SetType("ns3::AdhocWifiMac");
    NetDeviceContainer adhoc1 = wifi2.Install(wifiPhy, wifiMac, network1);
    NetDeviceContainer adhoc2 = wifi2.Install(wifiPhy2, wifiMac, network2);
    NetDeviceContainer adhoc3 = wifi2.Install(wifiPhy3, wifiMac, network3);

    NodeContainer wifiStaNodes, wifiStaNodes2, wifiStaNodes3, wifiStaNodes4;
    wifiStaNodes.Create(nWifi);
    wifiStaNodes2.Create(nWifi2);
    wifiStaNodes3.Create(nWifi3);
    wifiStaNodes4.Create(nWifi4);
    NodeContainer wifiApNode = adhoc.Get(0);
    NodeContainer wifiApNode2 = adhoc.Get(1);
    NodeContainer wifiApNode3 = adhoc.Get(2);
    NodeContainer wifiApNode4 = adhoc.Get(3);

    YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
    YansWifiPhyHelper phy;
    YansWifiPhyHelper phy2;
    YansWifiPhyHelper phy3;
    YansWifiPhyHelper phy4;
    phy.SetChannel(channel.Create());
    phy2.SetChannel(channel.Create());
    phy3.SetChannel(channel.Create());
    phy4.SetChannel(channel.Create());

    WifiHelper wifi;
    wifi.SetRemoteStationManager("ns3::AarfWifiManager");

    WifiMacHelper mac;
    Ssid ssid = Ssid("ns-3-ssid");
    mac.SetType("ns3::StaWifiMac",
                "Ssid", SsidValue(ssid),
                "ActiveProbing", BooleanValue(false));

    NetDeviceContainer staDevices, staDevices2, staDevices3, staDevices4;
    staDevices = wifi.Install(phy, mac, wifiStaNodes);
    staDevices2 = wifi.Install(phy2, mac, wifiStaNodes2);
    staDevices3 = wifi.Install(phy3, mac, wifiStaNodes3);
    staDevices4 = wifi.Install(phy4, mac, wifiStaNodes4);

    mac.SetType("ns3::ApWifiMac",
                "Ssid", SsidValue(ssid));

    NetDeviceContainer apDevices, apDevices2, apDevices3, apDevices4;
    apDevices = wifi.Install(phy, mac, wifiApNode);
    apDevices2 = wifi.Install(phy2, mac, wifiApNode2);
    apDevices3 = wifi.Install(phy3, mac, wifiApNode3);
    apDevices4 = wifi.Install(phy4, mac, wifiApNode4);

    wifi.SetRemoteStationManager("ns3::AarfWifiManager");
    mac.SetType("ns3::AdhocWifiMac");

    MobilityHelper mobility2;

    mobility2.SetPositionAllocator("ns3::GridPositionAllocator",
                                   "MinX", DoubleValue(5.0),
                                   "MinY", DoubleValue(5.0),
                                   "DeltaX", DoubleValue(5.0),
                                   "DeltaY", DoubleValue(10.0),
                                   "GridWidth", UintegerValue(3),
                                   "LayoutType", StringValue("RowFirst"));

    mobility2.SetMobilityModel("ns3::RandomWalk2dMobilityModel",
                               "Bounds", RectangleValue(Rectangle(-50, 50, -50, 50)));
    mobility2.Install(wifiStaNodes);

    mobility2.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility2.Install(wifiApNode);

    MobilityHelper mobility3;

    mobility3.SetPositionAllocator("ns3::GridPositionAllocator",
                                   "MinX", DoubleValue(0.0),
                                   "MinY", DoubleValue(0.0),
                                   "DeltaX", DoubleValue(5.0),
                                   "DeltaY", DoubleValue(10.0),
                                   "GridWidth", UintegerValue(3),
                                   "LayoutType", StringValue("RowFirst"));

    mobility3.SetMobilityModel("ns3::RandomWalk2dMobilityModel",
                               "Bounds", RectangleValue(Rectangle(-50, 50, -50, 50)));
    mobility3.Install(wifiStaNodes2);

    mobility3.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility3.Install(wifiApNode2);

    MobilityHelper mobility4;

    mobility4.SetPositionAllocator("ns3::GridPositionAllocator",
                                   "MinX", DoubleValue(10.0),
                                   "MinY", DoubleValue(10.0),
                                   "DeltaX", DoubleValue(5.0),
                                   "DeltaY", DoubleValue(10.0),
                                   "GridWidth", UintegerValue(3),
                                   "LayoutType", StringValue("RowFirst"));

    mobility4.SetMobilityModel("ns3::RandomWalk2dMobilityModel",
                               "Bounds", RectangleValue(Rectangle(-50, 50, -50, 50)));
    mobility4.Install(wifiStaNodes3);

    mobility4.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility4.Install(wifiApNode3);

    MobilityHelper mobility5;

    mobility5.SetPositionAllocator("ns3::GridPositionAllocator",
                                   "MinX", DoubleValue(15.0),
                                   "MinY", DoubleValue(15.0),
                                   "DeltaX", DoubleValue(5.0),
                                   "DeltaY", DoubleValue(10.0),
                                   "GridWidth", UintegerValue(3),
                                   "LayoutType", StringValue("RowFirst"));

    mobility5.SetMobilityModel("ns3::RandomWalk2dMobilityModel",
                               "Bounds", RectangleValue(Rectangle(-50, 50, -50, 50)));
    mobility5.Install(wifiStaNodes4);

    mobility5.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility5.Install(wifiApNode4);

    // TCP/IP stack setup and IP addresses assign to each device
    InternetStackHelper stack;
    stack.Install(wifiApNode);
    stack.Install(wifiApNode2);
    stack.Install(wifiApNode3);
    stack.Install(wifiApNode4);
    stack.Install(wifiStaNodes);
    stack.Install(wifiStaNodes2);
    stack.Install(wifiStaNodes3);
    stack.Install(wifiStaNodes4);

    Ipv4AddressHelper address;

    address.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer common_interface_1;
    common_interface_1 = address.Assign(adhoc1);

    address.SetBase("10.1.5.0", "255.255.255.0");
    Ipv4InterfaceContainer common_interface_2;
    common_interface_2 = address.Assign(adhoc2);

    address.SetBase("10.1.6.0", "255.255.255.0");
    Ipv4InterfaceContainer common_interface_3;
    common_interface_3 = address.Assign(adhoc3);

    address.SetBase("10.1.2.0", "255.255.255.0");
    Ipv4InterfaceContainer wifiInterface;
    wifiInterface = address.Assign(staDevices);
    address.Assign(apDevices);

    address.SetBase("10.1.3.0", "255.255.255.0");
    Ipv4InterfaceContainer wifiInterface2;
    wifiInterface2 = address.Assign(staDevices2);
    address.Assign(apDevices2);

    address.SetBase("10.1.4.0", "255.255.255.0");
    Ipv4InterfaceContainer wifiInterface3;
    wifiInterface3 = address.Assign(staDevices3);
    address.Assign(apDevices3);

    address.SetBase("10.1.7.0", "255.255.255.0");
    Ipv4InterfaceContainer wifiInterface4;
    wifiInterface3 = address.Assign(staDevices4);
    address.Assign(apDevices4);

    // Applications: two CBR streams each saturating the channel
    ApplicationContainer cbrApps;
    uint16_t cbrPort = 12345;
    OnOffHelper onOffHelper("ns3::UdpSocketFactory", InetSocketAddress(Ipv4Address("10.1.2.3"), cbrPort));
    onOffHelper.SetAttribute("PacketSize", UintegerValue(1400));
    onOffHelper.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    onOffHelper.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));

    onOffHelper.SetAttribute("DataRate", StringValue("3000000bps"));
    onOffHelper.SetAttribute("StartTime", TimeValue(Seconds(1.000000)));
    cbrApps.Add(onOffHelper.Install(wifiStaNodes2.Get(0)));

    ApplicationContainer cbrApps2;
    uint16_t cbrPort2 = 54321;
    OnOffHelper onOffHelper2("ns3::UdpSocketFactory", InetSocketAddress(Ipv4Address("10.1.7.3"), cbrPort2));
    onOffHelper2.SetAttribute("PacketSize", UintegerValue(1400));
    onOffHelper2.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    onOffHelper2.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));

    onOffHelper2.SetAttribute("DataRate", StringValue("3000000bps"));
    onOffHelper2.SetAttribute("StartTime", TimeValue(Seconds(1.000000)));
    cbrApps2.Add(onOffHelper2.Install(wifiStaNodes3.Get(0)));

    // FlowMonitor on all Nodes
    FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> monitor = flowmon.InstallAll();

    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    // Simultation time setup for 10 seconds
    Simulator::Stop(Seconds(10.0));
    AnimationInterface anim("ExposedTerminal.xml");
    Simulator::Run();

    // Flow Statistics
    monitor->CheckForLostPackets();
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowmon.GetClassifier());
    FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats();
    for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin(); i != stats.end(); ++i)
    {
        Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(i->first);
        std::cout << "Data going from IP Address " << t.sourceAddress << " to IP Address " << t.destinationAddress << "\n"
                  << endl;

        std::cout << "  Tx Packets: " << i->second.txPackets << "\n";
        std::cout << "  Tx Bytes:   " << i->second.txBytes << "\n";
        std::cout << "  Rx Packets: " << i->second.rxPackets << "\n";
        std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
        std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / 9.0 / 1000 / 1000 << " Mbps\n";
        std::cout << endl;
    }

    Simulator::Destroy();
}

int main(int argc, char *argv[])
{

    CommandLine cmd;
    cmd.Parse(argc, argv);

    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    cout << endl
         << "RtsCts is disabled" << endl
         << endl;
    ;
    exposedTerminal(false);
    cout << endl
         << "RtsCts is enabled" << endl
         << endl;
    exposedTerminal(true);

    return 0;
}