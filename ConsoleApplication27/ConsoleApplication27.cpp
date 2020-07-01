
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "Setting.h"

#pragma comment(lib, "winmm.lib")
#define NUMTHREAD 2
#define USEMUTEX
HANDLE threadMutex;
int gCounter;
std::vector<int> threadQueue;
struct person {
	int TravelTime;
	int Unloading;
	int ShoppingTime;
	int Max;
	int AppleNumber;
};
struct yaoya {
	int AppleNumber;
};
static struct person Sakura;
static struct person Tomoyo;
static struct yaoya  Akito;
void ini() {
	char direct[1024];
	getCurrentDirectory(direct);
	if (writeIntInput("さくらさん", "移動時間", 1, direct, "買い物客")) {}
	if (writeIntInput("さくらさん", "荷卸し時間", 3, direct, "買い物客")) {}
	if (writeIntInput("さくらさん", "取引時間", 1, direct, "買い物客")) {}
	if (writeIntInput("さくらさん", "りんご容量", 5, direct, "買い物客")) {}
	if (writeIntInput("さくらさん", "りんごの数", 0, direct, "買い物客")) {}
	if (writeIntInput("ともよさん", "移動時間", 2, direct, "買い物客")) {}
	if (writeIntInput("ともよさん", "荷卸し時間", 1, direct, "買い物客")) {}
	if (writeIntInput("ともよさん", "取引時間", 2, direct, "買い物客")) {}
	if (writeIntInput("ともよさん", "りんご容量", 3, direct, "買い物客")) {}
	if (writeIntInput("ともよさん", "りんごの数", 0, direct, "買い物客")) {}
	if (writeIntInput("あきとさん", "りんご総数", 100, direct, "八百屋")) {}

	Sakura.TravelTime = readInt("さくらさん", "移動時間", -1, direct, "買い物客");
	Sakura.Unloading = readInt("さくらさん", "荷卸し時間", -1, direct, "買い物客");
	Sakura.Max = readInt("さくらさん", "りんご容量", -1, direct, "買い物客");
	Sakura.ShoppingTime = readInt("さくらさん", "取引時間", -1, direct, "買い物客");
	Sakura.AppleNumber = readInt("さくらさん", "りんごの数", -1, direct, "買い物客");
	Tomoyo.TravelTime = readInt("ともよさん", "移動時間", -1, direct, "買い物客");
	Tomoyo.Unloading = readInt("ともよさん", "荷卸し時間", -1, direct, "買い物客");
	Tomoyo.Max = readInt("ともよさん", "りんご容量", -1, direct, "買い物客");
	Tomoyo.ShoppingTime = readInt("ともよさん", "取引時間", -1, direct, "買い物客");
	Tomoyo.AppleNumber = readInt("ともよさん", "りんごの数", -1, direct, "買い物客");
	Akito.AppleNumber=readInt("あきとさん","りんご総数",-1,direct,"八百屋");

}

void process(struct person *aa) {
	while (Akito.AppleNumber != 0) {
		Sleep((aa->TravelTime)*100);
		fprintf_s(stdout, "!try to start thread(%d) process\n", aa->TravelTime);
		threadQueue.push_back(aa->TravelTime);
#ifdef USEMUTEX
		WaitForSingleObject(threadMutex, INFINITE);
#endif
		fprintf_s(stdout, "start thread(%d) process\n", aa->TravelTime);
		int prev = gCounter;
		Sleep((aa->ShoppingTime)*100);
		if (Akito.AppleNumber < aa->Max) {
			aa->AppleNumber = Akito.AppleNumber + aa->AppleNumber;
			Akito.AppleNumber = 0;
		}
		else {
			Akito.AppleNumber -= aa->Max;
			aa->AppleNumber = aa->Max + aa->AppleNumber;
		}
		
		gCounter += aa->TravelTime;
		fprintf_s(stdout, "finish threadA(%d) process %d -> %d\n", aa->TravelTime, prev, gCounter);
		if (!threadQueue.empty()) {
			threadQueue.erase(threadQueue.begin());
		}
		if (!threadQueue.empty()) {
			fprintf_s(stdout, "thread queue =");
			for (auto itr = threadQueue.begin(); itr != threadQueue.end(); itr++) {
				fprintf_s(stdout, " %d", *itr);
			}
			fprintf_s(stdout, "\n\n");
		}
		else {
			fprintf_s(stdout, "finish all\n\n");
		}
#ifdef USEMUTEX
		ReleaseMutex(threadMutex);
#endif
		printf("%dのリンゴの個数%d\n",aa->TravelTime, aa->AppleNumber);
		Sleep((aa->Unloading + aa->TravelTime)*100);
	}
}
void threadSample(struct person *data) {
	process(data);
}

