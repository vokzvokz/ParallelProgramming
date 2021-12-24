#include <iostream>
#include <time.h>
#include <omp.h>

using namespace std;

int *fill_array(int size, int rand_array[]){
    for (int i = 0; i < size; i++) rand_array[i] = rand();
    return rand_array;
}

int main(int argc, char* argv[]) {
    int size;
    size = atoi(argv[1]);

    int rand_array[size];
    fill_array(size, rand_array);
    cout<<"CREATED ARRAY OF "<<size<<" ELEMENTS\n";
//    sequential
    clock_t tStart = clock();
    int max_val = rand_array[0];
    for(int el: rand_array)
        max_val = max(el, max_val);
    printf("SEQUENTIAL:\n\tTime taken: %.5fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
//    parallel
    for(int N=2;N<=10;N++){
        tStart = clock();
        max_val = rand_array[0];
# pragma omp parallel for reduction(max:max_val) num_threads(N)
        for(int el: rand_array)
            max_val = max(el, max_val);


        cout<<N<<" PARALLEL THREADS:\n";
        printf("\tTime taken: %.5fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    }
    return 0;
}











//#include <iostream>
//#include <omp.h>
//using namespace std;
//int main()
//{
//    const long int n = 40000;
//    double* a = new double[n];
//    double* b = new double[n];
//    double* c = new double[n];
//    for (long int i = 0; i < n; i++)
//    {
//        a[i] = (double)rand() / RAND_MAX;
//        b[i] = (double)rand() / RAND_MAX;
//    }
//    double time = omp_get_wtime();
//#pragma omp parallel shared(a,b,c)
//    for (long int i = 0; i < n; i++)
//    {
//        c[i] = a[i] + b[i];
//    }
//    cout << "c[100]=" << c[100] << endl;
//    cout << "Time = " << (omp_get_wtime() - time) << endl;
//    delete[] a;
//    delete[] b;
//    delete[] c;
//}