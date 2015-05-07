#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <time.h>
#include "functions.h"

void usage(void){
	const char *msg=
	"serial-add-list [OPTIONS]\n"
	"OPTIONS:\n"
	"-i\tthe name of the datafile to add the integers\n"
	"\tdefault default-make-list-file.dat\n";
	printf("usage: %s", msg);
	exit(0);
}

int main(int argc, char *argv[]){
	int num;
	char *ofile=NULL;
	int opt;
	while((opt=getopt(argc, argv, "i:"))!=-1){
		switch(opt){
			case 'i':
				ofile=strdup(optarg);
				break;
			case '?':
			case ':':
			default :
				usage();
				break;
		}
	}

	if(optind!=argc){
                printf("Unknow argument '%s'\n",argv[optind]);
                usage();
        } else if(strcmp(argv[optind-1], "--")==0){
                printf("Unknow argument '%s'\n",argv[optind-1]);
                usage();
        }

	if(ofile==NULL){
		ofile=strdup("default-make-list-file.dat");
	}

	struct timespec entire_start;
	clock_gettime(CLOCK_MONOTONIC,&entire_start);

	int i=0;
	long sum=0;
	int *A;

	read_list(ofile,&num,&A);

	struct timespec comp_start;
        clock_gettime(CLOCK_MONOTONIC,&comp_start);

	for(i=0;i<num;i++){
		sum+=A[i];
	}
	printf("SUM:%ld\n",sum);
	
	struct timespec entire_stop;
        clock_gettime(CLOCK_MONOTONIC,&entire_stop);

	printf("ENTIRE:%f\tCOMPUTATION:%f\n",
		(entire_stop.tv_sec-entire_start.tv_sec)+(entire_stop.tv_nsec-entire_start.tv_nsec)*1e-9,
		(entire_stop.tv_sec-comp_start.tv_sec)+(entire_stop.tv_nsec-comp_start.tv_nsec)*1e-9);
	free(ofile);
	free(A);
	return 0;
}
