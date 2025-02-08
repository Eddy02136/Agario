# Network

The Network class manages network communication for the client, handling TCP connections. It initializes and connects the sockets, and runs on a separate thread to handle incoming messages for the TCP protocol. It uses the SmartBuffer to send and receive messages, and relies on the Protocol class to handle the messages based on their operation codes.

| Function                 | Description                                                                                      |
|--------------------------|--------------------------------------------------------------------------------------------------|
| **connectToServer**        | Establishes a TCP connection to the server and logs the connection status.                       |
| **handleMessages**         | Continuously receives and processes TCP messages in a loop, handling any exceptions with error logging. |
| **receiveData**            | Receives a message from the server using the SmartBuffer. |
| **sendData**               | Sends a message to the server using the SmartBuffer. |