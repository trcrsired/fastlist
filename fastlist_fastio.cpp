#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tlhelp32.h>
#include <fast_io.h>
#include <fast_io_device.h>

int main() {
	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snap == INVALID_HANDLE_VALUE)
		return 1;

	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(snap, &entry)) {
		return 1;
	}
	fast_io::out_buf_type obf{fast_io::out()};
	do {
		println(obf,entry.th32ProcessID, "\t",
			entry.th32ParentProcessID , "\t",
			fast_io::mnp::code_cvt(fast_io::mnp::os_c_str(entry.szExeFile)));
	} while (Process32Next(snap, &entry));
}
