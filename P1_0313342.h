#include <iostream>
#include <cmath>

using namespace std;

class MatrixTerm
{
    friend class SparseMatrix;
private:
    int row,col;        //row:x  col:y
    double value;       //value=coefficient
};
class SparseMatrix
{
protected:              //設private不易取值
    int terms,capacity;
    MatrixTerm *smArray;//動態陣列
public:
    SparseMatrix():terms(0),capacity(10),smArray(nullptr)
    {}                                                                           //default constructor
    SparseMatrix(const SparseMatrix& a){ *this=a;}                               //copy constructor
    SparseMatrix& operator=(const SparseMatrix& a)                               //overload assignment operator
    {
        if(this==&a)
            return *this;       //如果自己等於自己則直接回傳自己
        
        terms=a.terms;
        capacity=a.capacity;
        smArray=new MatrixTerm[capacity];
        memcpy(smArray,a.smArray,sizeof(MatrixTerm)*capacity);
        return *this;
    }
    void SetTerms(const int *row, const int *column, const double *value, int n)
    {
        if(smArray!=nullptr)
        {
            delete [] smArray;
            smArray=nullptr;
            
        }                           //清空
        terms=n;
        smArray=new MatrixTerm[n];    //預設長度是n
        capacity=terms;
        for(int i=0;i<terms;i++)
        {
            //cout<<"row:"<<i<<"  =  "<<row[i]<<endl;
            smArray[i].row=row[i];
            smArray[i].col=column[i];
            smArray[i].value=value[i];
        }
        
        for(int i=0;i<terms;i++)        //sort
        {
            for(int j=i;j<terms;j++)
            {
                if((smArray[i].row<smArray[j+1].row)||((smArray[i].row==smArray[j+1].row)&&smArray[i].col<smArray[j+1].col))
                {
                    double temp1=smArray[j].row;
                    double temp2=smArray[j].col;
                    double temp3=smArray[j].value;
                    smArray[j].row=smArray[j+1].row;
                    smArray[j].col=smArray[j+1].col;
                    smArray[j].value=smArray[j+1].value;
                    smArray[j+1].row=temp1;
                    smArray[j+1].col=temp2;
                    smArray[j+1].value=temp3;
                }
            }
        }   //end sort
    }
    
    ~SparseMatrix(){ if(smArray) delete [] smArray;}    //destructor釋放記憶體空間
    
    void NewTerm(const int& Nrow,const int Ncol,const double Nvalue)
    {
         if(terms==capacity) //若陣列滿了，則new一個陣列再將原本的資料複製進去
        {
            capacity*=2;
            MatrixTerm* temp=new MatrixTerm[capacity];
            for(int i=0;i<terms;i++)
            {
                temp[i].row=smArray[i].row;
                temp[i].col=smArray[i].col;
                temp[i].value=smArray[i].value;
            }
            delete [] smArray;
            smArray=temp;
        }
        if(smArray==nullptr)
        {
            smArray=new MatrixTerm[capacity];
        }
        
        smArray[terms].row=Nrow;
        cout<<"Nrow="<<Nrow<<endl;
        smArray[terms].col=Ncol;
        smArray[terms].value=Nvalue;
        terms++;    //terms增加1
        
    }
    
