import tkinter as tk
import paho.mqtt.publish as publish
import paho.mqtt.client as mqtt
from paho.mqtt import client as mqtt_client

client_id="mqttx_170be1fb"

def publish_on():
    publish.single("manual_control", "on", hostname="broker.hivemq.com")

def publish_off():
    publish.single("manual_control", "off", hostname="broker.hivemq.com")

def on_subscribe(client, userdata, mid, granted_qos):
    print("Subscribed with QoS", granted_qos[0])

def on_message(client, userdata, msg):
    message = msg.payload.decode("utf-8")
    if msg.topic == "Temperature_G":
        temperature_value.set(message)
    elif msg.topic == "Humidity_G":
        humidity_value.set(message)
    elif msg.topic == "brightness_G":
        brightness_value.set(message)

# Create main window
window = tk.Tk()
window.title("SmartGarden")

# Create widgets for subscribing
temperature_label = tk.Label(window, text="Temperature:")
temperature_value = tk.StringVar()
temperature_entry = tk.Entry(window, textvariable=temperature_value, state='readonly')

humidity_label = tk.Label(window, text="Humidity:")
humidity_value = tk.StringVar()
humidity_entry = tk.Entry(window, textvariable=humidity_value, state='readonly')

brightness_label = tk.Label(window, text="Brightness:")
brightness_value = tk.StringVar()
brightness_entry = tk.Entry(window, textvariable=brightness_value, state='readonly')

# Create buttons for manual control
control_label = tk.Label(window, text="Control:")
on_button = tk.Button(window, text="On", command=publish_on)
off_button = tk.Button(window, text="Off", command=publish_off)

# Arrange widgets using grid
temperature_label.grid(row=0, column=0, padx=10, pady=10)
temperature_entry.grid(row=0, column=1, padx=10, pady=10)

humidity_label.grid(row=1, column=0, padx=10, pady=10)
humidity_entry.grid(row=1, column=1, padx=10, pady=10)

brightness_label.grid(row=2, column=0, padx=10, pady=10)
brightness_entry.grid(row=2, column=1, padx=10, pady=10)

control_label.grid(row=3, column=0, columnspan=2, padx=10, pady=10)
on_button.grid(row=4, column=0, padx=10, pady=10)
off_button.grid(row=4, column=1, padx=10, pady=10)

# Set up MQTT client for subscribing
subscribe_client = mqtt.Client(mqtt_client.CallbackAPIVersion.VERSION1, client_id)
subscribe_client.on_subscribe = on_subscribe
subscribe_client.on_message = on_message
subscribe_client.connect("broker.hivemq.com", 1883, 60)
subscribe_client.subscribe("Temperature_G")
subscribe_client.subscribe("Humidity_G")
subscribe_client.subscribe("brightness_G")

# Start the MQTT loop for the subscriber client
subscribe_client.loop_start()

# Start the GUI event loop
window.mainloop()
