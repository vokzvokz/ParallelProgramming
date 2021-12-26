#include <iostream>
#include <omp.h>

#define MAX_THREADS 10

#define in_range(var, max) for(auto var = 0; var < (max); var += 1)

using namespace std;

int randint(int n){
    return rand() % n;
}

int **generate_matrix(int size, int max_val=100){
    int** rand_matrix = new int* [size];
    in_range(i, size) {
        rand_matrix[i] = new int [size];
        in_range(j, size){
            rand_matrix[i][j] = (max_val==0)?0:randint(max_val);
        }
    }
    return rand_matrix;
}

void print_matrix(int **A, int size){
    in_range(i, size) {
        in_range(j, size){
            cout<<A[i][j]<<" ";
        }
        cout<<endl;
    }
}

struct experiment_result{
    double time_consumed_ijk, time_consumed_jki, time_consumed_kij;
    int** result;
};

#pragma clang diagnostic push
#pragma ide diagnostic ignored "DanglingPointer"
void delete_matrix(int** A, int size){
    in_range(i, size)
        delete[] A[i];
    delete[] A;
}
#pragma clang diagnostic pop

#pragma clang diagnostic push
#pragma ide diagnostic ignored "openmp-use-default-none"
experiment_result run_multiplication(int** A, int** B, int size, int n_threads, bool delete_result_matrix=true){
    experiment_result result{};
    // I J K
    result.result = generate_matrix(size, 0);
    double start_time = omp_get_wtime();
#pragma omp parallel for num_threads(n_threads) schedule(static)
    in_range(i, size){
        in_range(j, size){
            in_range(k, size){
                int buf = A[i][k] * B[k][j];
                #pragma omp atomic
                result.result[i][j] += buf;
            }
        }
    }
    result.time_consumed_ijk = omp_get_wtime() - start_time;
    start_time = omp_get_wtime();
    // J K I
    in_range(j, size){
        in_range(k, size){
            in_range(i, size){
                int buf = A[i][k] * B[k][j];
                #pragma omp atomic
                result.result[i][j] += buf;
            }
        }
    }
    result.time_consumed_jki = omp_get_wtime() - start_time;
    start_time = omp_get_wtime();
    // K I J
    in_range(k, size){
        in_range(i, size){
            in_range(j, size){
                int buf = A[i][k] * B[k][j];
                #pragma omp atomic
                result.result[i][j] += buf;
            }
        }
    }
    result.time_consumed_kij = omp_get_wtime() - start_time;
    if(delete_result_matrix)
        delete_matrix(result.result, size);
    return result;
}
#pragma clang diagnostic pop

int main(int argc, char* argv[]) {
    int size;
    size = atoi(argv[1]);

    int** A = generate_matrix(size);
    int** B = generate_matrix(size);

    experiment_result results[MAX_THREADS];

    for(int n_threads=1; n_threads<=MAX_THREADS; n_threads++) {
        results[n_threads-1] = run_multiplication(A, B, size, n_threads);
        cout << "RUNNING ON " << n_threads << " threads"<<endl;
        cout << "\tIJK: " << results[n_threads-1].time_consumed_ijk << endl;
        cout << "\tJKI: " << results[n_threads-1].time_consumed_jki << endl;
        cout << "\tKIJ: " << results[n_threads-1].time_consumed_kij << endl;
    }

    double effectiveness_ijk[MAX_THREADS-1];
    double effectiveness_jki[MAX_THREADS-1];
    double effectiveness_kij[MAX_THREADS-1];

    in_range(i, MAX_THREADS-1){
        effectiveness_ijk[i]=results[0].time_consumed_ijk/results[i+1].time_consumed_ijk;
        effectiveness_jki[i]=results[0].time_consumed_jki/results[i+1].time_consumed_jki;
        effectiveness_kij[i]=results[0].time_consumed_kij/results[i+1].time_consumed_kij;
    }

    cout<<"---------------------\nEFFECTIVENESS:\n";
    in_range(i, MAX_THREADS-1){
        cout<<i+2<<" THREADS\n";
        cout<<"\tIJK: "<<effectiveness_ijk[i]<<" ";
        cout<<"\tJKI: "<<effectiveness_jki[i]<<" ";
        cout<<"\tKIJ: "<<effectiveness_kij[i]<<endl;
    }

    delete_matrix(A, size);
    delete_matrix(B, size);
    return 0;
}