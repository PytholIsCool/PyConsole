#include <iostream>
#include <algorithm>
#include <Windows.h>
#include "PyConsole/Console.hpp"
#include <string>

using Color = Console::Color;

static void ToLower(std::string& s) {
	std::transform(s.begin(), s.end(), s.begin(),
		[](unsigned char c) { return std::tolower(c); });
}

void ShowCredits() {
	Console::Clear();

	Console::Write("1.) Me (", Color::LightGray);
	Console::Write("Pythol", Color::Purple);
	Console::WriteLine(") for making this", Color::LightGray);

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

void TestConsoleAPI() {
	Console::Clear();
	Console::SetTitle("PyConsole API Test");

	Console::WriteLine("Testing color output:", Color::LightBlue);
	Console::SetForegroundColor(Color::Red);
	Console::WriteLine("This should be red.");
	Console::SetForegroundColor(Color::Green);
	Console::WriteLine("This should be green.");
	Console::ResetColor();
	Console::WriteLine("This should be white.");
	Console::Pause();

	Console::Clear();
	Console::Write("Testing cursor movement... wait for it\n", Color::BabyBlue);
	Sleep(1000);
	Console::SetCursorPosition(0, 0); // Works
	Console::WriteLine("Cursor moved to top-left. Previously existing text was overridden.", Color::DarkGreen);
	Console::Pause();

	Console::Clear();
	Console::Debug("This is a debug message."); // Works
	Console::Error("This is an error message."); // Works
	Console::Pause();

	Console::Clear();
	Console::WriteLine("Type something: ");
	std::string line = Console::ReadLine(); // Works
	Console::Write("You typed: ", Color::LightYellow);
	Console::WriteLine(line, Color::LightBlue);
	Console::Pause();

	Console::Clear();
	Console::WriteLine("Press a key: ");
	char c = Console::ReadKey(); // Works
	Console::Write("You pressed: ");
	Console::WriteLine(std::string(1, c), Color::Rose);
	Console::Pause();

	Console::Clear();
	Console::WriteLine("Console Info Test:");
	Console::WriteLine("Width: " + std::to_string(Console::GetWidth()));
	Console::WriteLine("Height: " + std::to_string(Console::GetHeight()));
	Console::WriteLine("Cursor: (" + std::to_string(Console::GetCursorX()) + ", " + std::to_string(Console::GetCursorY()) + ")");
	Console::WriteLine("Press any key to test KeyAvailable...");
	while (!Console::KeyAvailable()) {}
	char key = Console::ReadKey();
	Console::Write("You pressed: ");
	Console::WriteLine(std::string(1, key));
	Console::Pause();

	Console::Clear();
	Console::WriteLine("Console title changed to: \"TestTitle\"");
	Console::SetTitle("TestTitle");
	Console::Pause();
}

int main() {
	bool isRunning = true;
	std::string input;
	input.reserve(5);
	std::string eIn;
	eIn.reserve(3);

	while (isRunning) {
		Console::Clear();
		Console::SetForegroundColor(Color::BabyBlue);
		Console::Write("Welcome to the PyConsole App. \nWhat would you like to do?\n");
		Console::WriteLine("1. Credits", Color::DarkPurple);
		Console::WriteLine("2. Run Console API Test", Color::DarkPurple);
		Console::WriteLine("3. Exit\n", Color::DarkPurple);
		Console::SetForegroundColor(Color::White);

		input = Console::ReadLine();

		if (input == "1") 
			ShowCredits();
		else if (input == "2") 
			TestConsoleAPI();
		else if (input == "3") {
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
