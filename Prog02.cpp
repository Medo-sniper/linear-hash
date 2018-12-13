//******************************************************************************
//
// Computer Science 2018
//
// Spring 2018
//
// Assignment # 02
//
// Due Date:  Thursday, April, 05
//
// Instructor: Dr. Hung-CHi Su; 
//
// Programmer: Ahmad Banisaeed
//
// Description: To simulate the memory system to run Linear Hashing program for an input file with many
//              records (integers).
//
// Input: unsorted records (integers) stored in a file
//
// Output: sorted records (integers) using Linear Hashing and store them in HD
//
//******************************************************************************
#include<iostream>
#include<fstream>
#include<string>
#include <stdlib.h>
#include<algorithm>
#include<unistd.h>
#include<stdio.h>
#include<math.h>
using namespace std;

const int B = 5;
const int BucketSize = 4;
int Disk[8192] = {0};

struct Buckets {
    int bucketnum;
    int data[BucketSize];
    int elements;
    int OF;
    int nextbucket;
    int st;
};

int i = 1;
int n = 1;
int r = 0;
int s;
int pn;
int m;
int d;
int totalbuckets = 0;
int availablebuckets;
int regbuckets = 0;
int ofbuckets = 0;
int* temp;
int records;
int x;

double utilisation = 0.80;
double res = 0.0;
double capacity = 0.0;
double epsilon = 0.1;
int newm = 1000;

