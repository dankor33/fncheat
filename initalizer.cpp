#include "initalizer.hpp"

#include "render.hpp"
#include "settings.hpp"

#include "driver.hpp"
#include "vdm_ctx/vdm_ctx.hpp"
#include "drv_image/drv_image.hpp"

#include <Windows.h>
#include <Uxtheme.h>
#include <dwmapi.h>

#include <iostream>
#include <iomanip>
#include <ctime>
#include <sapi.h> 
#include <thread>

using namespace std;

void textToSpeech(const std::string& text) {
    ISpVoice* pVoice = NULL;
    HRESULT hr = CoInitialize(NULL);
    if (SUCCEEDED(hr)) {
        hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);
        if (SUCCEEDED(hr)) {
            std::wstring wText(text.begin(), text.end()); // any shit added by laxiorr
            pVoice->Speak(wText.c_str(), SPF_DEFAULT, NULL);
            pVoice->Release();
        }
        CoUninitialize();
    }
}

void setTransparency(int transparency) { // any shit added by laxiorr// any shit added by laxiorr// any shit added by laxiorr// any shit added by laxiorr// any shit added by laxiorr
    HWND hwnd = GetConsoleWindow();
    if (hwnd == NULL) {
        std::cerr << "Error: Could not get console window handle." << std::endl;
        return;
    }
    LONG_PTR style = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    SetWindowLongPtr(hwnd, GWL_EXSTYLE, style | WS_EX_LAYERED);
    SetLayeredWindowAttributes(hwnd, 0, transparency, LWA_ALPHA);
}

void cr3_loop()
{
    for (;;)
    {
        mem::fetch_cr3();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}


bool initialize() {

    setTransparency(200); // any shit added by laxiorr

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    // initialize dll import hooks
    Star_Mouse();
    // set the process id to Fortnite
    if (FindWindowA_Spoofed(0, "Fortnite  "))
        mem::process_id = mem::find_process("FortniteClient-Win64-Shipping.exe");

    // load our driver
  //  load_driver();

    std::cout << " WELCOME IN SHIT PASTA" << std::endl;
    textToSpeech(" WELCOME IN SHIT PASTA"); // any shit added by laxiorr

    // check if the driver is found
    if (!mem::find_driver()) 
    {
        SetConsoleTextAttribute(h, 5);
        std::cout << "[";
        SetConsoleTextAttribute(h, 15);
        std::cout << "+";
        SetConsoleTextAttribute(h, 5);
        std::cout << "]";

        SetConsoleTextAttribute(h, 15);
        std::cout << " Driver wasn't found. Contact support in the server for more information." << std::endl;
        textToSpeech(" Driver wasn't found. Contact support in the server for more information."); // any shit added by laxiorr
    }

    // sigma rizz balls
    SetConsoleTextAttribute(h, 5);
    std::cout << "[";
    SetConsoleTextAttribute(h, 15);
    std::cout << "+";
    SetConsoleTextAttribute(h, 5);
    std::cout << "]";

    SetConsoleTextAttribute(h, 15);
    std::cout << " Press ENTER to start cheat initialization. (IN LOBBY)\n";
    textToSpeech(" Press ENTER to start cheat initialization. (IN LOBBY)"); // any shit added by laxiorr
   // 
    //std::cin.get();
    //cin.get();
    system("pause");

    SetConsoleTextAttribute(h, 5);
    std::cout << "\n[";
    SetConsoleTextAttribute(h, 15);
    std::cout << "+";
    SetConsoleTextAttribute(h, 5);
    std::cout << "]";
    SetConsoleTextAttribute(h, 15);
    std::cout << " Starting cheat initialization.." << std::endl;
    textToSpeech(" Starting cheat initialization.."); // any shit added by laxiorr

    // check if the driver was correctly loaded
   // if (util::get_module_base("mlx4_bus.sys")) 
    {
        while (true)
        {
            uintptr_t base_address = mem::find_image();
         //   uintptr_t cr3 = mem::fetch_cr3();
            std::thread([&]() { for (;;) { cr3_loop(); } }).detach();
            settings::caching::base_address = base_address;

            render();
        }
    }

    system("cls");

    return true;
}