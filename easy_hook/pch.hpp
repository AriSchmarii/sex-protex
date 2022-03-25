#pragma once
#include <windows.h>
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <functional>
#include <basetsd.h>
#include <mutex>

#include <MinHook.h>
#include "MinHook/include/MinHook.h"
#pragma comment(lib, "libMinHook-x64-v140-md.lib")

#include "scanner.hpp"

#pragma warning(disable : 4996) //due to std::wcscpy

class input {
public:
	constexpr explicit input(const char* info = { }, const std::int32_t key = { }) : info(info), key(key) { }
	~input() noexcept = default;
	void add_call(std::function<void()> callback) noexcept {
		if (GetAsyncKeyState(key) & 0x1) {
			callback();
			std::cout << info << std::endl;
		}
	}
public:
	const char* info;
	std::int32_t key;
};

#define create_hook(name, target, hk, og) \
if (MH_CreateHook(target, hk, (void**)&og) != MH_OK) \
   std::cout << "cant hook " << name << std::endl; \
else \
   std::cout << "hooked " << name << std::endl; \
/*
x-man: pattern scanner
senpai ari: easy-hook
senpai ari: fuck rid crashes lil rice boy
senpai ari: yes im the one who made this
aki: custom console color
hi phobos :3
*/
