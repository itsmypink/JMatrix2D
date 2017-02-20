#ifndef JMATRIX2D_H
#define JMATRIX2D_H

#include<deque>


template<class T>
class JMatrix2D
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
	T &					GetValue(unsigned, unsigned);									//获取指定行、列的值
	T &					GetValue(unsigned);												//获取指定索引的值，从0开始计数
	const unsigned &	GetCurIndex() const ;
	int					GetForwardMode();

	

	//Setters...
	void SetNextMode(FORWARDMODE);



	//overload operators...
	JMatrix2D<T> & operator ++();
	JMatrix2D<T> & operator --();



private:

	unsigned TotalRows;													//保存矩阵的行数
	unsigned TotalCols;													//保存矩阵的列数
	unsigned CurIndex;													//保存当前位置（从0开始计数）
	FORWARDMODE CurForwardMode;											//保存从当前点往下点流转的模式
	std::deque<T>		MatrixValue;									//表示存在矩阵中的值

	void ToNextInRowPriorityMode();										//按照RowPriority的模式进入到下一个索引点
	void ToNextInProximityMode();										//按照proximity的模式去下一个索引点
	void ToNextInSnakeMode();											//按照Snake的模式去下一个索引点

};

#endif



template<class T>
JMatrix2D<T>::JMatrix2D(unsigned rows, unsigned cols, FORWARDMODE nm) :
TotalRows(rows),
TotalCols(cols),
CurIndex(0)
{
	MatrixValue = std::deque<T>(rows*cols,static_cast<T>(0));
	CurForwardMode = nm;
	
	
}

template<class T>
T & JMatrix2D<T>::GetValue(unsigned row, unsigned col)
{
	return MatrixValue.at((row - 1)*TotalCols + col);
}


template<class T>
T & JMatrix2D<T>::GetValue(unsigned index)//注意，矩阵的索引号是从0开始的
{
	return MatrixValue.at(index);
}

template<class T>
void JMatrix2D<T>::SetNextMode(FORWARDMODE nm)
{
	CurForwardMode = nm;
}

template<class T>
int JMatrix2D<T>::GetForwardMode()
{
	return CurForwardMode;
}

template<class T>
const unsigned & JMatrix2D<T>::GetCurIndex() const
{
	return CurIndex;
}


template<class T>
JMatrix2D<T> &  JMatrix2D<T>::operator++()
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


template<class T>
void JMatrix2D<T>::ToNextInRowPriorityMode()
{
	if (CurIndex == -1 || CurIndex >= TotalRows*TotalCols - 1)
	{
		CurIndex = -1;
	}
	else
	{
		++CurIndex;
	}
}


template<class T>
void JMatrix2D<T>::ToNextInProximityMode()
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

template<class T>
void JMatrix2D<T>::ToNextInSnakeMode()
{

}

