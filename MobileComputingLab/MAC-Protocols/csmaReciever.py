import random 
import socket 
import time 

reciever_Port = 4040 
reciever_IP = "127.0.0.1"
station_Port = 4050 
station_IP = "127.0.0.1" 
stationSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) 
recieverSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) 
recieverSocket.bind((reciever_IP, reciever_Port)) 
while True: 
    data, add = recieverSocket.recvfrom(1024) 
    print(f"Frame recieved from client: {data.decode()}") 
    print("Sending acknowledgement") 
    random_time = random.randint(0, 3) 
    print(random_time) 
    time.sleep(random_time) 
    stationSocket.sendto("hi".encode(), (station_IP, station_Port))