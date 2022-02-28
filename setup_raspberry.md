
## Step-by-step

- Install Raspberry Pi OS Lite
    - goto boot device
        - add new file: ssh
	- insert SD card in raspberry -> Power up
	    - login: ssh pi@raspberrypi.local
	    - pw: raspberry
    - Unblock WiFi from rfkill (Check with rfkill list)
        ``` bash
        rfkill unblock 0
        ```
    - Add new user & password
        ``` bash
        sudo useradd -m mikrofab_user_pi -G sudo
	    sudo passwd mikrofab_user_pi
        ```
    - Change Hostname
        ``` bash
        sudo raspi-config
        ```
	    - Hostname -> MikrofabControl -> OK
        ``` bash
        sudo reboot
        ```
    - Login via ssh
        ``` bash
        ssh mikrofab_user_pi@MikrofabControl.local
        ```
    - Delete pi user:
        ``` bash
        sudo deluser -f pi
        sudo rm -Rf /home/pi
        ```
      
- Configure WiFi:
    - Open config file:
        ```bash
        sudo nano /etc/wpa_supplicant/wpa_supplicant.conf
        ```
    - Add following lines:
        ```bash
        ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
        update_config=1
        country=DE
      
        network={
            ssid="Mikrofab_01"
            psk="WLAN-PASSWORD"
        }
        ```
    - Add static IP
        - Open config file
            ``` bash
            sudo nano /etc/dhcpcd.conf 
            ```   
        - Set static IP
            ```bash
            interface wlan0
            static ip_address=192.168.88.100/24
            static routers=192.168.88.1 
            ```
        - Deactivate energy save mode
            ``` bash
            allow-hotplug wlan0
            iface wlan0 inet manual
            wireless-power off
            ```
    - Reboot
        ``` bash
        sudo reboot
        ```
- Update & Upgrade
    ```bash
    sudo apt-get update && sudo apt-get upgrade
    ```
- Install Docker
    ```bash
    curl -sSL https://get.docker.com | sh
    ```
- Add a Non-Root User to the Docker Group
    - Add users to the Docker group
        ```bash
        sudo usermod -aG docker [user_name]
        ```
    - Add permissions to the current user
        ```bash
        sudo usermod -aG docker ${USER}
        ```
    - Check if running
        ```bash
        groups ${USER}
        ```
      
- Install Docker-Compose
    ``` bash
    sudo apt-get install -y python3-pip
    sudo pip3 install docker-compose
    docker-compose version
    ```
- Reboot Raspberry

    ``` bash
    sudo reboot
    ```
  
- Install and configure Mosquitto:

    - Download and install
        ``` bash
        sudo docker pull eclipse-mosquitto
        ```
    - Start the Mosquitto broker
        ``` bash
        sudo docker run -it -p 1883:1883 -p 9001:9001 eclipse-mosquitto
        ```
  
- Install and configure InfluxDB

    - Download and install
        ```bash
        sudo docker pull influxdb
        ``
  
    - Start InfluxDB
        ``` bash
        sudo docker run -d -p 8086:8086 
        ```
    - The database will be started as daemon and the data will be stored in /var/lib/influxdb
    
    - Create Database and user
    
        - Start InfluxDB CLI
            ``` bash
            docker exec -it influxdb influx
            ```
            - Create Database machines
                ``` bash
                create database sensors
                ```
            - Create username & password
                ``` bash
                create user "telegraf" with password "telegraf"
                ```
            - Give user telegraf access to the database
                ``` bash
                grant all on sensors to telegraf
                ```
- Install and configure Telegraf

    - Download and install
        ``` bash
        sudo docker pull telegraf
        ```
      
    - Configuration (Create default)
        ``` bash
        sudo docker run --rm telegraf telegraf config > telegraf.conf
        ```
      
    - Change configuration
        - Open configuration
            ``` bash
            nano telegraf.conf
            ```
        - Change telegraf configuration
            ``` bash
            servers = ["tcp://raspberry_pi_ip:1883"]
            topics = [
              "sensors"
            ]
            data_format = "influx"
            ```     
        - Change output configuration
            ``` bash
            urls = ["http://raspberry_pi_ip:8086"]
            database = "sensors"
            skip_database_creation = true
            username = "telegraf"
            password = "telegraf"
            ```
          
    - Run Telegraf
        ``` bash
         sudo docker run  -v /home/pi/:/etc/telegraf:ro telegraf
        ```
      
- Install and configure Grafana

    - Download and install
        ``` bash
        sudo docker pull grafana/grafana
        ```
      
- Test Connection between Telegraf, InfluxDB and Mosquitto
    - Publish
        ``` curl
        curl -d 75 mqtt://example.com/home/bedroom/dimmer
        ```
    - Check Database
        ``` bash
        sudo docker exec -it influxdb influx
        ```
    
Sources:

https://www.eydam-prototyping.com/2021/01/09/smart-home-zentrale-mit-dem-raspberry-pi/