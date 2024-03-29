
#include "pch.h"
#include "mpi.h"
#include <time.h>
#include <ctime>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <math.h>


//new mergesort
void merge(int *arr, int l, int m, int r)
{
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;

	/* create temp arrays */
	int *L = (int*)malloc(n1 * sizeof(int));
	int *R = (int*)malloc(n2 * sizeof(int));

	/* Copy data to temp arrays L[] and R[] */
	for (i = 0; i < n1; i++)
		L[i] = arr[l + i];
	for (j = 0; j < n2; j++)
		R[j] = arr[m + 1 + j];

	/* Merge the temp arrays back into arr[l..r]*/
	i = 0; // Initial index of first subarray 
	j = 0; // Initial index of second subarray 
	k = l; // Initial index of merged subarray 
	while (i < n1 && j < n2)
	{
		if (L[i] <= R[j])
		{
			arr[k] = L[i];
			i++;
		}
		else
		{
			arr[k] = R[j];
			j++;
		}
		k++;
	}

	/* Copy the remaining elements of L[], if there
	   are any */
	while (i < n1)
	{
		arr[k] = L[i];
		i++;
		k++;
	}

	/* Copy the remaining elements of R[], if there
	   are any */
	while (j < n2)
	{
		arr[k] = R[j];
		j++;
		k++;
	}
}
void mergeSort(int *arr, int l, int r)
{
	if (l < r)
	{
		// Same as (l+r)/2, but avoids overflow for 
		// large l and h 
		int m = l + (r - l) / 2;

		// Sort first and second halves 
		mergeSort(arr, l, m);
		mergeSort(arr, m + 1, r);

		merge(arr, l, m, r);
	}
}
void FindMinMax(int *, int, int *);
void CalcMedian(int *, int, int);
int CaseEntireIsOverlap(int *, int *);
void PrintArray(int *, int);

void FindMinMax2(int *a, int size, int *minmax)
{
	int i = 0;
	if (size % 2 == 0)
	{
		if (a[0] > a[1])
		{
			minmax[1] = a[0];
			minmax[0] = a[1];
		}
		else
		{
			minmax[1] = a[1];
			minmax[0] = a[0];
		}
		i = 2;
	}
	else
	{
		minmax[1] = a[0];
		minmax[0] = a[0];
		i = 1;
	}
	while (i < size - 1)
	{
		if (a[i] > a[i + 1])
		{
			if (a[i] > minmax[1])
				minmax[1] = a[i];
			if (a[i + 1] < minmax[0])
				minmax[0] = a[i + 1];
		}
		else
		{
			if (a[i + 1] > minmax[1])
				minmax[1] = a[i + 1];
			if (a[i] < minmax[0])
				minmax[0] = a[i];
		}
		i += 2;
	}
}
//Count Overlap area
void CountOverlapElem(int *rec_minmax, int *array, int size, int& nonOverlapC, int& overlapC)
{
	for (size_t i = 0; i < size; i++)
	{
		if (array[i] >= rec_minmax[0] && array[i] <= rec_minmax[1])
		{
			overlapC++;
		}
		else
		{
			nonOverlapC++;
		}
	}
}
//Find Overlap Area And Non Overlap Area
void FindOverlap(int *array, int size, int *overlap, int *rec_minmax, int *nonOverlap)
{
	//for (size_t i = 0; i < size; i++)
	//{
	//	if (array[i] > rec_minmax[0] && array[i] < rec_minmax[1])
	//	{
	//		overlapC++;
	//	}
	//	else
	//	{
	//		nonOverlapC++;
	//	}
	//}
	//overlap = (int*)malloc(overlapC * sizeof(MPI_INT));
	//nonOverlap = (int*)malloc(nonOverlapC * sizeof(MPI_INT));
	int v = 0, b = 0;
	for (size_t i = 0; i < size; i++)
	{
		if (array[i] >= rec_minmax[0] && array[i] <= rec_minmax[1])
		{
			overlap[v] = array[i];
			v++;
		}
		else
		{
			nonOverlap[b] = array[i];
			b++;
		}
	}
	//for (size_t i = 0; i < size; i++)
	//{
	//	if (array[i] > rec_minmax[0] && array[i] < rec_minmax[1])
	//	{
	//		overlap[overlapC] = array[i];
	//		overlapC++;
	//		//printf("pverlap number %d \n", overlap[overlapC]);
	//	}
	//	else
	//	{
	//		nonOverlap[nonOverlapC] = array[i];
	//		nonOverlapC++;
	//	}
	//}
}

