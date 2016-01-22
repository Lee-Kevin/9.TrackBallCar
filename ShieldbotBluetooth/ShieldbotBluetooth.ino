/*
 * Shieldbot&Bluetooth.ino
 * A library for SeeedStudio Shield_Bot, Grove - Serial Bluetooth, Grove_Mini_Track_Ball
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

#include <SoftwareSerial.h>   //Software Serial Port
#include <Shieldbot.h>

#define RxD         6
#define TxD         7

#define DEBUG_ENABLED  1

SoftwareSerial blueToothSerial(RxD,TxD);

Shieldbot shieldbot = Shieldbot();

uint8_t recvData[5];

void setup()
{
    Serial.begin(9600);
    pinMode(RxD, INPUT);
    pinMode(TxD, OUTPUT);
    
    setupBlueToothConnection();
	
	shieldbot.setMaxSpeed(255);//255 is max
}

void loop()
{
	uint8_t i = 0;
	char direction = 0;
	uint8_t maxData;
	int val;
   
	while(blueToothSerial.available())
	{//check if there's any data sent from the remote bluetooth shield
		recvData[i] = blueToothSerial.read();
		Serial.print(recvData[i], DEC);
		i++;
		if(i == 5)
		{
			break;
		}
		Serial.print(" - ");
	}
	Serial.println(" ");
	
	getDirectionData(&direction, &maxData);
//	maxData = maxData - 48;
	Serial.print("direction: ");
    Serial.println(direction);
	Serial.print("maxData: ");
    Serial.println(maxData, DEC);
	
	val = map(maxData, 0, 30, 0, 127);
	Serial.print("val: ");
    Serial.println(val, DEC);
	if(val > 126)
	{
		val = 127;
	}
	Serial.print("val: ");
    Serial.println(val, DEC);
	
	
	switch (direction)
	{
		case 'U':
			shieldbot.drive(val,val);
			break;
		case 'D':
			val = val * (-1);
			shieldbot.drive(val,val);
			break;
		case 'L':
			shieldbot.drive(0,val);
			break;
		case 'R':
			shieldbot.drive(val,0);
			break;
		case 'C':
			shieldbot.drive(127, -127);   
//			delay(250);  
			break;
        default:
			shieldbot.drive(0, 0);
			break;
	}

	delay(250);
}


void getDirectionData(char *p_dir, uint8_t *p_max)
{
	uint8_t maxData;
	
	maxData = max(recvData[0], recvData[1]);
	maxData = max(maxData, recvData[2]);
	maxData = max(maxData, recvData[3]);
	maxData = max(maxData, recvData[4]);
	
	*p_max = maxData;
	
	if(recvData[4] > 0)
	{
		*p_dir = 'C';			// MOTION_CLICK
		return;
	}
	else if(maxData == recvData[0] && maxData > 0)
	{
		*p_dir = 'U';			// MOTION_UP
		return;
	}
	else if(maxData == recvData[1] && maxData > 0)
	{
		*p_dir = 'D';			// MOTION_DOWN
		return;
	}
	else if(maxData == recvData[2] && maxData > 0)
	{
		*p_dir = 'L';			// MOTION_LEFT
		return;
	}
	else if(maxData == recvData[3] && maxData > 0)
	{
		*p_dir = 'R';			// MOTION_RIGHT
		return;
	}
	else
	{
		*p_dir = 'N';			// NO MOTION
		return;
	}
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
	
	blueToothSerial.print("AT+NAMESeeedBTSlave");    // set the bluetooth name as "SeeedBTSlave" ,the length of bluetooth name must less than 12 characters.
	delay(400);
	checRecvData();
	
	blueToothSerial.print("AT+ROLES");             // set the bluetooth work in slaver mode
	delay(400); 
	checRecvData();
	
	blueToothSerial.print("AT+PIN8888");            
	delay(400); 
	checRecvData();
	
	blueToothSerial.print("AT+RADD?");            
	delay(400); 
	checRecvData();
	
	blueToothSerial.print("AT+LADD?");            
	delay(400); 
	checRecvData();
	
	blueToothSerial.print("AT+ADDR?");            
	delay(400); 
	checRecvData();
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