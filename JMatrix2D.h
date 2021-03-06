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
	const T &			GetValue(unsigned, unsigned) const;				//获取指定行、列的值
	const T &			GetValue(unsigned) const;						//获取指定索引的值，从0开始计数
	const int &			GetCurIndex() const;
	int &				GetCurIndex();
	int					GetForwardMode();
	int					GetCurRow();
	int					GetCurCol();
	int					GetCountFromStart();

	//Setters...
	void SetNextMode(FORWARDMODE);										//设置去向下一个点的模式
	void SetCurIndex(int);												//设置当前索引值
	bool SetTurnPara(unsigned srow,unsigned scol, unsigned length);		//设置本轮进行的形式，起始行、列、长度
	bool SetTurnPara(unsigned sindex, unsigned length);					//设置本轮进行的形式，起始行、序号
	bool SetTurnPara(unsigned srow, unsigned scol, unsigned endrow, unsigned endcol);			//设置本轮进行的形式，起始行、列、终止行、列


	//overload operators...
	JMatrix2D<T> & operator ++();
	JMatrix2D<T> & operator --();

	//is serial..
	bool isMatrixEnd();													//表示已经过了矩阵的最后一个元素
	bool isMatrixLast();												//表示是否在最后一个元素
	bool isTurnEnd();													//表示是否过了本轮的最后一个点
	bool isTurnLast();													//表示是否在本轮的最后一个点

	//functional
	void ResetMatrixValue(const T &);									//重置中矩阵中所有的值
	bool ResizeMatrix(unsigned,unsigned, const T &);					//重置矩阵的大小，并将新矩阵的赋予初始值T
	
private:

	unsigned TotalRows;													//保存矩阵的行数
	unsigned TotalCols;													//保存矩阵的列数
	unsigned StartRow;													//起始行
	unsigned StartCol;													//起始列
	unsigned LengthFromStart;											//想要运行的长度
	int		 CurIndex;													//保存当前位置（从0开始计数）
	FORWARDMODE		CurForwardMode;										//保存从当前点往下点流转的模式
	std::deque<T>	MatrixValue;										//表示存在矩阵中的值

	void ToNextInRowPriorityMode();										//按照RowPriority的模式进入到下一个索引点
	void ToNextInProximityMode();										//按照proximity的模式去下一个索引点
	void ToNextInSnakeMode();											//按照Snake的模式去下一个索引点

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
const T & JMatrix::JMatrix2D<T>::GetValue(unsigned index) const//注意，矩阵的索引号是从0开始的
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