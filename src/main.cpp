#include "PCH.h"

void MessageHandler(SFSE::MessagingInterface::Message* a_message)
{
	switch (a_message->type) {
	case SFSE::MessagingInterface::kPostLoad:
		{
			REL::Relocation<std::uintptr_t> SprintLimit{ REL::ID(500631) };

			float limit = 2000.0f;

			REL::WriteSafe(SprintLimit.address(), &limit, sizeof(float));
		}
		break;
	default:
		break;
	}
}

SFSE_EXPORT constinit auto SFSEPlugin_Version = []() noexcept {
	SFSE::PluginVersionData data{};

	data.PluginVersion(Version::MAJOR);
	data.PluginName(Version::PROJECT);
	data.AuthorName("Antonix35");
	data.UsesSigScanning(false);
	data.UsesAddressLibrary(true);
	data.HasNoStructUse(false);
	data.IsLayoutDependent(true);
	data.CompatibleVersions({ SFSE::RUNTIME_LATEST });

	return data;
}();

SFSEPluginLoad(const SFSE::LoadInterface* a_sfse)
{
	SFSE::Init(a_sfse, { .trampoline = true, .trampolineSize = 56 });
	SFSE::GetMessagingInterface()->RegisterListener(MessageHandler);
	return true;
}
