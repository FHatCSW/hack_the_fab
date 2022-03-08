## Creating and implementing self signed certificates

# Step-by-step

- Generate self signed certs according to [this script](https://gist.github.com/suru-dissanaike/fbb01a23cf9a138973732e76999c0d48)
    - Create folders:
        ``` bash
        sudo mkdir -p $HOME/docker/mosquitto/certs
        sudo mkdir -p /etc/mosquitto
        sudo mkdir -p /etc/mosquitto/certs
        ```
    - Adapt code accroding to microfab id:
        - **IMPORTANT**: Somehow the $ signs have a Slash. These need to be deleted in the file
        ``` bash
        #!/bin/bash

        IP="192.168.88.100"
        SUBJECT_CA="/C=DE/ST=Freudenstadt/L=Freudenstadt/O=CampusSchwarzwald/OU=CA/CN=$IP"
        SUBJECT_SERVER="/C=DE/ST=Freudenstadt/L=Freudenstadt/O=CampusSchwarzwald/OU=Server/CN=$IP"
        SUBJECT_CLIENT="/C=DE/ST=Freudenstadt/L=Freudenstadt/O=CampusSchwarzwald/OU=Client/CN=$IP"
        
        function generate_CA () {
           echo "$SUBJECT_CA"
           openssl req -x509 -nodes -sha256 -newkey rsa:2048 -subj "$SUBJECT_CA"  -days 365 -keyout ca.key -out ca.crt
        }
        
        function generate_server () {
           echo "$SUBJECT_SERVER"
           openssl req -nodes -sha256 -new -subj "$SUBJECT_SERVER" -keyout server.key -out server.csr
           openssl x509 -req -sha256 -in server.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out server.crt -days 365
        }
        
        function generate_client () {
           echo "$SUBJECT_CLIENT"
           openssl req -new -nodes -sha256 -subj "$SUBJECT_CLIENT" -out client.csr -keyout client.key 
           openssl x509 -req -sha256 -in client.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out client.crt -days 365
        }
        
        generate_CA
        generate_server
        generate_client
        
        ```
    - save it to a file ```sudo nano create-certs.sh```and execute it ``` bash create-certs.sh```
    - The self signed certificates will be created
    - Modify docker config
        - mosquitto.conf
            - If not existant create mosquitto.conf
                ``` bash
                sudo nano $HOME/docker/mosquitto/conf/mosquitto.conf
                ```
                - Add code:
                    ``` bash
                    port 8883

                    cafile /mosquitto/certs/ca.crt
                    certfile /mosquitto/certs/server.crt
                    keyfile /mosquitto/certs/server.key
                    
                    require_certificate true
                    use_identity_as_username true
                    ```
            - Change docker-compose.yml
                - Change the mosquitto service
                    ``` bash
                      mosquitto:
                        image: 'eclipse-mosquitto:latest'
                        container_name: mosquitto
                        restart: always
                        ports:
                          - '8883:8883'
                        volumes:
                          - /home/pi/docker/mosquitto/conf/mosquitto.conf:/mosquitto/config/mosquitto.conf
                          - /home/pi/docker/mosquitto/data/:/mosquitto/data/
                          - /home/pi/docker/mosquitto/log/:/mosquitto/log/
                          - /home/pi/docker/mosquitto/certs/:/mosquitto/certs/
                    ```
    - Modify telegraf.conf
        - Open file 
            ``` bash
            sudo nano /home/pi/docker/telegraf/telegraf.conf
            ```          
        - Modify MQTT config
            ``` bash
            [[inputs.mqtt_consumer]]
              servers = ["tcp://mosquitto:8883"]
          
            ## Optional TLS Config
              tls_ca = "/etc/mosquitto/certs/ca.crt"
              tls_cert = "/etc/mosquitto/certs/server.crt"
              tls_key = "/etc/mosquitto/certs/server.key"
            ```
    - Copy the certs to your local machine:
        - Make sure pi owns the certs (On Raspberry Pi):
            ``` bash
            sudo chown pi:pi -R /home/pi/docker/mosquitto/certs/
            ```
        - On you Laptop: Copy the files to a local folder ~/tmp_pi_certs
            ``` bash
            scp -r pi@MikrofabControl.local:/home/pi/docker/mosquitto/certs /Users/florianhandke/Downloads/certs
            ```
    - Restart Docker
        ``` bash
        sudo docker-compose down
        sudo docker-compose up -d
        ```
    - Transfer the following files to the ESP32 (following the instructions in [setup_esp.md](esp/setup_esp.md))
        - client.crt
        - client.key
        - ca.crt
    

## Check success

- Test if everything works
    - On your latop terminal
        ``` bash
        mosquitto_pub -h 192.168.88.100 -t test/florian --cafile ca.crt --cert client.crt --key client.key -m "hello world" -p 8883
        ```
        - Everything ok
            - Client
                ``` bash
                Client null sending CONNECT
                Client null received CONNACK (0)
                Client null sending PUBLISH (d0, q0, r0, m1, 'test/florian', ... (11 bytes))
                Client null sending DISCONNECT
                ```
            - Mosquitto Broker
                ``` bash
                1646638776: New connection from 192.168.88.253:54911 on port 8883.
                1646638776: New client connected from 192.168.88.253:54911 as auto-AC30ECCC-6A32-C5D5-D384-E959C58BD629 (p2, c1, k60, u'192.168.88.100').
                1646638776: Client auto-AC30ECCC-6A32-C5D5-D384-E959C58BD629 disconnected.
                ```
        - Something went wrong
            - Client
                ``` bash
                Client null sending CONNECT
                OpenSSL Error[0]: error:1416F086:SSL routines:tls_process_server_certificate:certificate verify failed
                Error: A TLS error occurred.
                ```
            - Mosquitto Broker
                ``` bash
                1646638715: New connection from 192.168.88.253:54875 on port 8883.
                1646638715: OpenSSL Error[0]: error:1404A3F2:SSL routines:ST_ACCEPT:sslv3 alert unexpected message
                1646638715: Client <unknown> disconnected: Protocol error.
                ```
    

- Sources:
    - https://medium.com/himinds/mqtt-broker-with-secure-tls-and-docker-compose-708a6f483c92
    - https://dasraspberrypi.de/transfer-files-raspberry-ssh-3/
    - https://forum.arduino.cc/t/mqtt-and-tls/665171/14
    - https://gist.github.com/eLement87/133cddc5bd0472daf5cb35a20bfd811e
    - https://github.com/debsahu/ESP_MQTT_Secure/blob/master/ESP32_MQTT_SSL/Arduino/ESP32_PubSubClient_SSL/ESP32_PubSubClient_SSL.ino
    - https://www.linuxfixes.com/2021/11/solved-telegraf-connection-to-mosquitto.html