#!/usr/bin/env python

import socket

def recv_until(conn, str):
	buf = ''
	while not str in buf:
		buf += conn.recv(1)
	return buf

def getValidSubnet(host):
	return '255.255.255.255/32'

def countHosts(subnet):
	return str((2)**(32-int(subnet.split("/")[1])))

def isSubnetValid(subnet, host):
	splitres = subnet.split("/")
	prefix = int(splitres[1])
	mask = [0] * 32
	for i in range (0, len(mask)):
	 	if i+1 <= prefix:
	 		mask[i] = 1
	 	else:
	 		mask[i] = 0
	arrNum = list(map(int, splitres[0].split(".")))
	
	bitstr = []
	for a in arrNum:
		bitstr.extend(list(format(a, '08b')))


	return 'T'
	
TCP_IP = 'localhost'
TCP_PORT = 8888

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))

data = recv_until(s, 'NIM: ')
nim = raw_input(data)
s.send(nim + '\n')

data = recv_until(s, 'Verify NIM: ')
nim = raw_input(data)
s.send(nim + '\n')

print (recv_until(s, '\n')[:-1])

# Phase 1
for i in range(100):
	recv_until(s, 'Host: ')
	host = recv_until(s, '\n')[:-1]
	recv_until(s, 'Subnet: ')
	s.send(getValidSubnet(host) + '\n')
print (recv_until(s, '\n')[:-1])

# Phase 2
for i in range(100):
	recv_until(s, 'Subnet: ')
	subnet = recv_until(s, '\n')[:-1]
	recv_until(s, 'Number of Hosts: ')
	s.send(countHosts(subnet) + '\n')
print (recv_until(s, '\n')[:-1])

# Phase 3
for i in range(100):
	recv_until(s, 'Subnet: ')
	subnet = recv_until(s, '\n')[:-1]
	recv_until(s, 'Host: ')
	host = recv_until(s, '\n')[:-1]
	s.send(isSubnetValid(subnet, host) + '\n')
print (recv_until(s, '\n')[:-1])

s.close()