    SparseMatrix Add(const SparseMatrix &b)
    {
        SparseMatrix temp;
        int aPos=0,bPos=0;  //aPos為原本smArray的index，bPos為b.smArray的index
        while((aPos<terms)&&(bPos<b.terms))
        {
            
            cout<<"---------"<<smArray[aPos].row<<","<<smArray[aPos].col<<","<<smArray[aPos].value<<endl;
            cout<<"---------"<<smArray[bPos].row<<","<<smArray[bPos].col<<","<<smArray[bPos].value<<endl;

            if(smArray[aPos].row==b.smArray[bPos].row&&smArray[aPos].col==b.smArray[bPos].col)  //同次數，add
            {
                double tempValue=smArray[aPos].value+b.smArray[bPos].value;
                cout<<"temp:"<<tempValue<<endl;
                if(tempValue)
                {
                    temp.NewTerm(smArray[aPos].row,smArray[aPos].col,tempValue);
                    cout<<"1:"<<temp.smArray[aPos].row<<","<<temp.smArray[aPos].col<<","<<temp.smArray[aPos].value<<endl;
                }
                aPos++; bPos++;
            }
            
            else if(smArray[aPos].row==b.smArray[bPos].row&&smArray[aPos].col!=b.smArray[bPos].col)
            {
                if(smArray[aPos].col>b.smArray[bPos].col)   //存this
                {
                    temp.NewTerm(smArray[aPos].row,smArray[aPos].col,smArray[aPos].value);
                    cout<<"2:"<<temp.smArray[aPos].row<<","<<temp.smArray[aPos].col<<","<<temp.smArray[aPos].value<<endl;
                    aPos++;
                }
                else        //存p
                {
                    temp.NewTerm(b.smArray[bPos].row,b.smArray[bPos].col,b.smArray[bPos].value);
                    cout<<"3:"<<temp.smArray[aPos].row<<","<<temp.smArray[aPos].col<<","<<temp.smArray[aPos].value<<endl;
                    bPos++;
                }
            }
            else
            {
                if(smArray[aPos].row>b.smArray[bPos].row)   //存this
                {
                    temp.NewTerm(smArray[aPos].row,smArray[aPos].col,smArray[aPos].value);
                    cout<<"4:"<<temp.smArray[aPos].row<<","<<temp.smArray[aPos].col<<","<<temp.smArray[aPos].value<<endl;
                    aPos++;
                }
                else        //存p
                {
                    temp.NewTerm(b.smArray[bPos].row,b.smArray[bPos].col,b.smArray[bPos].value);
                    cout<<"5:"<<temp.smArray[aPos].row<<","<<temp.smArray[aPos].col<<","<<temp.smArray[aPos].value<<endl;
                    bPos++;
                }
            }
        }//end while
        
        //把剩下的加入
        for(;aPos<terms;aPos++)
        {
            temp.NewTerm(smArray[aPos].row,smArray[aPos].col,smArray[aPos].value);
            cout<<"6:"<<temp.smArray[aPos].row<<","<<temp.smArray[aPos].col<<","<<temp.smArray[aPos].value<<endl;
        }
        for(;bPos<terms;bPos++)
        {
            temp.NewTerm(b.smArray[bPos].row,b.smArray[bPos].col,b.smArray[bPos].value);
            cout<<"7:"<<temp.smArray[aPos].row<<","<<temp.smArray[aPos].col<<","<<temp.smArray[aPos].value<<endl;
        }
        *this=temp;
        return *this;
    }
    
    int getRow(int i)const{return smArray[i].row;}
    int getCol(int i)const{return smArray[i].col;}
    double getValue(int i)const{return smArray[i].value;}
};

class SparsePoly2D:public SparseMatrix
{
public:
    SparsePoly2D():SparseMatrix(){}
    void SetTerms(const int *x_exp, const int *y_exp, const double *coef, int n)
    {
        SparseMatrix::SetTerms(x_exp,y_exp,coef,n);
    }
    SparsePoly2D Add(const SparsePoly2D &p)
    {
        (*this).SparseMatrix::Add(p);
        return *this;
    }
    double Eval(double x, double y)
    {
        double sum=(double)0;
        for(int i=0;i<terms;i++)
        {
            sum+=pow(x,getRow(i))*pow(y,getCol(i))*getValue(i);
        }
        return sum;
    }
    void Print()
    {
        for(int i=0;i<terms;i++)
        {
            cout<<"<"<<getRow(i)<<','<<getCol(i)<<','<<getValue(i)<<'>';
        }
    }
};
