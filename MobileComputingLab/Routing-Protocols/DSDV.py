class Entry:
    def __init__(self, dest, next_hop, cost, seq_no):
        self.dest = dest
        self.next_hop = next_hop
        self.seq_no = seq_no
        self.cost = cost
    def __str__(self): 
        return f'\t{self.dest},\t{self.next_hop},\t{self.cost},\t{self.seq_no}\t'
    
    
class Node:
    def __init__(self, name):
        self.name = name
        self.seq_no = 0
        self.neighbors = []
        self.routing_table = {}
        
    def add_neighbor(self, node):
        self.neighbors.append(node)
        node.neighbors.append(self)
        
    def __str__(self): 
        return f'{self.name}'
    
    def print_routing_table(self): 
        print(f"----------------- {self.name}'s' routing table -------------------") 
        print(' DestNode\tNextHop\tCost\tSeqNo')
        for node_entry in self.routing_table: 
            print(f'{self.routing_table[node_entry]}') 
        print(f'--------------------------------------------------------')
            
    def broadcast_table(self):
        self.routing_table[self.name].seq_no = self.seq_no
        self.seq_no += 2
        for node in self.neighbors:
            node.recieve_table(self.name, self.routing_table)
            
    def recieve_table(self, sender_name, sender_routing_table):
        made_update = False
        for key in self.routing_table.keys():
            sender_rountin_entry = sender_routing_table[key]
            own_routing_entry = self.routing_table[key]
            if(own_routing_entry.seq_no >= sender_rountin_entry.seq_no):
                continue
            # update the next entry
            if(own_routing_entry.cost > (sender_rountin_entry.cost + 1)):
            # make the update
                own_routing_entry.seq_no = sender_rountin_entry.seq_no
                own_routing_entry.dest = sender_rountin_entry.dest
                own_routing_entry.next_hop = sender_name
                own_routing_entry.cost = sender_rountin_entry.cost + 1
                made_update = True
        if made_update == True:
            self.broadcast_table()
            
            
INF = 1000


def print_routing_tables(network):
    for node in network:
        node.print_routing_table()
            
def main():    
    A = Node("A") 
    B = Node("B") 
    C = Node("C") 
    D = Node("D")
    
    A.add_neighbor(B)
    B.add_neighbor(C)
    C.add_neighbor(D)
    
    
    network = [A, B, C, D]
    
    print('---------- DSDV ALGORITHM ----------')
    for node in network:
    # update the routing table of node to accomodate its own entry
        for other_node in network:
            if node == other_node:
                own_entry = Entry(node, node, 0, node.seq_no)
                node.routing_table[node.name] = own_entry
            else: 
                init_entry = Entry(other_node, "", INF, -INF)
                node.routing_table[other_node.name] = init_entry
                
    print("BEFORE BROADCASTING")
    print_routing_tables(network)
    # A will start the broadcast
    A.broadcast_table()
    print("AFTER BROADCASTING")
    print_routing_tables(network)
        
if __name__ == '__main__':
    main()
