#pragma once
#include <codecvt>

struct PEB_LDR_DATA {
    uint32_t		Length;
    uint8_t			Initialized;
    uintptr_t		SsHandle;
    LIST_ENTRY		InLoadOrderModuleList;
    LIST_ENTRY		InMemoryOrderModuleList;
    LIST_ENTRY		InInitializationOrderModuleList;
    uintptr_t		EntryInProgress;
    uint8_t			ShutdownInProgress;
    uintptr_t		ShutdownThreadId;
};

struct UNICODE_STRING {
    uint16_t	Length;
    uint16_t	MaximumLength;
    wchar_t* Buffer;
};

struct STRING {
    uint16_t	Length;
    uint16_t	MaximumLength;
    char* Buffer;
};

struct CURDIR {
    UNICODE_STRING	DosPath;
    uintptr_t		Handle;
};

struct RTL_DRIVE_LETTER_CURDIR {
    uint16_t	Flags;
    uint16_t	Length;
    uint32_t	TimeStamp;
    STRING		DosPath;
};

struct RTL_pUser_PROCESS_PARAMETERS {
    uint32_t					MaximumLength;
    uint32_t					Length;
    uint32_t					Flags;
    uint32_t					DebugFlags;
    uintptr_t					ConsoleHandle;
    uint32_t					ConsoleFlags;
    uintptr_t					StandardInput;
    uintptr_t					StandardOutput;
    uintptr_t					StandardError;
    CURDIR						CurrentDirectory;
    UNICODE_STRING				DllPath;
    UNICODE_STRING				ImagePathName;
    UNICODE_STRING				CommandLine;
    uintptr_t					Environment;
    uint32_t					StartingX;
    uint32_t					StartingY;
    uint32_t					CountX;
    uint32_t					CountY;
    uint32_t					CountCharsX;
    uint32_t					CountCharsY;
    uint32_t					FillAttribute;
    uint32_t					WindowFlags;
    uint32_t					ShowWindowFlags;
    UNICODE_STRING				WindowTitle;
    UNICODE_STRING				DesktopInfo;
    UNICODE_STRING				ShellInfo;
    UNICODE_STRING				RuntimeData;
    RTL_DRIVE_LETTER_CURDIR		CurrentDirectores[32];
    uintptr_t					EnvironmentSize;
    uintptr_t					EnvironmentVersion;
    uintptr_t					PackageDependencyData;
    uint32_t					ProcessGroupId;
    uint32_t					LoaderThreads;
};

struct RTL_BALANCED_NODE {
    RTL_BALANCED_NODE* Children[2];
    RTL_BALANCED_NODE* Left;
    RTL_BALANCED_NODE* Right;
    uintptr_t			ParentValue;
};

