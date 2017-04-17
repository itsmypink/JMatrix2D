#ifndef JMATRIX2D_H
#define JMATRIX2D_H

#include<deque>

namespace JMatrix
{
	template<class T>	class JMatrix2D;
	template<class T>	class JMatrix3D;

	enum FORWARDMODE
	{
		RowPriority = 0x0001
		, Proximity = 0x0002
		//,Snake=0x0004
	};
}

using namespace JMatrix;

template<class T>
class JMatrix::JMatrix2D
{
public:
	explicit JMatrix2D(unsigned rows=1, unsigned cols=1, FORWARDMODE nm = RowPriority);

	//Getters...
	const T &			GetValue(unsigned, unsigned) const;				//��ȡָ���С��е�ֵ
	const T &			GetValue(unsigned) const;						//��ȡָ��������ֵ����0��ʼ����
	const int &			GetCurIndex() const;
	int &				GetCurIndex();
	int					GetForwardMode();
	int					GetCurRow();
	int					GetCurCol();
	int					GetCountFromStart();

	//Setters...
	void SetNextMode(FORWARDMODE);										//����ȥ����һ�����ģʽ
	void SetCurIndex(int);												//���õ�ǰ����ֵ
	bool SetTurnPara(unsigned srow,unsigned scol, unsigned length);		//���ñ��ֽ��е���ʽ����ʼ�С��С�����
	bool SetTurnPara(unsigned sindex, unsigned length);					//���ñ��ֽ��е���ʽ����ʼ�С����
	bool SetTurnPara(unsigned srow, unsigned scol, unsigned endrow, unsigned endcol);			//���ñ��ֽ��е���ʽ����ʼ�С��С���ֹ�С���


	//overload operators...
	JMatrix2D<T> & operator ++();
	JMatrix2D<T> & operator --();

	//is serial..
	bool isMatrixEnd();													//��ʾ�Ѿ����˾�������һ��Ԫ��
	bool isMatrixLast();												//��ʾ�Ƿ������һ��Ԫ��
	bool isTurnEnd();													//��ʾ�Ƿ���˱��ֵ����һ����
	bool isTurnLast();													//��ʾ�Ƿ��ڱ��ֵ����һ����

	//functional
	void ResetMatrixValue(const T &);									//�����о��������е�ֵ
	bool ResizeMatrix(unsigned,unsigned, const T &);					//���þ���Ĵ�С�������¾���ĸ����ʼֵT
	
private:

	unsigned TotalRows;													//������������
	unsigned TotalCols;													//������������
	unsigned StartRow;													//��ʼ��
	unsigned StartCol;													//��ʼ��
	unsigned LengthFromStart;											//��Ҫ���еĳ���
	int		 CurIndex;													//���浱ǰλ�ã���0��ʼ������
	FORWARDMODE		CurForwardMode;										//����ӵ�ǰ�����µ���ת��ģʽ
	std::deque<T>	MatrixValue;										//��ʾ���ھ����е�ֵ

	void ToNextInRowPriorityMode();										//����RowPriority��ģʽ���뵽��һ��������
	void ToNextInProximityMode();										//����proximity��ģʽȥ��һ��������
	void ToNextInSnakeMode();											//����Snake��ģʽȥ��һ��������

};





template<class T>
JMatrix::JMatrix2D<T>::JMatrix2D(unsigned rows, unsigned cols, FORWARDMODE nm) :
TotalRows(rows),
TotalCols(cols),
StartRow(1),
StartCol(1),
LengthFromStart(0),
CurIndex(0)
{
	MatrixValue = std::deque<T>(rows*cols,static_cast<T>(0));
	CurForwardMode = nm;
}


//getters...
template<class T>
const T & JMatrix::JMatrix2D<T>::GetValue(unsigned row, unsigned col) const
{
	return MatrixValue.at((row - 1)*TotalCols + col);
}


template<class T>
const T & JMatrix::JMatrix2D<T>::GetValue(unsigned index) const//ע�⣬������������Ǵ�0��ʼ��
{
	return MatrixValue.at(index);
}

template<class T>
int JMatrix::JMatrix2D<T>::GetForwardMode()
{
	return CurForwardMode;
}

template<class T>
const int & JMatrix::JMatrix2D<T>::GetCurIndex() const
{
	return CurIndex;
}

template<class T>
int &	JMatrix::JMatrix2D<T>::GetCurIndex()
{
	return CurIndex;
}
template<class T>
int		JMatrix::JMatrix2D<T>::GetCurRow()
{
	return CurIndex / TotalCols + 1;
}

template<class T>
int		JMatrix::JMatrix2D<T>::GetCurCol()
{
	return CurIndex%TotalCols + 1;
}

template<class T>
int		JMatrix::JMatrix2D<T>::GetCountFromStart()
{
	return CurIndex - (StartRow - 1)*TotalCols - StartCol + 1;
}


//Setters....

