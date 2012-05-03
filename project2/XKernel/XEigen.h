
#ifndef SUEIGEN_H
#define SUEIGEN_H


//나두 이클라스는 몬지 모르겠다.. 아 열라 괴롭다...
//걍 3D 겜 엔지 디자인책에 있는 소스 걍 옴겨돠 놓았음..
class SuEigen
{
public:
    SuEigen (int iSize);
    ~SuEigen ();

    // set the matrix for eigensolving
    float& Matrix (int iRow, int iCol);
    void SetMatrix (float** aafMat);

    // get the results of eigensolving (eigenvectors are columns of matrix)
    float GetEigenvalue (int i) const;
    float GetEigenvector (int iRow, int iCol) const;
    float* GetEigenvalue ();
    float** GetEigenvector ();

    // solve eigensystem
    void EigenStuff2 ();
    void EigenStuff3 ();
    void EigenStuff4 ();
    void EigenStuffN ();
    void EigenStuff  ();

    // solve eigensystem, use decreasing sort on eigenvalues
    void DecrSortEigenStuff2 ();
    void DecrSortEigenStuff3 ();
    void DecrSortEigenStuff4 ();
    void DecrSortEigenStuffN ();
    void DecrSortEigenStuff  ();

    // solve eigensystem, use increasing sort on eigenvalues
    void IncrSortEigenStuff2 ();
    void IncrSortEigenStuff3 ();
    void IncrSortEigenStuff4 ();
    void IncrSortEigenStuffN ();
    void IncrSortEigenStuff  ();

protected:
    int m_iSize;
    float** m_aafMat;
    float* m_afDiag;
    float* m_afSubd;

    // Householder reduction to tridiagonal form
    static void Tridiagonal2 (float** aafMat, float* afDiag,
        float* afSubd);
    static void Tridiagonal3 (float** aafMat, float* afDiag,
        float* afSubd);
    static void Tridiagonal4 (float** aafMat, float* afDiag,
        float* afSubd);
    static void TridiagonalN (int iSize, float** aafMat, float* afDiag,
        float* afSubd);

    // QL algorithm with implicit shifting, applies to tridiagonal matrices
    static bool QLAlgorithm (int iSize, float* afDiag, float* afSubd,
        float** aafMat);

    // sort eigenvalues from largest to smallest
    static void DecreasingSort (int iSize, float* afEigval,
        float** aafEigvec);

    // sort eigenvalues from smallest to largest
    static void IncreasingSort (int iSize, float* afEigval,
        float** aafEigvec);
};

//---------------------------------------------------------------------------
inline float& SuEigen::Matrix (int iRow, int iCol)
{
    return m_aafMat[iRow][iCol];
}
//---------------------------------------------------------------------------
inline float SuEigen::GetEigenvalue (int i) const
{
    return m_afDiag[i];
}
//---------------------------------------------------------------------------
inline float SuEigen::GetEigenvector (int iRow, int iCol) const
{
    return m_aafMat[iRow][iCol];
}
//---------------------------------------------------------------------------
inline float* SuEigen::GetEigenvalue ()
{
    return m_afDiag;
}
//---------------------------------------------------------------------------
inline float** SuEigen::GetEigenvector ()
{
    return m_aafMat;
}
//---------------------------------------------------------------------------

#endif
