#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tlhelp32.h>
#include <fast_io.h>
#include <fast_io_device.h>

extern "C" void* memcpy( void* dest, void const* src, std::size_t count )
{
	char const* s1 = reinterpret_cast<char const*>(src);
	char* s2 = reinterpret_cast<char*>(dest);
	while (count--) *s2++ = *s1++;
	return dest;
}

extern "C" void start(void)
{
	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snap == INVALID_HANDLE_VALUE)
		return;

	PROCESSENTRY32W entry;
	entry.dwSize = sizeof(PROCESSENTRY32W);

	if (!Process32FirstW(snap, &entry)) {
		return;
	}
	fast_io::out_buf_type obf{fast_io::out()};
	do {
		println(obf,entry.th32ProcessID, "\t",
			entry.th32ParentProcessID , "\t",
			fast_io::mnp::code_cvt(fast_io::mnp::os_c_str(entry.szExeFile)));
	} while (Process32NextW(snap, &entry));
}