template<class T>
void JMatrix::JMatrix2D<T>::SetNextMode(FORWARDMODE nm)
{
	CurForwardMode = nm;
}

template<class T>
void JMatrix::JMatrix2D<T>::SetCurIndex(int index)
{
	index<0 || index>TotalCols*TotalRows - 1 ? CurIndex = -1 : CurIndex = index;
}

template<class T>
bool JMatrix::JMatrix2D<T>::SetTurnPara(unsigned srow, unsigned scol, unsigned length)
{
	if (srow > TotalRows || scol > TotalCols || length < 0 || srow < 0 || scol < 0)
	{
		CurIndex = -1;
		return false;
	}
	if (length > TotalRows*TotalCols - (srow - 1)*TotalCols - scol)length = TotalRows*TotalCols - (srow - 1)*TotalCols - scol;

	StartRow = srow;
	StartCol = scol;
	LengthFromStart = length;
	CurIndex = (srow - 1)*TotalCols + scol - 1;
	return true;
}

template<class T>
bool JMatrix::JMatrix2D<T>::SetTurnPara(unsigned sindex, unsigned length)
{
	if (index > TotalRows*TotalCols - 1 || length <=0 || index < 0)
	{
		CurIndex = -1;
		return false;
	}
	if (length > TotalRows*TotalCols - 1 - index)length = TotalRows*TotalCols - 1 - index;

	StartRow = index/TotalCols+1;
	StartCol = index%TotalCols+1;
	LengthFromStart = length;
	CurIndex = index;
	return true;
}

template<class T>
bool JMatrix::JMatrix2D<T>::SetTurnPara(unsigned srow, unsigned scol, unsigned endrow, unsigned endcol)
{
	if ((srow >= 1 && srow <= TotalRows) && (scol >= 1 && scol <= TotalCols)
		&& (endrow >= 1 && endrow <= TotalRows) && (endcol >= 1 && endcol <= TotalCols)
		&& ((srow*TotalCols + TotalCols) <= (endrow*TotalCols + endcol)))
	{
		StartRow = srow;
		StartCol = scol;
		LengthFromStart = (endrow*TotalCols + endcol) - (srow*TotalCols + scol);
		CurIndex = (srow-1)*TotalCols + scol - 1;
		return true;
	}
	else
		return false;
}



//overload...

template<class T>
JMatrix2D<T> &  JMatrix::JMatrix2D<T>::operator++()
{
	switch (CurForwardMode)
	{
	case RowPriority:
		ToNextInRowPriorityMode();
		break;
	case Proximity:
		ToNextInProximityMode();
		break;
	default:
		break;
	}
	return *this;
}


//private functional...
template<class T>
void JMatrix::JMatrix2D<T>::ToNextInRowPriorityMode()
{
	if (CurIndex == -1 || isTurnLast())
	{
		CurIndex = -1;
	}
	else
	{
		++CurIndex;
	}
}

template<class T>
void JMatrix::JMatrix2D<T>::ToNextInProximityMode()
{
	if (CurIndex != -1)
	{
		int rowNum = CurIndex / TotalCols+1;
		int colNum = CurIndex%TotalCols+1;
		if (rowNum % 2)
			colNum < TotalCols?CurIndex++:CurIndex += TotalCols;
		else
			colNum == 1 ? CurIndex += TotalCols : CurIndex--;
		if (CurIndex / TotalCols + 1>TotalRows)CurIndex = -1;
	}
}



//is serial...

template<class T>
bool JMatrix::JMatrix2D<T>::isMatrixEnd()
{
	return CurIndex == -1;
}

template<class T>
bool JMatrix::JMatrix2D<T>::isMatrixLast()
{
	return CurIndex == TotalRows*TotalCols-1;
}

template<class T>
bool JMatrix::JMatrix2D<T>::isTurnEnd()
{
	if (CurIndex == -1 || CurIndex > (StartRow - 1)*TotalCols + StartCol + LengthFromStart - 2)
		return true;
	return false;
}

template<class T>
bool JMatrix::JMatrix2D<T>::isTurnLast()
{
	return CurIndex == (StartRow - 1)*TotalCols + StartCol + LengthFromStart - 1;
}



//functional...

template<class T>
void JMatrix::JMatrix2D<T>::ResetMatrixValue(const T& t)
{
	MatrixValue.clear();
	MatrixValue.assign(MatrixValue.size(), t);
	CurIndex = 0;
	StartRow = 1;
	StartCol = 1;
}

template<class T>
bool JMatrix::JMatrix2D<T>::ResizeMatrix(unsigned rows, unsigned cols,const T & value)
{
	int i_Rows = static_cast<int>(rows);
	int i_Cols = static_cast<int>(cols);
	if (i_Rows <= 0 || i_Cols <= 0)return false;
	TotalRows = i_Rows;
	TotalCols = i_Cols;
	MatrixValue.resize(rows*cols);
	ResetMatrixValue(value);
	return true;
}

#endif