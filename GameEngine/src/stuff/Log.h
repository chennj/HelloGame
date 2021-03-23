#pragma once

#include <memory>

#include "Core.h"
#include "spdlog\spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace SOMEENGINE
{
	class SOME_API Log
	{
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger()
		{
			return s_CoreLogger;
		}
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger()
		{
			return s_ClientLogger;
		}
	};

}

#define SO_CORE_TRACE(...)	::SOMEENGINE::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define SO_CORE_ERROR(...)	::SOMEENGINE::Log::GetCoreLogger()->error(__VA_ARGS__);
#define SO_CORE_WARN(...)	::SOMEENGINE::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define SO_CORE_INFO(...)	::SOMEENGINE::Log::GetCoreLogger()->info(__VA_ARGS__);
#define SO_CORE_FATAL(...)	::SOMEENGINE::Log::GetCoreLogger()->fatal(__VA_ARGS__);

#define SO_TRACE(...)		::SOMEENGINE::Log::GetClientLogger()->trace(__VA_ARGS__);
#define SO_ERROR(...)		::SOMEENGINE::Log::GetClientLogger()->error(__VA_ARGS__);
#define SO_WARN(...)		::SOMEENGINE::Log::GetClientLogger()->warn(__VA_ARGS__);
#define SO_INFO(...)		::SOMEENGINE::Log::GetClientLogger()->info(__VA_ARGS__);
#define SO_FATAL(...)		::SOMEENGINE::Log::GetClientLogger()->fatal(__VA_ARGS__);