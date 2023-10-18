// Kommunalka.cpp 
#include <iostream>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <string>
#include <vector>
#include <io.h>
#include <exception>
#include<algorithm>
#include<fstream>
#include <direct.h> 
using namespace std;

enum Color
{
	Black, Blue, Green, Cyan, Red, Magenta, Brown,
	LightGray, DarkGray, LightBlue, LightGreen, LightCyan, LightRed, LightMagenta, Yellow, White
};
HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
void setColor(Color text, Color background)
{
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}
enum Direction { Up = 72, Left = 75, Right = 77, Down = 80, Enter = 13, Esc = 27 };
void setCursor(int x, int y)
{
	COORD myCoords = { x,y };
	SetConsoleCursorPosition(hStdOut, myCoords);
}
struct abonent
{
	string login;
	string pass;
};

struct Kvitanciya
{
	int hotwater;
	int coldwater;
	int electro;
	int gas;
};
int X{}, Y{};
void setdataKvitanciya(Kvitanciya& obj)
{
	cout << "\nВведите показания горячей воды: ";
	cin >> obj.hotwater;
	cout << "\nВведите показания холодной воды: ";
	cin >> obj.coldwater;
	cout << "\nВведите показания электричества: ";
	cin >> obj.electro;
	cout << "\nВведите показания газа: ";
	cin >> obj.gas;
}

void printMenu(vector<string>masMenu, int punct, int X, int Y)
{
	for (size_t i = 0; i < masMenu.size(); i++)
	{
		if (i == punct)
		{
			setColor(Green, Black);
		}
		else
		{
			setColor(Magenta, Black);
		}
		setCursor(X, Y + i);
		cout << masMenu[i];
	}
	setColor(White, Black);

}
int choiseMenu(vector<string>masMenu, int X, int Y)
{
	int key;
	int punctNumber = 0;
	do
	{
		printMenu(masMenu, punctNumber, X, Y);
		key = _getch();//возвращает код нажатой клавиши
		switch (key)
		{
		case Up:
			if (punctNumber > 0)
			{
				punctNumber--;
			}
			break;

		case Down:
			if (punctNumber < masMenu.size() - 1)
			{
				punctNumber++;
			}
			break;
		case Enter:
			if (punctNumber == masMenu.size() - 1)
			{
				return -1;
			}
			return punctNumber;
			break;
		}
	} while (key != Esc);
	return -1;
}
struct tarif
{
	double khotwater = 171.34;
	double kcoldwater = 30.95;
	double kelectro = 4.02;
	double kotoplenie = 2136.65;
	double normatifotoplenie = 0.0234;
	double kgas = 9.5;
	double S;
	double tarifkapremont = 8.05;
	//показания горячей воды
	//коммунальные платежи за этот месяц минус предыдущий = что нам надо уплатить 
	// то, что мы получили умножить на коэффициент
}actualtarif; 
double summToPay(Kvitanciya currentmonth,Kvitanciya previousmonth)
{
	double hotwaterprice = (currentmonth.hotwater - previousmonth.hotwater) * actualtarif.khotwater;
	cout << "\nЗа горячую воду начислено: " << hotwaterprice << " рублей." << endl;
	double coldwaterprice = (currentmonth.coldwater - previousmonth.coldwater) * actualtarif.kcoldwater;
	cout << "\nЗа холодную воду начислено: " << coldwaterprice << " рублей." << endl;
	double electroprice = (currentmonth.electro - previousmonth.electro) * actualtarif.kelectro;
	cout << "\nЗа электричество начислено: " << electroprice << " рублей." << endl;
	double otoplenieprice = actualtarif.normatifotoplenie * actualtarif.S * actualtarif.kotoplenie;
	cout << "\nЗа отопление начислено: " << otoplenieprice << " рублей." << endl;
	double gasprice = (currentmonth.gas - previousmonth.gas) * actualtarif.kgas;
	cout << "\nЗа газ начислено: " << gasprice << " рублей." << endl;
	double kapremontprice = actualtarif.S * actualtarif.tarifkapremont;
	cout << "\nЗа Капитальный ремонт начислено: " << kapremontprice << " рублей." << endl;
	//кол-во жильцов и площадь взять из актуал тариф
	double summa = hotwaterprice + coldwaterprice + electroprice + otoplenieprice + gasprice + kapremontprice;
	cout << "\nИтого начислено: " << summa << " рублей." << endl;
	return summa; //вернуть сумму платежей
}