struct _PEB {
    uint8_t							InheritedAddressSpace;
    uint8_t							ReadImageFileExecOptions;
    uint8_t							BeingDebugged;
    uint8_t							BitField;
    //uchar							Padding0[ 4 ];
    uintptr_t						Mutant;
    uintptr_t						ImageBaseAddress;
    PEB_LDR_DATA* Ldr;
    RTL_pUser_PROCESS_PARAMETERS* ProcessParameters;
    uintptr_t						SubSystemData;
    uintptr_t						ProcessHeap;
    RTL_CRITICAL_SECTION* FastPebLock;
    uintptr_t						AtlThunkSListPtr;
    uintptr_t						IFEOKey;
    uint32_t						CrossProcessFlags;
    uint8_t							Padding1[4];
    uintptr_t						KernelCallbackTable;
    uintptr_t						pUserSharedInfoPtr;
    uint32_t						SystemReserved[1];
    uint32_t						AtlThunkSListPtr32;
    uintptr_t						ApiSetMap;
    uint32_t						TlsExpansionCounter;
    uint8_t							Padding2[4];
    uintptr_t						TlsBitmap;
    uint32_t						TlsBitmapBits[2];
    uintptr_t						ReadOnlySharedMemoryBase;
    uintptr_t						SparePvoid0;
    uintptr_t						ReadOnlyStaticServerData;
    uintptr_t						AnsiCodePageData;
    uintptr_t						OemCodePageData;
    uintptr_t						UnicodeCaseTableData;
    uint32_t						NumberOfProcessors;
    uint32_t						NtGlobalFlag;
    LARGE_INTEGER					CriticalSectionTimeout;
    uintptr_t						HeapSegmentReserve;
    uintptr_t						HeapSegmentCommit;
    uintptr_t						HeapDeCommitTotalFreeThreshold;
    uintptr_t						HeapDeCommitFreeBlockThreshold;
    uint32_t						NumberOfHeaps;
    uint32_t						MaximumNumberOfHeaps;
    uintptr_t						ProcessHeaps;
    uintptr_t						GdiSharedHandleTable;
    uintptr_t						ProcessStarterHelper;
    uint32_t						GdiDCAttributeList;
    uint8_t							Padding3[4];
    RTL_CRITICAL_SECTION* LoaderLock;
    uint32_t						OSMajorVersion;
    uint32_t						OSMinorVersion;
    uint16_t						OSBuildNumber;
    uint16_t						OSCSDVersion;
    uint32_t						OSPlatformId;
    uint32_t						ImageSubsystem;
    uint32_t						ImageSubsystemMajorVersion;
    uint32_t						ImageSubsystemMinorVersion;
    uint8_t							Padding4[4];
    uintptr_t						ActiveProcessAffinityMask;
#ifdef _WIN32
    uint32_t						GdiHandleBuffer[34];
#else
    uint32_t						GdiHandleBuffer[60];
#endif
    uintptr_t						PostProcessInitRoutine;
    uintptr_t						TlsExpansionBitmap;
    uint32_t						TlsExpansionBitmapBits[32];
    uint32_t						SessionId;
    uint8_t							Padding5[4];
    ULARGE_INTEGER					AppCompatFlags;
    ULARGE_INTEGER					AppCompatFlagspUser;
    uintptr_t						pShimData;
    uintptr_t						AppCompatInfo;
    UNICODE_STRING					CSDVersion;
    uintptr_t						ActivationContextData;
    uintptr_t						ProcessAssemblyStorageMap;
    uintptr_t						SystemDefaultActivationContextData;
    uintptr_t						SystemAssemblyStorageMap;
    uintptr_t						MinimumStackCommit;
    uintptr_t						FlsCallback;
    LIST_ENTRY						FlsListHead;
    uintptr_t						FlsBitmap;
    uint32_t						FlsBitmapBits[4];
    uint32_t						FlsHighIndex;
    uintptr_t						WerRegistrationData;
    uintptr_t						WerShipAssertPtr;
    uintptr_t						pUnused;
    uintptr_t						pImageHeaderHash;
    uint32_t						TracingFlags;
    uint8_t							Padding6[4];
    uint64_t						CsrServerReadOnlySharedMemoryBase;
    uintptr_t						TppWorkerpListLock;
    LIST_ENTRY						TppWorkerpList;
    uintptr_t						WaitOnAddressHashTable[128];
};

struct LDR_DATA_TABLE_ENTRY {
    LIST_ENTRY				InLoadOrderLinks;
    LIST_ENTRY				InMemoryOrderLinks;
    LIST_ENTRY				InInitializationOrderLinks;
    uintptr_t				DllBase;
    uintptr_t				EntryPoint;
    uint32_t				SizeOfImage;
    UNICODE_STRING			FullDllName;
    UNICODE_STRING			BaseDllName;
    uint8_t					FlagGroup[4];
    uint32_t				Flags;
    uint16_t				ObsoleteLoadCount;
    uint16_t				TlsIndex;
    LIST_ENTRY				HashLinks;
    uint32_t				TimeDateStamp;
    uintptr_t				EntryPointActivationContext;
    uintptr_t				Lock;
    uintptr_t				DdagNode;
    LIST_ENTRY				NodeModuleLink;
    uintptr_t				LoadContext;
    uintptr_t				ParentDllBase;
    uintptr_t				SwitchBackContext;
    RTL_BALANCED_NODE		BaseAddressIndexNode;
    RTL_BALANCED_NODE		MappingInfoIndexNode;
    uintptr_t				OriginalBase;
    LARGE_INTEGER			LoadTime;
    uint32_t				BaseNameHashValue;
    uint32_t				LoadReason;
    uint32_t				ImplicitPathOptions;
    uint32_t				ReferenceCount;
};
__forceinline std::string ws2s(const std::wstring& wstr)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.to_bytes(wstr);
}

namespace PE {
    class Module {
    private:
        LDR_DATA_TABLE_ENTRY* m_ldr_entry;
        IMAGE_DOS_HEADER* m_dos;
        IMAGE_NT_HEADERS* m_nt;

    public:
        __forceinline Module() :
            m_ldr_entry{ nullptr }, m_dos{ nullptr }, m_nt{ nullptr } {}

        __forceinline Module(LDR_DATA_TABLE_ENTRY* ldr_entry, IMAGE_DOS_HEADER* dos, IMAGE_NT_HEADERS* nt) :
            m_ldr_entry{ ldr_entry }, m_dos{ dos }, m_nt{ nt } {}

        __forceinline Address GetBase() const {
            return (m_ldr_entry) ? Address(m_ldr_entry->DllBase) : Address{};
        }

        // valid check.
        __forceinline operator bool() const {
            return GetBase();
        }

        // valid check.
        __forceinline bool operator !() const {
            return !GetBase();
        }

