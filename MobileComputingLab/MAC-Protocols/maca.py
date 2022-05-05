import Terminal 
# send rts 
def main(): 
    a = Terminal.Terminal("A", 2, 0, 2) 
    b = Terminal.Terminal("B", 3, 0, 2) 
    c = Terminal.Terminal("C", 0, 0, 2) 
    d = Terminal.Terminal("D", 5, 0, 2) 
    terminals = [c, a, b, d] 
    print(f'\t\t\t{Terminal.Terminal.algorithm.upper()} algorithm') 
    # print terminals in coverage range of a 
    for terminal in terminals: 
        for t in terminals: 
            if terminal != t: 
                if terminal.in_coverage_range(t): 
                    terminal.nodes_in_coverage_range.append(t) 
                    a.send_rts("B") 
                    print('------------------------------------------------------------------------')
                    print('------------------------------------------------------------------------') 
                    print('------------------------------------------------------------------------') 
                    a.send_rts("D") 
                    
main()