//---------------------------------Finds Median-------------------------------------
// Function to find max 
int maximum(int a, int b)
{
	return a > b ? a : b;
}
// Function to find minimum 
int minimum(int a, int b)
{
	return a < b ? a : b;
}
double FindMedian(int *overlap, int overlapC, int *otherOverlap, int otherOverlapC)
{
	//Merge overlaps
	double Median = NULL;
	int combineOverlapCounts = overlapC + otherOverlapC;
	int *CombinedOverlaps = (int*)malloc((combineOverlapCounts) * sizeof(MPI_INT));
	for (size_t i = 0; i < combineOverlapCounts; i++)
	{
		for (size_t j = 0; j < overlapC; j++)
		{
			CombinedOverlaps[i] = overlap[j];
			i++;
		}
		for (size_t k = 0; k < otherOverlapC; k++)
		{
			CombinedOverlaps[i] = otherOverlap[k];
			i++;
		}
	}
	//Sort Combined Overlaps
	int *tmp_array = (int*)malloc((combineOverlapCounts) * sizeof(int));
	mergeSort(CombinedOverlaps, 0, (combineOverlapCounts - 1));
	//Calculate Median
	if ((combineOverlapCounts) % 2 == 0)
	{
		//even
		Median = (CombinedOverlaps[((combineOverlapCounts) / 2)] + CombinedOverlaps[((combineOverlapCounts) / 2) - 1]) / 2;
		//printf("Median is %d \n", Median);
	}
	else
	{
		//odd
		int InterNumRound = round((combineOverlapCounts) / 2);
		Median = CombinedOverlaps[InterNumRound];
		//printf("Median is %d \n", Median);
	}
	return Median;
}
double FindMedianSecondWay(int *a, int n, int *b, int m)
{
	int min_index = 0, max_index = n, i, j, median;

	while (min_index <= max_index)
	{
		i = (min_index + max_index) / 2;
		j = ((n + m + 1) / 2) - i;


		if (i < n && j > 0 && b[j - 1] > a[i])
			min_index = i + 1;
		else if (i > 0 && j < m && b[j] < a[i - 1])
			max_index = i - 1;
		// we have found the desired halves. 
		else
		{
			if (i == 0)
				median = b[j - 1];

			// b[] so we returning the last element in  
			// a[] from the first half. 
			else if (j == 0)
				median = a[i - 1];
			else
				median = maximum(a[i - 1], b[j - 1]);
			break;
		}
	}
	if ((n + m) % 2 == 1)
		return (double)median;

	// Elements from a[] in the second half is an empty set.     
	if (i == n)
		return (median + b[j]) / 2.0;

	// Elements from b[] in the second half is an empty set. 
	if (j == m)
		return (median + a[i]) / 2.0;

	return (median + minimum(a[i], b[j])) / 2.0;
}
//---------------------------------Finds Median-------------------------------------

//-------------------------------Special Case-------------------------------------------------------------------------------------------------------------------------
//Count Elements for Send
void SpecialCountElementsForSendS(double median, int *overlap, int overlapC, int& arrayForSendC, int& arrayNotSendC)
{
	for (size_t i = 0; i < overlapC; i++)
	{
		if (overlap[i] < median)
		{
			arrayForSendC++;
		}
		else
		{
			arrayNotSendC++;
		}
	}
}
//Find Elements For Send(Sends the elements bigger from median)
void SpecialCountElementsForSendB(double median, int *nonOverlap, int nonOverlapC, int *overlap, int overlapC, int& arrayForSendC, int& arrayNotSendC)
{
	for (size_t i = 0; i < overlapC; i++)
	{
		if (overlap[i] >= median)
		{
			arrayForSendC++;
		}
		else
		{
			arrayNotSendC++;
		}
	}
	for (size_t i = 0; i < nonOverlapC; i++)
	{
		if (nonOverlap[i] >= median)
		{
			arrayForSendC++;
		}
	}
}
//Find Elements For Send(Sends the elements smaller from median)
void SpecialFindElementsForSendS(double median, int *overlap, int overlapC, int *arrayForSend, int *arrayNotSend)
{
	int v = 0, b = 0;
	for (size_t i = 0; i < overlapC; i++)
	{
		if (overlap[i] < median)
		{
			arrayForSend[v] = overlap[i];
			v++;
		}
		else
		{
			arrayNotSend[b] = overlap[i];
			b++;
		}
	}
}
//Find Elements For Send(Sends the elements bigger from median)
void SpecialFindElementsForSendB(double median, int *nonOverlap, int nonOverlapC, int *overlap, int overlapC, int *arrayForSend, int *arrayNotSend)
{
	int v = 0, b = 0;
	for (size_t i = 0; i < overlapC; i++)
	{
		if (overlap[i] >= median)
		{
			arrayForSend[v] = overlap[i];
			v++;
		}
		else
		{
			arrayNotSend[b] = overlap[i];
			b++;
		}
	}
	for (size_t i = 0; i < nonOverlapC; i++)
	{
		if (nonOverlap[i] >= median)
		{
			arrayForSend[v] = nonOverlap[i];
			nonOverlap[i] = -1;
			v++;
		}
	}
}
//-------------------------------Special Case-------------------------------------------------------------------------------------------------------------------------

