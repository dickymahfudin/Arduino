#include "dism.h"

String dism::dism_get(String id)
{
	String a = "http://nodemcudism.000webhostapp.com/dism/api/post/read_single.php?id=";
	String b = id;
	a += b;
	HTTPClient http;
	http.begin(a);
	int httpCode = http.GET();

	if (httpCode > 0)
	{
		// Parsing
		const size_t bufferSize = JSON_OBJECT_SIZE(2) + 30;
		DynamicJsonBuffer jsonBuffer(bufferSize);
		JsonObject &root = jsonBuffer.parseObject(http.getString());
		//const char* id = root["id"];
		const char *category_id = root["category_id"];
		String data = category_id;
		return (data);
	}

	else
	{
		return ("ERROR");
	}
	http.end();
}

String dism::dism_set(String id, String value)
{
	const size_t bufferSize = JSON_OBJECT_SIZE(2);
	DynamicJsonBuffer jsonBuffer(bufferSize);
	String a = id;
	String b = value;
	JsonObject &root = jsonBuffer.createObject();
	root["id"] = a;
	root["category_id"] = b;
	int len = root.measureLength() + 1;
	char rootstr[len];
	root.printTo(rootstr, len);
	HTTPClient http;
	http.begin("http://nodemcudism.000webhostapp.com/dism/api/post/update.php");
	http.addHeader("Content-Type", "application/json");
	int httpCode = http.POST(rootstr);
	http.end();
}
