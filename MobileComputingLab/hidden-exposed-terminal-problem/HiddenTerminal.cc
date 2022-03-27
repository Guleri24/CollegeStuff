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

void hiddenTerminal(bool enableRtsCts)
{
  // Enable or disable RTS/CTS
  UintegerValue ctsThr = (enableRtsCts ? UintegerValue(2200) : UintegerValue(100));
  Config::SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold", ctsThr);

  uint32_t nWifi = 3, nWifi2 = 3, nWifi3 = 3;

  NodeContainer adhoc;
  adhoc.Create(3);
  NodeContainer network1, network2;
  network1.Add(adhoc.Get(0));
  network1.Add(adhoc.Get(1));
  network2.Add(adhoc.Get(1));
  network2.Add(adhoc.Get(2));
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();
  positionAlloc->Add(Vector(0.0, 0.0, 0.0));
  positionAlloc->Add(Vector(5.0, 0.0, 0.0));
  positionAlloc->Add(Vector(10.0, 0.0, 0.0));
  mobility.SetPositionAllocator(positionAlloc);
  mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobility.Install(adhoc);

  // Propagation Loss Matrix with 200 dB default loss and a set symmetric loss 0 <-> 1 to 50 dB
  Ptr<MatrixPropagationLossModel> lossModel = CreateObject<MatrixPropagationLossModel>();
  lossModel->SetDefaultLoss(200);
  lossModel->SetLoss(network1.Get(0)->GetObject<MobilityModel>(), network1.Get(1)->GetObject<MobilityModel>(), 50);
  Ptr<MatrixPropagationLossModel> lossModel2 = CreateObject<MatrixPropagationLossModel>();
  lossModel2->SetDefaultLoss(200);
  lossModel2->SetLoss(network2.Get(1)->GetObject<MobilityModel>(), network2.Get(0)->GetObject<MobilityModel>(), 50);

  // WiFi Communication channel setup
  Ptr<YansWifiChannel> wifiChannel = CreateObject<YansWifiChannel>();
  wifiChannel->SetPropagationLossModel(lossModel);
  wifiChannel->SetPropagationDelayModel(CreateObject<ConstantSpeedPropagationDelayModel>());

  Ptr<YansWifiChannel> wifiChannel2 = CreateObject<YansWifiChannel>();
  wifiChannel2->SetPropagationLossModel(lossModel2);
  wifiChannel2->SetPropagationDelayModel(CreateObject<ConstantSpeedPropagationDelayModel>());

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
  wifiPhy3.SetChannel(wifiChannel);
  YansWifiPhyHelper wifiPhy4;
  wifiPhy4.SetChannel(wifiChannel);
  YansWifiPhyHelper wifiPhy5;
  wifiPhy5.SetChannel(wifiChannel);

  WifiMacHelper wifiMac;
  wifiMac.SetType("ns3::AdhocWifiMac");
  NetDeviceContainer adhoc1 = wifi2.Install(wifiPhy, wifiMac, network1);
  NetDeviceContainer adhoc2 = wifi2.Install(wifiPhy2, wifiMac, network2);

  NodeContainer wifiStaNodes, wifiStaNodes2, wifiStaNodes3;
  wifiStaNodes.Create(nWifi);
  wifiStaNodes2.Create(nWifi2);
  wifiStaNodes3.Create(nWifi3);
  NodeContainer wifiApNode = adhoc.Get(0);
  NodeContainer wifiApNode2 = adhoc.Get(1);
  NodeContainer wifiApNode3 = adhoc.Get(2);

  YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
  YansWifiPhyHelper phy;
  YansWifiPhyHelper phy2;
  YansWifiPhyHelper phy3;
  phy.SetChannel(channel.Create());
  phy2.SetChannel(channel.Create());
  phy3.SetChannel(channel.Create());

  WifiHelper wifi;
  wifi.SetRemoteStationManager("ns3::AarfWifiManager");

  WifiMacHelper mac;
  Ssid ssid = Ssid("ns-3-ssid");
  mac.SetType("ns3::StaWifiMac",
              "Ssid", SsidValue(ssid),
              "ActiveProbing", BooleanValue(false));

  NetDeviceContainer staDevices, staDevices2, staDevices3;
  staDevices = wifi.Install(phy, mac, wifiStaNodes);
  staDevices2 = wifi.Install(phy2, mac, wifiStaNodes2);
  staDevices3 = wifi.Install(phy3, mac, wifiStaNodes3);

  mac.SetType("ns3::ApWifiMac",
              "Ssid", SsidValue(ssid));

  NetDeviceContainer apDevices, apDevices2, apDevices3;
  apDevices = wifi.Install(phy, mac, wifiApNode);
  apDevices2 = wifi.Install(phy2, mac, wifiApNode2);
  apDevices3 = wifi.Install(phy3, mac, wifiApNode3);

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

  mobility3.SetPositionAllocator("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue(10.0),
                                 "MinY", DoubleValue(10.0),
                                 "DeltaX", DoubleValue(5.0),
                                 "DeltaY", DoubleValue(10.0),
                                 "GridWidth", UintegerValue(3),
                                 "LayoutType", StringValue("RowFirst"));

  mobility3.SetMobilityModel("ns3::RandomWalk2dMobilityModel",
                             "Bounds", RectangleValue(Rectangle(-50, 50, -50, 50)));
  mobility3.Install(wifiStaNodes3);

  mobility3.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobility3.Install(wifiApNode3);

  // TCP/IP stack setup and IP addresses assign to each device
  InternetStackHelper stack;
  stack.Install(wifiApNode);
  stack.Install(wifiApNode2);
  stack.Install(wifiApNode3);
  stack.Install(wifiStaNodes);
  stack.Install(wifiStaNodes2);
  stack.Install(wifiStaNodes3);

  Ipv4AddressHelper address;

  address.SetBase("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer common_interface_1;
  common_interface_1 = address.Assign(adhoc1);

  address.SetBase("10.1.5.0", "255.255.255.0");
  Ipv4InterfaceContainer common_interface_2;
  common_interface_2 = address.Assign(adhoc2);

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

  // Applications: two CBR streams each saturating the channel
  ApplicationContainer cbrApps;
  uint16_t cbrPort = 12345;
  OnOffHelper onOffHelper("ns3::UdpSocketFactory", InetSocketAddress(Ipv4Address("10.1.3.3"), cbrPort));
  onOffHelper.SetAttribute("PacketSize", UintegerValue(1400));
  onOffHelper.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
  onOffHelper.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));

  onOffHelper.SetAttribute("DataRate", StringValue("3000000bps"));
  onOffHelper.SetAttribute("StartTime", TimeValue(Seconds(1.000000)));
  cbrApps.Add(onOffHelper.Install(wifiStaNodes.Get(0)));

  onOffHelper.SetAttribute("DataRate", StringValue("3000000bps"));
  onOffHelper.SetAttribute("StartTime", TimeValue(Seconds(1.000000)));
  cbrApps.Add(onOffHelper.Install(wifiStaNodes3.Get(0)));

  // FlowMonitor on all Nodes
  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll();

  Ipv4GlobalRoutingHelper::PopulateRoutingTables();

  // Simultation time setup for 10 seconds
  Simulator::Stop(Seconds(10.0));
  AnimationInterface anim("HiddenTerminal.xml");
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
  hiddenTerminal(false);
  cout << endl
       << "RtsCts is enabled" << endl
       << endl;
  hiddenTerminal(true);

  return 0;
}