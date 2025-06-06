const mqtt = require('mqtt');

// Connect to the MQTT broker over WebSocket
const client = mqtt.connect('wss://exmple.com/mqtt', {
    username:"*****",
    password:"*****"
}); // Update the URL as needed

client.on('connect', () => {
    console.log('Connected to MQTT broker');
    // Subscribe to the topic
    client.subscribe('/led', (err) => {
        if (err) {
            console.error('Subscription error:', err);
        } else {
            console.log('Subscribed to your/topic');
        }
    });
});

// Listen for messages
client.on('message', (topic, message) => {
    // message is a Buffer
    console.log(`Received message: ${message.toString()} from topic: ${topic}`);
});

// Handle errors
client.on('error', (err) => {
    console.error('Connection error:', err);
});

// Clean up on exit
process.on('SIGINT', () => {
    client.end(() => {
        console.log('Disconnected from MQTT broker');
        process.exit(0);
    });
});
