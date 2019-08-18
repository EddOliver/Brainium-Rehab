import uuid
import json
import paho.mqtt.client as mqtt
import time

time.sleep(2)

# Brain MQTT CREDENTIALS

mqtt_user_name = 'oauth2-user'
mqtt_password = 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'  # copy and paste here external client id from your account
user_id = 'USERNUMBER'  # copy and paste here your user id
device_id = 'XXXXXXXXXXXXXXXXXXXX'  # copy and paste here your device id
ind=""

alerts_topic = '/v1/users/{user_id}/in/alerts'.format(user_id=user_id)
acc_norm_datasource_topic = '/v1/users/{user_id}/in/devices/{device_id}/datasources/MOTION'.format(
    user_id=user_id,
    device_id=device_id)

ca_cert_path = 'cacert.crt'


def on_connect(client, userdata, flags, rc):
    print('Client 1 Connected with result code {code}'.format(code=rc))

def on_disconnect(client, userdata, rc):
    if rc != 0:
        print ("Unexpected MQTT disconnection. Will auto-reconnect")

def on_message(client, userdata, msg):
    global ind
    global mqttc
    ind= json.loads(str(msg.payload.decode("utf-8")))
    print(ind[0]['name'])
    # CLOUD MQTT CREDENTIALS
    client2 = mqtt.Client()
    client2.username_pw_set("USER", "PASSWORD")
    client2.connect("m12.cloudmqtt.com", 12345)
    client2.publish("/ArmTopic",ind[0]['name'])

def main():    
    client1 = mqtt.Client(client_id="7624876298468192648726348726938", transport='websockets')
    client_iad1=str(uuid.uuid4())
    print(client_iad1)
    client1.on_connect = on_connect
    client1.on_message = on_message
    client1.on_disconnect = on_disconnect
    client1.tls_set(ca_certs=ca_cert_path)
    client1.username_pw_set(mqtt_user_name, mqtt_password)
    client1.connect('ns01-wss.brainium.com', 443)
    client1.subscribe(acc_norm_datasource_topic)
    client1.loop_forever()

if __name__ == "__main__":
    main()