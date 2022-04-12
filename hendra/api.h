void getUser (String uid) {
  Serial.println(uid);
  HTTPClient http;
  http.begin(baseUrl + "/api/user?uid=" + uid);
  int httpCode = http.GET();
  if (httpCode > 0)
  {
    String response = http.getString();
    const size_t capacity = JSON_OBJECT_SIZE(2) + 30;
    DynamicJsonDocument doc(capacity);
    deserializeJson(doc, response);
    String status = doc["status"];
    String nama = doc["nama"];
    user[0] =  status;
    user[1] =  nama;
  }
  http.end();
}

void createUser(String nama, String uid) {
  HTTPClient http;
  Serial.println(nama);
  Serial.println(uid);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.begin(baseUrl + "/api/user");

  String data = "nama=" + nama + "&uid=" + uid;
  // Send HTTP POST request
  int httpResponseCode = http.POST(data);
  Serial.println(http.getString());
  http.end();
}

void createResult(String selection, String uid) {
  HTTPClient http;
  http.begin(baseUrl + "/api/post/result?selection=" + selection + "&uid="+ uid);
  int httpCode = http.GET();
  if (httpCode > 0)
  {
    String response = http.getString();
    const size_t capacity = JSON_OBJECT_SIZE(2) + 30;
    DynamicJsonDocument doc(capacity);
    deserializeJson(doc, response);
    String status = doc["status"];
    String nama = doc["nama"];
    user[0] =  status;
    user[1] =  nama;
  }
  http.end();
}

void resetResult () {
  HTTPClient http;
  http.begin(baseUrl + "/api/reset/result");
  int httpCode = http.GET();

  http.end();
}

void getCandidate () {
  HTTPClient http;
  http.begin(baseUrl + "/api/candidate");
  int httpCode = http.GET();
  if (httpCode > 0)
  {
    String response = http.getString();
    const size_t capacity = JSON_ARRAY_SIZE(2) + 2*JSON_OBJECT_SIZE(4) + 160;
    DynamicJsonDocument doc(capacity);
    deserializeJson(doc, response);

    JsonObject root_0 = doc[0];
    String nama0 = root_0["nama"];
    candidate[0] = nama0; 

    JsonObject root_1 = doc[1];    
    String nama1 = root_1["nama"];
    candidate[1] = nama1; 
  }
  http.end();
}