#!/usr/bin/env python

import socket

def recv_until(conn, str):
	buf = ''
	while not str in buf:
		buf += conn.recv(1)
	return buf

def getValidSubnet(host):
	return '0.0.0.0/0'

def countHosts(subnet):
	return str((2)**(32-int(subnet.split("/")[1])))

def isSubnetValid(subnet, host):
	splitres = subnet.split("/")
	prefix = int(splitres[1])
	
	arrSubnet = list(map(int, splitres[0].split(".")))	
	bitSubnet = []
	for a in arrSubnet:
		bitSubnet.extend(list(format(a, '08b')))

	arrHost = list(map(int, host.split(".")))
	bitHost = []
	for a in arrHost:
		bitHost.extend(list(format(a, '08b')))
	
	for i in range(0, prefix):
		if (bitSubnet[i] != bitHost[i]):
			return 'F'

	return 'T'
	
TCP_IP = 'hmif.cf'
TCP_PORT = 9999

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
	print (i, host, getValidSubnet(host))
print (recv_until(s, '\n')[:-1])

# Phase 2
for i in range(100):
	recv_until(s, 'Subnet: ')
	subnet = recv_until(s, '\n')[:-1]
	recv_until(s, 'Number of Hosts: ')
	s.send(countHosts(subnet) + '\n')
	print (i, subnet, countHosts(subnet))
print (recv_until(s, '\n')[:-1])

# Phase 3
for i in range(100):
	recv_until(s, 'Subnet: ')
	subnet = recv_until(s, '\n')[:-1]
	recv_until(s, 'Host: ')
	host = recv_until(s, '\n')[:-1]
	s.send(isSubnetValid(subnet, host) + '\n')
	print (i, subnet, host, isSubnetValid(subnet, host))
print (recv_until(s, '\n')[:-1])

s.close()
