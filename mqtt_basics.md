
## Das wichtigste über MQTT

1. Das MQTT-Protokoll basiert auf TCP/IP und sowohl der Client als auch der Broker benötigen einen TCP/IP-Stack.

2. Sowohl Publisher als auch Subscriber werden als MQTT-Client betrachtet.

3. Die MQTT-Verbindung wird immer zwischen einem Client und dem Broker hergestellt, kein Client ist direkt mit einem anderen Client verbunden.

4. Sobald die Verbindung hergestellt ist, hält der Broker sie so lange offen, bis der Client einen Trennungsbefehl sendet oder die Verbindung abbricht.

5. MQTT unterstützt Geräte hinter einem NAT-Gerät (z.B. Router oder Firewall).

6. Der Client Identifier (kurz ClientId) ist ein Identifikator für jeden MQTT-Client, der sich mit einem MQTT-Broker verbindet.

7. Das Keep Alive ist ein Zeitintervall, zu dem sich die Clients verpflichten, indem sie regelmäßig PING Request Nachrichten an den Broker senden. Der Broker antwortet mit einer PING-Antwort und dieser Mechanismus ermöglicht es beiden Seiten festzustellen, ob die andere Seite noch am Leben und erreichbar ist.

8. Die Verbindung wird eingeleitet, indem ein Client eine "Connect Command"-Nachricht an den Broker sendet. Der Broker antwortet mit einem "Connect Ack" und einem Statuscode. In der folgenden Tabelle sehen Sie alle Antwortcodes auf einen Blick.

## Wie steht es um die Sicherheit von MQTT

Die Authentisierung bei MQTT ist völlig optional, und selbst wenn eine Authentisierung durchgeführt wird, wird standardmäßig keine Verschlüsselung verwendet. 
Die Folge: Credentials werden im Klartext kommuniziert.
Mit einem Man-in-the-Middle Angriff können zudem leicht Passwörter ermittelt werden um das System zu kompromittieren.


Einen Überblick über unsichere Implementierungen von MQTT kann [shodan](shodan.io) bieten. Wenn wir hier nach `port:1883 MQTT` sind weltweit 367.846 Clients verteilt, die unverschlüsselt kommunizieren.

![](./src/shodan_mqtt.png)
[Quelle: shodan.io am 10.03.2022]

### Was sagt die MQTT org dazu

> MQTT Client and Server implementations SHOULD offer Authentication, Authorization and secure communication options, such as those discussed in Chapter 5. 
Applications concerned with critical infrastructure, personally identifiable information, or other personal or sensitive information are strongly advised to use these security capabilities.
    [[mqtt.org](https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Toc3901014)]



Wie bei anderen Protokollen wie OPC UA bietet MQTT die Möglichkeit einer sicheren Implementierung. Die Umsetzung von Sicherheit ist allerdings optional.

Selbst wenn eine Authentifizierung (user:password) durchgeführt wird, wird standardmäßig keine Verschlüsselung verwendet 



  
### Subscribe to every topic

``` python
import paho.mqtt.client as mqtt
import time
import os

HOST = "127.0.0.1"
PORT = 1883

def on_connect(client, userdata, flags, rc):
	client.subscribe('#', qos=1)
	client.subscribe('$SYS/#')

def on_message(client, userdata, message):
	print('Topic: %s | QOS: %s  | Message: %s' % (message.topic, message.qos, message.payload))

def main():
	client = mqtt.Client()
	client.on_connect = on_connect
	client.on_message = on_message
	client.connect(HOST, PORT)
	client.loop_start()
	time.sleep(10)
	#client.loop_stop()

if __name__ == "__main__":
	main()
```