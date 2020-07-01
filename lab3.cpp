#include "lab3.h"
#include <windows.h> 
#include <stdio.h>

using namespace std;

#define THREADCOUNT 12

HANDLE Thread[THREADCOUNT];
HANDLE Mutex;
HANDLE SemI, SemH, SemG, SemF, SemE, SemFirst, SemSecond;

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

DWORD WINAPI threads_acdkmn(LPVOID text)
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

DWORD WINAPI thread_b(LPVOID text)
{
	for (int i = 0; i < 3; ++i)
	{
		WaitForSingleObject(Mutex, INFINITE);
		cout << (char const*)text << flush;
		ReleaseMutex(Mutex);
		computation();
	}

	WaitForSingleObject(Thread[0], INFINITE);
	ReleaseSemaphore(SemFirst, 1, NULL);

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
		WaitForSingleObject(Mutex, INFINITE);
		cout << (char const*)text << flush;
		ReleaseMutex(Mutex);
		computation();
	}

	WaitForSingleObject(Thread[2], INFINITE); //c
	WaitForSingleObject(Thread[3], INFINITE); //d
	WaitForSingleObject(Thread[1], INFINITE); //b

	ReleaseSemaphore(SemSecond, 1, NULL);

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

	WaitForSingleObject(Thread[4], INFINITE); //e

	for (int i = 0; i < 3; ++i)
	{
		WaitForSingleObject(Mutex, INFINITE);
		cout << (char const*)text << flush;
		ReleaseMutex(Mutex);
		computation();
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
		cout << "CreateSemaphore error: SemG" << GetLastError() << endl;
		return 1;
	}

	SemF = CreateSemaphore(NULL, 0, 1, NULL);
	if (SemF == NULL)
	{
		cout << "CreateSemaphore error: SemF" << GetLastError() << endl;
		return 1;
	}

	SemE = CreateSemaphore(NULL, 1, 1, NULL);
	if (SemE == NULL)
	{
		cout << "CreateSemaphore error: SemE" << GetLastError() << endl;
		return 1;
	}

	SemFirst = CreateSemaphore(NULL, 0, 1, NULL);
	if (SemFirst == NULL)
	{
		cout << "CreateSemaphore error: SemFirst" << GetLastError() << endl;
		return 1;
	}

	SemSecond = CreateSemaphore(NULL, 0, 1, NULL);
	if (SemSecond == NULL)
	{
		cout << "CreateSemaphore error: SemSecond" << GetLastError() << endl;
		return 1;
	}

	char const* texts[] = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "k", "m", "n" };

	Thread[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threads_acdkmn, (void*)texts[0], 0, &ThreadID); // a
	if (Thread[0] == NULL)
	{
		cout << "CreateThread error: " << texts[0] << GetLastError() << endl;
		return 1;
	}

	Thread[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_b, (void*)texts[1], 0, &ThreadID); // b
	if (Thread[1] == NULL)
	{
		cout << "CreateThread error: " << texts[1] << GetLastError() << endl;
		return 1;
	}
	
	WaitForSingleObject(Thread[0], INFINITE);
	WaitForSingleObject(SemFirst, INFINITE);

	Thread[2] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threads_acdkmn, (void*)texts[2], 0, &ThreadID); // c
	if (Thread[2] == NULL)
	{
		cout << "CreateThread error: " << texts[2] << GetLastError() << endl;
		return 1;
	}

	Thread[3] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threads_acdkmn, (void*)texts[3], 0, &ThreadID); //d
	if (Thread[3] == NULL)
	{
		cout << "CreateThread error:  " << texts[3] << GetLastError() << endl;
		return 1;
	}

	Thread[4] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_e, (void*)texts[4], 0, &ThreadID); //e
	if (Thread[4] == NULL)
	{
		cout << "CreateThread error:  " << texts[4] << GetLastError() << endl;
		return 1;
	}

	WaitForSingleObject(Thread[2], INFINITE);
	WaitForSingleObject(Thread[1], INFINITE);
	WaitForSingleObject(Thread[3], INFINITE);
	WaitForSingleObject(SemSecond, INFINITE);

	Thread[5] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_f, (void*)texts[5], 0, &ThreadID); // f
	if (Thread[5] == NULL)
	{
		cout << "CreateThread error:  " << texts[5] << GetLastError() << endl;
		return 1;
	}

	Thread[6] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_g, (void*)texts[6], 0, &ThreadID); //g
	if (Thread[6] == NULL)
	{
		cout << "CreateThread error:  " << texts[6] << GetLastError() << endl;
		return 1;
	}

	Thread[7] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_h, (void*)texts[7], 0, &ThreadID); //h
	if (Thread[7] == NULL)
	{
		cout << "CreateThread error:  " << texts[7] << GetLastError() << endl;
		return 1;
	}

	Thread[8] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_i, (void*)texts[8], 0, &ThreadID); //i
	if (Thread[8] == NULL)
	{
		cout << "CreateThread error:  " << texts[8] << GetLastError() << endl;
		return 1;
	}

	WaitForSingleObject(Thread[6], INFINITE);
	WaitForSingleObject(Thread[5], INFINITE);
	WaitForSingleObject(Thread[4], INFINITE);
	WaitForSingleObject(Thread[7], INFINITE);

	Thread[9] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threads_acdkmn, (void*)texts[9], 0, &ThreadID); //k
	if (Thread[9] == NULL)
	{
		cout << "CreateThread error:  " << texts[9] << GetLastError() << endl;
		return 1;
	}

	Thread[10] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threads_acdkmn, (void*)texts[10], 0, &ThreadID); //m
	if (Thread[10] == NULL)
	{
		cout << "CreateThread error:  " << texts[10] << GetLastError() << endl;
		return 1;
	}

	WaitForSingleObject(Thread[10], INFINITE);
	WaitForSingleObject(Thread[9], INFINITE);
	WaitForSingleObject(Thread[8], INFINITE);

	Thread[11] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threads_acdkmn, (void*)texts[11], 0, &ThreadID); //n
	if (Thread[11] == NULL)
	{
		cout << "CreateThread error:  " << texts[11] << GetLastError() << endl;
		return 1;
	}

	WaitForSingleObject(Thread[11], INFINITE);

	CloseHandle(SemSecond);
	CloseHandle(SemFirst);
	CloseHandle(SemE);
	CloseHandle(SemF);
	CloseHandle(SemG);
	CloseHandle(SemH);
	CloseHandle(SemI);
	CloseHandle(Mutex);

	cout << endl;

	return 0;
}