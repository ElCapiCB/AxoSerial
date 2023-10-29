// TESTER.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <AxoSerial.h>




int main(){

    axo::Serial a(19200, 8, ONESTOPBIT, NOPARITY);

    std::vector<std::string> comms;

    comms = a.getComms();
    
    for (unsigned int i = 0; i < comms.size(); i++) {
        std::cout << i << " - " << comms[i] << std::endl;
    }

    std::cout << std::endl << "Seleccione el puerto: ";
    unsigned int selection = 0;

    std::cin >> selection;

    if (selection > comms.size()) {
        std::cout << "[ERROR]: Numero mayor a la cantidad de puertos activos";
        return -1;
    }


    a.init(axo::CharToLPCWSTR(comms[selection].c_str()));

    unsigned char buff[3] = { 0 };
    float val = 0;

    unsigned char code = 0;
    std::cout << "code: ";
    std::cin >> code;
    std::cout << std::endl;

    while (1) {
        a.write(code);

        if (a.available()) {
            a.get(buff, 2);
        }

        val = (float(buff[0] << 8 | buff[1])) * 5.0 / 1023.0;
        std::cout << val << std::endl;

        Sleep(100);
    }
    
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
