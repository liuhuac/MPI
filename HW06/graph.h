/*--------------------------- graph.h  -----------------------------
 |  Purpose: Header file for graph.c.
 |
 |  Author: Liuhua Chen
 |
 |  Time: 2014.11.19
 *-------------------------------------------------------------------*/
#ifndef GRAPH_H
#define GRAPH_H
int read_matrix(char *file_name, int *n, double ***A);
int write_matrix (char *file_name, int n, double **A);
void print_matrix(int n, double **A);
#endif
