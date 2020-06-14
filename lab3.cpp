#include "lab3.h"
#include <windows.h> 
#include <stdio.h>

using namespace std;

#define THREADCOUNT 5

HANDLE Thread[THREADCOUNT];
HANDLE Mutex;
HANDLE SemI, SemH, SemG, SemF, SemE;

unsigned int lab3_thread_graph_id() 
{
    return 2;
}

const char* lab3_unsynchronized_threads()
{
    return "ikm";
}

const char* lab3_sequential_threads()
{
    return "ihgfe";
}

DWORD WINAPI threads_unsynchronized(LPVOID text)
{
	for (int i = 0; i < 3; ++i)
	{
		WaitForSingleObject(Mutex, INFINITE);
		cout << (char const*)text << flush;
		ReleaseMutex(Mutex);
		computation();
	}
	return 0;
}

DWORD WINAPI thread_e(LPVOID text)
{
	for (int i = 0; i < 3; ++i)
	{
		WaitForSingleObject(SemE, INFINITE);
		WaitForSingleObject(Mutex, INFINITE);
		cout << (char const*)text << flush;
		ReleaseMutex(Mutex);
		computation();
		ReleaseSemaphore(SemF, 1, NULL);
	}
	return 0;
}

DWORD WINAPI thread_f(LPVOID text)
{
	for (int i = 0; i < 3; ++i)
	{
		WaitForSingleObject(SemF, INFINITE);
		WaitForSingleObject(Mutex, INFINITE);
		cout << (char const*)text << flush;
		ReleaseMutex(Mutex);
		computation();
		ReleaseSemaphore(SemG, 1, NULL);
	}
	return 0;
}

DWORD WINAPI thread_g(LPVOID text)
{
	for (int i = 0; i < 3; ++i)
	{
		WaitForSingleObject(SemG, INFINITE);
		WaitForSingleObject(Mutex, INFINITE);
		cout << (char const*)text << flush;
		ReleaseMutex(Mutex);
		computation();
		ReleaseSemaphore(SemH, 1, NULL);
	}
	return 0;
}

DWORD WINAPI thread_h(LPVOID text)
{
	for (int i = 0; i < 3; ++i)
	{
		WaitForSingleObject(SemH, INFINITE);
		WaitForSingleObject(Mutex, INFINITE);
		cout << (char const*)text << flush;
		ReleaseMutex(Mutex);
		computation();
		ReleaseSemaphore(SemI, 1, NULL);
	}
	return 0;
}

DWORD WINAPI thread_i(LPVOID text)
{
	for (int i = 0; i < 3; ++i)
	{
		WaitForSingleObject(SemI, INFINITE);
		WaitForSingleObject(Mutex, INFINITE);
		cout << (char const*)text << flush;
		ReleaseMutex(Mutex);
		computation();
		ReleaseSemaphore(SemE, 1, NULL);
	}
	return 0;
}

