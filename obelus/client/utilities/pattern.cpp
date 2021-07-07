#include "csgo.hpp"

std::uint8_t* pattern::Scan(const char* module_name, const char* signature) noexcept {
	const auto module_handle = GetModuleHandleA(module_name);

	if (!module_handle)
		return nullptr;

	static auto pattern_to_byte = [](const char* pattern) {
		auto bytes = std::vector<int>{};
		auto start = const_cast<char*>(pattern);
		auto end = const_cast<char*>(pattern) + std::strlen(pattern);

		for (auto current = start; current < end; ++current) {
			if (*current == '?') {
				++current;

				if (*current == '?')
					++current;

				bytes.push_back(-1);
			}
			else {
				bytes.push_back(std::strtoul(current, &current, 16));
			}
		}
		return bytes;
	};

	auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module_handle);
	auto nt_headers =
		reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(module_handle) + dos_header->e_lfanew);

	auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
	auto pattern_bytes = pattern_to_byte(signature);
	auto scan_bytes = reinterpret_cast<std::uint8_t*>(module_handle);

	auto s = pattern_bytes.size();
	auto d = pattern_bytes.data();

	for (auto i = 0ul; i < size_of_image - s; ++i) {
		bool found = true;

		for (auto j = 0ul; j < s; ++j) {
			if (scan_bytes[i + j] != d[j] && d[j] != -1) {
				found = false;
				break;
			}
		}
		if (found)
			return &scan_bytes[i];
	}

	return nullptr;
}
pattern::patterns_t pattern::FindAll(Address start, size_t len, const std::string& pat) {
	pattern::patterns_t out{};
	Address	   result;

	for (;; ) {
		// find result.
		result = Find(start, len, pat);
		if (!result)
			break;

		// if we arrived here we found something.
		out.push_back(result);

		// set new len.
		len = (start + len) - (result + 1);

		// new start point.
		start = result + 1;
	}

	return out;
}

Address pattern::Find(Address start, size_t len, const std::string& pat) {
	uint8_t* scan_start, * scan_end;
	std::vector< pattern_byte_t > pattern{};
	std::stringstream			  stream{ pat };
	std::string				      w;

	if (!start || !len || pat.empty())
		return{};

	// split spaces and convert to hex.
	while (stream >> w) {
		// wildcard.
		if (w[0] == '?')
			pattern.push_back({ 0, true });

		// valid hex digits.
		else if (std::isxdigit(w[0]) && std::isxdigit(w[1]))
			pattern.push_back({ (uint8_t)std::strtoul(w.data(), 0, 16), false });
	}

	scan_start = start.as< uint8_t* >();
	scan_end = scan_start + len;

	// find match.
	auto result = std::search(scan_start, scan_end, pattern.begin(), pattern.end(),
		[](const uint8_t b, const pattern_byte_t& p) {
			// byte matches or it's a wildcard.
			return b == p.first || p.second;
		});

	// nothing found.
	if (result == scan_end)
		return{};

	return (uintptr_t)result;
}

static std::pair<void*, std::size_t> getModuleInformation(const char* name) noexcept
{
	if (HMODULE handle = GetModuleHandleA(name)) {
		if (MODULEINFO moduleInfo; GetModuleInformation(GetCurrentProcess(), handle, &moduleInfo, sizeof(moduleInfo)))
			return std::make_pair(moduleInfo.lpBaseOfDll, moduleInfo.SizeOfImage);
	}
	return {};
}

[[nodiscard]] static auto generateBadCharTable(std::string_view pattern) noexcept
{
	assert(!pattern.empty());

	std::array<std::size_t, (std::numeric_limits<std::uint8_t>::max)() + 1> table;

	auto lastWildcard = pattern.rfind('?');
	if (lastWildcard == std::string_view::npos)
		lastWildcard = 0;

	const auto defaultShift = (std::max)(std::size_t(1), pattern.length() - 1 - lastWildcard);
	table.fill(defaultShift);

	for (auto i = lastWildcard; i < pattern.length() - 1; ++i)
		table[static_cast<std::uint8_t>(pattern[i])] = pattern.length() - 1 - i;

	return table;
}

std::uintptr_t pattern::Get(const char* moduleName, std::string_view pattern)
{
	static auto id = 0;
	++id;

	const auto [moduleBase, moduleSize] = getModuleInformation(moduleName);

	if (moduleBase && moduleSize) {
		const auto lastIdx = pattern.length() - 1;
		const auto badCharTable = generateBadCharTable(pattern);

		auto start = static_cast<const char*>(moduleBase);
		const auto end = start + moduleSize - pattern.length();

		while (start <= end) {
			int i = lastIdx;
			while (i >= 0 && (pattern[i] == '?' || start[i] == pattern[i]))
				--i;

			if (i < 0)
				return reinterpret_cast<std::uintptr_t>(start);

			start += badCharTable[static_cast<std::uint8_t>(start[lastIdx])];
		}
	}

	MessageBoxA(NULL, (" #" + std::to_string(id) + '!').c_str(), "", MB_OK | MB_ICONWARNING);

	return 0;
}