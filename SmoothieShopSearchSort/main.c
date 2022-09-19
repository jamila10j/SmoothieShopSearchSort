#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "leak_detector_c.h"

typedef struct coordinate {
    int x;
    int y;
}coordinate;

coordinate currentPosition;

void mergeSort(coordinate coords[], int startIndex, int endIndex, int t);
void insertionSort(coordinate coords[], int startIndex, int endIndex);
void merge(coordinate coords[], int startIndex, int mid, int endIndex);


coordinate* ReadData(FILE* inFile, int numCoordinates) 
{
    coordinate* fileCoordinates = (coordinate*) malloc(numCoordinates * sizeof(coordinate));

    for (int i = 0; i < numCoordinates; i++)
    {
        fscanf(inFile, "%d", &fileCoordinates[i].x);
        fscanf(inFile, "%d", &fileCoordinates[i].y);
    }

    return fileCoordinates;
}

int CompareTo(coordinate ptrPT1, coordinate ptrPT2)
{
    int distPositionToP1 = sqrt(pow(currentPosition.x - ptrPT1.x, 2) + pow(currentPosition.y - ptrPT1.y, 2));
    int distPositionToP2 = sqrt(pow(currentPosition.x - ptrPT2.x, 2) + pow(currentPosition.y - ptrPT2.y, 2));


    if (ptrPT1.x == ptrPT2.x && ptrPT1.y == ptrPT2.y) return 0;
    else if (distPositionToP1 < distPositionToP2) return -1;
    else if (distPositionToP1 > distPositionToP2) return 1;
    else if (distPositionToP1 == distPositionToP2)
    {
      if (ptrPT1.x == ptrPT2.x)
      {
          if (ptrPT1.y < ptrPT2.y) return -1;
          else if (ptrPT1.y > ptrPT2.y) return 1;
      }
      else if (ptrPT1.x < ptrPT2.x) return -1;
      else if (ptrPT1.x > ptrPT2.x) return 1;
    }
}


void mergeSort(coordinate coords[], int startIndex, int endIndex, int t)
{
    int mid = (startIndex + endIndex) / 2;
  
    if (endIndex - startIndex > t)
    {
      mergeSort(coords, 0, mid, t);
      mergeSort(coords, mid + 1, endIndex, t);
    }
  else
  {
    insertionSort(coords, startIndex, endIndex);
  }

    merge(coords, startIndex, mid, endIndex);
}

void merge(coordinate coords[], int startIndex, int mid, int endIndex)
{
    int i, j, k;
    int n1 = mid - startIndex;
    int n2 =  endIndex - mid + 1;

    coordinate *L = (coordinate*) malloc(n1*sizeof(coordinate));
    coordinate *R = (coordinate*) malloc(n2*sizeof(coordinate));
    for (i = 0; i < n1; i++)
        L[i] = coords[startIndex + i];
    for (j = 0; j < n2; j++)
      R[j] = coords[mid + j];

    i = 0;
    j = 0; 
    k = startIndex;
    while (i < n1 && j < n2)
    {
        if (CompareTo(L[i], R[j]) == -1)
        {
            coords[k] = L[i];
            i++;
        }
        else if (CompareTo(L[i], R[j]) == 1)
        {
            coords[k] = R[j];
            j++;
        }
        else if (CompareTo(L[i], R[j]) == 0)
        {
            coords[k] = L[i];
            i++;
        }
        k++;
    }
  
    while (i < n1)
    {
        coords[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        coords[k] = R[j];
        j++;
        k++;
    }
    free(L);
    free(R);
}

void insertionSort(coordinate coords[], int startIndex, int endIndex)
{
    int coordsLength = endIndex + 1; 
    coordinate hand;
    int i, j;

    for (i = 1; i < coordsLength; i++)
    {
        hand = coords[i];

        for (j = i - 1; j >= 0; j--)
        {
            if (CompareTo(hand, coords[j]) == -1) 
              coords[j + 1] = coords[j];
            else
                break;
        }

        coords[j + 1] = hand;
    }
}

coordinate* PointsToSearch(FILE* inFile, int numPoints)
{
    coordinate* searchArray = (coordinate*) malloc(numPoints * sizeof(coordinate));

    for (int i = 0; i < numPoints; i++)
    {
        fscanf(inFile, "%d", &searchArray[i].x);
        fscanf(inFile, "%d", &searchArray[i].y);
    }

    return searchArray;
}

int BinaryPointSearch(coordinate shopLocations[], int shopLocationsLength, coordinate pointToSearch)
{
    int startIndex = 0;
    int endIndex = shopLocationsLength - 1;
    int mid;


    while (startIndex <= endIndex)
    {
        mid = (startIndex + endIndex) / 2;

        if (CompareTo(pointToSearch, shopLocations[mid]) == 0)
        {
            return mid;
        }
        else if (CompareTo(pointToSearch, shopLocations[mid]) == -1)
        {
            endIndex = mid - 1;
        }
        else if (CompareTo(pointToSearch, shopLocations[mid]) == 1)
        {
            startIndex = mid + 1;
        }
        
    }

    return -1;  
}

void PrintToFileAndConsole(coordinate shopLocations[], int shopLocationsLength, coordinate pointsToSearch[], int pointsToSearchLength, FILE* inFile, FILE* outFile)
{
    int returnValue; 

    for (int i = 0; i < shopLocationsLength; i++)
    {
        printf("%d %d\n", shopLocations[i].x, shopLocations[i].y);
        fprintf(outFile, "%d %d\n", shopLocations[i].x, shopLocations[i].y);
    }

    for (int i = 0; i < pointsToSearchLength; i++)
    {
        returnValue = BinaryPointSearch(shopLocations, shopLocationsLength, pointsToSearch[i]);

        if (returnValue == -1)
        {
            printf("%d %d not found\n", pointsToSearch[i].x, pointsToSearch[i].y);
            fprintf(outFile, "%d %d not found\n", pointsToSearch[i].x, pointsToSearch[i].y);
        }
        else
        {
            printf("%d %d found at rank %d\n", pointsToSearch[i].x, pointsToSearch[i].y, returnValue + 1);
            fprintf(outFile, "%d %d found at rank %d\n", pointsToSearch[i].x, pointsToSearch[i].y, returnValue + 1);
        }
    }
}


int main(void)
{
    atexit(report_mem_leak);

    FILE* inFile = fopen("in.txt", "r");
    FILE* outFile = fopen("out.txt", "w");

    fscanf(inFile, "%d", &currentPosition.x);
    fscanf(inFile, "%d", &currentPosition.y);

    int numShops;
    fscanf(inFile, "%d", &numShops);

    int numSearchPoints;
    fscanf(inFile, "%d", &numSearchPoints);

    int threshold;
    fscanf(inFile, "%d", &threshold);


    coordinate* coordinatesArray = ReadData(inFile, numShops);
    mergeSort(coordinatesArray, 0, numShops - 1, threshold);

    coordinate* pointsToSearch = PointsToSearch(inFile, numSearchPoints);
    PrintToFileAndConsole(coordinatesArray, numShops, pointsToSearch, numSearchPoints, inFile, outFile);



    free(coordinatesArray);
    free(pointsToSearch);

    return 0;
}