        __forceinline operator Address() const {
            return (m_ldr_entry) ? Address(m_ldr_entry->DllBase) : Address{};
        }

        __forceinline IMAGE_DOS_HEADER* GetDOS() const {
            return (m_dos) ? m_dos : nullptr;
        }

        __forceinline IMAGE_NT_HEADERS* GetNT() const {
            return (m_nt) ? m_nt : nullptr;
        }

        __forceinline size_t GetCodeSize() const {
            return (m_nt) ? (size_t)m_nt->OptionalHeader.SizeOfCode : 0u;
        }

        __forceinline size_t GetImageSize() const {
            return (m_nt) ? (size_t)m_nt->OptionalHeader.SizeOfImage : 0u;
        }

        __forceinline std::string GetName() const {
            std::string     out;
            std::wstring    wide_name;

            if (!m_ldr_entry)
                return {};

            // get wide name.
            wide_name = std::wstring(m_ldr_entry->BaseDllName.Buffer, m_ldr_entry->BaseDllName.Length >> 1);

            // truncate from a unicode string to a multi-byte string.
            std::wstring f = std::wstring(wide_name.begin(), wide_name.end());

            std::string y = ws2s(f);
            out = y;

            // convert out name to lowercase.
            std::transform(out.begin(), out.end(), out.begin(), ::tolower);

            return out;
        }
    };

    // container type for all modules / export / exports.
    using modules_t = std::vector< Module >;
    using export_t = std::pair< hash32_t, Address >;
    using exports_t = std::vector< export_t >;

    // get DOS / NT headers from file.
    __forceinline bool GetFileHeaders(Address base, PIMAGE_DOS_HEADER& out_dos, PIMAGE_NT_HEADERS& out_nt) {
        auto dos = base.as< IMAGE_DOS_HEADER* >();

        // check for invalid DOS / DOS signature.
        if (!dos || dos->e_magic != IMAGE_DOS_SIGNATURE /* 'MZ' */)
            return false;

        // get NT headers.
        auto nt = (IMAGE_NT_HEADERS*)((uintptr_t)dos + dos->e_lfanew);

        // check for invalid NT / NT signature.
        if (!nt || nt->Signature != IMAGE_NT_SIGNATURE /* 'PE\0\0' */)
            return false;

        // set out dos and nt.
        out_dos = dos;
        out_nt = nt;

        return true;
    }

    // get all modules from PEB ldr.
    static __declspec(noinline) bool GetAllModules(modules_t& out) {
        _PEB* peb;
        LIST_ENTRY* list;
        LDR_DATA_TABLE_ENTRY* ldr_entry;
        IMAGE_DOS_HEADER* dos{ nullptr };
        IMAGE_NT_HEADERS* nt{ nullptr };

        // TEB->ProcessEnvironmentBlock.
        peb = (_PEB*)__readfsdword(0x30);
        if (!peb)
            return false;

        // get module linked list.
        list = &peb->Ldr->InMemoryOrderModuleList;

        // iterate linked list.
        for (auto it = list->Flink; it != list; it = it->Flink) {
            // get current entry.
            ldr_entry = CONTAINING_RECORD(it, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
            if (!ldr_entry)
                continue;

            // get file headers and ensure it's a valid PE file.
            if (!GetFileHeaders(ldr_entry->DllBase, dos, nt))
                continue;

            // push module to out vector.
            out.push_back({ ldr_entry, dos, nt });
        }

        return !out.empty();
    }

    // gets a module from PEB ldr using a string.
    static __declspec(noinline) Module GetModule(std::string name) {
        modules_t modules;

        // get all modules.
        if (!GetAllModules(modules))
            return {};

        // convert name to lower-case.
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);

        // iterate all modules.
        for (const auto& m : modules) {
            // compare name.
            if (m.GetName() == name)
                return m;
        }

        return {};
    }

    // gets a module from PEB ldr using a hash.
    static __declspec(noinline) Module GetModule(const hash32_t name) {
        modules_t modules;

        // get all modules.
        if (!GetAllModules(modules))
            return {};

        // iterate all modules.
        for (const auto& m : modules) {
            // compare name hash.
            if (FNV1a::get(m.GetName()) == name)
                return m;
        }

        return {};
    }

    // fwd declare GetExport ( by string )...
    static __declspec(noinline) Address GetExport(const Module& module, const std::string& name);

