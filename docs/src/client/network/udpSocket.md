# UdpSocket

The UdpSocket class manages a UDP socket for sending and receiving data. It inherits from the Socket class and allows for initializing the socket, connecting to the server, sending, and receiving messages through the send and receive functions. It handles errors by throwing exceptions if an operation fails and closes the socket upon destruction.

| Functions                          | Description                                                                                       |
|-------------------------------------|---------------------------------------------------------------------------------------------------|
| init                              | Create a UDP socket using socket(), then save it in the singleton. Throw an exception if the creation fails. |
| send                              | Send a message through the UDP socket using sendto(). Throw an exception if the sending fails.    |
| receive                           | Receive a message through the UDP socket using recvfrom(), then return a SmartBuffer containing the received data. Throw an exception if the reception fails. |