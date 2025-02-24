for (int i = 0; i < N; i++) {  
    for (int j = 0; j < N; j++) {  
        for (int k = 0; k < N; k++) {  
            C[i][j] += A[i][k] * B[k][j];  
        }
    }
}


for (int i = 0; i < N; i++) {  
    for (int k = 0; k < N; k++) {  
        for (int j = 0; j < N; j++) {  
            C[i][j] += A[i][k] * B[k][j];  
        }
    }
}
