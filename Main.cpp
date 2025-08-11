#include <iostream>
#include <algorithm>
#include <Windows.h>
#include "PyConsole/include/Console.h"
#include <string>

using namespace PyConsole;

static void ToLower(std::string& s) {
	std::transform(s.begin(), s.end(), s.begin(),
		[](unsigned char c) { return std::tolower(c); });
}

static void ShowCredits() {
	Console::Clear();
	Console::SetTitle("PyConsole - Credits");

	Console::Write("1.) Me (", RGB(192, 192, 192));
	Console::Write("Pythol", RGB(128, 0, 128));
	Console::WriteLine(") for making this", RGB(192, 192, 192));

	Console::Write("2.) ", RGB(192, 192, 192));
	Console::Write("Voids ", RGB(0, 191, 255));
	Console::WriteLine("for reminding me to finish and polish it", RGB(192, 192, 192));

	Console::Write("3.) ", RGB(192, 192, 192));
	Console::WriteLine("My amazing girlfriend for existing <3\n", RGB(255, 102, 204));

	Console::Write("Press ", RGB(64, 0, 64));
	Console::Write("ENTER ", RGB(0, 191, 255));
	Console::WriteLine("to return", RGB(64, 0, 64));

	Console::ReadLine();
}

static void TestConsoleAPI() {
	Console::Clear();
	Console::SetTitle("PyConsole - API Test");

	Console::WriteLine("Testing color output:", RGB(173, 216, 230));
	Console::SetForegroundColor(RGB(128, 0, 0));
	Console::WriteLine("This should be red.");
	Console::SetForegroundColor(RGB(0, 128, 0));
	Console::WriteLine("This should be green.");
	Console::ResetColor();
	Console::WriteLine("This should be white.");
	Console::Pause();

	Console::Clear();
	Console::Write("Testing cursor movement... wait for it\n", RGB(0, 191, 255));
	Sleep(1000);
	Console::SetCursorPosition(0, 0);
	Console::WriteLine("Cursor moved to top-left. Previously existing text was overridden.", RGB(0, 100, 0));
	Console::Pause();

	Console::Clear();
	Console::Debug("This is a debug message.");
	Console::Error("This is an error message.");
	Console::Pause();

	Console::Clear();
	Console::WriteLine("Type something: ");
	std::string line = Console::ReadLine();
	Console::Write("You typed: ", RGB(255, 255, 224));
	Console::WriteLine(line, RGB(173, 216, 230));
	Console::Pause();

	Console::Clear();
	Console::WriteLine("Press a key: ");
	char c = Console::ReadKey();
	Console::Write("You pressed: ");
	Console::WriteLine(std::string(1, c), RGB(255, 102, 204));
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

static void TestRGB() {
	Console::Clear();
	Console::SetTitle("PyConsole - RGB Test");

	Console::WriteLine("Basic Colors:");
	Console::Write("Red  ", RGB(255, 0, 0));
	Console::Write("Green  ", RGB(0, 255, 0));
	Console::Write("Blue  ", RGB(0, 0, 255));
	Console::Write("Yellow  ", RGB(255, 255, 0));
	Console::Write("Cyan  ", RGB(0, 255, 255));
	Console::Write("Magenta\n", RGB(255, 0, 255));

	Console::Write("Red  ", RGB(130, 0, 0));
	Console::Write("Green  ", RGB(0, 130, 0));
	Console::Write("Blue  ", RGB(0, 0, 130));
	Console::Write("Yellow  ", RGB(130, 130, 0));
	Console::Write("Cyan  ", RGB(0, 130, 130));
	Console::Write("Magenta\n", RGB(130, 0, 130));

	Console::WriteLine("\nGray Variants:");
	Console::Write("Dark  ", RGB(64, 64, 64));
	Console::Write("Mid  ", RGB(128, 128, 128));
	Console::Write("Light\n", RGB(192, 192, 192));

	auto PrintGradient = [](COLORREF from, COLORREF to, const std::string& label) {
		Console::Write("\n" + label + ": ");
		const int steps = 40;
		for (int i = 0; i <= steps; ++i) {
			int r = GetRValue(from) + (GetRValue(to) - GetRValue(from)) * i / steps;
			int g = GetGValue(from) + (GetGValue(to) - GetGValue(from)) * i / steps;
			int b = GetBValue(from) + (GetBValue(to) - GetBValue(from)) * i / steps;
			Console::Write("█", RGB(r, g, b)); // to use special characters like these and have the compiler recognise it, you need to save the file with the UTF-8 encoding (without the signature)
		}                                      // ts took ages to figure out
		Console::Write("\n");
		};

	// Gradients
	PrintGradient(RGB(255, 0, 0), RGB(0, 0, 255), "Red -> Blue");
	PrintGradient(RGB(255, 255, 0), RGB(0, 255, 0), "Yellow -> Green");
	PrintGradient(RGB(255, 105, 180), RGB(135, 206, 235), "Pink -> Sky");
	PrintGradient(RGB(0, 0, 0), RGB(255, 255, 255), "Black -> White");

	Console::WriteLine("\nDone! Press ENTER to return...");
	Console::ReadLine();
}

int main() {
	bool isRunning = true;
	std::string input;
	input.reserve(5);
	std::string eIn;
	eIn.reserve(3);

	while (isRunning) {
		Console::Clear();
		Console::SetTitle("PyConsole");
		Console::SetForegroundColor(RGB(0, 191, 255));
		Console::Write("Welcome to the PyConsole App. \nWhat would you like to do?\n");
		Console::WriteLine("1. Credits", RGB(64, 0, 64));
		Console::WriteLine("2. Run Console API Test", RGB(64, 0, 64));
		Console::Write("3. ", RGB(64, 0, 64));
		Console::Write("R", RGB(255, 0, 0));
		Console::Write("G", RGB(0, 255, 0));
		Console::Write("B", RGB(0, 0, 255));
		Console::WriteLine(" Testing", RGB(64, 0, 64));
		Console::WriteLine("4. Exit\n", RGB(64, 0, 64));
		Console::SetForegroundColor(RGB(255, 255, 255));

		input = Console::ReadLine();

		if (input == "1")
			ShowCredits();
		else if (input == "2")
			TestConsoleAPI();
		else if (input == "3")
			TestRGB();
		
		else if (input == "4") {
			Console::Clear();
			Console::WriteLine("Are you sure you want to exit?\nY/N", RGB(128, 0, 0));
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

// Didn't bother to rewrite this since it's just test code