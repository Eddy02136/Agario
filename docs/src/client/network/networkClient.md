# NetworkClient

The NetworkClient class manages network communication for the client, handling both TCP and UDP connections. It initializes and connects the sockets, and runs two separate threads to handle incoming messages for TCP and UDP protocols. It uses the SmartBuffer to send and receive messages, and relies on the Protocol class to handle the messages based on their operation codes. Errors during communication are logged using the Logger class.

| Functions                          | Description                                                                                       |
|-------------------------------------|---------------------------------------------------------------------------------------------------|
| init                              | Initializes both the TCP and UDP sockets by calling their init() methods.                        |
| connectTCP                        | Establishes a TCP connection to the server and logs the connection status.                       |
| connectUDP                       | Initializes UDP communication and sends a default message to the server.                         |
| run                             | Creates and runs two threads to handle TCP and UDP messages concurrently.                         |
| handleTcpMessages                 | Continuously receives and processes TCP messages in a loop, handling any exceptions with error logging. |
| handleUdpMessages                 | Continuously receives and processes UDP messages in a loop, handling exceptions by logging errors. |