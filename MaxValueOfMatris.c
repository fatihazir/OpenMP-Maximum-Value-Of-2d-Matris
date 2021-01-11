#include "stdio.h"
#include "stdlib.h"
#include "omp.h"


int main(int argc, char *argv[]){

    //Time variables
    double begin,end,timeSpent;


    //Necessary inputs
    int threadAmountThatUserPrefers;
    int matrisRowLength;
    int matrisColumnLength;

    printf("How many thread would you like to use? ");
    scanf("%d", &threadAmountThatUserPrefers);

    printf("Type n for nxn size matris: ");
    scanf("%d", &matrisRowLength);
    matrisColumnLength = matrisRowLength;

    int rowLenghtPerThread = matrisRowLength/threadAmountThatUserPrefers;



    //Initiate the matris
    double M[matrisRowLength][matrisColumnLength];

    #pragma omp parallel
    {
        int i, j;

        for (i = 0; i < matrisRowLength ; ++i)
            for (j = 0; j < matrisColumnLength; ++j)
                M[i][j] = rand();
    }



    //Display the matris
//    for (int i = 0; i < matrisRowLength; ++i) {
//        for (int j = 0; j < matrisColumnLength; ++j) {
//            printf("  %lf", M[i][j]);
//        }
//        printf("\n");
//    }



    //Sequantial code
//    int prevRowValue = 0;
//    int nextRowValue = RowLenghtPerThread;
//    int tempBiggest = 0;
//
//    while (prevRowValue < matrisRowLength)
//    {
//        //printf("Between %d -- %d:\n",prevRowValue,nextRowValue);
//        for (int i = prevRowValue; i < nextRowValue; ++i)
//        {
//            for (int j = 0; j < matrisColumnLength + 1; ++j) {
//                //printf("%lf   ", M[i][j]);
//                if (M[i][j] > tempBiggest)
//                    tempBiggest = M[i][j];
//            }
//            //printf("\n");
//        }
//        prevRowValue = nextRowValue;
//        nextRowValue += RowLenghtPerThread;
//    }
//
//    printf("TempBiggest: %d\n", tempBiggest);
//


    int n = threadAmountThatUserPrefers;
    int tempBiggests[n];
    double theBiggest = 0;

    printf("Paralel section begins:\n");

    //timer begins
    begin = omp_get_wtime();

    #pragma omp parallel num_threads(threadAmountThatUserPrefers)
    {
        int threadId = omp_get_thread_num();
        int startingIndex = threadId *2;
        int endingIndex = startingIndex + rowLenghtPerThread;
        int tempBiggest = 0;

        #pragma omp for schedule(static,1)
        for (int i = startingIndex; i < endingIndex ; ++i)
        {
            for (int j = 0; j < matrisColumnLength + 1; ++j) {
                if (M[startingIndex][j] > tempBiggest)
                {
                    tempBiggest = M[startingIndex][j];
                    tempBiggests[threadId] = tempBiggest;
                }
            }
        }

        #pragma omp barrier // 'single' is also fits here.
        {
           for (int i = 0; i < threadAmountThatUserPrefers; ++i) {
                if(tempBiggests[i] > theBiggest)
               {
                   theBiggest = tempBiggests[i];
               }

          }
        }

    }
    end = omp_get_wtime();
    timeSpent = end - begin;
    printf("Time spent: %f\n",timeSpent);
    printf("Paralel section ends.\n");
    printf("The biggest number is: %f\n", theBiggest);

    return 0;
}

