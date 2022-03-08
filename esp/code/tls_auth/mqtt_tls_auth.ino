#include <WiFi.h>
//#include <WiFiClientSecure.h>  //included WiFiClientSecure does not work!
#include "src/dependencies/WiFiClientSecure/WiFiClientSecure.h" //using older WiFiClientSecure
#include <time.h>
#include <PubSubClient.h>
#include<ArduinoJson.h>
//#include "secrets.h"

#ifndef SECRET
  const char ssid[] = "Mikrofab_01";
  const char pass[] = "mikrofoo123";

  #define HOSTNAME "192.168.88.100"

  const char *MQTT_HOST = "192.168.88.100";
  const int MQTT_PORT = 8883;
  const char *MQTT_USER = ""; // leave blank if no credentials used
  const char *MQTT_PASS = ""; // leave blank if no credentials used

  const char* local_root_ca = \
    "-----BEGIN CERTIFICATE-----\n" \
    "MIIDrzCCApegAwIBAgIUF+YLxfGaPPn26VARAUtsR7uflf0wDQYJKoZIhvcNAQEL\n" \
    "BQAwZzELMAkGA1UEBhMCREUxCzAJBgNVBAgMAkJXMRUwEwYDVQQHDAxGcmV1ZGVu\n" \
    "c3RhZHQxDDAKBgNVBAoMA0NTVzENMAsGA1UECwwEcm9vdDEXMBUGA1UEAwwOMTky\n" \
    "LjE2OC44OC4xMDAwHhcNMjIwMzA0MTQyNjA4WhcNMjIwNDAzMTQyNjA4WjBnMQsw\n" \
    "CQYDVQQGEwJERTELMAkGA1UECAwCQlcxFTATBgNVBAcMDEZyZXVkZW5zdGFkdDEM\n" \
    "MAoGA1UECgwDQ1NXMQ0wCwYDVQQLDARyb290MRcwFQYDVQQDDA4xOTIuMTY4Ljg4\n" \
    "LjEwMDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAOcbb3XW1toKze9o\n" \
    "SsP6LkkRXdm329LUfJtHoOTa5eVIj4e3sOTAfvII1w8jDRPvib2qrMajwYC8mSB6\n" \
    "0kIofUMQMUcDSWFs1IxozdK5gj6i7eTSLulbrfszE7IowoVlvFGSbzCaB9XMWkSx\n" \
    "rRVCHJKJFC+pQPc8Bwdtc9RpHBL0tk9GQ6vR2M2V5hnHDmn/NEpT1pFEQz0daYkr\n" \
    "CbyKW+N0ZvUlOPmXfVKjp51rze2yrRkr3JVy5+YH6BE3mlIiohgGht0jyn+uJdmK\n" \
    "hkBgYHChc06JakAQnB/V1TfcFJFDha9R1M+YQdD+rwbEU7nmGoA3B8CQVXwTcicv\n" \
    "4V97pikCAwEAAaNTMFEwHQYDVR0OBBYEFIzbYkLVXmum5wo7M7MOOHExax7UMB8G\n" \
    "A1UdIwQYMBaAFIzbYkLVXmum5wo7M7MOOHExax7UMA8GA1UdEwEB/wQFMAMBAf8w\n" \
    "DQYJKoZIhvcNAQELBQADggEBALN7VsJ0EaRjA9jBDOSPpkABqLqGazxTx5JEPjxu\n" \
    "BCFfIfSPZ+OahS0QoZkT8WGogCL8YPbtyCBa0X05Z/Sn7c5xjES6p4UQpCZ+LmAI\n" \
    "nw+auGPG5PtmLXtzSLy8rWBo354RWrCnaUhRiuqgfyS5VO6CPn09oUyYoZAOG5sp\n" \
    "BlSpFZ37oIXFAF1D3iwnaXsA1Sn+0GFCJYVR1AsZfvxHBwZXDi3Kxh/6aJs0TjCw\n" \
    "nVepxs7XVR7g3KaVNMbsFeojQM8gZmy8/4VkhjY+MosCVRBsvE+mQyWLcofkOmD5\n" \
    "DlSo0MmPxEa8M6OhBYgiRWv7x2aS0Kv1TeW4wycj463KsYA=\n" \
    "-----END CERTIFICATE-----";

  const char* local_client_crt = \
    "-----BEGIN CERTIFICATE-----\n" \
    "MIIDVzCCAj8CFER6g3NDASyUIbHn8TW/HUIYLlHyMA0GCSqGSIb3DQEBCwUAMGcx\n" \
    "CzAJBgNVBAYTAkRFMQswCQYDVQQIDAJCVzEVMBMGA1UEBwwMRnJldWRlbnN0YWR0\n" \
    "MQwwCgYDVQQKDANDU1cxDTALBgNVBAsMBHJvb3QxFzAVBgNVBAMMDjE5Mi4xNjgu\n" \
    "ODguMTAwMB4XDTIyMDMwNDE0MjYxMloXDTIyMDQwMzE0MjYxMlowaTELMAkGA1UE\n" \
    "BhMCREUxCzAJBgNVBAgMAkJXMRUwEwYDVQQHDAxGcmV1ZGVuc3RhZHQxDDAKBgNV\n" \
    "BAoMA0NTVzEPMA0GA1UECwwGY2xpZW50MRcwFQYDVQQDDA4xOTIuMTY4Ljg4LjEw\n" \
    "MDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMCSreeZjAxqBjIXZCdM\n" \
    "vrdcXT1iZd5HvlA4sCcbshE+wvRMKgBvF1tTAQCn6HX8iRMe8008TTWMaXcBhzEV\n" \
    "VriaWtEj25z7o6iHCzPXPj4CKl6eMIQL6phHLTaChSijVwfRsLdooRb+fSYgO+tw\n" \
    "GxJaJu8n2FJV59H2hFgsiByFaFpkT7CINFyTSIo4bm1lezJL/MTvT8olMWvh3WhX\n" \
    "yphDyvF8aOAkWVspo1MJA7eYexFPat0W8TBbRdgYVPr2nlKXficuiexM6Q/T7/V+\n" \
    "wppmDYf0xHQGzahkmlBjBFj4aPPLNGRCKvFMbOxwk544RA8itN/dALwjcqWtt8a3\n" \
    "RycCAwEAATANBgkqhkiG9w0BAQsFAAOCAQEALVfyqZS1yiaesL7DZT6CDXhDKbhL\n" \
    "Lb4W02qjJzuacwn9x6MJa+ooRlhW7CSgkLZUb9It3L/ayKdUhXeUzDoR1pyIgxhK\n" \
    "rGO+A2QZOsyeCiE6gqwEf/4VZuObJnqw7fLphaYiwxz7qBPLcaic8L1mf3SxlluR\n" \
    "J3r6kwR2wXwj5wv7eRg+pKSehP1WnuIPxiAl5PSPh3LJtMGNYZGtArofK2cS0c87\n" \
    "5WGrhqKX2hENPDhEk1xEm5+JYMcxoHiPaj8Erm9RYAPnfA1WERd0lCzdrxTI88nM\n" \
    "bX2xvXdHjoZ+mcwFhE6898QisPQwv98lbY9hzZDXlv/sulggasxOAdN6PA==\n" \
    "-----END CERTIFICATE-----";

  const char* local_client_key = \
    "-----BEGIN PRIVATE KEY-----\n" \
    "MIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQDAkq3nmYwMagYy\n" \
    "F2QnTL63XF09YmXeR75QOLAnG7IRPsL0TCoAbxdbUwEAp+h1/IkTHvNNPE01jGl3\n" \
    "AYcxFVa4mlrRI9uc+6Oohwsz1z4+AipenjCEC+qYRy02goUoo1cH0bC3aKEW/n0m\n" \
    "IDvrcBsSWibvJ9hSVefR9oRYLIgchWhaZE+wiDRck0iKOG5tZXsyS/zE70/KJTFr\n" \
    "4d1oV8qYQ8rxfGjgJFlbKaNTCQO3mHsRT2rdFvEwW0XYGFT69p5Sl34nLonsTOkP\n" \
    "0+/1fsKaZg2H9MR0Bs2oZJpQYwRY+GjzyzRkQirxTGzscJOeOEQPIrTf3QC8I3Kl\n" \
    "rbfGt0cnAgMBAAECggEAGQ+9xCNOf5sYvVIf/aFWPKSvmbqBiWEK2oPWIPDQxr/F\n" \
    "PWfgG3hZsM/nlH9FmH1KFYv0m/uaDLr+SrePMYsWYyXrZcZl4VM6ZEdW5ONBPW6o\n" \
    "tvGdXMH0QOcacjqgO76uJ91ZQzVBqa1q+hg91BEd5NCIup0T4Ps/FSsg5By/hf54\n" \
    "q9E84LYzhW42bZyeO7d5BhJDeMQpuYyqen94fzyGNO0Hm2iOYgQhSQVspyWjSANW\n" \
    "PCoSK8PkbI6OmJmMGivRYi5oPUIjzcZrFZLcayBkuYMa9HgMoZ7tnFoEa8OwZIyS\n" \
    "DLaBG5yPSMUS+9c25uRg5delD45mFnsCoq9+Me1iSQKBgQDxlIaIXDD5m+V+Z50o\n" \
    "ufRnT+873CTdRuxX4tKpNyXJlF0WUvmc6po/TYNhpP5G0dDpft8ePZO9Kalo0RHd\n" \
    "/+eInE13gXAI+rReve6waZdhfuWAvecLeD8+68X1mH1NOSHouq41PyNv0hc9pqKz\n" \
    "xL3lx0JVS70ULM+1k7C3ZKl/FQKBgQDMEUwfsiLQ3rO8ojMvHpMuyqRDeYkHhFIm\n" \
    "2YgirAwNKGwyhRz2FVcYycOATfRVal5MOMYiAaFkk8wINvSN+xdh5AnmmegiCcBf\n" \
    "8EmekzuyVN3LFWv3qk0mAVX0DiC4d8idqNfoLgtnfVMls0s5zVA7slMBJQZFjwYl\n" \
    "StVTLrXcSwKBgD/b7yQkmp2g5C1h3l1hqoWUz7yXGPeVQ+yHfQBWH5sFqQW5sVoV\n" \
    "eei56LzrDHxUAlF2xCwOQvuCA9I9UefJOb8W8jNg9+invP4+XtYrU2TmUmyRxn11\n" \
    "r+JGGPIu+RBsxdXrYxmYMxngA1y8q82xeDFBVvuNJdIrdkydWd1gsp1xAoGAUwgu\n" \
    "uy7WL7qN05ED1oZQSNe3IqVO9joG2zvjwofSl3/9FklHUXG5cKKX45rUcfTHzafE\n" \
    "Jo4XRz9D44vI0gR6rSAGT0gMRd1F40N4EQfDYHpbjIh419r5bR9aA+XDtos2m7/Z\n" \
    "qeUiSseZj1t2nlxF6yRR1cNk2+SoEYBzcFcvNCUCgYEAyWK5NvDp8o/i+YVe92BD\n" \
    "OyULX6Wd/mNpSY5gR+nrT3SCLBJ322K7wqDFJlovxNa28G+8OxxTr9nORWiItQAp\n" \
    "7+aN3bwvnWTHXVCeTDpNuDDYhhMh2OgOl76nF4KDoyzCJdgvnNKEvAUj673vXP9i\n" \
    "QDY0B/Lb/NRZNEokKOvbnWo=\n" \
    "-----END PRIVATE KEY-----";

