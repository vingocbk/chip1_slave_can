#include "chip1_slave_can.h"

void receiveDataCan(){
  	CAN_frame_t rx_frame;
    if (xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3 * portTICK_PERIOD_MS) == pdTRUE) {

        if (rx_frame.FIR.B.FF == CAN_frame_std) {
        	printf("New standard frame");
        }
        else {
        	printf("New extended frame");
        }

        if (rx_frame.FIR.B.RTR == CAN_RTR) {
        	printf(" RTR from %d, DLC %d\r\n", rx_frame.MsgID,  rx_frame.FIR.B.DLC);
        }
        else {
        	printf(" from %d, DLC %d, Data ", rx_frame.MsgID,  rx_frame.FIR.B.DLC);
			for (int i = 0; i < rx_frame.FIR.B.DLC; i++) {
				printf("%d ", rx_frame.data.u8[i]);
			}
			printf("\n");
        }

        if(rx_frame.MsgID == 1){      //changedelay   
          	Serial.println("change delay");
			switch (rx_frame.data.u8[0])	//mode run want change
			{
			case 1:							//mode 1
				delayMode1 = rx_frame.data.u8[1];
				Serial.print("Writed: ");
				Serial.println(delayMode1);
				EEPROM.write(EEPROM_delayMode1, char(delayMode1));
				EEPROM.commit();
				break;
			case 2:							//mode 2
				delayMode2 = rx_frame.data.u8[1];
				Serial.print("Writed: ");
				Serial.println(delayMode2);
				EEPROM.write(EEPROM_delayMode2, char(delayMode2));
				EEPROM.commit();
				break;
			case 3:							//mode 3
				delayMode3 = rx_frame.data.u8[1];
				Serial.print("Writed: ");
				Serial.println(delayMode3);
				EEPROM.write(EEPROM_delayMode3, char(delayMode3));
				EEPROM.commit();
				break;
			case 4:							//mode 4
				delayMode4 = rx_frame.data.u8[1];
				Serial.print("Writed: ");
				Serial.println(delayMode4);
				EEPROM.write(EEPROM_delayMode4, char(delayMode4));
				EEPROM.commit();
				break;
			case 5:							//mode 5
				delayMode5 = rx_frame.data.u8[1];
				Serial.print("Writed: ");
				Serial.println(delayMode5);
				EEPROM.write(EEPROM_delayMode5, char(delayMode5));
				EEPROM.commit();
				break;
			case 6:							//mode 6
				delayMode6 = rx_frame.data.u8[1];
				Serial.print("Writed: ");
				Serial.println(delayMode6);
				EEPROM.write(EEPROM_delayMode6, char(delayMode6));
				EEPROM.commit();
				break;
			case 7:							//mode 7
				delayMode7 = rx_frame.data.u8[1];
				Serial.print("Writed: ");
				Serial.println(delayMode7);
				EEPROM.write(EEPROM_delayMode7, char(delayMode7));
				EEPROM.commit();
				break;
			case 8:							//mode 8
				delayMode8 = rx_frame.data.u8[1];
				Serial.print("Writed: ");
				Serial.println(delayMode8);
				EEPROM.write(EEPROM_delayMode8, char(delayMode8));
				EEPROM.commit();
				break;
			case 9:							//mode 9
				delayMode9 = rx_frame.data.u8[1];
				Serial.print("Writed: ");
				Serial.println(delayMode9);
				EEPROM.write(EEPROM_delayMode9, char(delayMode9));
				EEPROM.commit();
				break;
			
			default:
				break;
			}

        }
        else if(rx_frame.MsgID == 2){ //change mode run
          	Serial.println("change mode run");
			modeRun = rx_frame.data.u8[0];
			turnOffAll();
			while(millis() % 2000 != 0){
				countStep = 0;
				statusModeRunning = modeRun;
				int countStepMode3 = 0;
				int countStepMode4 = 1;
				int countStepMode6 = 1;
				if(modeRun == 10){
					statusModeRunning = 1;
				}
            }
            countTimeMillis = millis();
            Serial.println("Run");

        }



    }
}


