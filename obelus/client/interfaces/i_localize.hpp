#pragma once

class ILocalize {
public:
	wchar_t* find(const char* token_name) {
		using original_fn = wchar_t* (__thiscall*)(ILocalize*, const char*);
		return (*(original_fn * *)this)[12](this, token_name);
	}
};
