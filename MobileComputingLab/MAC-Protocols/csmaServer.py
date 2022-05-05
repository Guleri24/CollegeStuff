import socket 
import time 
reciever_Port = 4040 
reciever_IP = "127.0.0.1" 
station_Port = 4050 
station_IP = "127.0.0.1" 
recieverSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) 
stationSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) 
stationSocket.bind((station_IP, station_Port)) 

def main(): 
    c = 0 
    frame_to_send = "hello, Bob!".encode() 
    is_transmission_successful = False 
    IFG_time = 2 
    time_bound = 2
    max_c = 3 
    while not is_transmission_successful: 
        print(f'Try number: {c + 1}') 
        # wait for IFG_time 
        time.sleep(IFG_time) 
        cur_time = time.time() 
        stationSocket.sendto(frame_to_send, (reciever_IP, reciever_Port)) 
        # start waiting for the reciever 
        data = stationSocket.recvfrom(1024) 
        turn_around_time = time.time() - cur_time 
        is_transmission_successful = turn_around_time <= time_bound 
        if not is_transmission_successful: 
            print(f"Didn't recieve acknowledgement from reciever in the time bound.. trying again") 
            c += 1 
            if c > max_c: 
                print(f"Couldn't send data to the reciever.. backed off {max_c} times") 
                break 
        if is_transmission_successful: 
            print("Successfully transmitted data to the reciever in the time bound") 
            
main()