    // gets the address of an exported function in a module by hash.
    static __declspec(noinline) Address GetExport(const Module& module, const hash32_t name) {
        Address     export_ptr, fwd_export_ptr;
        size_t      delim;
        std::string export_name, fwd_str, fwd_module, fwd_export;

        if (!module)
            return {};

        auto nt = module.GetNT();
        auto base = module.GetBase();

        // get export data directory.
        auto data_dir = &nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
        if (!data_dir->VirtualAddress)
            return {};

        // get export dir.
        auto dir = Address::RVA< IMAGE_EXPORT_DIRECTORY* >(base, data_dir->VirtualAddress);
        if (!dir)
            return {};

        // names / funcs / ordinals ( all of these are RVAs ).
        auto names = Address::RVA< uint32_t* >(base, dir->AddressOfNames);
        auto funcs = Address::RVA< uint32_t* >(base, dir->AddressOfFunctions);
        auto ords = Address::RVA< uint16_t* >(base, dir->AddressOfNameOrdinals);
        if (!names || !funcs || !ords)
            return {};

        // iterate names array.
        for (size_t i{ 0u }; i < dir->NumberOfNames; ++i) {
            export_name = Address::RVA< const char* >(base, names[i]);
            if (export_name.empty())
                continue;

            // compare hash.
            if (FNV1a::get(export_name) == name) {
                // get export ptr.
                export_ptr = Address::RVA(base, funcs[ords[i]]);

                // it's a forwarded export, we must resolve it.
                if (export_ptr >= (uintptr_t)dir && export_ptr < ((uintptr_t)dir + data_dir->Size)) {
                    // get forwarder string.
                    fwd_str = export_ptr.as< const char* >();

                    // forwarders have a period as the delimiter.
                    delim = fwd_str.find_last_of('.') + 1;
                    if (delim == std::string::npos)
                        return {};

                    // get forwarder module name.
                    fwd_module = fwd_str.substr(0, delim);

                    // append "dll" to fwd_module.
                    fwd_module += 'd';
                    fwd_module += 'l';
                    fwd_module += 'l';

                    // get forwarder export name.
                    fwd_export = fwd_str.substr(delim);

                    // get real export ptr ( recursively ).
                    fwd_export_ptr = GetExport(GetModule(fwd_module), fwd_export);
                    if (!fwd_export_ptr)
                        return {};

                    // return ptr to forwarded export.
                    return fwd_export_ptr;
                }

                // return ptr to normal export.
                return export_ptr;
            }
        }

        return {};
    }

    // returns an array containing all exports in the specified module.
    static __declspec(noinline) exports_t GetAllExports(const Module& module) {
        Address     export_ptr, fwd_export_ptr;
        size_t      delim;
        std::string export_name, fwd_str, fwd_module, fwd_export;
        exports_t   exports;

        if (!module)
            return{};

        auto nt = module.GetNT();
        auto base = module.GetBase();

        // get export data directory.
        auto data_dir = &nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
        if (!data_dir->VirtualAddress)
            return{};

        // get export dir.
        auto dir = Address::RVA< IMAGE_EXPORT_DIRECTORY* >(base, data_dir->VirtualAddress);
        if (!dir)
            return{};

        // names / funcs / ordinals ( all of these are RVAs ).
        auto names = Address::RVA< uint32_t* >(base, dir->AddressOfNames);
        auto funcs = Address::RVA< uint32_t* >(base, dir->AddressOfFunctions);
        auto ords = Address::RVA< uint16_t* >(base, dir->AddressOfNameOrdinals);
        if (!names || !funcs || !ords)
            return{};

        // iterate names array.
        for (size_t i{ 0u }; i < dir->NumberOfNames; ++i) {
            export_name = Address::RVA< const char* >(base, names[i]);
            if (export_name.empty())
                continue;

            // get export ptr.
            export_ptr = Address::RVA(base, funcs[ords[i]]);

            // it's a forwarded export, we must resolve it.
            if (export_ptr >= (uintptr_t)dir && export_ptr < ((uintptr_t)dir + data_dir->Size)) {
                // get forwarder string.
                fwd_str = export_ptr.as< const char* >();

                // forwarders have a period as the delimiter.
                delim = fwd_str.find_last_of('.') + 1;
                if (delim == std::string::npos)
                    continue;

                // get forwarder module name.
                fwd_module = fwd_str.substr(0, delim);

                // append "dll" to fwd_module.
                fwd_module += 'd';
                fwd_module += 'l';
                fwd_module += 'l';

                // get forwarder export name.
                fwd_export = fwd_str.substr(delim);

                // get real export ptr ( recursively ).
                fwd_export_ptr = GetExport(GetModule(fwd_module), fwd_export);
                if (!fwd_export_ptr)
                    continue;

                exports.push_back({ FNV1a::get(export_name), fwd_export_ptr });
            }

            // return ptr to normal export.
            else
                exports.push_back({ FNV1a::get(export_name), export_ptr });
        }

        return exports;
    }

    // gets the address of an exported function in a module by string.
    static __declspec(noinline) Address GetExport(const Module& module, const std::string& name) {
        return GetExport(module, FNV1a::get(name));
    }
}


