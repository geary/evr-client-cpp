#include "stdafx.h"
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string>

#include "EaseAnalytics.h"
#include "EasePushID.h"

using namespace std;

namespace EaseAnalytics {

	Events::Events() :
		_javaScriptStartTime( JavaScriptTime() )
	{
	}

	Events::~Events() {
	}

	void Events::SetApiKey( string key ) {
		_apiKey = key;
	}

	void Events::SetExperienceID( string id ) {
		_experienceID = id;
	}

	void Events::SessionBegin(
		string deviceID,
		string hmdName,
		string hmdVersion,
		string osName,
		string cpuName,
		int cpuCores,
		int systemMemory,
		string gpuName,
		int gpuMemory,
		string graphicsDriver
	) {
		AddEvent( "ST", {
			DeTab( deviceID ),
			DeTab( hmdName ),
			DeTab( hmdVersion ),
			DeTab( osName ),
			DeTab( cpuName ),
			to_string( cpuCores ),
			to_string( systemMemory ),
			DeTab( gpuName ),
			to_string( gpuMemory ),
			DeTab( graphicsDriver )
		});
		PostEvents();
	}

	void Events::SessionEnd() {
		AddEvent( "SE" );
		PostEvents();
	}

	void Events::MarkerAdd(
		string name,
		float posX, float posY, float posZ
	) {
		AddEvent( "MA", {
			DeTab( name ),
			PosStr( posX ), PosStr( posY ), PosStr( posZ )
		});
	}

	void Events::MarkerRemove(
		string name,
		float posX, float posY, float posZ
	) {
		AddEvent( "MR", {
			DeTab( name ),
			PosStr( posX ), PosStr( posY ), PosStr( posZ )
		});
	}

	void Events::MarkerEnter(
		string name,
		float posX, float posY, float posZ
	) {
		AddEvent( "ME", {
			DeTab( name ),
			PosStr( posX ), PosStr( posY ), PosStr( posZ )
		});
	}

	void Events::MarkerExit(
		string name,
		float posX, float posY, float posZ
	) {
		AddEvent( "MX", {
			DeTab( name ),
			PosStr( posX ), PosStr( posY ), PosStr( posZ )
		});
	}

	void Events::Presence(
		float posX, float posY, float posZ,
		float rotX, float rotY, float rotZ,
		float framesPerSecond,
		long long memoryUsed
	) {
		AddEvent( "P", {
			PosStr( posX ), PosStr( posY ), PosStr( posZ ),
			PosStr( rotX ), PosStr( rotY ), PosStr( rotZ ),
			FpsStr( framesPerSecond ),
			to_string( memoryUsed )
		});
	}

	void Events::AddEvent(
		string type,
		const initializer_list<string>& values
	) {
		stringstream event;
		event << type << "\t" << TimeStamp();
		for( auto value : values ) {
			event << "\t" << value;
		}
		_events.push_back( event.str() );
	}

	void Events::PostEvents() {
		if( _events.size() == 0 ) return;

		stringstream payload;
		payload << "H"
			<< "\t" << TimeStamp()
			<< "\t" << "0"
			<< "\t" << _apiKey
			<< "\t" << _sessionID;
		for( auto event : _events ) {
			payload << "\n" << event;
		}

		_events.clear();

		stringstream url;
		url << _apiUrl << "/client/" << _experienceID << "/events";

		HttpPost( url.str(), payload.str(), false );
	}

	long Events::TimeStamp() {
		return JavaScriptTime() - _javaScriptStartTime;
	}

	string Events::DeTab( string str ) {
		replace( str.begin(), str.end(), '\t', ' ' );
		return str;
	}

	string Events::FpsStr( float x ) {
		return FloatStr( x, 1 );
	}

	string Events::PosStr( float x ) {
		return FloatStr( x, 4 );
	}

	string Events::FloatStr( float x, int precision ) {
		stringstream ss;
		ss << fixed << setprecision(precision) << x;
		auto s = ss.str();
		while( s.back() == '0'  ||  s.back() == '.' ) {
			s.erase( s.length() - 1, 1 );
		}
		return s;
	}

	void Events::HttpResult(
		int status,
		string content
	) {
		status, content;
	}

	string Events::NewPushID() {
		char id[21];
		EaseAnalytics::GeneratePushID( id );
		return string( id );
	}

}
