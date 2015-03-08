============================================
TCSS558 PROJECT 4
2 PHASE COMMIT USING RPC 
Authors: Danny Kristiyanto &
         Kevin E. Anderson
Version: 2015.03.07
===========================================


USAGE:
	 tcss558 [role]  
EXAMPLE:
	 To run the client software, provide client as argument (e.g. tcss558 client) 
	 To run the server software, provide server as argument (e.g. tcss558 server).
NOTE: 
	File serverlist.txt is required. The file should contains the list of available server, one server per line.
	In order for the system to work properly, all servers in the list must be fully responding and accessble from any of the server nodes in the list.  

=========================
CHOOSING CLIENT OR SERVER
=========================
TCSS558 interprets the number of command line entries to determine whether the user wants the client or server.
DO NOT provide extra command line options.  Behavior is not guaranteed in such cases.
RPC is used as the underlying communication method. 

================
USING THE CLIENT
================
Upon launch the user will be offered a selection of operations: 1) PUT 2) GET 3) DEL 4) RUN SCRIPT.
If Option 1,2, and 3, is selected, user will be prompted to input the key, value, and to which server the request will be addressed.
Option 4 will generate a 5 PUT, 5 GET, and 5 DEL to any random server in the list.  
All activity will be stored in the file "client.log."

================
USING THE SERVER
================
The server will run indefinitely until a kill command (or ctr+c) is sent to the program.
No user interaction is provided. Server will display all events and activities.
All activity will be stored in the file "server.log."


===============
SERVER FAILURES
===============
Chaos has been introduced into this version of the distributed system.  For each message
sent to an acceptor or learner there is a 1% chance of failure.  A failure will cause the
program to throw an error to the screen and exit completely.  The system should continue
to work as designed after the timeouts for RPC expire for any messages pending responses
from the failed server.