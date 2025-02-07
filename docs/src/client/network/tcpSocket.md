# TcpSocket

The TcpSocket class represents a TCP socket that allows connecting to a server, sending, and receiving data. It inherits from the Socket class, with specific methods to initialize the socket (init), establish a connection (connect), send data (send), and receive data (receive). It handles errors by throwing exceptions in case of network operation failures and uses a singleton to store the active TCP socket.

| Functions                          | Description                                                                                       |
|-------------------------------------|---------------------------------------------------------------------------------------------------|
| init                              | Create a TCP socket using socket() and save it in the singleton. Throw an exception in case of creation failure. |
| connect                           | Try to connect to the TCP server using connect(). Throw an exception if the connection fails.     |
| send                              | Send a message through the TCP socket using send(). Throw an exception if the sending fails.      |
| receive                           | Receive a message through the TCP socket using recv(), then return a SmartBuffer containing the received data. Throw an exception if the reception fails. |
