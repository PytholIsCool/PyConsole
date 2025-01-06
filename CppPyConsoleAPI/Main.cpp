#include <iostream>
#include <algorithm>
#include <Windows.h>
#include "PyConsole/Console.hpp"
using Color = Console::Color;

static void ToLower(std::string& s) {
	std::transform(s.begin(), s.end(), s.begin(),
		[](unsigned char c) { return std::tolower(c); });
}

int main() {
	bool isRunning = true;
	std::string input;
	input.reserve(5); //Over-allocating a bit but i may as well. free will cant be fully accounted for
	std::string eIn;
	eIn.reserve(3); //Doing the same here, but a bit less since it'll be used less

	while (isRunning) {
		Console::SetForegroundColor(Color::BabyBlue);

		Console::Write("This is my test Console App and/or API. \nWhat would you like to do?\n");
		Console::WriteLine("1. Credits", Color::DarkPurple);
		Console::WriteLine("2. Exit \n", Color::DarkPurple);

		Console::SetForegroundColor(Color::White);

		input = Console::ReadLine();

		if (input == "1") {
			Console::Clear();

			Console::Write("1.) Me (", Color::LightGray);
			Console::Write("Pythol", Color::Purple);
			Console::WriteLine(") for making this", Console::Color::LightGray);

			Console::Write("2.) ", Color::LightGray);
			Console::Write("Voids ", Color::BabyBlue);
			Console::WriteLine("for reminding me to finish and polish it", Color::LightGray);

			Console::Write("3.) ", Color::LightGray);
			Console::WriteLine("My amazing girlfriend for existing <3\n", Color::Rose);

			Console::Write("Press ", Color::DarkPurple);
			Console::Write("ENTER ", Color::BabyBlue);
			Console::WriteLine("to return", Color::DarkPurple);

			Console::ReadLine();
		}
		else if (input == "2") {
			Console::Clear();

			Console::WriteLine("Are you sure you want to exit?\nY/N", Color::Red);
			ToLower(eIn = Console::ReadLine());
			if (eIn == "y" || eIn == "yes")
				isRunning = false;
		}
		else {
			Console::WriteLine("Invalid input. Try again.");
			Console::ReadLine();
		}

		Console::Clear();
		input.clear();
		input.shrink_to_fit();
		eIn.clear();
		eIn.shrink_to_fit();
	}
}