# Socket

The Socket class manages the creation and closure of a network socket. During instantiation, it configures the server address (IP address and port) using the inet_pton method to validate the IP address. If the address is invalid, an exception is thrown. The close method checks if the socket is valid before closing it and resetting its value.