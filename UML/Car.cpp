#include<iostream>
#include<conio.h>
#include<thread>
using namespace std;
using std::cout;
using std::cin;
using std::endl;

#define tab "\t"

#define Escape 27
#define Enter 13

#define MIN_TANK_VOLUME 28
#define MAX_TANK_VOLUME 80


class Tank
{
	const int VOLUME;
	double fuel_level;
public:
	int get_VOLUME()const
	{
		return VOLUME;
	}
	double get_fuel_level()const
	{
		return fuel_level;
	}
	void fill(double fuel)
	{
		if (fuel < 0)return;
		fuel_level += fuel;
		if (fuel_level > VOLUME) fuel_level = VOLUME;
	}
	double give_fuel(double amount)
	{
		fuel_level -= amount;
		if (fuel_level < 0)fuel_level = 0;
		return fuel_level;
	}
	//				constructors;
	Tank(int volume = 40):VOLUME(
	volume<MIN_TANK_VOLUME?MIN_TANK_VOLUME:
	volume>MAX_TANK_VOLUME?MAX_TANK_VOLUME:
	volume)
	{
		fuel_level = 0;
		cout << "Tank is ready" << endl;
	}
	~Tank()
	{
		cout << "Tank is over" << endl;
	}
	//			Methos:
	void info()const
	{
		cout << "Volume: " << VOLUME << " liters.\n";
		cout << "Fuel lvl: " << fuel_level << " liters.\n";
	}

};
#define MIN_ENGINE_CONSUMPTION 4
#define MAX_ENGINE_CONSUMPTION 30
class Engine
{
	double consumption;
	double consumtion_per_second;
	bool is_started;
public:
	double get_consumption()const
	{
		return consumption;
	}
	double get_consumtion_per_second()const
	{
		return consumtion_per_second;
	}
	bool started()const
	{
		return is_started;
	}
	void start()
	{
		is_started = true;
	}
	void stop()
	{
		is_started = false;
	}
	void set_consumption(double consumption)
	{
		if (consumption < MIN_ENGINE_CONSUMPTION)consumption = MIN_ENGINE_CONSUMPTION;
		if (consumption > MAX_ENGINE_CONSUMPTION)consumption = MAX_ENGINE_CONSUMPTION;
		this->consumption = consumption;
		set_consumtion_per_second();
	}
	void set_consumtion_per_second()
	{
		consumtion_per_second = consumption * 3e-5;
	}

	Engine(double consumption)
	{
		set_consumption(consumption);
		is_started = false;
		cout << "Engine is ready" << endl;
	}
	~Engine()
	{
		cout << "Engine is over" << endl;
	}
	void info()const
	{
		cout << "consumption per 100 km: " << consumption << " liters.\n";
		cout << "consumption per second: " << consumtion_per_second << " liters.\n";
		cout << "Engine is " << (is_started ? "started" : "stopped") << endl;

	}
};

#define MAX_SPEED_LOW_LIMIT 40
#define MAX_SPEED_HIGH_LIMIT 400
class Car
{
	Engine engine;
	Tank tank;
	int speed;
	const int MAX_SPEED;
	bool driver_inside;
	struct Treads
	{
		std::thread panel_thread;
		std::thread engine_idle_thread;

	}threads;
public:
	Car(const Engine& engine, const Tank& tank, int max_speed)
		:engine(engine), tank(tank), speed(0),
		MAX_SPEED(
			max_speed < MAX_SPEED_LOW_LIMIT ? MAX_SPEED_LOW_LIMIT :
			max_speed > MAX_SPEED_HIGH_LIMIT ? MAX_SPEED_HIGH_LIMIT :
			max_speed
		)
	{
		driver_inside = false;
	}
	Car(int consumption, int volume, int max_speed)
		:engine(consumption),
		tank(volume),
		speed(0),
		MAX_SPEED
		(
			max_speed < MAX_SPEED_LOW_LIMIT ? MAX_SPEED_LOW_LIMIT :
			max_speed > MAX_SPEED_HIGH_LIMIT ? MAX_SPEED_HIGH_LIMIT :
			max_speed
		)
	{
		driver_inside = false;
	}
	~Car()
	{
		cout << "Finita la comedia" << endl;
	}

	void fill(double fuel)
	{
		tank.fill(fuel);
	}
	void get_in()
	{
		driver_inside = true;
		threads.panel_thread = std::thread(&Car::panel, this);
	}
	void get_out()
	{
		driver_inside = false;
		if(threads.panel_thread.joinable())threads.panel_thread.join();
		system("CLS");
		cout << "You are out of your car" << endl;
	}
	void start()
	{
		if (driver_inside && tank.get_fuel_level())
		{
			engine.start();
			threads.engine_idle_thread = std::thread(&Car::engine_idle, this);
		}
	}
	void stop()
	{
		engine.stop();
		if (threads.engine_idle_thread.joinable())threads.engine_idle_thread.join();
	}

	void control()
	{
		cout << "Your car is ready to go, press Enter to get in." << endl;
		char key = 0;
		do
		{
			if (_kbhit())
			{
				key = _getch();
				switch (key)
				{
				case 'F': case 'f':
					if (driver_inside)
					{
						cout << "C����, �� ����� �����: " << endl;
					}
					else
					{
						double fuel;
						cout << "������� ����� �������: "; cin >> fuel;
						tank.fill(fuel);
					}
					break;
				case 'E': case 'e': case Enter:
					if (driver_inside)get_out();
					else get_in();
					break;
				case 'I': case 'i':
					engine.started() ? stop() : start();
					break;
				case Escape:
					get_out();
				}
			}
			if (tank.get_fuel_level() == 0) stop();
		} while (key != Escape);
		
	}
	void engine_idle()
	{
		while (engine.started() && tank.give_fuel(engine.get_consumtion_per_second()))
			std::this_thread::sleep_for(1s);
	}

	void panel()const
	{
		while (driver_inside)
		{
			system("CLS");
			cout << "Fuel level:\t" << tank.get_fuel_level() << " liters.\n";
			cout << "Engine is " << (engine.started() ? "started" : "stopped") << endl;
			if (tank.get_fuel_level() <= 5)
			{
				cout << "LOW FUEL";
			}
			cout << endl;
			std::this_thread::sleep_for(1s);
		
		} 
	}

	void info()const
	{
		cout << "\n============================================================\n" << endl;
		engine.info();
		tank.info();
		cout << "Max speed: " << MAX_SPEED << "km/h. \n";
		cout << "\n============================================================\n" << endl;
	}

};

//#define TANK_CHEAK
//#define ENGINE_CHEAK

void main()
{
	setlocale(LC_ALL, "");

#ifdef TANK_CHEAK
	Tank tank;
	double fuel;
	//do
	//{
	//	cout << "������� ����� �������: "; cin >> fuel;
	//	tank.fill(fuel);
	//	tank.info();
	//} while (true);
#endif // TANK_CHEAK
#ifdef ENGINE_CHEAK
	Engine engine(20);
	engine.info();
#endif // EN

	//Car bmw(engine, tank, 280); bmw.info();
	//Car bmw(Engine(12), Tank(80), 280); bmw.info();
	Car bmw(12, 80, 280);
	//bmw.fill(25);
	//bmw.info();
	bmw.control();


}