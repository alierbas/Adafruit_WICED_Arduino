/*********************************************************************
 This is an example for our Feather WIFI modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

#include <adafruit_feather.h>
#include <adafruit_mqtt.h>
#include "certificate_mosquitto.h"

/* This sketch connect to the public MQTT server (with/without TLS)
 * and publish message to a topic every 5 seconds.
 *
 * For publish server detail see http://test.mosquitto.org/
 *  - Port 1883 : MQTT, unencrypted
 *  - Port 8883 : MQTT, encrypted (TLS)
 *
 * Note: may You need an MQTT desktop client such as
 * - The lightweight Java client included in this repo: org.eclipse.paho.mqtt.utility-1.0.0.jar or
 * - Full desktop client MQTT.fx https://learn.adafruit.com/desktop-mqtt-client-for-adafruit-io/installing-software
 *
 * To run this demo
 * 1. Change SSID/PASS
 * 2. Decide whether you want to use TLS/SSL or not (USE_TLS)
 * 3. Change CLIENTID, TOPIC, PUBLISH_MESSAGE, WILL_MESSAGE if you want
 * 4. Compile and run
 * 5. Use MQTT desktop client to connect to same sever and subscribe to the defined topic
 * to monitor the published message.
 */

#define WLAN_SSID         "yourSSID"
#define WLAN_PASS         "yourPass"

#define USE_TLS           0

#define BROKER_HOST       "test.mosquitto.org"
#define BROKER_PORT       (USE_TLS ? 8883 : 1883 )

// Uncomment to set your own ClientID, otherwise a random ClientID is used
//#define CLIENTID          "Adafruit Feather"

#define TOPIC             "adafruit/feather"
#define WILL_MESSAGE      "Goodbye!!"

AdafruitMQTT      mqtt;
AdafruitMQTTTopic mqttTopic(&mqtt, TOPIC, MQTT_QOS_EXACTLY_ONCE);

char old_value = '0';
char value = '0';

/**************************************************************************/
/*!
    @brief  The setup function runs once when the board comes out of reset
*/
/**************************************************************************/
void setup()
{
  Serial.begin(115200);

  // Wait for the USB serial port to connect. Needed for native USB port only
  while (!Serial) delay(1);

  Serial.println("MQTT Publish using Publisher Example\r\n");

  // Print all software versions
  Feather.printVersions();

  while ( !connectAP() )
  {
    delay(500); // delay between each attempt
  }

  // Connected: Print network info
  Feather.printNetwork();

  // Tell the MQTT client to auto print error codes and halt on errors
  mqtt.err_actions(true, true);

  // Set ClientID if defined
  #ifdef CLIENTID
  mqtt.clientID(CLIENTID);
  #endif

  // Last will must be set before connecting since it is part of the connection data
  mqtt.will(TOPIC, WILL_MESSAGE, MQTT_QOS_AT_LEAST_ONCE);

  // Connect to broker
  Serial.printf("Connecting to " BROKER_HOST " port %d ... ", BROKER_PORT);
  if (USE_TLS)
  {  
    // Disable default RootCA to save SRAM since we don't need to
    // access any other site except test.mosquitto.org
    Feather.useDefaultRootCA(false);

    // mosquitto CA is pre-generated using pycert.py
    Feather.addRootCA(rootca_certs, ROOTCA_CERTS_LEN);

    // Connect with SSL/TLS
    mqtt.connectSSL(BROKER_HOST, BROKER_PORT);
  }else
  {
    mqtt.connect(BROKER_HOST, BROKER_PORT);
  }
  Serial.println("OK");

  // Subscribe with callback
  mqttTopic.subscribe(subscribed_callback);

  Serial.println("Please use desktop client to subcribe to \'" TOPIC "\' to monitor");

  // Inital publish
  Serial.printf("Publishing \'%d\' ... ", value);
  mqttTopic.print( value ); // use .write to send in binary format
  Serial.println("OK");
}

/**************************************************************************/
/*!
    @brief  This loop function runs over and over again
*/
/**************************************************************************/
void loop()
{
  // value changed due to subscribed callback
  if (old_value != value)
  {
    // check if still subscribed
    if ( mqttTopic.subscribed() )
    {
      old_value = value;
      Serial.println();
      Serial.printf("Publishing \'%c\' ... \r\n", value);
      mqttTopic.print( value ); // use .write to send in binary format
    }
  }
}

/**************************************************************************/
/*!
    @brief  MQTT subscribe event callback handler

    @param  topic      The topic causing this callback to fire
    @param  message    The new value associated with 'topic'

    @note   'topic' and 'message' are UTF8Strings (byte array), which means
            they are not null-terminated like C-style strings. You can
            access its data and len using .data & .len, although there is
            also a Serial.print override to handle UTF8String data types.
*/
/**************************************************************************/
void subscribed_callback(UTF8String topic, UTF8String message)
{
  // copy received data to value
  memcpy(&value, message.data, 1);
  
  // Print out topic name and message
  Serial.printf("["); Serial.print(topic); Serial.printf("]");
  Serial.print(" : value = ") ;
  Serial.println(value);

  // increase value by 1
  value++;

  // wrap around
  if (value > '9') value = '0';

  // Unsubscribe if we received an "stop" message
  // Won't be able to echo anymore
  if ( message == "stop" )
  {
    Serial.print("Unsubscribing ... ");
    mqttTopic.unsubscribe(); // Will halt if fails
    Serial.println("OK");
  }
}

/**************************************************************************/
/*!
    @brief  Connect to defined Access Point
*/
/**************************************************************************/
bool connectAP(void)
{
  // Attempt to connect to an AP
  Serial.print("Attempting to connect to: ");
  Serial.println(WLAN_SSID);

  if ( Feather.connect(WLAN_SSID, WLAN_PASS) )
  {
    Serial.println("Connected!");
  }
  else
  {
    Serial.printf("Failed! %s (%d)", Feather.errstr(), Feather.errno());
    Serial.println();
  }
  Serial.println();

  return Feather.connected();
}
