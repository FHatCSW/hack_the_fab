# Basic Auth Concept

## Steps to Bring the Prototype Stack Up

1. Create a network for your stack:

        docker network create iotstack

2. Encrypting the Passwords for Mosquitto Broker:

    ```bash
    cd prototype/
    docker run -it --rm -v $(pwd)/mosquitto/config:/mosquitto/config eclipse-mosquitto mosquitto_passwd -U /mosquitto/config/passwd
    ```

    If there is no response from the command the passwords are encrypted. You can see the encrypted passwords using:

        cat mosquitto/config/passwd

3. Bring the stack up:

        USER_ID="$(id -u)" GRP_ID="$(id -g)" docker-compose -f docker-compose.prototype.yml up -d


##  Ports

| Component   | Port  |
| ----------  | ----- |
| `influxdb`  | 8086 (internal)  |
| `telegraf`  | n/a (internal)  |
| `grafana`   | 3000 (internal) |
| `mosquitto` | 1883 (mqtt), 1884 (ws) (internal) |

## Component Level Security

### Mosquitto MQTT Broker

The `mosquitto/config/passwd` file has two users in it:


|   username  |  password  |                         role                         |
|:-----------:|:----------:|:----------------------------------------------------:|
| `pubclient` | `mikrofoo123` | Publishing Data to MQTT Broker. For IoT Sensor Nodes |
| `subclient` | `mikrofoo123` |       Subscribing to MQTT Broker. For Telegraf       |

The file needs to be encrypted in order for the Broker to accept it. Passwords in Mosquitto cannot be plain-text.

See __Step 2__ to encrypt your Mosquitto Broker Passwords.

### Telegraf

The configuration file (`telegraf.toml`) will use the following environment variables to write data into
InfluxDB

    INFLUXDB_USER=mikrofab
    INFLUXDB_USER_PASSWORD=mikrofoo123

The data will be written to a database called `edge` (`INFLUXDB_DB` in `prototype.env`)

Telegraf will use the following environment variables to subscribe to Mosquitto Broker

    TG_MOSQUITTO_USERNAME=subclient
    TG_MOSQUITTO_PASSWORD=mikrofoo123


### InfluxDB

- You can control the admin user and password via `INFLUXDB_ADMIN_USER` and `INFLUXDB_ADMIN_PASSWORD` variables in `basic_auth.env`
> `INFLUXDB_USER` can _have read and write privileges ONLY if_ `INFLUXDB_DB` is assigned. If there is no database assigned then the `INFLUXDB_USER` will not have any privileges.


### Grafana
Grafana container will use the following environment variables to set up an admin account

    GF_ADMIN_USERNAME=admin
    GF_ADMIN_PASSWORD=mikrofoo123


## Mosquitto Websocket Client using Paho-MQTT-Python


```python
import paho.mqtt.client as mqtt
import sys
HOST = '192.168.88.100'
PORT = 1884

CLIENT_ID='mikrofab_client'

def on_connect(mqttc, obj, flags, rc):
    print("rc: "+str(rc))

def on_message(mqttc, obj, msg):
    print(msg.topic+" "+str(msg.qos)+" "+str(msg.payload))

def on_publish(mqttc, obj, mid):
    print("mid: "+str(mid))

def on_subscribe(mqttc, obj, mid, granted_qos):
    print("Subscribed: "+str(mid)+" "+str(granted_qos))

def on_log(mqttc, obj, level, string):
    print(string)

mqttc = mqtt.Client(CLIENT_ID, transport="websockets")


mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_publish = on_publish
mqttc.on_subscribe = on_subscribe
mqttc.on_log = on_log

mqttc.connect(HOST, PORT, 60)

mqttc.subscribe('test/test', 0)

try:
        mqttc.loop_forever()

except KeyboardInterrupt:
        mqttc.loop_stop()
        mqttc.disconnect()
        sys.exit()
```