void setup() {
	// put your setup code here, to run once:
	WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
	Serial.begin(115200);
	EEPROM.begin(256);
	pinMode(L1, OUTPUT);
	pinMode(L2, OUTPUT);
	pinMode(L3, OUTPUT);
	pinMode(L4, OUTPUT);
	pinMode(L5, OUTPUT);
	pinMode(L6, OUTPUT);
	pinMode(L7, OUTPUT);
	pinMode(L8, OUTPUT);
	pinMode(L9, OUTPUT);
	pinMode(LED_WIFI, OUTPUT);
	delay(10);
	digitalWrite(L1, LOW);
	digitalWrite(L2, LOW);
	digitalWrite(L3, LOW);
	digitalWrite(L4, LOW);
	digitalWrite(L5, LOW);
	digitalWrite(L6, LOW);
	digitalWrite(L7, LOW);
	digitalWrite(L8, LOW);
	digitalWrite(L9, LOW);
	digitalWrite(LED_WIFI, HIGH);


	//read EEPROM
	if(EEPROM.read(EEPROM_delayMode1) != 255 && EEPROM.read(EEPROM_delayMode1) != 0){
		delayMode1 = EEPROM.read(EEPROM_delayMode1);
	}
	if(EEPROM.read(EEPROM_delayMode2) != 255 && EEPROM.read(EEPROM_delayMode2) != 0){
		delayMode2 = EEPROM.read(EEPROM_delayMode2);
	}
	if(EEPROM.read(EEPROM_delayMode3) != 255 && EEPROM.read(EEPROM_delayMode3) != 0){
		delayMode3 = EEPROM.read(EEPROM_delayMode3);
	}
	if(EEPROM.read(EEPROM_delayMode4) != 255 && EEPROM.read(EEPROM_delayMode4) != 0){
		delayMode4 = EEPROM.read(EEPROM_delayMode4);
	}
	if(EEPROM.read(EEPROM_delayMode5) != 255 && EEPROM.read(EEPROM_delayMode5) != 0){
		delayMode5 = EEPROM.read(EEPROM_delayMode5);
	}
	if(EEPROM.read(EEPROM_delayMode6) != 255 && EEPROM.read(EEPROM_delayMode6) != 0){
		delayMode6 = EEPROM.read(EEPROM_delayMode6);
	}


	CAN_cfg.speed = CAN_SPEED_125KBPS;
	CAN_cfg.tx_pin_id = GPIO_NUM_32;
	CAN_cfg.rx_pin_id = GPIO_NUM_33;
	CAN_cfg.rx_queue = xQueueCreate(rx_queue_size, sizeof(CAN_frame_t));
	// Init CAN Module
	ESP32Can.CANInit();



	WiFi.softAPdisconnect();
    WiFi.disconnect();
    server.close();
    delay(500);
    WiFi.mode(WIFI_AP_STA);
	

    IPAddress APIP(192, 168, 4, 1);
    IPAddress gateway(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.softAPConfig(APIP, gateway, subnet);


	WiFi.softAP("config 1", "123456789");

	server.on("/", HTTP_GET, []() {
		server.sendHeader("Connection", "close");
		server.send(200, "text/html", loginIndex);
	});
	server.on("/serverIndex", HTTP_GET, []() {
		server.sendHeader("Connection", "close");
		server.send(200, "text/html", serverIndex);
	});
	/*handling uploading firmware file */
	server.on("/update", HTTP_POST, []() {
		server.sendHeader("Connection", "close");
		server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
		ESP.restart();
	}, []() {
		HTTPUpload& upload = server.upload();
		if (upload.status == UPLOAD_FILE_START) {
		Serial.printf("Update: %s\n", upload.filename.c_str());
		if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
			Update.printError(Serial);
		}
		} else if (upload.status == UPLOAD_FILE_WRITE) {
		/* flashing firmware to ESP*/
		if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
			Update.printError(Serial);
		}
		} else if (upload.status == UPLOAD_FILE_END) {
		if (Update.end(true)) { //true to set the size to the current progress
			Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
		} else {
			Update.printError(Serial);
		}
		}
	});

	// server.on("/change", HTTP_POST, ChangeDelay);
	// server.on("/setmode", HTTP_POST, setModeRun);
	server.begin();


	while(millis() <= 2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(modeRun == 1){
    mode1();
  }
  else if(modeRun == 2){
    mode2();
  }
  else if(modeRun == 3){
    mode3();
  }
  else if(modeRun == 4){
    mode4();
  }
  else if(modeRun == 5){
    mode5();
  }
  else if(modeRun == 6){
    mode6();
  }
  else if(modeRun == 7){
    turnOffAll();
  }
  else if(modeRun == 8){
    turnOnAll();
  }
  else if(modeRun == 10){
    runAll();
  }

  receiveDataCan();
  server.handleClient();

  
}
