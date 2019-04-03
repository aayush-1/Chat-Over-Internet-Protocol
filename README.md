# concurrency-10
concurrency-10 created by GitHub Classroom

README

I. FILE LIST
-----------------------------------------------------------------------------------------
server.cpp       -> Code that creates a socket and assigns a port to start the communication and waits for the client to connect</br>
client.cpp       -> Code that establishes connection and communicates with the server</br>
Readme           -> This file </br>
Presentation     -> Low level design </br> 


II. EXECUTION DETAILS
-----------------------------------------------------------------------------------------
For Server: </br>
	Terminal-1 </br> 
	-$g++ server.cpp -o server -lpthread </br>
	-$./server > \<output> </br>

	Terminal-2 
	-$tty    
	<output>                           (check the terminal address and use that in Terminal-1) 


For Client: </br>
	Terminal-1 </br>
	-$g++ client.cpp -o client -lpthread </br>
	-$./client > \<output> </br>

	Terminal-2 
	-$tty    
	<output>                           (check the terminal address and use that in Terminal-1)	
