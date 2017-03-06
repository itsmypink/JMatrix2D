#ifndef JMATRIX2D_H
#define JMATRIX2D_H

#include<deque>

namespace JMatrix
{

	template<class T>	class JMatrix2D;
	template<class T>	class JMatrix3D;

}

using namespace JMatrix;

template<class T>
class JMatrix::JMatrix2D
{
public:

	enum FORWARDMODE
	{
		RowPriority = 0x0001
		,Proximity  =0x0002
		,Snake=0x0004
	};

	explicit JMatrix2D(unsigned, unsigned, FORWARDMODE nm = RowPriority);

	//Getters...
	const T &			GetValue(unsigned, unsigned) const;				//获取指定行、列的值
	const T &			GetValue(unsigned) const;						//获取指定索引的值，从0开始计数
	const int &			GetCurIndex() const ;
	int &				GetCurIndex();
	int					GetForwardMode();

	//Setters...
	void SetNextMode(FORWARDMODE);										//设置去向下一个点的模式
	void SetCurIndex(int);												//设置当前索引值
	void SetTurnMode(unsigned,unsigned srow = 1, unsigned scol = 1);	//设置本轮进行的形式，起始行、列、长度

	//overload operators...
	JMatrix2D<T> & operator ++();
	JMatrix2D<T> & operator --();

	//is serial..
	bool isMatrixEnd();													//表示已经过了矩阵的最后一个元素
	bool isMatrixLast();												//表示是否在最后一个元素
	bool isTurnEnd();													//表示是否过了本轮的最后一个点
	bool isTurnLast();													//表示是否在本轮的最后一个点

	//functional
	void RestMatrixValue(const T &);									//重置中矩阵中所有的值
	bool ResizeMatrix(unsigned,unsigned, const T &);										//重置矩阵的大小，并将新矩阵的赋予初始值T

private:

	unsigned TotalRows;													//保存矩阵的行数
	unsigned TotalCols;													//保存矩阵的列数
	unsigned StartRow;													//起始行
	unsigned StarCol;													//起始列
	unsigned LengthFromStart;											//想要运行的长度
	int		 CurIndex;													//保存当前位置（从0开始计数）
	FORWARDMODE		CurForwardMode;										//保存从当前点往下点流转的模式
	std::deque<T>	MatrixValue;										//表示存在矩阵中的值

	void ToNextInRowPriorityMode();										//按照RowPriority的模式进入到下一个索引点
	void ToNextInProximityMode();										//按照proximity的模式去下一个索引点
	void ToNextInSnakeMode();											//按照Snake的模式去下一个索引点

};

#endif



template<class T>
JMatrix::JMatrix2D<T>::JMatrix2D(unsigned rows, unsigned cols, FORWARDMODE nm) :
TotalRows(rows),
TotalCols(cols),
StartRow(1),
StarCol(1),
LengthFromStart(1),
CurIndex(0)
{
	MatrixValue = std::deque<T>(rows*cols,static_cast<T>(0));
	CurForwardMode = nm;
}


//getters...
template<class T>
T & JMatrix::JMatrix2D<T>::GetValue(unsigned row, unsigned col)
{
	return MatrixValue.at((row - 1)*TotalCols + col);
}


template<class T>
T & JMatrix::JMatrix2D<T>::GetValue(unsigned index)//注意，矩阵的索引号是从0开始的
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
void JMatrix::JMatrix2D<T>::SetTurnMode(unsigned length, unsigned srow, unsigned scol)
{
	if (length > TotalRows*TotalCols - (srow - 1)*TotalCols + scol + 1 || srow > TotalRows || scol > TotalCols || length < 0 || srow < 0 || scol < 0)
		CurIndex = -1;
	else
	{
		StartRow = srow;
		StarCol = scol;
		LengthFromStart = length;
		CurIndex = (srow - 1)*TotalCols + scol + 1;
	}
}


//overload...

template<class T>
JMatrix2D<T> &  JMatrix::JMatrix2D<T>::operator++()
{
	switch (CurForwardMode)
	{
	case RowPriority:
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
	if (CurIndex == -1 || CurIndex > (StartRow - 1)*TotalCols + StarCol + LengthFromStart - 2)
		return true;
	return false;
}

template<class T>
bool JMatrix::JMatrix2D<T>::isTurnLast()
{
	return CurIndex == (StartRow - 1)*TotalCols + StarCol + LengthFromStart - 2;
}



//functional...

template<class T>
void JMatrix::JMatrix2D<T>::RestMatrixValue(T t)
{
	MatrixValue.clear();
	MatrixValue.assign(MatrixValue.size(), t);
	CurIndex = 0;
	StartRow = 1;
	StartCol = 1;
}

template<class T>
bool JMatrix::JMatrix2D<T>::ResizeMatrix(unsigned rows, unsigned cols,const T &)
{
	int i_Rows = static_cast<int>rows;
	int i_Cols = static_cast<int>cols;
	if (i_Rows <= 0 || i_Cols <= 0)return false;
	TotalRows = i_Rows;
	TotalCols = i_Cols;
	RestMatrixValue(T);
	return true;
}