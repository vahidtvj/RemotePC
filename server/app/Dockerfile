FROM nginx:alpine

# Install required dependencies
# mosquitto is optional
RUN apk add --no-cache openssl mosquitto mosquitto-clients

COPY mosquitto.conf /etc/mosquitto/mosquitto.conf
COPY passwd /etc/mosquitto/passwd

RUN mkdir -p /mosquitto/data /var/log/mosquitto

RUN chown mosquitto:mosquitto /etc/mosquitto/passwd && \
    chmod 640 /etc/mosquitto/passwd

# Expose WebSocket tunnel port
EXPOSE 4000

# Copy entrypoint script
COPY entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

# Set entrypoint
ENTRYPOINT ["/entrypoint.sh"]
