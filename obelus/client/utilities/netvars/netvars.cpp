#include "netvars.hpp"
#include "../../interfaces/interfaces.hpp"
#include "../csgo.hpp"


using netvar_key_value_map = std::unordered_map< uint32_t, uintptr_t >;
using netvar_table_map = std::unordered_map< uint32_t, netvar_key_value_map >;


void AddPropsForTable(netvar_table_map& table_map, const uint32_t table_name_hash, const std::string& table_name, RecvTable* table, const bool dump_vars, std::map< std::string, std::map< uintptr_t, std::string > >& var_dump, const size_t child_offset = 0)
{
	for (auto i = 0; i < table->m_nProps; ++i) {
		auto& prop = table->m_pProps[i];

		if (prop.m_pDataTable && prop.m_nElements > 0) {
			if (std::string(prop.m_pVarName).substr(0, 1) == std::string("0"))
				continue;

			AddPropsForTable(table_map, table_name_hash, table_name, prop.m_pDataTable, dump_vars, var_dump, prop.m_Offset
				+ child_offset);
		}

		auto name = std::string(prop.m_pVarName);

		if (name.substr(0, 1) != "m" /*&& name.substr( 0, 1 ) != "b"*/)
			continue;

		const auto name_hash = fnv::hash(prop.m_pVarName);
		const auto offset = uintptr_t(prop.m_Offset) + child_offset;

		table_map[table_name_hash][name_hash] = offset;

		if (dump_vars)
			var_dump[table_name][offset] = prop.m_pVarName;
	}
}

void InitProps(netvar_table_map& table_map)
{
	const auto dump_vars = true;  //true if netvar dump

	std::map< std::string, std::map< uintptr_t, std::string > > var_dump;
	for (auto client_class = interfaces::client->get_client_classes();
		client_class;
		client_class = client_class->m_pNext) {
		const auto table = reinterpret_cast<RecvTable*>(client_class->m_pRecvTable);
		const auto table_name = table->m_pNetTableName;
		const auto table_name_hash = fnv::hash(table_name);

		if (table == nullptr)
			continue;

		AddPropsForTable(table_map, table_name_hash, table_name, table, dump_vars, var_dump);
	}
}

uintptr_t Netvars::GetNetVar(const uint32_t table,
	const uint32_t prop) {
	static netvar_table_map map = {};
	if (map.empty())
		InitProps(map);

	if (map.find(table) == map.end())
		return 0;

	netvar_key_value_map& table_map = map.at(table);
	if (table_map.find(prop) == table_map.end())
		return 0;

	return table_map.at(prop);
}

void Netvars::FindAndStoreDataMaps() {
	pattern::patterns_t matches{};

	// sanity.
	if (!client::m_client_dll)
		return;

	static auto test = [](HMODULE hModule, const char* szSignature) -> uint8_t* {
		static auto pattern_to_byte = [](const char* pattern) {
			auto bytes = std::vector<int>{};
			auto start = const_cast<char*>(pattern);
			auto end = const_cast<char*>(pattern) + strlen(pattern);

			for (auto current = start; current < end; ++current) {
				if (*current == '?') {
					++current;
					if (*current == '?')
						++current;
					bytes.push_back(-1);
				}
				else {
					bytes.push_back(strtoul(current, &current, 16));
				}
			}
			return bytes;
		};

		//auto Module = GetModuleHandleA(szModule);

		auto dosHeader = (PIMAGE_DOS_HEADER)hModule;
		auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)hModule + dosHeader->e_lfanew);

		auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
		auto patternBytes = pattern_to_byte(szSignature);
		auto scanBytes = reinterpret_cast<std::uint8_t*>(hModule);

		auto s = patternBytes.size();
		auto d = patternBytes.data();

		for (auto i = 0ul; i < sizeOfImage - s; ++i) {
			bool found = true;
			for (auto j = 0ul; j < s; ++j) {
				if (scanBytes[i + j] != d[j] && d[j] != -1) {
					found = false;
					break;
				}
			}
			if (found) {

				return &scanBytes[i];
			}
		}

		return nullptr;
	};

	matches = pattern::FindAll(client::m_client_dll, XOR("C7 05 ? ? ? ? ? ? ? ? C7 05 ? ? ? ? ? ? ? ? C3 CC"));
	matches;
	if (matches.empty())
		return;

	for (auto& m : matches)
		StoreDataMap(m);
}

void Netvars::initialize() {
	ClientClass* list;

	// sanity check on client.
	if (!interfaces::client)
		return;

	// grab linked list.
	list = interfaces::client->get_client_classes();
	if (!list)
		return;

	// iterate list of netvars.
	for (; list != nullptr; list = list->m_pNext)
		StoreDataTable(list->m_pRecvTable->m_pNetTableName, list->m_pRecvTable);

	// find all datamaps
	FindAndStoreDataMaps();


	console::log("[setup] netvars initialized!\n");
}
void Netvars::SetupClassData() {
	ClientClass* list;

	// clear old shit.
	m_client_ids.clear();

	// sanity check on client.
	if (!interfaces::client)
		return;

	// grab linked list.
	list = interfaces::client->get_client_classes();
	if (!list)
		return;

	// iterate list.
	for (; list != nullptr; list = list->m_pNext)
		m_client_ids[FNV1a::get(list->m_pNetworkName)] = list->m_ClassID;
}
void Netvars::StoreDataTable(const std::string& name, RecvTable* table, size_t offset) {
	hash32_t	var, base{ FNV1a::get(name) };
	RecvProp* prop;
	RecvTable* child;

	// iterate props
	for (int i{}; i < table->m_nProps; ++i) {
		prop = &table->m_pProps[i];
		child = prop->m_pDataTable;

		// we have a child table, that contains props.
		if (child && child->m_nProps > 0)
			StoreDataTable(name, child, prop->m_Offset + offset);

		// hash var name.
		var = FNV1a::get(prop->m_pVarName);

		// insert if not present.
		if (!m_offsets[base][var].m_offset) {
			m_offsets[base][var].m_datamap_var = false;
			m_offsets[base][var].m_prop_ptr = prop;
			m_offsets[base][var].m_offset = (size_t)(prop->m_Offset + offset);
		}
	}
}

void Netvars::StoreDataMap(Address ptr) {
	datamap_t* map;
	hash32_t            base, var;
	typedescription_t* entry;

	// get datamap and verify.
	map = ptr.at(2).sub(4).as< datamap_t* >();

	if (!map || !map->m_size || map->m_size > 200 || !map->m_desc || !map->m_name)
		return;

	// hash table name.
	base = FNV1a::get(map->m_name);

	for (int i{}; i < map->m_size; ++i) {
		entry = &map->m_desc[i];
		if (!entry->m_name)
			continue;

		// hash var name.
		var = FNV1a::get(entry->m_name);

		// if we dont have this var stored yet.
		if (!m_offsets[base][var].m_offset) {
			m_offsets[base][var].m_datamap_var = true;
			m_offsets[base][var].m_offset = (size_t)entry->m_offset[TD_OFFSET_NORMAL];
			m_offsets[base][var].m_prop_ptr = nullptr;
		}
	}
}