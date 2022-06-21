import math

#threads
import threading
import time

#sockets
import socket
import sys
from ctypes import *

global env
#global task_list
STATUS_GENERAL   = 10
STATUS_LOCATION  = 11
STATUS_VELOCITY  = 12
STATUS_BATTERY   = 13

class StatusMessage(Structure):
    _fields_ = [("idMessage", c_uint32),
                ("typeMessage", c_uint32),
                ("idUAV", c_uint32),
                ("velocity", c_float),
                ("battery", c_float),
                ("location_x", c_float),
                ("location_y", c_float),
                ("location_z", c_float),]

class ConnectUAV:
    def __init__(self, data):
        self.data = data

    def printAllStatus(env):
        print('ID ', env.swarm.clusters[0].agents[2].id)
        print('Status ', env.swarm.clusters[0].agents[2].status)
        print('Nick ', env.swarm.clusters[0].agents[2].nick)
        print('currTask ', env.swarm.clusters[0].agents[2].currTask)
        print('currTask ', env.swarm.clusters[0].agents[2].energyLevel)
        print('fuel ', env.swarm.clusters[0].agents[2].fuel)
        print('initial_position ', env.swarm.clusters[0].agents[2].initial_position)
        print('initial_orientations ', env.swarm.clusters[0].agents[2].initial_orientations)
        print('target ', env.swarm.clusters[0].agents[2].target.get_position())
        print('last_position ', env.swarm.clusters[0].agents[2].last_position)
        print('last_orientation ', env.swarm.clusters[0].agents[2].last_orientation)
        print('last_orientationxyz ', env.swarm.clusters[0].agents[2].last_orientationxyz)
        print('velocidade ', env.swarm.clusters[0].agents[2].last_v)
        print('real: ',env.swarm.clusters[0].agents[2].last_v[0])
        print('real: ',env.swarm.clusters[0].agents[2].last_v[1])
        print('real: ',env.swarm.clusters[0].agents[2].last_v[2])
        speed = math.sqrt(env.swarm.clusters[0].agents[2].last_v[0]**2 + env.swarm.clusters[0].agents[2].last_v[1]**2 + env.swarm.clusters[0].agents[2].last_v[2]**2)*10000
        print('velocidade real: ', speed)
        print('last_action ', env.swarm.clusters[0].agents[2].last_action)
        print('avail ', env.swarm.clusters[0].agents[2].avail)

    def startThread(self):
        server_addr = ('localhost', 1111)
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        try:
            s.connect(server_addr)
            print("Connected to {:s}".format(repr(server_addr)))
            while(True):
                print('1 - aguardando mensagem!')
                buff = s.recv(sizeof(StatusMessage))
                print('2 - aguardando mensagem!')
                msgReceived = StatusMessage.from_buffer_copy(buff)
                print('3 - aguardando mensagem!')
                print("Received idMessage={:d}, typeMessage={:d}, idUAV={:d}".format(msgReceived.idMessage,
                                                                    msgReceived.typeMessage,
                                                                    msgReceived.idUAV))
                print(self.data.status)

                print("Status: ", self.data.status)
                if(msgReceived.typeMessage == STATUS_LOCATION):
                    print("LOCATION")
                    print('last_position ', self.data.last_position)
                    print('last_position ', self.data.last_position[0])
                    msgToSend = msgReceived
                    msgToSend.location_x = self.data.last_position[0]
                    nsent = s.send(msgToSend)
                    print("Sent {:d} bytes".format(nsent))
                elif(msgReceived.typeMessage == STATUS_VELOCITY):
                    print("VELOCITY")
                    velocity = math.sqrt(self.data.last_v[0]**2 + self.data.last_v[1]**2 + self.data.last_v[2]**2)*10000
                    print('velocidade real: ', velocity)
                    msgToSend = msgReceived
                    msgToSend.velocity = velocity
                    nsent = s.send(msgToSend)
                    print("Sent {:d} bytes".format(nsent))
                elif(msgReceived.typeMessage == STATUS_BATTERY):
                    print("BATTERY")
                    #msgToSend = StatusMessage(1, 13, random.uniform(-10, 30))
                    msgToSend = msgReceived
                    msgToSend.battery = self.data.energyLevel
                    nsent = s.send(msgToSend)
                    print("Sent {:d} bytes".format(nsent))
                else:
                    print("Deu não!")

        except AttributeError as ae:
            print("Error creating the socket: {}".format(ae))
        except socket.error as se:
            print("Exception on socket: {}".format(se))
        # finally:
        #     print("Closing socket")
        #     s.close()
        
    def makeConnection(self):
        print('Iniciou Thread')
        threading.Thread(target=self.startThread).start() #, args=('vez de execução', )).start()
        print('Terminou Thread')