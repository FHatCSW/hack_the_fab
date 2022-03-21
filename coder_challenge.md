![](./src/coder_challenge_.png)

## Details

+ Die von den Maschinen (Client) generierten Daten werden über **MQTT oder OPC UA** an den Leitrechner (Server) gesendet. Genaue Details zu der Architektur findet ihr [hier](micro_fab_concept.md).

+ Die **Kommunikation ist aktuell unverschlüsselt** und es gibt keine sichere Authentisierung

+ OPC UA und MQTT bieten verschiedene Mechanismen für eine sichere Authentisierung und Verschlüsselung. Weitere Details zu OPC UA und MQTT findet ihr [hier](protocols.md)

+ Welche Mechanismen ihr testet und implementiert ist **euch überlassen**. 

## Wifi Zugang

+ Die Zugangsdaten zum Wifi-Router findet ihr auf der Vorderseite des Routers

## Hilfestellung

### Implementierung Zertifikate

### Protokolle

## Zielsystem

Die umgesetzten Sicherheitsmechnaismen beruhen auf Public-Key Kryptographie. 
Der Kern von Public Key Kryptographie besteht aus einer vertrauensvollen Instanz, die Zertifikate für Entitäten wie den Raspberry Pi oder ESP32 austellt, wiederruft oder erneuert. 

### Provisioning
Wie Zertifikate ausgestellt werden veranschaulicht die folgende Grafik, mit dem der ESP32 und RaspberryPi ihre Zertifikate erhalten und verwenden. 

Die vertrauensvolle Instanz bildet die Certificate Authority. 
Nachdem in Schritt 1. der Raspberry Pi einen sog. Certificate Sigining Request  an die Certificate Authority gestellt, hat prüft diese die Gültigkeit der Anfrage.
Im nächsten Schritt stellt diese das das digital signierte Client- und CA-Zertifikat aus. Durch die digitale Signatur wird die Authentizität und Gültigkeit des öffentlichen Schlüssels von der CA bestätigt. 

Der Ausstellungsprozess von Zertifikaten, stellt einen wesentlichen Schritt in einer Public-Key Infrastruktur dar. 
Welches Gerät berechtigt ist, ein Zertifikat zu erhalten, sollte bereits in der Entwicklung definiert werden.

### Authentication
Der gleiche Ablauf wird auch auf dem ESP32 durchgeführt. Somit haben beide Geräte die notwenigen Zertifikate um 

- sich gegenseitig zu authentifizieren,

- die Signaturen der Ca zu überprüfen


![](./src/pki.png)