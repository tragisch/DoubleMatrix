#ifndef MISC_UR_H
#define MISC_UR_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*******************************/
/*  String Functions           */
/*******************************/

/**
 * @brief write the content of a file in a string.
 *
 * @param path
 * @return char*
 */
char *file2string(char *path);

/**
 * @brief write string to file (from stackoverflow)
 *
 * @param filepath
 * @param data
 * @return int '0' if suceeded
 */
int string2file(char *filepath, char *data);

/**
 * @brief write a string of chars between start an end to string buffer
 *
 * @param str string
 * @param buffer result
 * @param start start char
 * @param end end char
 */
void slice(const char *str, char *buffer, size_t start, size_t end);

/**
 * @brief myString Definition
 *
 */
typedef struct _myString {
  char *str;
  int len;
  int capacity;
} myString;

myString *mystring_init(const char *src);
int mystring_cat(myString *pms, const char *src);
int mystring_destroy(myString *pms);

/*******************************/
/*  Random Functions           */
/*******************************/

double randomDouble();
double randomDouble_betweenBounds(uint32_t min, uint32_t max);
uint32_t randomInt();
uint32_t randomInt_upperBound(uint32_t limit);
uint32_t randomInt_betweenBounds(uint32_t min, uint32_t max);

/*******************************/
/*  Double Arrays              */
/*******************************/

void printDoubleArray(double *p_array, unsigned int length, int method);
void printDoubleArray_Zeros(double *p_array, unsigned int length);
void printDoubleArray_Points(double *p_array, unsigned int length);
double *createRandomDoubleArray(unsigned int length);

#endif // MISC_UR_H