//Count Elements for Send
void CountElementsForSendB(double median, int *overlap, int overlapC, int& arrayForSendC, int& arrayNotSendC)
{
	for (size_t i = 0; i < overlapC; i++)
	{
		if (overlap[i] >= median)
		{
			arrayForSendC++;
		}
		else
		{
			arrayNotSendC++;
		}
	}
}
//Find Elements For Send(Sends the elements bigger from median)
void FindElementsForSendB(double median, int *overlap, int overlapC, int *arrayForSend, int *arrayNotSend)
{
	int v = 0, b = 0;
	for (size_t i = 0; i < overlapC; i++)
	{
		if (overlap[i] >= median)
		{
			arrayForSend[v] = overlap[i];
			v++;
		}
		else
		{
			arrayNotSend[b] = overlap[i];
			b++;
		}
	}
}
//Count Elements for Send
void CountElementsForSendS(double median, int *overlap, int overlapC, int& arrayForSendC, int& arrayNotSendC)
{
	for (size_t i = 0; i < overlapC; i++)
	{
		if (overlap[i] < median)
		{
			arrayForSendC++;
		}
		else
		{
			arrayNotSendC++;
		}
	}
}
//Find Elements For Send(Sends the elements smaller from median)
void FindElementsForSendS(double median, int *overlap, int overlapC, int *arrayForSend, int *arrayNotSend)
{
	int v = 0, b = 0;
	for (size_t i = 0; i < overlapC; i++)
	{
		if (overlap[i] < median)
		{
			arrayForSend[v] = overlap[i];
			v++;
		}
		else
		{
			arrayNotSend[b] = overlap[i];
			b++;
		}
	}
}

