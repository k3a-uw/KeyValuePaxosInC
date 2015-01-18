/*
 ============================================================================
 Name        : main.c
 Author      : Kevin Anderson <k3a@uw.edu> & Daniel Kristiyanto <danielkr@uw.edu>
 Version     : 2015.01.10
 Description : TCSS558 Project 1
 ============================================================================
 */

#ifndef MAIN_H
  #include "main.h"
#endif

int main(int argc, char *argv[]) {
	// to test udp server
//	udp_server_main(argc, argv);

	// to test a TCP SERVER
	//tcp_server_main(argc, argv);

	// to test a UDP CLIENT
	//udp_client_main(argc, argv);

	// to test a TCP CLIENT
	tcp_client_main(argc, argv);

/*  OLD CODE THAT TESTS THE RUN ARGUMENTS
	if (strcmp(argv[0], "client") == 0)
	{
		client_main();
	} else if (strcmp(argv[0], "client") == 0) {
		server_main();
	} else {
		puts("Invalid Run Configuration.");
		return -1;
	}
*/
}




int test_kv()
{
	// CONSTRUCT A NEW LIST
	kv * my_list = kv_new();

	// FILL THE LIST WITH SOME NEW KEYS
	for (int i = 0; i < 16; i++)
		kv_put(my_list, i*2, i * i);

	// DISPLAY WHAT IS IN THE CURRENT LIST
	kv_print(my_list);

	// TEST DELETE, PUT AND GET
	printf("===========================\n");
	printf("Removing the value from 10\n\n");

	kv_del(my_list, 10);
	kv_put(my_list, 10, -100);
	kv_print(my_list);
	int err; //ERROR FOR GET NEEDS FINALIZED.
	printf("And now getting what is in 14: [%d]", kv_get(my_list, 14, &err));

}
