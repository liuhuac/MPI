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

	time_t entire_start=time(NULL);

	int i=0;
	long sum=0;
	int *A;

	read_list(ofile,&num,&A);

	time_t comp_start=time(NULL);

	for(i=0;i<num;i++){
		sum+=A[i];
	}
	printf("%ld\n",sum);
	
	time_t entire_stop=time(NULL);

	printf("ENTIRE:%d\tCOMPUTATION:%d\n",(int)(entire_stop-entire_start),(int)(entire_stop-comp_start));
	free(ofile);
	free(A);
	return 0;
}
