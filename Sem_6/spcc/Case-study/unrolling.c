


    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {  // Each k processed one by one
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }



    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k += 2) {  // Unrolling factor of 2
                C[i][j] += A[i][k] * B[k][j] + A[i][k+1] * B[k+1][j];
            }
        }
    }