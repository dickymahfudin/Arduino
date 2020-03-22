
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "WString.h"
#include <Arduino.h>

class dism{   

	public:
	String dism_get(String id);
	String dism_set(String id, String value);
};