//Send and Receive Exchange
void SendReceiveExchange(int *send, int sendC, int *receive, int& recC, int dest, int tag)
{
	MPI_Send(send, sendC, MPI_INT, dest, tag, MPI_COMM_WORLD);
	MPI_Status status;
	MPI_Probe(dest, tag, MPI_COMM_WORLD, &status);
	MPI_Get_count(&status, MPI_INT, &recC);
	receive = (int*)malloc(recC * sizeof(MPI_INT));
	MPI_Recv(receive, recC, MPI_INT, dest, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}
//Receive and Send Exchange
void ReceiveSendExchange(int *send, int sendC, int *receive, int& recC, int dest, int tag)
{
	MPI_Status status;
	MPI_Probe(dest, tag, MPI_COMM_WORLD, &status);
	MPI_Get_count(&status, MPI_INT, &recC);
	receive = (int*)malloc(recC * sizeof(MPI_INT));
	MPI_Recv(receive, recC, MPI_INT, dest, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	MPI_Send(send, sendC, MPI_INT, dest, tag, MPI_COMM_WORLD);
}
void CountNewSubarray(int *nonOverlap, int nonOverlapC, int *notSend, int notSendC, int *recArr, int recArrC, int& size)
{
	int newSize = 0;
	for (int k = 0; k < nonOverlapC; k++)
	{
		if (nonOverlap[k] != -1)
		{
			newSize++;
		}
	}
	for (int l = 0; l < notSendC; l++)
	{
		newSize++;
	}
	for (int m = 0; m < recArrC; m++)
	{
		newSize++;
	}
	size = newSize;
}
//Unite The New Array
void NewSubarray(int *nonOverlap, int nonOverlapC, int *notSend, int notSendC, int *recArr, int recArrC, int *subarray, int size)
{
	int i = 0;
	for (int k = 0; k < nonOverlapC; k++)
	{
		if (nonOverlap[k] != -1)
		{
			subarray[i] = nonOverlap[k];
			i++;
		}
	}
	for (int l = 0; l < notSendC; l++)
	{
		subarray[i] = notSend[l];
		i++;
	}
	for (int m = 0; m < recArrC; m++)
	{
		subarray[i] = recArr[m];
		i++;
	}

	//Sort Each New Array again
	mergeSort(subarray, 0, (size - 1));
}



int main(int argc, char* argv[])
{
	/********** Create and populate the array **********/
	int n = 10000000;
	int *original_array = (int*)malloc(n * sizeof(MPI_INT));

	//printf("This is the unsorted array: ");
	for (int c = 0; c < n; c++)
	{
		original_array[c] = rand() % (n);
		//printf("%d ", original_array[c]);
	}
	//printf("\n");
	//printf("\n");
	/********** Initialize MPI **********/
	int world_rank;
	int world_size;
	MPI_Init(&argc, &argv);
	double tStart = clock();
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	if (world_size == 1)
	{
		mergeSort(original_array, 0, (n - 1));

		printf("\n");
		double tStop = clock();
		printf("%f secs\n", (tStop - tStart) / CLOCKS_PER_SEC);
		MPI_Finalize();
		return 0;
	}
	/********** Print the original array **********/
	if (world_rank == 0)
	{
		printf("\n");
		//printf("This is the unsorted array: ");
		//PrintArray(original_array, n);
		printf("\n");
	}
	/********** Divide the array in equal-sized chunks **********/
	int size = n / world_size;

	/********** Send each subarray to each process **********/
	int *sub_array = (int*)malloc(size * sizeof(MPI_INT));
	MPI_Scatter(original_array, size, MPI_INT, sub_array, size, MPI_INT, 0, MPI_COMM_WORLD);

	/********** Perform the mergesort on each process **********/
	mergeSort(sub_array, 0, (size - 1));
	//printf("This is the sorted array: ");
	//PrintArray(sub_array, size);
	//---------------------------------------------------------------------------------------------------------------------------------------------------------
	int logofThreads = log2(world_size);
	for (int i = 0; i < logofThreads; i++)
	{
		int step = pow(2, i + 1);
		for (int k = 0; k < world_size; k += step)
		{
			for (int j = 0; j < pow(4, i) / pow(2, i); j++)
			{
				for (int l = 0; l < pow(2, i); l++)
				{
					int Rank1FromGoup = k + j;
					int step2 = k + l;
					int Ranks2FromGroup = (int)pow(2, i) + step2;
					//Find Min And Max
					int *minmax = (int*)malloc(2 * sizeof(MPI_INT));
					FindMinMax2(sub_array, size, minmax);
					//FindMinMax(sub_array, size, minmax);
					int *recv_minmax = (int*)malloc(2 * sizeof(MPI_INT));
					//Overlapped Intervals array and Not Overlapped array
					int OverlapCount = 0;
					int NonOverlapCount = 0;
					int OtherOverlapCount = 0;
					int *ArrOverlap = NULL, *ArrNonOverlap = NULL, *ArrOtherOverlap = NULL;
					//Median
					double Median = NULL;
					//Arrays For Send And Recieve Elements
					int ElementsForSendCount = 0, RecievedArrCount = 0, ElemNotSendedC = 0;
					int *ArrayToSend = NULL, *RecievedArr = NULL, *ArrayNotSend = NULL;

					if (world_rank == Rank1FromGoup)
					{
						//Send and Recieve MinMax
						MPI_Send(minmax, 2, MPI_INT, Ranks2FromGroup, 0, MPI_COMM_WORLD);
						MPI_Recv(recv_minmax, sizeof(recv_minmax), MPI_INT, Ranks2FromGroup, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

						//Store The Overlapped Intervals And the Not Overlapped
						CountOverlapElem(recv_minmax, sub_array, size, NonOverlapCount, OverlapCount);
						ArrOverlap = (int*)malloc(OverlapCount * sizeof(MPI_INT));
						ArrNonOverlap = (int*)malloc(NonOverlapCount * sizeof(MPI_INT));
						FindOverlap(sub_array, size, ArrOverlap, recv_minmax, ArrNonOverlap);

						//Find Where to Send the Bigger Numbers And where the smallers
						bool BiggerLeft = false;
						bool BiggerRight = false;
						if (minmax[1] > recv_minmax[1] && minmax[0] > recv_minmax[0])
						{
							BiggerRight = false; BiggerLeft = true;
						}
						else if (minmax[1] < recv_minmax[1] && minmax[0] < recv_minmax[0])
						{
							BiggerRight = true; BiggerLeft = false;
						}
						else
						{
							BiggerRight = false; BiggerLeft = true;
						}
						//FInd if array is entire overlap
						int SpecialCase = 0;
						SpecialCase = CaseEntireIsOverlap(minmax, recv_minmax);
						if (minmax[0] == recv_minmax[0] && minmax[1] == recv_minmax[1])
						{
							SpecialCase = 1;
						}

						//----------Find the median By merging the two intervals--------------------------->
						MPI_Send(ArrOverlap, OverlapCount, MPI_INT, Ranks2FromGroup, 1, MPI_COMM_WORLD);
						MPI_Status status;
						MPI_Probe(Ranks2FromGroup, 1, MPI_COMM_WORLD, &status);
						MPI_Get_count(&status, MPI_INT, &OtherOverlapCount);
						ArrOtherOverlap = (int*)malloc(OtherOverlapCount * sizeof(MPI_INT));
						MPI_Recv(ArrOtherOverlap, OtherOverlapCount, MPI_INT, Ranks2FromGroup, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

						//Go to next step if there is no overlap
						if ((OtherOverlapCount + OverlapCount) <= 1)
						{
							if (OverlapCount == 1)
							{
								BiggerRight = false; BiggerLeft = true;
							}
							else if (OtherOverlapCount == 1)
							{
								BiggerRight = true; BiggerLeft = false;
							}
							if ((OtherOverlapCount + OverlapCount) == 1)
							{
								continue;
							}
						}

						//combined overlaps
						//Median = FindMedian(ArrOverlap, OverlapCount, ArrOtherOverlap, OtherOverlapCount); //palios tropos
						Median = FindMedianSecondWay(ArrOverlap, OverlapCount, ArrOtherOverlap, OtherOverlapCount);
						//Median = minmax[1] / 2;
						//printf("Median is %f \n", Median);
						//----------Find the median By merging the two intervals--------------------------->
						if (SpecialCase == 0)
						{
							if (BiggerLeft == false && BiggerRight == true)
							{
								//Find elements for send and the elements that stay
								CountElementsForSendB(Median, ArrOverlap, OverlapCount, ElementsForSendCount, ElemNotSendedC);
								ArrayToSend = (int*)malloc(ElementsForSendCount * sizeof(MPI_INT));
								ArrayNotSend = (int*)malloc(ElemNotSendedC * sizeof(MPI_INT));
								FindElementsForSendB(Median, ArrOverlap, OverlapCount, ArrayToSend, ArrayNotSend);

								//Send and receive the specified elements
								//SendReceiveExchange(ArrayToSend, ElementsForSendCount, RecievedArr, RecievedArrCount,Ranks2FromGroup,1);
								MPI_Send(ArrayToSend, ElementsForSendCount, MPI_INT, Ranks2FromGroup, 2, MPI_COMM_WORLD);
								MPI_Status status2;
								MPI_Probe(Ranks2FromGroup, 2, MPI_COMM_WORLD, &status2);
								MPI_Get_count(&status2, MPI_INT, &RecievedArrCount);
								RecievedArr = (int*)malloc(RecievedArrCount * sizeof(MPI_INT));
								MPI_Recv(RecievedArr, RecievedArrCount, MPI_INT, Ranks2FromGroup, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

								//Unite the new elements that recieved with the old ones
								//More Specific Unite RecievedArr,ArrayNotOverlap,ArrayNotSend
								CountNewSubarray(ArrNonOverlap, NonOverlapCount, ArrayNotSend, ElemNotSendedC, RecievedArr, RecievedArrCount, size);
								sub_array = (int*)malloc(size * sizeof(MPI_INT));
								NewSubarray(ArrNonOverlap, NonOverlapCount, ArrayNotSend, ElemNotSendedC, RecievedArr, RecievedArrCount, sub_array, size);

								/*	printf("Rank %d,iteration %d sorted array: ", world_rank, i);
									printf("[%d-%d] \n", sub_array[0], sub_array[size - 1]);*/
									//PrintArray(sub_array, size);
							}
							else if (BiggerLeft == true && BiggerRight == false)
							{
								//Find elements for send and the elements that stay
								CountElementsForSendS(Median, ArrOverlap, OverlapCount, ElementsForSendCount, ElemNotSendedC);
								ArrayToSend = (int*)malloc(ElementsForSendCount * sizeof(MPI_INT));
								ArrayNotSend = (int*)malloc(ElemNotSendedC * sizeof(MPI_INT));
								FindElementsForSendS(Median, ArrOverlap, OverlapCount, ArrayToSend, ArrayNotSend);

								//Send and receive the specified elements
								//SendReceiveExchange(ArrayToSend, ElementsForSendCount, RecievedArr, RecievedArrCount,Ranks2FromGroup,1);
								MPI_Send(ArrayToSend, ElementsForSendCount, MPI_INT, Ranks2FromGroup, 1, MPI_COMM_WORLD);
								MPI_Status status;
								MPI_Probe(Ranks2FromGroup, 1, MPI_COMM_WORLD, &status);
								MPI_Get_count(&status, MPI_INT, &RecievedArrCount);
								RecievedArr = (int*)malloc(RecievedArrCount * sizeof(MPI_INT));
								MPI_Recv(RecievedArr, RecievedArrCount, MPI_INT, Ranks2FromGroup, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

								//Unite the new elements that recieved with the old ones
								//More Specific Unite RecievedArr,ArrayNotOverlap,ArrayNotSend
								CountNewSubarray(ArrNonOverlap, NonOverlapCount, ArrayNotSend, ElemNotSendedC, RecievedArr, RecievedArrCount, size);
								sub_array = (int*)malloc(size * sizeof(MPI_INT));
								NewSubarray(ArrNonOverlap, NonOverlapCount, ArrayNotSend, ElemNotSendedC, RecievedArr, RecievedArrCount, sub_array, size);

								/*	printf("Rank %d,iteration %d sorted array: ", world_rank, i);
									printf("[%d-%d] \n", sub_array[0], sub_array[size - 1]);*/
									//PrintArray(sub_array, size);
							}
						}
						else if (SpecialCase != 0)
						{
							if (SpecialCase == 1)
							{
								//Find elements for send and the elements that stay
								SpecialCountElementsForSendB(Median, ArrNonOverlap, NonOverlapCount, ArrOverlap, OverlapCount, ElementsForSendCount, ElemNotSendedC);
								ArrayToSend = (int*)malloc(ElementsForSendCount * sizeof(MPI_INT));
								ArrayNotSend = (int*)malloc(ElemNotSendedC * sizeof(MPI_INT));
								SpecialFindElementsForSendB(Median, ArrNonOverlap, NonOverlapCount, ArrOverlap, OverlapCount, ArrayToSend, ArrayNotSend);

								//Send and receive the specified elements
								MPI_Send(ArrayToSend, ElementsForSendCount, MPI_INT, Ranks2FromGroup, 1, MPI_COMM_WORLD);
								MPI_Status status;
								MPI_Probe(Ranks2FromGroup, 1, MPI_COMM_WORLD, &status);
								MPI_Get_count(&status, MPI_INT, &RecievedArrCount);
								RecievedArr = (int*)malloc(RecievedArrCount * sizeof(MPI_INT));
								MPI_Recv(RecievedArr, RecievedArrCount, MPI_INT, Ranks2FromGroup, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

								//Unite the new elements that recieved with the old ones
								//More Specific Unite RecievedArr,ArrayNotOverlap,ArrayNotSend
								CountNewSubarray(ArrNonOverlap, NonOverlapCount, ArrayNotSend, ElemNotSendedC, RecievedArr, RecievedArrCount, size);
								sub_array = (int*)malloc(size * sizeof(MPI_INT));
								NewSubarray(ArrNonOverlap, NonOverlapCount, ArrayNotSend, ElemNotSendedC, RecievedArr, RecievedArrCount, sub_array, size);

								/*	printf("Rank %d,iteration %d sorted array: ", world_rank, i);
									printf("[%d-%d] \n", sub_array[0], sub_array[size - 1]);*/
							}
							else if (SpecialCase == 2)
							{
								//Find elements for send and the elements that stay
								SpecialCountElementsForSendS(Median, ArrOverlap, OverlapCount, ElementsForSendCount, ElemNotSendedC);
								ArrayToSend = (int*)malloc(ElementsForSendCount * sizeof(MPI_INT));
								ArrayNotSend = (int*)malloc(ElemNotSendedC * sizeof(MPI_INT));
								SpecialFindElementsForSendS(Median, ArrOverlap, OverlapCount, ArrayToSend, ArrayNotSend);

								//Send and receive the specified elements
								MPI_Status status2;
								MPI_Probe(Ranks2FromGroup, 2, MPI_COMM_WORLD, &status2);
								MPI_Get_count(&status2, MPI_INT, &RecievedArrCount);
								RecievedArr = (int*)malloc(RecievedArrCount * sizeof(MPI_INT));
								MPI_Recv(RecievedArr, RecievedArrCount, MPI_INT, Ranks2FromGroup, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
								MPI_Send(ArrayToSend, ElementsForSendCount, MPI_INT, Ranks2FromGroup, 2, MPI_COMM_WORLD);

								//Unite the new elements that recieved with the old ones
								//More Specific Unite RecievedArr,ArrayNotOverlap,ArrayNotSend
								CountNewSubarray(ArrNonOverlap, NonOverlapCount, ArrayNotSend, ElemNotSendedC, RecievedArr, RecievedArrCount, size);
								sub_array = (int*)malloc(size * sizeof(MPI_INT));
								NewSubarray(ArrNonOverlap, NonOverlapCount, ArrayNotSend, ElemNotSendedC, RecievedArr, RecievedArrCount, sub_array, size);

								/*		printf("Rank %d,iteration %d sorted array: ", world_rank, i);
										printf("[%d-%d] \n", sub_array[0], sub_array[size - 1]);*/
							}
						}
					}
					else if (world_rank == Ranks2FromGroup)
					{
						//Send and Recieve MinMax
						MPI_Recv(recv_minmax, sizeof(recv_minmax), MPI_INT, Rank1FromGoup, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
						MPI_Send(minmax, 2, MPI_INT, Rank1FromGoup, 0, MPI_COMM_WORLD);

						//Store The Overlapped Intervals And the Not Overlapped
						CountOverlapElem(recv_minmax, sub_array, size, NonOverlapCount, OverlapCount);
						ArrOverlap = (int*)malloc(OverlapCount * sizeof(MPI_INT));
						ArrNonOverlap = (int*)malloc(NonOverlapCount * sizeof(MPI_INT));
						FindOverlap(sub_array, size, ArrOverlap, recv_minmax, ArrNonOverlap);

						//Find Where to Send the Bigger Numbers And where the smallers
						bool BiggerLeft = false;
						bool BiggerRight = false;
						if (minmax[1] > recv_minmax[1] && minmax[0] > recv_minmax[0])
						{
							BiggerRight = true; BiggerLeft = false;
						}
						else if (minmax[1] < recv_minmax[1] && minmax[0] < recv_minmax[0])
						{
							BiggerRight = false; BiggerLeft = true;
						}
						else
						{
							BiggerRight = false; BiggerLeft = true;
						}
						//FInd if array is entire overlap
						char SpecialCase = NULL;
						SpecialCase = CaseEntireIsOverlap(minmax, recv_minmax);
						if (minmax[0] == recv_minmax[0] && minmax[1] == recv_minmax[1])
						{
							SpecialCase = 2;
						}
						//----------Find the median By merging the two intervals--------------------------->
						MPI_Status status;
						MPI_Probe(Rank1FromGoup, 1, MPI_COMM_WORLD, &status);
						MPI_Get_count(&status, MPI_INT, &OtherOverlapCount);
						ArrOtherOverlap = (int*)malloc(OtherOverlapCount * sizeof(MPI_INT));
						MPI_Recv(ArrOtherOverlap, OtherOverlapCount, MPI_INT, Rank1FromGoup, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
						MPI_Send(ArrOverlap, OverlapCount, MPI_INT, Rank1FromGoup, 1, MPI_COMM_WORLD);

						//Go to next step if there is no overlap
						if ((OtherOverlapCount + OverlapCount) <= 1)
						{
							if (OverlapCount == 1)
							{
								BiggerRight = true; BiggerLeft = false;
							}
							else if (OtherOverlapCount == 1)
							{
								BiggerRight = false; BiggerLeft = true;
							}
							if ((OtherOverlapCount + OverlapCount) == 1)
							{
								continue;
							}
						}

						//combined overlaps
						//Median = FindMedian(ArrOverlap, OverlapCount, ArrOtherOverlap, OtherOverlapCount); palios tropos
						Median = FindMedianSecondWay(ArrOverlap, OverlapCount, ArrOtherOverlap, OtherOverlapCount);
						//printf("Median is %f \n", Median);
						//----------Find the median By merging the two intervals--------------------------->
						if (SpecialCase == 0)
						{
							if (BiggerLeft == false && BiggerRight == true)
							{
								//Find elements for send and the elements that stay
								CountElementsForSendS(Median, ArrOverlap, OverlapCount, ElementsForSendCount, ElemNotSendedC);
								ArrayToSend = (int*)malloc(ElementsForSendCount * sizeof(MPI_INT));
								ArrayNotSend = (int*)malloc(ElemNotSendedC * sizeof(MPI_INT));
								FindElementsForSendS(Median, ArrOverlap, OverlapCount, ArrayToSend, ArrayNotSend);

								//Send and receive the specified elements
								//ReceiveSendExchange(ArrayToSend, ElementsForSendCount, RecievedArr, RecievedArrCount, Rank1FromGoup, 1);
								MPI_Status status2;
								MPI_Probe(Rank1FromGoup, 2, MPI_COMM_WORLD, &status2);
								MPI_Get_count(&status2, MPI_INT, &RecievedArrCount);
								RecievedArr = (int*)malloc(RecievedArrCount * sizeof(MPI_INT));
								MPI_Recv(RecievedArr, RecievedArrCount, MPI_INT, Rank1FromGoup, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
								MPI_Send(ArrayToSend, ElementsForSendCount, MPI_INT, Rank1FromGoup, 2, MPI_COMM_WORLD);

								//Unite the new elements that recieved with the old ones
								//More Specific Unite RecievedArr,ArrayNotOverlap,ArrayNotSend
								//-----------------------------------
								CountNewSubarray(ArrNonOverlap, NonOverlapCount, ArrayNotSend, ElemNotSendedC, RecievedArr, RecievedArrCount, size);
								sub_array = (int*)malloc(size * sizeof(MPI_INT));
								NewSubarray(ArrNonOverlap, NonOverlapCount, ArrayNotSend, ElemNotSendedC, RecievedArr, RecievedArrCount, sub_array, size);

								/*		printf("Rank %d,iteration %d sorted array: ", world_rank, i);
										printf("[%d-%d] \n", sub_array[0], sub_array[size - 1]);*/
										//PrintArray(sub_array, size);
							}
							else if (BiggerLeft == true && BiggerRight == false)
							{
								//Find elements for send and the elements that stay
								CountElementsForSendB(Median, ArrOverlap, OverlapCount, ElementsForSendCount, ElemNotSendedC);
								ArrayToSend = (int*)malloc(ElementsForSendCount * sizeof(MPI_INT));
								ArrayNotSend = (int*)malloc(ElemNotSendedC * sizeof(MPI_INT));
								FindElementsForSendB(Median, ArrOverlap, OverlapCount, ArrayToSend, ArrayNotSend);

								//Send and receive the specified elements
								//ReceiveSendExchange(ArrayToSend, ElementsForSendCount, RecievedArr, RecievedArrCount, Rank1FromGoup, 1);
								MPI_Status status;
								MPI_Probe(Rank1FromGoup, 1, MPI_COMM_WORLD, &status);
								MPI_Get_count(&status, MPI_INT, &RecievedArrCount);
								RecievedArr = (int*)malloc(RecievedArrCount * sizeof(MPI_INT));
								MPI_Recv(RecievedArr, RecievedArrCount, MPI_INT, Rank1FromGoup, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
								MPI_Send(ArrayToSend, ElementsForSendCount, MPI_INT, Rank1FromGoup, 1, MPI_COMM_WORLD);

								//Unite the new elements that recieved with the old ones
								//More Specific Unite RecievedArr,ArrayNotOverlap,ArrayNotSend
								//-----------------------------------
								CountNewSubarray(ArrNonOverlap, NonOverlapCount, ArrayNotSend, ElemNotSendedC, RecievedArr, RecievedArrCount, size);
								sub_array = (int*)malloc(size * sizeof(MPI_INT));
								NewSubarray(ArrNonOverlap, NonOverlapCount, ArrayNotSend, ElemNotSendedC, RecievedArr, RecievedArrCount, sub_array, size);

								/*	printf("Rank %d,iteration %d sorted array: ", world_rank,i);
									printf("[%d-%d] \n", sub_array[0], sub_array[size - 1]);*/
									//PrintArray(sub_array, size);
							}
						}
						else if (SpecialCase != 0)
						{
							if (SpecialCase == 1)
							{
								//Find elements for send and the elements that stay
								SpecialCountElementsForSendB(Median, ArrNonOverlap, NonOverlapCount, ArrOverlap, OverlapCount, ElementsForSendCount, ElemNotSendedC);
								ArrayToSend = (int*)malloc(ElementsForSendCount * sizeof(MPI_INT));
								ArrayNotSend = (int*)malloc(ElemNotSendedC * sizeof(MPI_INT));
								SpecialFindElementsForSendB(Median, ArrNonOverlap, NonOverlapCount, ArrOverlap, OverlapCount, ArrayToSend, ArrayNotSend);

								//Send and receive the specified elements
								MPI_Send(ArrayToSend, ElementsForSendCount, MPI_INT, Rank1FromGoup, 2, MPI_COMM_WORLD);
								MPI_Status status2;
								MPI_Probe(Rank1FromGoup, 2, MPI_COMM_WORLD, &status2);
								MPI_Get_count(&status2, MPI_INT, &RecievedArrCount);
								RecievedArr = (int*)malloc(RecievedArrCount * sizeof(MPI_INT));
								MPI_Recv(RecievedArr, RecievedArrCount, MPI_INT, Rank1FromGoup, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

								//Unite the new elements that recieved with the old ones
								//More Specific Unite RecievedArr,ArrayNotOverlap,ArrayNotSend
								CountNewSubarray(ArrNonOverlap, NonOverlapCount, ArrayNotSend, ElemNotSendedC, RecievedArr, RecievedArrCount, size);
								sub_array = (int*)malloc(size * sizeof(MPI_INT));
								NewSubarray(ArrNonOverlap, NonOverlapCount, ArrayNotSend, ElemNotSendedC, RecievedArr, RecievedArrCount, sub_array, size);

								//printf("Rank %d,iteration %d sorted array: ", world_rank, i);
								//printf("[%d-%d] \n", sub_array[0], sub_array[size - 1]);
								//PrintArray(sub_array, size);
							}
							else if (SpecialCase == 2)
							{
								//Find elements for send and the elements that stay
								SpecialCountElementsForSendS(Median, ArrOverlap, OverlapCount, ElementsForSendCount, ElemNotSendedC);
								ArrayToSend = (int*)malloc(ElementsForSendCount * sizeof(MPI_INT));
								ArrayNotSend = (int*)malloc(ElemNotSendedC * sizeof(MPI_INT));
								SpecialFindElementsForSendS(Median, ArrOverlap, OverlapCount, ArrayToSend, ArrayNotSend);
								//Send and receive the specified elements
								MPI_Status status;
								MPI_Probe(Rank1FromGoup, 1, MPI_COMM_WORLD, &status);
								MPI_Get_count(&status, MPI_INT, &RecievedArrCount);
								RecievedArr = (int*)malloc(RecievedArrCount * sizeof(MPI_INT));
								MPI_Recv(RecievedArr, RecievedArrCount, MPI_INT, Rank1FromGoup, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
								MPI_Send(ArrayToSend, ElementsForSendCount, MPI_INT, Rank1FromGoup, 1, MPI_COMM_WORLD);

								CountNewSubarray(ArrNonOverlap, NonOverlapCount, ArrayNotSend, ElemNotSendedC, RecievedArr, RecievedArrCount, size);
								sub_array = (int*)malloc(size * sizeof(MPI_INT));
								NewSubarray(ArrNonOverlap, NonOverlapCount, ArrayNotSend, ElemNotSendedC, RecievedArr, RecievedArrCount, sub_array, size);

								//printf("Rank %d,iteration %d sorted array: ", world_rank, i);
								/*printf("[%d-%d] \n", sub_array[0], sub_array[size - 1]);*/
								//PrintArray(sub_array, size);
							}
						}
					}
				}
				//MPI_Barrier(MPI_COMM_WORLD);
			}
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------
	printf("Rank %d sorted array: ", world_rank);
	printf("[%d-%d]", sub_array[0], sub_array[size - 1]);
	//PrintArray(sub_array, size);
	/********** Finalize MPI **********/
	if (world_rank == 0)
	{
		printf("\n");
		double tStop = clock();
		printf("%f secs\n", (tStop - tStart) / CLOCKS_PER_SEC);
	}

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
}

void FindMinMax(int *a, int size, int *minmax)
{
	minmax[0] = a[0];
	minmax[1] = a[0];

	for (int i = 0; i < size; i++)
	{
		if (a[i] > minmax[1])
		{
			minmax[1] = a[i];
		}
		if (a[i] < minmax[0])
		{
			minmax[0] = a[i];
		}
	}
}
void CalcMedian(int *a, int size, int Median)
{
	//find if interval is odd or even
	if (size % 2 == 0)
	{
		//even
		Median = a[(size / 2)];
		//printf("the median is %d \n",Median);
	}
	else
	{
		//odd
		int InterNumRound = round(size / 2);
		Median = a[InterNumRound - 2];
	}
}
int CaseEntireIsOverlap(int *minmax, int *otherminmax)
{
	//if (x == 1)
	//{
	//	return 1;//left
	//}
	//else if (x == 2)
	//{
	//	return 2;//right
	//}
	if (minmax[0] <= otherminmax[0] && minmax[1] >= otherminmax[1])
	{
		return 1;//left
	}
	else if (minmax[0] >= otherminmax[0] && minmax[1] <= otherminmax[1])
	{
		return 2;//right
	}
	else
	{
		return 0;//Not special case
	}
}
void PrintArray(int *a, int size)
{
	for (size_t i = 0; i < size; i++)
	{
		printf("%d, ", a[i]);
	}
	printf("\n");
}