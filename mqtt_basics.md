
1. Das MQTT-Protokoll basiert auf TCP/IP und sowohl der Client als auch der Broker benötigen einen TCP/IP-Stack.

2. Sowohl Publisher als auch Subscriber werden als MQTT-Client betrachtet.

3. Die MQTT-Verbindung wird immer zwischen einem Client und dem Broker hergestellt, kein Client ist direkt mit einem anderen Client verbunden.

4. Sobald die Verbindung hergestellt ist, hält der Broker sie so lange offen, bis der Client einen Trennungsbefehl sendet oder die Verbindung abbricht.

5. MQTT unterstützt Geräte hinter einem NAT-Gerät (z.B. Router oder Firewall).

6. Der Client Identifier (kurz ClientId) ist ein Identifikator für jeden MQTT-Client, der sich mit einem MQTT-Broker verbindet.

7. Das Keep Alive ist ein Zeitintervall, zu dem sich die Clients verpflichten, indem sie regelmäßig PING Request Nachrichten an den Broker senden. Der Broker antwortet mit einer PING-Antwort und dieser Mechanismus ermöglicht es beiden Seiten festzustellen, ob die andere Seite noch am Leben und erreichbar ist.

8. Die Verbindung wird eingeleitet, indem ein Client eine "Connect Command"-Nachricht an den Broker sendet. Der Broker antwortet mit einem "Connect Ack" und einem Statuscode. In der folgenden Tabelle sehen Sie alle Antwortcodes auf einen Blick.