int lab3_init()
{
	DWORD ThreadID;

	Mutex = CreateMutex(NULL, FALSE, NULL);
	if (Mutex == NULL)
	{
		cout << "CreateMutex error " << GetLastError() << endl;
		return 1;
	}

	SemI = CreateSemaphore(NULL, 0, 1, NULL);
	if (SemI == NULL)
	{
		cout << "CreateSemaphore error: SemH" << GetLastError() << endl;
		return 1;
	}

	SemH = CreateSemaphore(NULL, 0, 1, NULL);
	if (SemH == NULL)
	{
		cout << "CreateSemaphore error: SemI " << GetLastError() << endl;
		return 1;
	}

	SemG = CreateSemaphore(NULL, 0, 1, NULL);
	if (SemG == NULL)
	{
		cout << "CreateSemaphore error: SemM" << GetLastError() << endl;
		return 1;
	}

	SemF = CreateSemaphore(NULL, 0, 1, NULL);
	if (SemF == NULL)
	{
		cout << "CreateSemaphore error: SemM" << GetLastError() << endl;
		return 1;
	}

	SemE = CreateSemaphore(NULL, 1, 1, NULL);
	if (SemE == NULL)
	{
		cout << "CreateSemaphore error: SemM" << GetLastError() << endl;
		return 1;
	}

	int count = 0;

	char const* textsAB[] = { "a", "b" };
	
	for (int i = 0; i < 2; ++i)
	{
		Thread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threads_unsynchronized, (void*)textsAB[i], 0, &ThreadID);
		if (Thread[i] == NULL)
		{
			cout << "CreateThread error: " << textsAB[i] << GetLastError() << endl;
			return 1;
		}
		else ++count;
	}

	for (int i = 0; i < count; ++i) {
		WaitForSingleObject(Thread[i], INFINITE);
	}

	count = 0;

	char const* textsBCDE[] = {"b", "c", "d", "e"}; 

	for (int i = 0; i < 4; ++i)
	{
		Thread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threads_unsynchronized, (void*)textsBCDE[i], 0, &ThreadID);
		if (Thread[i] == NULL)
		{
			cout << "CreateThread error: " << textsBCDE[i] << GetLastError() << endl;
			return 1;
		}
		else ++count;
	}

	for (int i = 0; i < count; ++i) {
		WaitForSingleObject(Thread[i], INFINITE);
	}

	char const* textsEFGHI[] = { "e", "f", "g", "h", "i" };

	Thread[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_e, (void*)textsEFGHI[0], 0, &ThreadID);
	if (Thread[0] == NULL)
	{
		cout << "CreateThread error:  " << textsEFGHI[0] << GetLastError() << endl;
		return 1;
	}

	Thread[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_f, (void*)textsEFGHI[1], 0, &ThreadID);
	if (Thread[1] == NULL)
	{
		cout << "CreateThread error:  " << textsEFGHI[1] << GetLastError() << endl;
		return 1;
	}

	Thread[2] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_g, (void*)textsEFGHI[2], 0, &ThreadID);
	if (Thread[2] == NULL)
	{
		cout << "CreateThread error:  " << textsEFGHI[2] << GetLastError() << endl;
		return 1;
	}

	Thread[3] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_h, (void*)textsEFGHI[3], 0, &ThreadID);
	if (Thread[3] == NULL)
	{
		cout << "CreateThread error:  " << textsEFGHI[3] << GetLastError() << endl;
		return 1;
	}

	Thread[4] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_i, (void*)textsEFGHI[4], 0, &ThreadID);
	if (Thread[4] == NULL)
	{
		cout << "CreateThread error:  " << textsEFGHI[4] << GetLastError() << endl;
		return 1;
	}

	for (int i = 0; i < 5; ++i) {
		WaitForSingleObject(Thread[i], INFINITE);
	}

	count = 0;

	char const* textsIKM[] = {"i", "k", "m"};

	for (int i = 0; i < 3; ++i)
	{
		Thread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threads_unsynchronized, (void*)textsIKM[i], 0, &ThreadID);
		if (Thread[i] == NULL)
		{
			cout << "CreateThread error: " << textsIKM[i] << GetLastError() << endl;
			return 1;
		}
		else ++count;
	}

	for (int i = 0; i < count; ++i) {
		WaitForSingleObject(Thread[i], INFINITE);
	}

	char const* textsN = "n";
	Thread[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threads_unsynchronized, (void*)textsN, 0, &ThreadID);
	if (Thread[0] == NULL)
	{
		cout << "CreateThread error: " << textsN << GetLastError() << endl;
		return 1;
	}

	WaitForSingleObject(Thread[0], INFINITE);

	CloseHandle(SemE);
	CloseHandle(SemF);
	CloseHandle(SemG);
	CloseHandle(SemH);
	CloseHandle(SemI);
	CloseHandle(Mutex);

	cout << endl;

    return 0;
}
