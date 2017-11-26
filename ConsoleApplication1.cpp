// ConsoleApplication1.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"
#include <string>
#include <iostream>
using namespace std;

#define L_7BITEXTENDED 10

char sevenbitextended[L_7BITEXTENDED][2] = {
	{'\f',   0x0A},   // '\u000a',    // <FF>
	{'^',    0x14},   // '\u0014',    // CIRCUMFLEX ACCENT
	{'{',    0x28},   // '\u0028',    // LEFT CURLY BRACKET
	{'}',    0x29},   // '\u0029',    // RIGHT CURLY BRACKET
	{'\\',   0x2F},   // '\u002f',    // REVERSE SOLIDUS
	{'[',    0x3C},   // '\u003c',    // LEFT SQUARE BRACKET
	{'~',    0x3D},   // '\u003d',    // TILDE
	{']',    0x3E},   // '\u003e',    // RIGHT SQUARE BRACKET
	{'|',    0x40},   // '\u0040',    // VERTICAL LINE \u007c
	{'€', 0x65}  // '\u0065'     // EURO SIGN â‚¬
};


int gsm7bit_to_ascii(char *in, int bytlen, char *out)
{
	int i = 0, j = 0, count_bit = 0;								/*	count_bit - количество значащих бит в промежуточной переменной data
																		i - счетчик входного массива
																		j - счетчик выходного массива	*/

	int data = 0x0;													/*Промежуточная переменная для вычленения 7-и битного слова*/

	for( i = 0, j = 0;i < bytlen | count_bit > 0;j++)  
	{
		if((count_bit + 8) < 32)
		{
			int mask = 0;						
			for(int k = 0; k < count_bit; k++) mask |= (1 << k);	/*Создание маски значащих битов в промежуточной переменной data*/
			if(i<bytlen)
			{
				data = (int)(in[i] << count_bit) | (data & mask);
				i++;									
				count_bit += 8;
			}
		}
		out[j] = data & 0x7f;
		data >>= 7;
		count_bit -= 7;
		if(j >= 1 && out[j-1] == 0x1b)								/*Проверка на символы  ^ { } [ ] \ ~ | [пробел] €*/
		{
			for(int k = 0; k < L_7BITEXTENDED; k++)
			{
				if(out[j] == sevenbitextended[k][1])
				{
					out[j-1] = sevenbitextended[k][0];
					out[j] = 0;
					j--;
				}
			}
		}
	}
	out[j++] = 0;
	return j;
}

int _tmain(int argc, _TCHAR* argv[])
{
	char gsm7Bit[] = { 
	0x1B,0x1E,0xB2,0xCC,0x66,0xBF,0xAF,0x6F,0x39,0x9B,0x1C,0xDA,0xA4,0x1A };	/*    Запись [HelloWorld!} в Gsm7Bit кодировке    
																						  конвертер ASCII в  Gsm7Bit    http://smstools3.kekekasvi.com/topic.php?id=288   */
	char ascii[128]={0};														/*    Результирующий массив     */

	gsm7bit_to_ascii(gsm7Bit,sizeof(gsm7Bit), ascii);

	printf("\nGsm 7bit:\t");

	for(int i = 0; i < sizeof(gsm7Bit); i++)
		printf(" %02x", gsm7Bit[i]&0xff);

	printf("\n\nHex ASCII:\t");

	for(int i=0; i < sizeof(gsm7Bit); i++)
		printf(" %02x", ascii[i]);

	printf("\n\nString ASCII:\t%s\n", ascii);

	system("pause");
	
	return 0;
}

