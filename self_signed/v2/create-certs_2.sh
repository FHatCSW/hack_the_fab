#!/bin/sh

# Generate self signed root CA cert
openssl req -nodes -x509 -newkey rsa:2048 -keyout ca.key -out ca.crt -subj "/C=DE/ST=BW/L=Freudenstadt/O=CSW/OU=Certificate Authority"

## Broker certs

# Generate server cert to be signed
openssl req -nodes -newkey rsa:2048 -keyout broker_server.key -out broker_server.csr -subj "/C=DE/ST=BW/L=Freudenstadt/O=CSW/OU=server/CN=192.168.88.100"

# Sign the server cert
openssl x509 -req -in broker_server.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out broker_server.crt

# Create server PEM file
cat broker_server.key broker_server.crt > broker_server.pem


# Generate client cert to be signed
openssl req -nodes -newkey rsa:2048 -keyout broker_client.key -out broker_client.csr -subj "/C=DE/ST=BW/L=Freudenstadt/O=CSW/OU=client/CN=192.168.88.100"

# Sign the client cert
openssl x509 -req -in broker_client.csr -CA ca.crt -CAkey ca.key -CAserial ca.srl -out broker_client.crt

# Create client PEM file
cat broker_client.key broker_client.crt > broker_client.pem

# Fingerprint of the broker CA
openssl x509 -in  broker_server.crt -sha1 -noout -fingerprint

# Verify Server Certificate
openssl verify -purpose sslserver -CAfile ca.crt broker_server.crt

# Verify Client Certificate
openssl verify -purpose sslclient -CAfile ca.crt broker_client.crt

## Docker certs

# Generate server cert to be signed
openssl req -nodes -newkey rsa:2048 -keyout docker_server.key -out docker_server.csr -subj "/C=DE/ST=BW/L=Freudenstadt/O=CSW/OU=server/CN=192.168.88.100"

# Sign the server cert
openssl x509 -req -in docker_server.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out docker_server.crt

# Create server PEM file
cat docker_server.key docker_server.crt > docker_server.pem


# Generate client cert to be signed
openssl req -nodes -newkey rsa:2048 -keyout docker_client.key -out docker_client.csr -subj "/C=DE/ST=BW/L=Freudenstadt/O=CSW/OU=client/CN=mosquitto"

# Sign the client cert
openssl x509 -req -in docker_client.csr -CA ca.crt -CAkey ca.key -CAserial ca.srl -out docker_client.crt

# Create client PEM file
cat docker_client.key docker_client.crt > docker_client.pem

# Fingerprint of the docker CA
openssl x509 -in  docker_server.crt -sha1 -noout -fingerprint

# Verify Server Certificate
openssl verify -purpose sslserver -CAfile ca.crt docker_server.crt

# Verify Client Certificate
openssl verify -purpose sslclient -CAfile ca.crt docker_client.crt