int main() {
	fprintf(stdout, "Pass with variable\n");

	ini();
	
	int count = 0;
	HANDLE threadHandler[NUMTHREAD];
	DWORD threadID[NUMTHREAD];
	threadMutex = CreateMutex(NULL, FALSE, NULL);//ミューテックス生成
	
		fprintf_s(stdout, "create thread %s\n", "作ります");
		threadHandler[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadSample, &Sakura, 0, &threadID[0]);
		threadHandler[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadSample, &Tomoyo, 0, &threadID[0]);
	

	for (int i = 0; i < NUMTHREAD; i++) {
		WaitForSingleObject(threadHandler[i], INFINITE);
		CloseHandle(threadHandler[i]);
	}

	CloseHandle(threadMutex);
	char direct[1024];
	getCurrentDirectory(direct);
	fprintf(stdout, "End \n");
	writeIntOutput("さくらさん", "りんごの数", Sakura.AppleNumber, direct, "買い物");
	writeIntOutput("ともよさん", "りんごの数", Tomoyo.AppleNumber, direct, "買い物");
	return 0;
}
/*int gCounter;
int flag[2] = { 0,0 };
int flag2[2] = { 0,0 };
int wastetime[ 2 ] = { 0,0 };
char aaaa[1020];
std::vector<int> threadQueue;
void process(int waitTime) {
	int a1 = 0,a2=0,time=0;
	gCounter = 0;
		fprintf_s(stdout, "!try to start threadA(%d) process\n", waitTime);
		threadQueue.push_back(waitTime);
#ifdef USEMUTEX//他のスレッドがあるかどうか
		
		WaitForSingleObject(threadMutex, INFINITE);
#endif
		
		fprintf_s(stdout, "start threadA(%d) process\n", waitTime);
		int prev = gCounter;
		Sleep(1000);//スリープ
		if (waitTime == 100) {
			flag[0] += 100;
		}
		if (waitTime == 200) {
			flag[1] += 100;
		}
		
		gCounter += waitTime;//総合時間

		fprintf_s(stdout, "finish threadA(%d) process %d -> %d\n", waitTime,
			prev, gCounter);
		if (!threadQueue.empty()) {
			threadQueue.erase(threadQueue.begin());
		}
		if (!threadQueue.empty()) {
			fprintf_s(stdout, "threadA queue =");
			for (auto itr = threadQueue.begin(); itr != threadQueue.end(); itr++)
			{
				fprintf_s(stdout, " %d", *itr);
			}
			fprintf_s(stdout, "\n\n");
		}
		else {
			fprintf_s(stdout, "finish all\n\n");
		}
#ifdef USEMUTEX
		ReleaseMutex(threadMutex);
#endif
		if (waitTime == 100) {
			flag2[0] = 0;
		}
		if (waitTime == 200) {
			flag2[1] =0;
		}
}
void threadSample(LPVOID data) {
	process((int)data);
}
int main() {
	getCurrentDirectory(aaaa);
	char aaa[1024];
	struct person {
		int TravelTime;
		int Unloading;
		int ShoppingTime;
		int Max;
		int AppleNumber;
	};
	struct yaoya {
		int AppleNumber;
	};
	static struct person Sakura = { 1,3,1,5,0 };
	static struct person Tomoyo = { 2,1,2,3,0 };
	static struct yaoya  Akito = { 100 };
	fprintf(stdout, "Pass with variable\n");
	int count = 0;
	HANDLE threadHandler[NUMTHREAD];
	DWORD threadID[NUMTHREAD];
	threadMutex = CreateMutex(NULL, FALSE, NULL);// ミュ テックス生成 ー
	for (int aa = 0;; aa += 100) {
		if (Akito.AppleNumber <= 0) {
			break;
		}
		if (flag[0] >= 0&& flag[0] < Sakura.TravelTime * 100) {
			flag[0] += 100;
		}
		else if (flag[0] >= Sakura.TravelTime * 100 && flag[0] < (Sakura.ShoppingTime + Sakura.TravelTime + wastetime[0])*100) {
			threadHandler[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadSample,
				(LPVOID)(Sakura.ShoppingTime*100), 0, &threadID[0]);
			WaitForSingleObject(threadHandler[0], INFINITE);
			
			/*
			if (flag[0] == Sakura.TravelTime * 100) {
				if (Akito.AppleNumber < Sakura.Max) {
					process(Sakura.ShoppingTime*100);
					Sakura.AppleNumber = Akito.AppleNumber + Sakura.AppleNumber;
					Akito.AppleNumber = 0;
				}
				else {
					Akito.AppleNumber -= Sakura.Max;
					Sakura.AppleNumber = Sakura.Max + Sakura.AppleNumber;
				}
			}*/
			
		/*}
		else if (flag[0] >= (Sakura.ShoppingTime + Sakura.TravelTime + wastetime[0])*100 && flag[0] < (Sakura.ShoppingTime + Sakura.TravelTime+ Sakura.TravelTime + Sakura.Unloading + wastetime[0])*100) {
			if (flag2[0] == 0) {
				flag2[0] = 1;
				if (flag[0] == (Sakura.ShoppingTime + Sakura.TravelTime + wastetime[0]) * 100) {
					if (Akito.AppleNumber < Sakura.Max) {
						process(Sakura.ShoppingTime * 100);
						Sakura.AppleNumber = Akito.AppleNumber + Sakura.AppleNumber;
						Akito.AppleNumber = 0;
					}
					else {
						Akito.AppleNumber -= Sakura.Max;
						Sakura.AppleNumber = Sakura.Max + Sakura.AppleNumber;
					}
				}
			}
			if (flag2[1] == 0) {
				flag[0] += 100;
			}
		}
		if(flag[0]>=(Sakura.ShoppingTime + Sakura.TravelTime + Sakura.TravelTime + Sakura.Unloading + wastetime[0]) * 100)  {
			flag[0] = 0;
			wastetime[0] = 0;
		}
		
		if (flag[1] >= 0&&flag[1] < Tomoyo.TravelTime * 100) {
			flag[1] += 100;
		}
		else if (flag[1] >= Tomoyo.TravelTime * 100 && flag[1] < (Tomoyo.ShoppingTime + Tomoyo.TravelTime + wastetime[1]) * 100) {

			threadHandler[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadSample,
				(LPVOID)(Tomoyo.ShoppingTime*100), 0, &threadID[1]);
			
			
			/*if (flag[1] == Tomoyo.TravelTime * 100) {
				if (Akito.AppleNumber < Tomoyo.Max) {
					process(Sakura.ShoppingTime * 100);
					Tomoyo.AppleNumber = Akito.AppleNumber + Tomoyo.AppleNumber;
					Akito.AppleNumber = 0;
				}
				else {
					Akito.AppleNumber -= Tomoyo.Max;
					Tomoyo.AppleNumber = Tomoyo.Max + Tomoyo.AppleNumber;
				}
			}*/
			
		/*}
		else if (flag[1] >= (Tomoyo.ShoppingTime + Tomoyo.TravelTime + wastetime[1]) * 100 && flag[1] < (Tomoyo.ShoppingTime + Tomoyo.TravelTime + Tomoyo.TravelTime + Tomoyo.Unloading + wastetime[1]) * 100) {
			if (flag2[1] == 0) {
				flag2[1] = 1;
				if (flag[1] == (Tomoyo.ShoppingTime + Tomoyo.TravelTime + wastetime[1]) * 100) {
					if (Akito.AppleNumber < Tomoyo.Max) {
						process(Sakura.ShoppingTime * 100);
						Tomoyo.AppleNumber = Akito.AppleNumber + Tomoyo.AppleNumber;
						Akito.AppleNumber = 0;
					}
					else {
						Akito.AppleNumber -= Tomoyo.Max;
						Tomoyo.AppleNumber = Tomoyo.Max + Tomoyo.AppleNumber;
					}
				}
			}
			if (flag2[0] == 0) {
				flag[1] += 100;
			}
		}
		if (flag[1] >= (Tomoyo.ShoppingTime + Tomoyo.TravelTime + Tomoyo.TravelTime + Tomoyo.Unloading + wastetime[1]) * 100) {
			flag[1] = 0;
			wastetime[1] = 0;
		}
		
		printf("経過時間%dは%d\n",flag[1],Akito.AppleNumber);
		printf("リンゴの数sakura%d個tomoyo%d個", Sakura.AppleNumber,Tomoyo.AppleNumber);
	}
	
	for (int i = 0; i < NUMTHREAD; i++) {
		int waitTime;
		if (i == 0) {
			 waitTime = Sakura.ShoppingTime * 100;
		}
		else {
			 waitTime = Tomoyo.ShoppingTime * 100;
		}
		fprintf_s(stdout, "create thread %d\n", waitTime);
		threadHandler[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadSample,
			(LPVOID)Sakura.ShoppingTime, 0, &threadID[i]);
	}
	for (int i = 0; i < NUMTHREAD; i++) {
		WaitForSingleObject(threadHandler[i], INFINITE);
		CloseHandle(threadHandler[i]);
	}
	CloseHandle(threadMutex);
	fprintf(stdout, "End \n");
	return 0;
}*/
