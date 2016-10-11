#pragma once

#ifdef _WIN32
//  Windows
#include <Windows.h>
inline double get_cpu_time() {
	FILETIME a, b, c, d;
	if (GetProcessTimes(GetCurrentProcess(), &a, &b, &c, &d) != 0) {
		unsigned long long kernel = c.dwLowDateTime | ((unsigned long long)c.dwHighDateTime << 32);
		unsigned long long user = d.dwLowDateTime | ((unsigned long long)d.dwHighDateTime << 32);
		return (double)(kernel + user) * 0.0000001;
	}
	else { return 0; } // TODO: Handle error
}

#else
//  Posix/Linux
#include <time.h>
double get_cpu_time() { return (double)clock() / CLOCKS_PER_SEC; }
#endif
