AUTHOR: Noah Cribelar
INTENDED USE: CS 371 Client-Server Application for Chat

HOW TO COMPILE:

There are two different files to compile, therefore there are two different compilation commands.

To compile the server.cpp file, run "g++ -o runServer server.cpp" in the command line.

To compile the client.cpp file, run "g++ -o runClient client.cpp" in the command line.

This will produce two executables, runServer and runClient.

HOW TO EXECUTE:

To execute the server-side program, run "./runServer 127.0.0.1 12344" in the command line. The 12344 is the port number, 12344 worked best for me. 127.0.0.1 is obviously localhost IP address, but it can be any IP address your machine can access and prompt as from the network. Make sure that your IP address that you use is on the same network as both machines being used.

To execute the client-side program, run "./runClient 127.0.0.1 12344" in the command. Again, 12344 is the port number and can be changed if needed as well, as well as 127.0.0.1 being the same localhost. 

First, execute the server-side program, as the client needs something to connect to.
Next, execute the client-side program, which will then connect if you have used a valid IP address for your machine and the same port number.

The client will initiate the first message, after which the server can respond, then the client can respond back, and so on.
If the client wants to close the connection, simply type in !QUIT on the client-side connection and send it. The client will close connection and the server will wait for a new connection.

The server can also close the connection in my implementation of the program, because I think it's important for the server to actually be able to close connections to its own machine. Again, use !QUIT.

Since the server is always supposed to wait for an incoming connection after starting up, the only way to close the program running the server application is by using Ctrl-C. No other specifications were made in the documentation of how to set up the application.



