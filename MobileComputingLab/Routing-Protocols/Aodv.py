# Ad Hoc On-demand Distace Vector

class Packet:
    def __init__(self, source, source_seq, source_bcast_id, dest):
        self.source = source
        self.source_seq = source_seq
        self.source_bcast_id = source_bcast_id
        self.dest = dest 
        self.dest_seq = ""
        self.hop_cnt = 0

    def copy(self):
        new_packet = Packet(self.source, self.source_seq, self.source_bcast_id, self.dest)
        new_packet.hop_cnt = self.hop_cnt + 1
        return new_packet

class Node:
    def __init__(self, name):
        self.name = name
        self.neighbours = []
        self.routing_table = {}
        self.seq_no = 0
        self.broadcast_id = 0
        self.recieved_packets = []
    
    def add_neighbour(self, node):
        self.neighbours.append(node)
        node.neighbours.append(self)
    

    def get_neighbours(self):
        neighbours = [str(node) for node in self.neighbours]
        return neighbours
    
    def broadcase_RREQ(self, packet):
        for node in self.neighbours:
            node.recieve_RREQ(self, packet)
            
    def print_routing_table(self): 
        print(f'------------------------------- {self} routing --------------------------------') 
        print(f'Dest\t Next Hop\t Hop Count\t Seq. No') 
        print(f'----------------------------------------------------------------------------')
        for key in self.routing_table.keys():
            entry = self.routing_table[key] 
            nextRow = '\t\t'.join([str(node) for node in entry]) 
            print(f'{nextRow}')
        print(f'----------------------------------------------------------------------------')
        
        
    def entry_in_routing_table(self, dest):
        return str(dest) in self.routing_table.keys()
    
    def already_recieved_packet(self, packet):
        return packet.source_bcast_id in self.recieved_packets
    
    def add_routing_entry(self, entry, sender):
        self.routing_table[entry.source.name] = [entry.source, sender, entry.hop_cnt, entry.source_seq]
        
    def get_entry_from_routing_table(self, node):
        return self.routing_table[node.name]
    
    def recieve_RREQ(self, sender, packet : Packet): 
        print(f'Node {self} recieved RREQ from Node {sender}')
        if self.already_recieved_packet(packet):
            return
        self.recieved_packets.append(packet.source_bcast_id)
        new_packet = packet.copy()
        self.add_routing_entry(new_packet, sender)
        if self.entry_in_routing_table(packet.dest): 
            print(f'Node {self} recieve RREQ from Node {sender}... Sending RREP to Node {packet.source}')
            return
        
        if new_packet.dest == self: 
            print(f'{self} recieved the RREQ packet from Node {sender}.. Sending a reply to Node {sender}')
            RREP_Packet = Packet(self, self.seq_no, self.broadcast_id, packet.source)
            self.send_RREP(packet.source, RREP_Packet)
            return
        self.broadcase_RREQ(new_packet)
        
    def send_RREP(self, node, packet):
        routing_table_entry = self.get_entry_from_routing_table(node)
        next_hop_node = routing_table_entry[1]
        next_hop_node.recieve_RREP(self, packet)
        
        
    def recieve_RREP(self, sender, packet): 
        print(f'Node {self} recieved RREP from Node {sender}.. meant for Node {packet.dest}')
        if self == packet.dest: 
            print(f'Node {self} successfully recieved RREP from Node {packet.source}')
            return

        ## add an entry in table
        new_entry = packet.copy()
        self.add_routing_entry(new_entry, sender)
        self.send_RREP(new_entry.dest, new_entry)
        
        
    def __str__(self) -> str:
        return f'{self.name}'
    
def print_routing_tables(network):
    for node in network:
        node.print_routing_table()
        print("\n")
    
def aodv(): 
    A = Node("A") 
    B = Node("B") 
    C = Node("C") 
    D = Node("D") 
    E = Node("E")
    
    # Assign the neighbour relationship to the created nodes
    A.add_neighbour(B)
    B.add_neighbour(C)
    C.add_neighbour(D)
    E.add_neighbour(B)
    E.add_neighbour(C)

    network = [A, B, C, D, E]
    # A will start RREQ broadcase
    print("\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++")
    print("Initial Routing Table Setup")
    print("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++")
    RREQ_Packet = Packet(A, A.seq_no, A.broadcast_id, E)
    print_routing_tables(network)
    A.add_routing_entry(RREQ_Packet, A)
    A.recieved_packets.append(RREQ_Packet.source_bcast_id)
    
    print("\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++")
    print("Steps happened throught out routing")
    print("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++")
    A.broadcase_RREQ(RREQ_Packet)

    print("\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++")
    print("After the completing of Routing between the Nodes")
    print("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++")
    print_routing_tables(network)
    
if __name__ == "__main__":
    aodv()

