#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include "SerialClass.h"
#include <string>
#include <vector>
#include <windows.h>
#include <time.h>
#include <math.h>
#include <fstream>

using namespace std;

string getInput() {
    string out = "";
    for(int i='A'; i<='Z'; i++)
        if(GetKeyState(i) & 0x8000) out +=(i-'A'+'a');
    if(out.size()!=0) return out;
    else return "-";
}

int main(int argc, _TCHAR* argv[]) {

    Serial *SP = new Serial("\\\\\.\\COM6");

    if(SP->IsConnected()) std::cout << "Connected" << std::endl;
    else {
         std::cout<<"Connection error"<< std::endl;
         return 0;
    }

    DWORD bytesSend;
    string last = "";

    while(SP->IsConnected()) {

            string press = getInput();

            if(last!=press) {
                cout << "Sending (" << press << ")..."<< endl;
                string output = press + '\n';

                for(int i=0; i<output.size(); i++)
                    BOOL retVal = WriteFile(SP->hSerial, &output[i],sizeof(char),&bytesSend, NULL);

                ClearCommError(SP->hSerial, &SP->errors, &SP->status);
            }
            last = press;
    }
    cout << "Connection error" << endl;

    return 0;
}
