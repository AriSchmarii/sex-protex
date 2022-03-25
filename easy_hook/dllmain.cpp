#include "pch.hpp"

bool g_running = { true };

using CVoiceSessionInviteHandler = bool(*)(__int64 a1);
CVoiceSessionInviteHandler vsi_handler{};

bool (*ogVoiceSessionInviteHandler)(__int64 a1) = { nullptr };
bool hkVoiceSessionInviteHandler(__int64 a1) { //5th index in the vtable of CVoiceSessionInvite sex sex
	std::cout << "vsi blocked" << std::endl;
	return FALSE;
}

DWORD mainThread(LPVOID lp_param) {
	if (!init_console())
		return EXIT_FAILURE;
	MH_Initialize();
	std::cout << R"(
 ____                 ____            _            
/ ___|  _____  __    |  _ \ _ __ ___ | |_ _____  __
\___ \ / _ \ \/ /____| |_) | '__/ _ \| __/ _ \ \/ /
 ___) |  __/>  <_____|  __/| | | (_) | ||  __/>  < 
|____/ \___/_/\_\    |_|   |_|  \___/ \__\___/_/\_\
                                                                                                              
----------------------------------------------------------------------------)" << std::endl;
	try {
		pattern_batch batch;
		batch.add("vsih", "48 83 EC 28 4C 8D 89 ? ? ? ? 4C 8D 41 78 ", [=](ptr_manage ptr) {
			vsi_handler = ptr.as<CVoiceSessionInviteHandler>();
		});
		batch.run();
		create_hook("vsih", vsi_handler, hkVoiceSessionInviteHandler, ogVoiceSessionInviteHandler);
		MH_EnableHook(MH_ALL_HOOKS);
		std::cout << "----------------------------------------------------------------------------" << std::endl; //please don't insult me :c
		std::cout << "f5 - unload" << std::endl;
		while (g_running) {
			input("unloaded", VK_F5).add_call([]() -> void { g_running = false; });
		}
		MH_DisableHook(MH_ALL_HOOKS);
		MH_RemoveHook(MH_ALL_HOOKS);
		MH_Uninitialize();
		FreeConsole();
		FreeLibraryAndExitThread(static_cast<HMODULE>(lp_param), EXIT_SUCCESS);
	}
	catch (std::exception& exc) {
		std::cout << exc.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

BOOL APIENTRY DllMain(HMODULE h_module, DWORD  ul_reason_for_call, LPVOID lp_reserved) {
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(h_module);
		CreateThread(NULL, NULL, static_cast<LPTHREAD_START_ROUTINE>(mainThread), h_module, NULL, NULL);
	}
	return TRUE;
}
