/*
 * MiniTrackball&Bluetooth.ino
 * A library for SeeedStudio Grove_Mini_Track_Ball and Grove - Serial Bluetooth
 *
 * Copyright (c) 2012 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : Wu Ruibin
 * Create Time: Dec 2015
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "MiniTrackball.h"
#include "Wire.h"
#include <SoftwareSerial.h>                         // Software Serial Port

#define RxD         6
#define TxD         7

#define DEBUG_ENABLED  1

MTB MiniTrackball;

SoftwareSerial blueToothSerial(RxD,TxD);

uint8_t track_data[5];

void setup()
{
    Serial.begin(9600);
  	Wire.begin();
    pinMode(RxD, INPUT);
    pinMode(TxD, OUTPUT);
    
    setupBlueToothConnection();
    //wait 1s and flush the serial buffer
    delay(1000);
    Serial.flush();
    blueToothSerial.flush();
}

void loop()
{
//	uint8_t track_data[5];
    
    MiniTrackball.GetTrackData(track_data);
	
	Serial.println("+------------------------------------------------------+");
    Serial.print("MOTION_UP:    ");
    Serial.print(track_data[0], DEC);
    Serial.print("\t");
    Serial.print("MOTION_DOWN:  ");
    Serial.println(track_data[1], DEC);
    Serial.print("MOTION_LEFT:  ");
    Serial.print(track_data[2], DEC);
    Serial.print("\t");
    Serial.print("MOTION_RIGHT: ");
    Serial.println(track_data[3], DEC);
    Serial.print("MOTION_CLICK: ");
    Serial.println(track_data[4], DEC);
    Serial.println("+------------------------------------------------------+");
    Serial.println(" ");

	blueToothSerial.print(char(track_data[0]));
	blueToothSerial.print(char(track_data[1]));
	blueToothSerial.print(char(track_data[2]));
	blueToothSerial.print(char(track_data[3]));
	blueToothSerial.print(char(track_data[4]));
	
    delay(250);
}


/***************************************************************************
 * Function Name: setupBlueToothConnection
 * Description:  initilizing bluetooth connction
 * Parameters: 
 * Return: 
***************************************************************************/
void setupBlueToothConnection()
{
    blueToothSerial.begin(9600);  
	
	blueToothSerial.print("AT");
	delay(400);
	checRecvData();
	
	blueToothSerial.print("AT+DEFAULT");             // Restore all setup value to factory setup
	delay(2000); 
	checRecvData();
	
	blueToothSerial.print("AT+BAUD4");              // Set baud rate 9600. 1:1200; 2:2400; 3:4800; 4:9600; 5:19200; 6:38400; 7:57600; 8:115200 
	delay(400); 
	checRecvData();
	
	blueToothSerial.print("AT+NAMESeeedMaster");    // set the bluetooth name as "SeeedMaster" ,the length of bluetooth name must less than 12 characters.
	delay(400);
	checRecvData();
	
	blueToothSerial.print("AT+ROLEM");             // set the bluetooth work in master mode
	delay(400); 
	checRecvData();
	
//	blueToothSerial.print("AT+AUTH1");            
//	delay(400);    
	
	blueToothSerial.print("AT+CLEAR");             // Clear connected device mac address
	delay(400);   
	checRecvData();
	
	blueToothSerial.print("AT+PIN8888");            
	delay(400); 
	checRecvData();
	
	blueToothSerial.print("AT+SCAN?");            
	delay(5000); 
	checRecvData();
	
//    blueToothSerial.flush();
}

/***************************************************************************
 * Function Name: checRecvData
 * Description:  check if there's any data sent from the remote bluetooth module
 * Parameters: 
 * Return: 
***************************************************************************/
void checRecvData()
{
	char recvChar;
	while(blueToothSerial.available())
	{
		recvChar = blueToothSerial.read();
		Serial.print(recvChar);
	}
	Serial.println(" ");
}
