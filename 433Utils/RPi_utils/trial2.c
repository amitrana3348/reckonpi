#include <curl/curl.h>


#include "../rc-switch/RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

RCSwitch mySwitch;     

int main (int argc, char *argv[]) {
	
	
	
     // This pin is not the first pin on the RPi GPIO header!
     // Consult https://projects.drogon.net/raspberry-pi/wiringpi/pins/
     // for more information.
     int PIN = 2;
     
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
           URL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl == NULL) {
        return 128;
    }

    //char* jsonObj = "{ \"serviceName\" : \"CALL\" , \"serviceCode\" : \"99099\" ,\"reckonID\" : \"11\" , \"active\" : \"false\" }";
	
	// i have kept this body blank
	char* jsonObj = "{}";
    struct curl_slist *headers = NULL;
    printf("\n\nPrinting now\n\n");
    
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "charsets: utf-8");

    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8080/api/rnotify/click/803640");
    
    //curl_easy_setopt(curl, CURLOPT_URL, mystr);

    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonObj);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcrp/0.1");

    res = curl_easy_perform(curl);

    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return res;
        }
    
        fflush(stdout);
        mySwitch.resetAvailable();
      }
      usleep(100); 
  
  }

	
	
	
}
