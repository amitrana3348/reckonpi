/*
  RFSniffer

  Usage: ./RFSniffer [<pulseLength>]
  [] = optional

  Hacked from http://code.google.com/p/rc-switch/
  by @justy to provide a handy RF code sniffer
*/

#include "../rc-switch/RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <curl/curl.h>

     
     
RCSwitch mySwitch;
 

char mystr[150];// = "http://127.0.0.1:8080/api/rnotify/click/";

void hit_curl(char my[])
{
		CURL *curl;
		 CURLcode res;
		 
		 curl_global_init(CURL_GLOBAL_ALL);
		 curl = curl_easy_init();
		 if (curl == NULL) 
		 {
			printf("Curl Error");
			//return 128;
		 }
		 char* jsonObj = "{}";
		 struct curl_slist *headers = NULL;
		 
		 printf("\n\nPrinting now\n\n");
		
		headers = curl_slist_append(headers, "Accept: application/json");
		headers = curl_slist_append(headers, "Content-Type: application/json");
		headers = curl_slist_append(headers, "charsets: utf-8");

		curl_easy_setopt(curl, CURLOPT_URL, my);
		
		//curl_easy_setopt(curl, CURLOPT_URL, mystr);

		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonObj);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcrp/0.1");

		res = curl_easy_perform(curl);
		printf("URL hit");
		curl_easy_cleanup(curl);
		curl_global_cleanup();
		printf("%i",res);
}
int main(int argc, char *argv[]) 
{
  
     // This pin is not the first pin on the RPi GPIO header!
     // Consult https://projects.drogon.net/raspberry-pi/wiringpi/pins/
     // for more information.
     int PIN = 2;
     
     /********* CURL CODE HERE ************************************/
     
    
     /********* CURL CODE HERE ************************************/
     
     if(wiringPiSetup() == -1) 
     {
       printf("wiringPiSetup failed, exiting...");
       return 0;
     }

     int pulseLength = 0;
     if (argv[1] != NULL) pulseLength = atoi(argv[1]);

     mySwitch = RCSwitch();
     if (pulseLength != 0) mySwitch.setPulseLength(pulseLength);
     mySwitch.enableReceive(PIN);  // Receiver on interrupt 0 => that is pin #2
     
    
     while(1) 
     {
  
      if (mySwitch.available()) 
      {
    
        int value = mySwitch.getReceivedValue();
    
        if (value == 0) 
        {
          printf("Unknown encoding\n");
        } 
        
        else 
        {    
           printf("Received %i\n", mySwitch.getReceivedValue() );
           
           printf("Remote value: %d\n", value );
           sprintf(mystr,"http://127.0.0.1:8080/api/rnotify/click/%i",value);
           printf("%s",mystr);
           hit_curl(mystr);
           
           /*
           curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8080/api/rnotify/click/803640");
			
		    //curl_easy_setopt(curl, CURLOPT_URL, mystr);

		   curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
		   curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		   curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonObj);
		   curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcrp/0.1");

		   res = curl_easy_perform(curl);
		   printf("URL hit");
		   curl_easy_cleanup(curl);
		   curl_global_cleanup();
		   printf("%i",res);*/
        }
    
        fflush(stdout);
        mySwitch.resetAvailable();
      }
      usleep(100); 
  
  }

  exit(0);


}

