// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

#define BITS_IN_ONE_MEM sizeof(TELEM) * 8

TBitField::TBitField(int len)
{
  if (len < 0)
    throw "negative length";
	BitLen = len; 
	MemLen = (len - 1) / (8 * sizeof(TELEM)) + 1; 
	pMem = new TELEM[len]; 
	if (pMem != NULL) 
	for (int i = 0; i < MemLen; i++) 
	{ 
		pMem[i] = 0; 
	} 
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen=bf.BitLen;
	MemLen=bf.MemLen;
	pMem=new TELEM [MemLen];
	for (int i=0; i<MemLen; i++)
		pMem[i]=bf.pMem[i];
}

TBitField::~TBitField()
{
	delete [] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if ((n>=0) && (n < BitLen))
		return n/(sizeof(TELEM) * 8);
	else 
		throw 1;
}


TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
  return 1 << (n - GetMemIndex(n) * sizeof(TELEM) * 8);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n >= 0) && (n < BitLen))
		pMem[GetMemIndex(n)] |= GetMemMask(n);
	else
		throw 1;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n >= 0) && (n < BitLen))
		pMem[GetMemIndex(n)] &= ~GetMemMask(n);
	else
		throw 1;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n >= 0) && (n < BitLen))
		return ((pMem[GetMemIndex(n)] & GetMemMask(n)) != 0); 
	else
		throw 1;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание 
{
		BitLen=bf.BitLen;
		MemLen=bf.MemLen;
		delete [] pMem;
    	pMem= new TELEM [MemLen];
	for (int i=0; i<MemLen; i++)
		pMem[i]=bf.pMem[i];
  return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
  int res = 1;  
	if (BitLen != bf.BitLen) 
		res = 0;  
	else 
	for (int i = 0; i < MemLen; i++)  
	{ 
		if (pMem[i] != bf.pMem[i]) 
		{
			res = 0; 
			break;
		}
	} 
	return res; 
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return !(*this == bf);
}


TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{	
	int size;
	if (bf.BitLen>BitLen)
		size=bf.BitLen;
	else
		size = BitLen;
    TBitField temp(size);
	for (int i=0; i<MemLen; i++) 
		temp.pMem[i]=pMem[i];
	for (int i=0; i<bf.MemLen; i++)
		temp.pMem[i]|=bf.pMem[i];
	return temp;
}


TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int len = BitLen;
	if (bf.BitLen > len)
		len = bf.BitLen;
	TBitField temp(len);
	for (int i = 0; i < MemLen; i++)
		temp.pMem[i] = pMem[i];
	for (int i = 0; i < temp.MemLen; i++)
		temp.pMem[i] &= bf.pMem[i];
	return temp;
}


TBitField TBitField::operator~(void) // отрицание
{
TBitField temp(*this); 
	for (int i = 0; i < MemLen - 1; i++)  
	{ 
		temp.pMem[i] = ~temp.pMem[i];  
	} 
	for (int i = sizeof(TELEM)* 8 * (MemLen - 1); i < BitLen; i++) 
	{ 
		if ((temp).GetBit(i) == 1)  
			temp.ClrBit(i);  
		else 
			temp.SetBit(i);  
	} 
	return temp;  
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int i=0;
	char ch;
	do
	istr >> ch;
	while (ch!= ' ');
	while (1)
	{
		istr >> ch;
			 if (ch == '0')
				 bf.ClrBit(i++);
			 else
				 if (ch == '1')
					 bf.SetBit(i++);
				 else 
					 break;
	}
  return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	char ch = ' ';
	ostr << "{ ";
	for (int i=0; i < bf.GetLength() ; ++i)
		if (bf.GetBit(i))
			ostr << ch << i << ' ';
	ostr << "}";
	return ostr;
}
