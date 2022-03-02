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
        
        function copy_keys_to_broker () {
           sudo cp ca.crt /etc/mosquitto/certs/
           sudo cp server.crt /etc/mosquitto/certs/
           sudo cp server.key /etc/mosquitto/certs/
        }
        
        generate_CA
        generate_server
        generate_client
        copy_keys_to_broker
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

                    cafile /mosquitto/config/certs/ca.crt
                    certfile /mosquitto/config/certs/server.crt
                    keyfile /mosquitto/config/certs/server.key
                    
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
    - Copy the certs to your local machine:
        - Make sure pi owns the certs (On Raspberry Pi):
            ``` bash
            sudo chown pi:pi -R /home/pi/docker/mosquitto/certs/
            ```
        - On you Laptop: Copy the files to a local folder ~/tmp_pi_certs
            ``` bash
            scp -r pi@MikrofabControl.local:/home/pi/docker/mosquitto/certs ~/tmp_pi_certs
            ```
    - Transfer the following files to the ESP32 (following the instructions in [setup_esp.md](setup_esp.md))
        - client.crt
        - client.key
        - ca.crt
    

- Sources:
    - https://medium.com/himinds/mqtt-broker-with-secure-tls-and-docker-compose-708a6f483c92
    - https://dasraspberrypi.de/transfer-files-raspberry-ssh-3/
    - https://forum.arduino.cc/t/mqtt-and-tls/665171/14