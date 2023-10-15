#include <array>
#include <cstdio>
#include <iostream>
#include <map>
#include <memory> 
#include <string>  
#include <stdio.h>  
#include <stdlib.h> 
#include <stdexcept>
#include <vector> 
#include <cgicc/CgiDefs.h> 
#include <cgicc/Cgicc.h> 
#include <cgicc/HTTPHTMLHeader.h> 
#include <cgicc/HTMLClasses.h>

std::string exec(const char* cmd) {
  std::array <char, 128> buffer;
  std::string result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
  if (!pipe) {
    throw std::runtime_error("popen() failed");
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}

int main () {
   std::map<std::string, std::string> commands;
   commands["bmp"] = "./bmp180.cgi";
   commands["ina"] = "./ina.cgi";
   commands["relayOn"] = "Relay is ON\n";
   cgicc::Cgicc formData;
   // Set title
   std::cout << "Content-type:text/html\r\n\r\n";
   std::cout << "<html>\n";
   std::cout << "<head>\n";
   std::cout << "<title>Sensors data</title>\n";
   std::cout << "</head>\n";
   std::cout << "<body>\n";       
   // current command
   cgicc::form_iterator fi = formData.getElement("cmd");  
   if( !fi->isEmpty() &&fi != (*formData).end()) {
      if(commands[fi->getValue()][2]=='b') {
         // execute program
         std::cout << "BMP180: ";
         std::cout<<exec(commands[fi->getValue()].c_str())<<std::endl;
         std::cout << "<br/>\n";
      }
      else if (commands[fi->getValue()][2]=='i') {
         // execute program
         std::cout << "INA219: ";
         std::cout<<exec(commands[fi->getValue()].c_str())<<std::endl;
         std::cout << "<br/>\n";
      }
      else {
      // execute another program or bash-script
         std::cout<<commands[fi->getValue()]<<std::endl;
      }
      std::cout << "<br/>\n";    
      std::cout << "Last command was: " << **fi << std::endl;  
   } else {
      std::cout << "Unknown command" << std::endl;  
   }   
   std::cout << "<br/>\n";
   std::cout << "</body>\n";
   std::cout << "</html>\n";
   
   return 0;
}
