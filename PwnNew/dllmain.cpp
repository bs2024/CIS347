// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <windows.h>
#include <tlhelp32.h>
#include <vector>
#include <iostream>

// get base memory address for DLL
uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
{
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				if (!_wcsicmp(modEntry.szModule, modName))
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}

// Patch bytes over an address. Change thenr evert permissions
void Patch(BYTE* dst, BYTE* src, unsigned int size)
{
	DWORD oldValue;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldValue);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldValue, &oldValue);
}

// Locate memory address
uintptr_t LocateDirectMemoryAddress(uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t address = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		address = *(uintptr_t*)address;
		address += offsets[i];
	}
	return address;
}

// PwnHook
DWORD APIENTRY PwnHook(HMODULE hModule)
{
	//vars
	bool speedHack = false;
	bool flyHack = false;
	bool superFlyHack = false;
	float speed = 18.0f;
	float jump = 10.0f;
	float superJump = 2000.0f;
	float xCoordinate = 0.0;
	float yCoordinate = 0.0;
	float zCoordinate = 0.0;
	DWORD procId = GetCurrentProcessId();
	uintptr_t moduleBase = GetModuleBaseAddress(procId, L"GameLogic.dll");
	// Need to find location of these vars. Somewhere in the Player class I think.
	float defaultJump = *(float*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2C, 0x128 });
	float defaultSpeed = *(float*)LocateDirectMemoryAddress(moduleBase + 0x78B34, {});
	float defaultSpeedJump = *(float*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2C, 0x124 });
	int objectTP = -1;

	//console
	AllocConsole();
	FILE* consoleStream;
	freopen_s(&consoleStream, "CONOUT$", "w", stdout);
	std::cout << "ᕙ(ಠ_ಠ)ᕗ PwnHook v1.0 - Injection Successful ᕙ(ಠ_ಠ)ᕗ\n";
	std::cout << "PID: " << procId << "\n";
	std::cout << "GameLogic.dll modAddress: " << moduleBase << "\n";
	std::cout << "Current JHT: " << defaultJump << "\n";
	std::cout << "Current Speed: " << defaultSpeed << "\n";

	//haxors
	while (true)
	{
		// Speed hack on F1
		if (GetAsyncKeyState(VK_F1))
		{
			speedHack = !speedHack;
			if (speedHack)
			{
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x78B34, {}), (BYTE*)&speed, sizeof(speed));
				std::cout << "Speed Hack ON \n";
			}
			else
			{
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x78B34, {}), (BYTE*)&defaultSpeed, sizeof(defaultSpeed));
				std::cout << "Speed Hack OFF \n";
			}
			Sleep(800);
		}

		// Jump hack on F2
		if (GetAsyncKeyState(VK_F2))
		{
			flyHack = !flyHack;
			if (flyHack)
			{
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2C, 0x128 }), (BYTE*)&jump, sizeof(jump));
				std::cout << "Jump Hack ON \n";
			}
			else
			{
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2C, 0x128 }), (BYTE*)&defaultJump, sizeof(defaultJump));
				std::cout << "Jump Hack OFF \n";
			}
			Sleep(800);
		}

		// Super Jump on F3
		if (GetAsyncKeyState(VK_F3))
		{
			superFlyHack = !superFlyHack;
			if (superFlyHack)
			{
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2C, 0x124 }), (BYTE*)&superJump, sizeof(superJump));
				std::cout << "Speed Jump Hack ON \n";
			}
			else
			{
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2C, 0x124 }), (BYTE*)&defaultSpeedJump, sizeof(defaultSpeedJump));
				std::cout << "Speed Jump Hack OFF \n";
			}
			Sleep(800);
		}

		//set teleport
		if (GetAsyncKeyState(VK_F4))
		{
			/*
			//unbearable revenge location
			xCoordinate = -7127.182129;
			yCoordinate = 64546.24609;
			zCoordinate = 3627.743408;

			Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x90 }), (BYTE*)&xCoordinate, sizeof(xCoordinate));
			Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x94 }), (BYTE*)&yCoordinate, sizeof(yCoordinate));
			Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x98 }), (BYTE*)&zCoordinate, sizeof(zCoordinate));
			std::cout << "Unbearable Tree \n";*/


			std::cout << "Teleport Location Set \n";
			xCoordinate = *(float*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x90 });
			yCoordinate = *(float*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x94 });
			zCoordinate = *(float*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x98 });
			/*std::cout << xCoordinate, " \n";
			std::cout << yCoordinate, " \n";
			std::cout << zCoordinate, " \n";*/

			Sleep(800);
		}

		//teleport
		if (GetAsyncKeyState(VK_F5))
		{
			std::cout << "Teleporting to set location \n";
			Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x90 }), (BYTE*)&xCoordinate, sizeof(xCoordinate));
			Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x94 }), (BYTE*)&yCoordinate, sizeof(yCoordinate));
			Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x98 }), (BYTE*)&zCoordinate, sizeof(zCoordinate));
		}

		if (GetAsyncKeyState(VK_F6))
		{
			if (objectTP < 22) {
				objectTP++;
				Sleep(800);

			}
			else {
				objectTP = 0;
			}
		}

		switch (objectTP) {
		case 0:
			if (!(xCoordinate == -25045 && yCoordinate == 18085 && zCoordinate == 260)) {
				std::cout << "Golden Egg 1\n";
				xCoordinate = -25045;
				yCoordinate = 18085;
				zCoordinate = 260;
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x90 }), (BYTE*)&xCoordinate, sizeof(xCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x94 }), (BYTE*)&yCoordinate, sizeof(yCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x98 }), (BYTE*)&zCoordinate, sizeof(zCoordinate));
			}
			break;
		case 1:
			if (!(xCoordinate == -51570 && yCoordinate == -61215 && zCoordinate == 5020)) {
				std::cout << "Golden Egg 2\n";
				xCoordinate = -51570;
				yCoordinate = -61215;
				zCoordinate = 5020;
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x90 }), (BYTE*)&xCoordinate, sizeof(xCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x94 }), (BYTE*)&yCoordinate, sizeof(yCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x98 }), (BYTE*)&zCoordinate, sizeof(zCoordinate));
			}
			break;
		case 2:
			if (!(xCoordinate == 24512 && yCoordinate == 69682 && zCoordinate == 2659)) {
				std::cout << "Golden Egg 3\n";
				xCoordinate = 24512;
				yCoordinate = 69682;
				zCoordinate = 2659;
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x90 }), (BYTE*)&xCoordinate, sizeof(xCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x94 }), (BYTE*)&yCoordinate, sizeof(yCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x98 }), (BYTE*)&zCoordinate, sizeof(zCoordinate));
			}
			break;
		case 3:
			if (!(xCoordinate == 60453 && yCoordinate == -17409 && zCoordinate == 2939)) {
				std::cout << "Golden Egg 4\n";
				xCoordinate = 60453;
				yCoordinate = -17409;
				zCoordinate = 2939;
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x90 }), (BYTE*)&xCoordinate, sizeof(xCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x94 }), (BYTE*)&yCoordinate, sizeof(yCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x98 }), (BYTE*)&zCoordinate, sizeof(zCoordinate));
			}
			break;
		case 4:
			if (!(xCoordinate == 1522 && yCoordinate == 14966 && zCoordinate == 7022)) {
				std::cout << "Golden Egg 5\n";
				xCoordinate = 1522;
				yCoordinate = 14966;
				zCoordinate = 7022;
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x90 }), (BYTE*)&xCoordinate, sizeof(xCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x94 }), (BYTE*)&yCoordinate, sizeof(yCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x98 }), (BYTE*)&zCoordinate, sizeof(zCoordinate));
			}
			break;
		case 5:
			if (!(xCoordinate == 11604 && yCoordinate == -13131 && zCoordinate == 411)) {
				std::cout << "Golden Egg 6\n";
				xCoordinate = 11604;
				yCoordinate = -13131;
				zCoordinate = 411;
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x90 }), (BYTE*)&xCoordinate, sizeof(xCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x94 }), (BYTE*)&yCoordinate, sizeof(yCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x98 }), (BYTE*)&zCoordinate, sizeof(zCoordinate));
			}
			break;
		case 6:
			if (!(xCoordinate == -72667 && yCoordinate == -53567 && zCoordinate == 1645)) {
				std::cout << "Golden Egg 7\n";
				xCoordinate = -72667;
				yCoordinate = -53567;
				zCoordinate = 1645;
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x90 }), (BYTE*)&xCoordinate, sizeof(xCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x94 }), (BYTE*)&yCoordinate, sizeof(yCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x98 }), (BYTE*)&zCoordinate, sizeof(zCoordinate));
			}
			break;
		case 7:
			if (!(xCoordinate == 48404 && yCoordinate == 28117 && zCoordinate == 704)) {
				std::cout << "Golden Egg 8\n";
				xCoordinate = 48404;
				yCoordinate = 28117;
				zCoordinate = 704;
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x90 }), (BYTE*)&xCoordinate, sizeof(xCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x94 }), (BYTE*)&yCoordinate, sizeof(yCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x98 }), (BYTE*)&zCoordinate, sizeof(zCoordinate));
			}
			break;
		case 8:
			if (!(xCoordinate == 65225 && yCoordinate == -5740 && zCoordinate == 4928)) {
				std::cout << "Golden Egg 9\n";
				xCoordinate = 65225;
				yCoordinate = -5740;
				zCoordinate = 4928;
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x90 }), (BYTE*)&xCoordinate, sizeof(xCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x94 }), (BYTE*)&yCoordinate, sizeof(yCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x98 }), (BYTE*)&zCoordinate, sizeof(zCoordinate));
			}
			break;
		case 9:
			if (!(xCoordinate == -2778 && yCoordinate == -11035 && zCoordinate == 10504)) {
				std::cout << "Ballmer Peak Egg\n";
				xCoordinate = -2778;
				yCoordinate = -11035;
				zCoordinate = 10504;
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x90 }), (BYTE*)&xCoordinate, sizeof(xCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x94 }), (BYTE*)&yCoordinate, sizeof(yCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x98 }), (BYTE*)&zCoordinate, sizeof(zCoordinate));
			}
			break;
		case 10:
			if (!(xCoordinate == -6101 && yCoordinate == -10956 && zCoordinate == 10636)) {
				std::cout << "Ballmer Peak Poster\n";
				xCoordinate = -6101;
				yCoordinate = -10956;
				zCoordinate = 10636;
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x90 }), (BYTE*)&xCoordinate, sizeof(xCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x94 }), (BYTE*)&yCoordinate, sizeof(yCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x98 }), (BYTE*)&zCoordinate, sizeof(zCoordinate));
			}
			break;
		case 11:
			if (!(xCoordinate == -43655 && yCoordinate == -55820 && zCoordinate == 322)) {
				std::cout << "Great Balls of Fire\n";
				xCoordinate = -43655;
				yCoordinate = -55820;
				zCoordinate = 322;
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x90 }), (BYTE*)&xCoordinate, sizeof(xCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x94 }), (BYTE*)&yCoordinate, sizeof(yCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x98 }), (BYTE*)&zCoordinate, sizeof(zCoordinate));
			}
			break;
		case 12:
			if (!(xCoordinate == 260255 && yCoordinate == -248555 && zCoordinate == 1415)) {
				std::cout << "Michael Angelo\n";
				xCoordinate = 260255;
				yCoordinate = -248555;
				zCoordinate = 1415;
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x90 }), (BYTE*)&xCoordinate, sizeof(xCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x94 }), (BYTE*)&yCoordinate, sizeof(yCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x98 }), (BYTE*)&zCoordinate, sizeof(zCoordinate));
			}
			break;
		case 13:
			if (!(xCoordinate == -7894 && yCoordinate == 64482 && zCoordinate == 2663)) {
				std::cout << "Bear Chest\n";
				xCoordinate = -7894;
				yCoordinate = 64482;
				zCoordinate = 2663;
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x90 }), (BYTE*)&xCoordinate, sizeof(xCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x94 }), (BYTE*)&yCoordinate, sizeof(yCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x98 }), (BYTE*)&zCoordinate, sizeof(zCoordinate));
			}
			break;
		case 14:
			if (!(xCoordinate == 252920 && yCoordinate == -245380 && zCoordinate == 1170)) {
				std::cout << "Cow Chest\n";
				xCoordinate = 252920;
				yCoordinate = -245380;
				zCoordinate = 1170;
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x90 }), (BYTE*)&xCoordinate, sizeof(xCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x94 }), (BYTE*)&yCoordinate, sizeof(yCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x98 }), (BYTE*)&zCoordinate, sizeof(zCoordinate));
			}
			break;
		case 15:
			if (!(xCoordinate == 0 && yCoordinate == 0 && zCoordinate == 0)) {
				std::cout << "Giant Rat\n";
				xCoordinate = 0;
				yCoordinate = 0;
				zCoordinate = 0;
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x90 }), (BYTE*)&xCoordinate, sizeof(xCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x94 }), (BYTE*)&yCoordinate, sizeof(yCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x98 }), (BYTE*)&zCoordinate, sizeof(zCoordinate));
			}
			break;
		case 16:
			if (!(xCoordinate == -53539 && yCoordinate == -44246 && zCoordinate == 358)) {
				std::cout << "Lost Cave Bush\n";
				xCoordinate = -53539;
				yCoordinate = -44246;
				zCoordinate = 358;
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x90 }), (BYTE*)&xCoordinate, sizeof(xCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x94 }), (BYTE*)&yCoordinate, sizeof(yCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x98 }), (BYTE*)&zCoordinate, sizeof(zCoordinate));
			}
			break;
		case 17:
			if (!(xCoordinate == 21553 && yCoordinate == 41232 && zCoordinate == 2133)) {
				std::cout << "Farmer\n";
				xCoordinate = 21553;
				yCoordinate = 41232;
				zCoordinate = 2133;
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x90 }), (BYTE*)&xCoordinate, sizeof(xCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x94 }), (BYTE*)&yCoordinate, sizeof(yCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x98 }), (BYTE*)&zCoordinate, sizeof(zCoordinate));
			}
			break;
		case 18:
			if (!(xCoordinate == 50876 && yCoordinate == -5243 && zCoordinate == 1523)) {
				std::cout << "LavaChest\n";
				xCoordinate = 50876;
				yCoordinate = -5243;
				zCoordinate = 1523;
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x90 }), (BYTE*)&xCoordinate, sizeof(xCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x94 }), (BYTE*)&yCoordinate, sizeof(yCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x98 }), (BYTE*)&zCoordinate, sizeof(zCoordinate));
			}
			break;
		case 19:
			if (!(xCoordinate == -3055 && yCoordinate == 23005 && zCoordinate == 2275)) {
				std::cout << "Blocky Chest\n";
				xCoordinate = -3055;
				yCoordinate = 23005;
				zCoordinate = 2275;
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x90 }), (BYTE*)&xCoordinate, sizeof(xCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x94 }), (BYTE*)&yCoordinate, sizeof(yCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x98 }), (BYTE*)&zCoordinate, sizeof(zCoordinate));
			}
			break;
		case 20:
			if (!(xCoordinate == -37463 && yCoordinate == -18050 && zCoordinate == 2416)) {
				std::cout << "Gun Shop Owner\n";
				xCoordinate = -37463;
				yCoordinate = -18050;
				zCoordinate = 2416;
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x90 }), (BYTE*)&xCoordinate, sizeof(xCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x94 }), (BYTE*)&yCoordinate, sizeof(yCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x98 }), (BYTE*)&zCoordinate, sizeof(zCoordinate));
			}
			break;
		case 21:
			if (!(xCoordinate == -41084 && yCoordinate == -16256 && zCoordinate == 2270)) {
				std::cout << "Justin Tolerable\n";
				xCoordinate = -41084;
				yCoordinate = -16256;
				zCoordinate = 2270;
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x90 }), (BYTE*)&xCoordinate, sizeof(xCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x94 }), (BYTE*)&yCoordinate, sizeof(yCoordinate));
				Patch((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x2c, 0x148, 0x4, 0x280, 0x98 }), (BYTE*)&zCoordinate, sizeof(zCoordinate));
			}
			break;

		}
	}
	// Almost forgot to close the console and kill the thread lol
	fclose(consoleStream);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

// DLLMain
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)PwnHook, hModule, 0, 0);
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