void updatesystem(Buckets *Bucket) {
    Bucket[0].st = records + 1;
    Bucket[0].data[0] = i;
    Bucket[0].data[1] = n;
    Bucket[0].data[2] = r;
    Bucket[0].elements = 3;
    Bucket[0].nextbucket = 1;
    Bucket[0].OF = 0;
    for (int k = 0; k < Bucket[0].elements; k++)
         Disk[Bucket[0].st + k] =   Bucket[0].data[k];
    Disk[Bucket[0].st + B - 1] =   Bucket[0].nextbucket;
    cout << "i = " << Bucket[0].data[0] << "\n"; 
    cout << "n = " << Bucket[0].data[1] << "\n";
    cout << "r = " << Bucket[0].data[2] << "\n";
}
void printBuckets(struct Buckets* Bucket) {
    int s;
    for (int k = 0; k <= n; k++) {
        cout << "\nBucket [" << k << "] :";
        cout << "\nhas elements " << Bucket[k].elements << "\n";
        cout << "Records included: ";
        for ( int j = 0; j < Bucket[k].elements; j++)          
                cout << Bucket[k].data[j] << "  ";
        cout << "\npoints to " << Bucket[k].nextbucket;
        cout << "\n\n";
        if (Bucket[k].nextbucket >= 1000) {
            s = Bucket[k].nextbucket;
            while ( s >=  1000) {
                cout << "Bucket [" << k << "]  has OverFolow Bucket[" << s << "] :";
                cout << "\nhas elements " << Bucket[s].elements << "\n";
                cout << "Records included: ";
                for ( int j = 0; j < Bucket[k].elements; j++) {
                      if (Bucket[s].data[j] != 0)
                           cout << Bucket[s].data[j] << "  ";
                    }
            cout << "\npoints to " << Bucket[s].nextbucket << "\n";
                s = Bucket[s].nextbucket;
            }                
        }
        sleep(1);
    }
    cout <<"\n\n";
    return;
}
void create(Buckets *Bucket, int nn) {
    Bucket[nn].st = records + 1 + nn * B;
    Bucket[nn].bucketnum = nn;
    Bucket[nn].OF = 0;
    Bucket[nn].nextbucket = nn + 1;
    for (int j = 0; j < BucketSize; j++)
         Bucket[nn].data[j] = 0;
    Bucket[nn].elements = 0;
Disk[Bucket[nn].st  + B - 1] =   Bucket[nn].nextbucket;
}
void insert(Buckets *Bucket, int m, int val) {
       if ( m <= n) {
           if ( Bucket[m].elements < BucketSize) { // if Bucket is exist and not full
              Bucket[m].data[Bucket[m].elements] = val;
              Disk[Bucket[m].st + Bucket[m].elements] = Bucket[m].data[Bucket[m].elements];
              Bucket[m].elements += 1;       
           } else if ( Bucket[m].OF != 0 && Bucket[m].elements == BucketSize) { // if Bucket is full and has not- full overflow bucket
                  int mm = m;
                  while (Bucket[mm].OF != 0 && Bucket[mm].elements < BucketSize) {
                        mm = Bucket[mm].nextbucket;
                  }
                        if(Bucket[mm].elements < BucketSize) {
                           Bucket[mm].data[Bucket[mm].elements] = val;
                           Disk[Bucket[mm].st + Bucket[mm].elements] = Bucket[mm].data[Bucket[mm].elements];
                           Bucket[mm].elements += 1; 
                        } else {
                        // create new over flow bucket
                        newm += 1;
                        create(Bucket,newm);
                        Bucket[mm].nextbucket = newm;
                        Bucket[newm].nextbucket = m + 1;
                        Bucket[newm].data[Bucket[newm].elements] = val;
                        Disk[Bucket[newm].st + Bucket[newm].elements] = Bucket[newm].data[Bucket[newm].elements];
                        Bucket[newm].elements += 1; 
                        Bucket[newm].OF = 1; 
                        }
           } else if (Bucket[m].OF == 0 && Bucket[m].elements == BucketSize) {//if bucket is full and does not have overflow bucket  
             // create new over flow bucket
                        newm += 1;
                        create(Bucket,newm);
                        Bucket[m].nextbucket = newm;
                        Bucket[newm].nextbucket = m + 1;
                        Bucket[newm].data[Bucket[newm].elements] = val;
                        Disk[Bucket[newm].st + Bucket[newm].elements] = Bucket[newm].data[Bucket[newm].elements];
                        Bucket[newm].elements += 1; 
                        Bucket[newm].OF = 1; 
           } 
       }
}
void update(Buckets *Bucket, int pn) {
    s = Bucket[pn].elements;
    int on = pn;
    if (Bucket[on].OF != 0) {
        while (Bucket[on].OF != 0) {
              on = Bucket[on].nextbucket;
              s += Bucket[on].elements;      
        }
     }
     on = pn;
     cout << " s = " << s << " on = " << on << " pn = " << pn << endl;
     temp = (int *) malloc ((s - 1) * sizeof(int));
     int ss = s;
     for (int j  = 1; j <= Bucket[pn].elements  && ss != 0; j++) {
              temp[j - 1] = Disk[Bucket[pn].st  + j - 1];
              Disk[Bucket[pn].st  + j - 1] = 0;
              Bucket[pn].data[j - 1] = 0;
              ss -= 1;
     }
     Bucket[pn].elements = 0;
     if (Bucket[pn].OF != 0 && ss != 0) {
                  while (Bucket[pn].OF != 0) {
                    pn = Bucket[pn].nextbucket;
                    //int jj = 0;
                    for (int j  = 1; j <= Bucket[pn].elements  && ss != 0; j++) {
                           temp[j - 1] =   Disk[Bucket[pn].st  + j - 1];
                           Disk[Bucket[pn].st  + j - 1] = 0;
                           Bucket[pn].data[j - 1] = 0;
                           Bucket[pn].elements -= 1;
                           ss -= 1;
                      }
                  }     
   }
   cout << " temp: ";
   for (int j = 0; j < s; j++) 
       cout << temp[j] <<  " ";
   cout << endl;
  
   for (int j = 0; j < s; j++) {
       int d = pow(2,i);
       int m = (temp[j] % d) + 1;
       if (m > n)
           m = m / 2;
       insert(Bucket, m, temp[j]);
    }
free(temp);
printBuckets(Bucket);
}

