#include<iostream>
#include<thread>
using namespace std;

bool finish = false;


void Plus()
{
	while (!finish)
	{
		cout << "+ ";
		std::this_thread::sleep_for(1s);
	}
}
void Minus()
{
	while (!finish)
	{
		cout << "- ";
		std::this_thread::sleep_for(1s);
	}
}

void main()
{
	setlocale(LC_ALL, "");
	//Plus();
	//Minus();
	std::thread plus_thread(Plus);
	std::thread minus_thread(Minus);
	//std::thread plus_thread = std::thread(Plus);
/*	std::thread minus_thread = std::thread(Minus)*/;

	cin.get();
	finish = true;
	//minus_thread.join();
	//plus_thread.join();
	if (minus_thread.joinable())minus_thread.join();
	if (plus_thread.joinable())plus_thread.join();
}