#endif

const char MQTT_SUB_TOPIC[] = "test/" HOSTNAME "/in";
const char MQTT_PUB_TOPIC[] = "test/" HOSTNAME "/out";
char output[128];
time_t now;

WiFiClientSecure net;
PubSubClient client(net);

/*
 * JSON Data Format
*/
const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(3);
DynamicJsonDocument doc(capacity);
JsonObject data = doc.createNestedObject("data");

void mqtt_connect()
{
    while (!client.connected()) {
    Serial.print("Time:");
    Serial.print(ctime(&now));
    Serial.print("MQTT connecting");
    if (client.connect(HOSTNAME, MQTT_USER, MQTT_PASS)) {
      Serial.println("connected");
      client.subscribe(MQTT_SUB_TOPIC);
    } else {
      Serial.print("failed, status code =");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      /* Wait 5 seconds before retrying */
      delay(5000);
    }
  }
  
}

void receivedCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Received [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
}

void wifi_connect()
{
  if (WiFi.status() != WL_CONNECTED) {
    // WIFI
      Serial.println();
      Serial.print("===> WIFI ---> Connecting to ");
      Serial.println(ssid);

      delay(10);
      WiFi.setHostname(HOSTNAME);
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid, pass);
      //WiFi.config(IPAddress(ip_static), IPAddress(ip_gateway), IPAddress(ip_subnet), IPAddress(ip_dns));

      int Attempt = 0;
      while (WiFi.status() != WL_CONNECTED) {
        Serial.print(". ");
        Serial.print(Attempt);

        delay(100);
        Attempt++;
      if (Attempt == 150)
      {
        Serial.println();
        Serial.println("-----> Could not connect to WIFI");

        ESP.restart();
        delay(200);
      }

    }
      Serial.println();
      Serial.print("===> WiFi connected");
      Serial.print(" ------> IP address: ");
      Serial.println(WiFi.localIP());
    }
  }

void setup()
{
  Serial.begin(115200);

  wifi_connect();

  Serial.print("Setting time using SNTP");
  configTime(-5 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  now = time(nullptr);
  while (now < 1510592825) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));

  net.setCACert(local_root_ca);
  net.setCertificate(local_client_crt);
  net.setPrivateKey(local_client_key);
  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(receivedCallback);
  mqtt_connect();
}

void loop()
{
  now = time(nullptr);
  if (WiFi.status() != WL_CONNECTED)
  {
    wifi_connect();
  }
  else
  {
    if (!client.connected())
    {
      mqtt_connect();
    }
    else
    {
        data["temp"].set(random(20, 25));
        data["humid"].set(random(80, 85));
        data["hall"].set(hallRead());
      
        // Output Format
        serializeJson(doc, output);
        
        Serial.println(output);
        client.publish(MQTT_PUB_TOPIC, output);
        delay(5000); // Publish data every 5 seconds to the Broker
      client.loop();
    }
  }
}
