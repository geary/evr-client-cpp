// EaseVR-Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <chrono>
#include <conio.h>
#include <iostream>
#include <thread>
#include "EaseAnalytics.h"

using namespace std;

class TestEvents : public EaseAnalytics::Events {

	void HttpPost(
		string url,
		string content,
		bool wantResult
	) override {
		wantResult;
		cout << "POST to " << url.c_str() << "\n" << content.c_str() << "\n\n";
	}

};

void wait( int milliseconds ) {
	this_thread::sleep_for( chrono::milliseconds(milliseconds) );
}

int main() {

	auto testMarker = "TestMarker";

	auto _easeEvents = new TestEvents();
	_easeEvents->SetApiKey( "TestApiKey" );
	_easeEvents->SetExperienceID( "TestExperienceID" );

	wait( 123 );

	_easeEvents->SessionBegin(
		"Device ID",
		"HMD Name",
		"HMD Version",
		"OS Name/Version",
		"CPU Name",
		8,  // Number of CPUs
		1024*1024*1024,  // Main memory
		"GPU name",
		1024*1024*1024,  // GPU memory
		"Graphics Driver"
	);

	_easeEvents->MarkerAdd(
		testMarker,
		1.0f, 2.2f, 3.34f
	);

	_easeEvents->Presence(
		1.0f, 2.0f, 3.0f,
		4.4f, 5.55f, 6.789f,
		99.0f, 12345678
	);

	_easeEvents->MarkerEnter(
		testMarker,
		1.0f, 2.0f, 3.0f
	);

	_easeEvents->Presence(
		10.0f, 20.0f, 30.0f,
		40.0f, 50.0f, 60.0f,
		99.123f, 23456789
	);

	wait( 3210 );

	_easeEvents->MarkerExit(
		testMarker,
		1, 2, 3
	);

	_easeEvents->MarkerRemove(
		testMarker,
		1, 2, 3
	);

	wait( 456 );

	_easeEvents->SessionEnd();

	delete _easeEvents;

	_getch();

	return 0;
}