int main()
{
	setlocale(LC_ALL, "Rus");
	srand(time(0));
	system("cls");
	int index;

	X = 35;
	Y = 2;
	setCursor(X, Y);
	cout << "Калькулятор расчета квартплаты." << endl;
	X = 45;
	Y = 4;
	setCursor(X, Y);
	vector <string> menu{ "Регистрация","Авторизация" };
	index = choiseMenu(menu, X, Y);
	system("cls");
	abonent user;
	abonent temp;
	tarif Tarif;
	bool isCorrect = true;
	//регистрация
	Kvitanciya currentmonth, previousmonth;
	//текстовый файлик этого месяца
	time_t rtime;
	time(&rtime);
	tm currentTime;
	localtime_s(&currentTime, &rtime);
	int month, year;
	month = currentTime.tm_mon + 1;
	year = currentTime.tm_year + 1900;
	char* mas = new char[10];
	string fileName = "."; _itoa_s(month, mas, 10, 10);
	fileName += "\\";
	fileName += mas;
	fileName += "_";
	_itoa_s(year, mas, 10, 10);
	fileName += mas;
	fileName += ".txt";

	//текстовый файлик прошлого месяца
	time_t rtime1;
	time(&rtime1);
	tm currentTime1;
	localtime_s(&currentTime1, &rtime1);
	int prevmonth, year1;
	prevmonth = currentTime1.tm_mon;
	year1 = currentTime1.tm_year + 1900;
	char* mas1 = new char[10];
	string FileName = "."; _itoa_s(prevmonth, mas1, 10, 10);
	FileName += "\\";
	FileName += mas1;
	FileName += "_";
	_itoa_s(year1, mas1, 10, 10);
	FileName += mas1;
	FileName += ".txt";
	/*cout << FileName;*/

	if (index != -1)
	{
		X = 45;
		Y = 4;
		setCursor(X, Y);
		cout << "Придумайте логин: " << endl;
		X = 45;
		Y = 5;
		setCursor(X, Y);
		cin >> user.login;
		ifstream readF("abonent.txt");
		if (readF.is_open())
		{
			while (!readF.eof())
			{
				readF >> temp.login >> temp.pass;
				if (temp.login.compare(user.login) == 0)
				{
					cout << "Логин занят!" << endl;
					isCorrect = false;
					break;
				}
			}
			readF.close();
		}
		if (isCorrect)
		{
			ofstream writeF("abonent.txt", ios::app);
			X = 45;
			Y = 6;
			setCursor(X, Y);
			cout << "Придумайте пароль: " << endl;
			X = 45;
			Y = 7;
			setCursor(X, Y);
			cin >> user.pass;
			//если файл не пуст - записывать перед новым юзером \n
			/*writeF <<"\nЛогин: " << user.login << " "<<"Пароль: " << user.pass;*/
			writeF << " " << user.login << " " << user.pass;
			writeF.close();
			X = 45;
			Y = 8;
			setCursor(X, Y);
			cout << "Регистрация прошла успешно!\n";
			Sleep(2000);
			system("cls");
			_mkdir(user.login.c_str()); //создаем папку с именем как логин пользователя 
			/*string path = user.login + "UserInfo.txt";*/
			string path3 = user.login + "/" + user.login + "UserInfo.txt";
			writeF.open(path3);
			X = 45;
			Y = 4;
			setCursor(X, Y);
			cout << "Введите площадь квартиры: " << endl;
			X = 45;
			Y = 5;
			setCursor(X, Y);
			cin >> actualtarif.S;
			if (writeF.is_open())
			{
				writeF << actualtarif.S;
				writeF.close();
			}

			/*cout << fileName;*/
			//создаем файловый поток в котором создаем файл по этому адресу fileName
			X = 45;
			Y = 7;
			setCursor(X, Y);
			cout << "Введите показания счетчиков за текущий месяц: " << endl;
			setdataKvitanciya(currentmonth);
			string path2 = user.login + "/" + fileName;
			writeF.open(path2);
			if (writeF.is_open())
			{
				writeF << currentmonth.coldwater << " " << currentmonth.hotwater << " " << currentmonth.electro << " " << currentmonth.gas;
				writeF.close();
			}

			X = 45;
			Y = 17;
			setCursor(X, Y);
			cout << "Введите показания счетчиков за прошлый месяц: " << endl;
			setdataKvitanciya(previousmonth);
			string path = user.login + "/" + FileName;
			writeF.open(path);
			if (writeF.is_open())
			{
				writeF << previousmonth.coldwater << " " << previousmonth.hotwater << " " << previousmonth.electro << " " << previousmonth.gas;
				writeF.close();
			}
			X = 45;
			Y = 28;
			setCursor(X, Y);
			cout << "Начисления коммунальных платежей:" << endl;
			summToPay(currentmonth, previousmonth);
		}
	}
	else //авторизация
	{
		do
		{
			X = 45;
			Y = 4;
			setCursor(X, Y);
			cout << "Введите логин: " << endl;
			X = 45;
			Y = 5;
			setCursor(X, Y);
			getline(cin, user.login);
			ifstream readF("abonent.txt");
			if (readF.is_open())
			{
				isCorrect = false;
				while (!readF.eof())
				{

					readF >> temp.login >> temp.pass;
					if (temp.login.compare(user.login) == 0)
					{
						X = 45;
						Y = 6;
						setCursor(X, Y);
						cout << "Логин принят!" << endl;
						isCorrect = true;
						break;
					}

				}
				readF.close();
				if (isCorrect)
				{
					X = 45;
					Y = 7;
					setCursor(X, Y);
					cout << "Введите пароль: " << endl;
					X = 45;
					Y = 8;
					setCursor(X, Y);
					getline(cin, user.pass);
					if (temp.pass.compare(user.pass) == 0)
					{
						X = 45;
						Y = 9;
						setCursor(X, Y);
						cout << "Пароль принят" << endl;
						Sleep(2000);
						system("cls");
						X = 45;
						Y = 4;
						setCursor(X, Y);
						cout << "Показания счетчиков за прошлый месяц: " << endl;
						string path = user.login + "/" + FileName;
						readF.open(path);

						if (readF.is_open())
						{
							readF >> previousmonth.coldwater >> previousmonth.hotwater >> previousmonth.electro >> previousmonth.gas;
							cout << "\n Горячая вода: " << previousmonth.hotwater;
							cout << "\n Холодная вода: " << previousmonth.coldwater;
							cout << "\n Электричество: " << previousmonth.electro;
							cout << "\n Газ: " << previousmonth.gas;
							readF.close();
						}
						else
						{
							do
							{
								prevmonth--;
								FileName = "."; _itoa_s(prevmonth, mas1, 10, 10);
								FileName += "\\";
								FileName += mas1;
								FileName += "_";
								_itoa_s(year1, mas1, 10, 10);
								FileName += mas1;
								FileName += ".txt";
								readF.open(path);
							} while (!readF.is_open());

							readF >> previousmonth.coldwater >> previousmonth.hotwater >> previousmonth.electro >> previousmonth.gas;
							cout << "\n Горячая вода: " << previousmonth.hotwater;
							cout << "\n Холодная вода: " << previousmonth.coldwater;
							cout << "\n Электричество: " << previousmonth.electro;
							cout << "\n Газ: " << previousmonth.gas;
							readF.close();
						}
						string path3 = user.login + "/" + user.login + "UserInfo.txt";
						readF.open(path3); 
						if (readF.is_open())
						{
							readF >> actualtarif.S;
							cout << "\n Площадь квартиры: " << actualtarif.S<<endl;
							readF.close();
						}
						ofstream writeF;
						X = 45;
						Y = 12;
						setCursor(X, Y);
						cout << "Введите показания счетчиков за текущий месяц: " << endl;
						setdataKvitanciya(currentmonth);
						string path2 = user.login + "/" + fileName;
						writeF.open(path2);
						if (writeF.is_open())
						{
							writeF << currentmonth.coldwater << " " << currentmonth.hotwater << " " << currentmonth.electro << " " << currentmonth.gas;
							writeF.close();
						}
						X = 45;
						Y = 22;
						setCursor(X, Y);
						cout << "Начисления коммунальных платежей:" << endl;
						summToPay(currentmonth, previousmonth);
					}
				}
			}
		} while (!isCorrect);
	}
}