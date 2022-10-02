void ZeroMatrix(double * A, size_t N);
void RandomMatrix(double * A, size_t N);
double CalcMatMulTime_ijk(double * A, double * B, double * C, size_t N);
double CalcMatMulTime_jik(double * A, double * B, double * C, size_t N);
double CalcMatMulTime_kij(double * A, double * B, double * C, size_t N);
double CalcMatMulTime_kij_opt(double * A, double * B, double * C, size_t N);
double CalcMatMulTime_habr(double * A, double * B, double * C, size_t N);
void Show(double * C, size_t N);
void Sum(double * A, double * B, double * C, size_t N);
void Diff(double * A, double * B, double * C, size_t N);
void Split(double * A, double * A11, double * A12, double * A21, double * A22, size_t N, size_t N_split);
void Join(double * A, double * A11, double * A12, double * A21, double * A22, size_t N, size_t N_split);
void Strassen_2x2(double * A, double * B, double * C);
void Strassen(double * A, double * B, double * C, size_t N);

