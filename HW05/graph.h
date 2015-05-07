/*--------------------------- graph.h  -----------------------------
 |  Purpose: Header file for graph.c.
 |
 |  Author: Liuhua Chen
 |
 |  Time: 2014.11.11
 *-------------------------------------------------------------------*/
#ifndef GRAPH_H
#define GRAPH_H
void read_graph(char *file_name, int *n, int ***A);
void write_graph (char *file_name, int n, int **A);
void print_graph(int n, int **A);
#endif