int main() {
    cout << "\n\nSystem Setup  *******************************************************************************\n";
    int k = 0;   // counter

    ifstream File;   // file handler
    File.open("data3.txt");
    while(!File.eof()) { // copying records from file to Disk
        k++;
        File >> Disk[k];
        File.ignore(1); // I supposed numbers are separted with colon ",". If separated by a space, no need for ignore()
    }
    k++;
    File >> Disk[k];
    Disk[0] = k - 1;   // number of records we have to sort
    File.close();
    
    records = Disk[0] - 1;
    Disk[0] = records;
    cout << "\nRecords " << Disk[0] << "\n";
    cout << "Data records from file: \n";

    for(int k = 1; k <= records; k++)  // showing records on screen
        cout << Disk[k] << " ";
    
    cout << "\n\nBucket Size: " << BucketSize << "\n";
    cout << "records " << records << "\n";
    
    cout << "\n\nLinear Hashing  *******************************************************************************\n";
    availablebuckets = (8192 - records) / B;
    cout << "Available Buckets is " << availablebuckets << endl;
    struct Buckets Bucket[availablebuckets];
  
    int w = 0;
    for (int k = 1; k <= records; k++) 
        cout << Disk[k] << " ";
    cout << "\n";
    w = 0;
    r = 1;
    n = 1;
    i = 0;
    create(Bucket, 1);
    while ( r < BucketSize) {
        Bucket[n].data[w] = Disk[r];
      Disk[Bucket[n].st + w] = Bucket[n].data[w];
      Bucket[n].elements += 1;
        w += 1;
        r += 1;
    }
    w = 0;
    // r -= 1;
    while ( r <= records) {
        capacity = r / (n * BucketSize * 1.0);
        res = utilisation - capacity;
        if (res <  epsilon) {
            if (n < pow(2,i)) {
                n += 1;
                pn = n / 2;
                if (pn == 0) 
                    pn = 1;
                Bucket[n].nextbucket = n + 1;
                cout << "Time to add Bucket[" << n << "]\n";
                cout << "and rehash Bucket[" << pn << "]  , i = " << i << "\n";
                updatesystem(Bucket);
                create(Bucket, n); 
                update(Bucket, pn); 
            }
            else if ( n >= pow(2,i)) {
                i += 1;
                n += 1;
                pn = n / 2;
                cout << "Time to add Bucket[" << n << "]\n";
                cout << "and rehash Bucket[" << pn << "]  , i = " << i << "\n";
                updatesystem(Bucket);
                create(Bucket, n); 
                update(Bucket, pn);
             }
        }
        d = pow(2,i);
        m = Disk[r] % d + 1;
        if (m > n) 
            m = m / 2;
       insert(Bucket, m, Disk[r]);
       r += 1;
    }
    r--;
    updatesystem(Bucket);
    w = 0;
    x = 0;
    Disk[0] = records;

   int lookup;
   cout <<"\n\nPlease enter a record (an integer) to look for ( -1 to exit): ";
   cin >> lookup;
   while (lookup != -1) {
       d = pow(2,i);
       m = lookup % d + 1;
       if ( m > n)
           m = m / 2;
       int found = 0;
       for (int k = 0; k < Bucket[m].elements; k++) {
           if (Bucket[m].data[k] == lookup) {
           cout << "The record " << lookup << " found in Bucket[" << m <<"] \n";
           found = 1;
           break;
           }
       }
       if ( found == 0 && Bucket[m].nextbucket >= 1000) {
        s = Bucket[m].nextbucket;
        do {
            for (int k = 0; k < Bucket[s].elements; k++) {
                if (Bucket[s].data[k] == lookup) {
                    cout << "The record " << lookup << " found in Over Flow Bucket[" << s <<"] of Bucket[" << m << "]\n";
                    found = 1;
                    break;
                    }
                }
            if ( found == 0 && Bucket[s].nextbucket >= 1000) {
                s = Bucket[s].nextbucket;
                }
           } while (  found == 0 && s >= 1000);
        }

       if (found == 0)
            cout << "The record " << lookup << " is not found.\n";
       if (lookup != -1) {
           cout <<"\nPlease enter a record (an integer) to look for ( -1 to exit): ";
           cin >> lookup;
       }
   }
   cout << "\n\n";
    return 0;
}
