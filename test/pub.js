const mqtt = require('mqtt');

const state = process.argv[2] || 'on';
// Connect to the MQTT broker over WebSocket
const client = mqtt.connect('wss://exmple.com/mqtt', {
    username:"*****",
    password:"*****"
}); // Update the URL as needed

client.on('connect', () => {
    console.log('Connected to MQTT broker');
    client.publish('/led', state, (err) => {
        if (err) {
            console.error('Publish error:', err);
        } else {
            console.log('Message published');
        }
        client.end(); // Close the connection
    });
});

client.on('error', (err) => {
    console.error('Connection error:', err);
});
