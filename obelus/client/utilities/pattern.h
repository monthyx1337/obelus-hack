#pragma once

namespace pattern 
{
    using patterns_t = std::vector< Address >;
    using pattern_byte_t = std::pair< uint8_t, bool >;

    Address Find(Address start, size_t len, const std::string& pat);
	std::uint8_t* Scan(const char* module_name, const char* signature) noexcept;
	patterns_t FindAll(Address start, size_t len, const std::string& pat);

    static patterns_t FindAll(const PE::Module& module, const std::string& pat) 
	{
        return FindAll(module.GetBase(), module.GetImageSize(), pat);
    }

    static Address Find(const PE::Module& module, const std::string& pat)
    {
        return Find(module.GetBase(), module.GetImageSize(), pat);
    }

	template <typename T>
	static constexpr auto Rel2Abs(uintptr_t address) noexcept
	{
		return (T)(address + 4 + *reinterpret_cast<std::int32_t*>(address));
	}


	std::uintptr_t Get(const char* moduleName, std::string_view pattern);

}