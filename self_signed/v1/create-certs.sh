#!/bin/sh

# Generate self signed root CA cert
openssl req -nodes -x509 -newkey rsa:2048 -keyout ca.key -out ca.crt -subj "/C=DE/ST=BW/L=Freudenstadt/O=CSW/OU=root/CN=192.168.88.100"

# Generate server cert to be signed
openssl req -nodes -newkey rsa:2048 -keyout server.key -out server.csr -subj "/C=DE/ST=BW/L=Freudenstadt/O=CSW/OU=server/CN=192.168.88.100"

# Sign the server cert
openssl x509 -req -in server.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out server.crt

# Create server PEM file
cat server.key server.crt > server.pem


# Generate client cert to be signed
openssl req -nodes -newkey rsa:2048 -keyout client.key -out client.csr -subj "/C=DE/ST=BW/L=Freudenstadt/O=CSW/OU=client/CN=192.168.88.100"

# Sign the client cert
openssl x509 -req -in client.csr -CA ca.crt -CAkey ca.key -CAserial ca.srl -out client.crt

# Create client PEM file
cat client.key client.crt > client.pem

# Verify Server Certificate
openssl verify -purpose sslserver -CAfile ca.crt server.crt

# Verify Client Certificate
openssl verify -purpose sslclient -CAfile ca.crt client.crt

# Fingerprint of the broker CA
openssl x509 -in  server.crt -sha1 -noout -fingerprint