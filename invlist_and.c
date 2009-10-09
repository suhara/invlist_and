#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFLEN 1024000

int
main (int argc, char *argv[])
{
  if (argc < 2) {
    fprintf(stderr, "Input query.\n");
    exit(1);
  }

  char *buff = malloc(sizeof(char) * BUFFLEN);
  if (buff == NULL) {
    fprintf(stderr, "Failed to allocate memory.\n");
    exit(1);
  }

  int query_num = argc - 1;

  char *query_list[ query_num ];
  int df[ query_num ];
  int *invlist[ query_num ];

  int i;
  for (i = 0; i < argc - 1; i++) {

    query_list[ i ] = malloc(sizeof(char) * (strlen(argv[i + 1] + 1)) );
    if (query_list[ i ] == NULL) {
      fprintf(stderr, "Failed to allocate memory.\n");
      exit(1);
    }
    strcpy(query_list[ i ], argv[i + 1]);

    char *filename = argv[i + 1];
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
      fprintf(stderr, "Failed to open %s\n", filename);
      exit(1);
    }

    if (fgets(buff, BUFFLEN, fp) != NULL) {
      char *token = strtok(buff, " ");
      df[ i ] = atoi(token);

      invlist[ i ] = malloc(sizeof(int) * df[ i ]);
      if (invlist[ i ] == NULL) {
	fprintf(stderr, "Failed to allocate memroy for invlist.\n");
	exit(1);
      }

      int count = 0;
      while ((token = strtok(NULL, " ")) != NULL && count < df[ i ] ) {
	invlist[ i ][ count ] = atoi( token );
	count++;
      }

    } else {
      fprintf(stderr, "Failed to read line.\n");
    }
  }


  // Print inverted list
  printf("inverted lists for input query ===\n");
  for (i = 0; i < query_num; i++) {

    int j;
    printf("%s -> ", query_list[ i ]);
    printf("%d: ", df[ i ]);

    for (j = 0; j < df[ i ]; j++) {
      printf("%d ", invlist[ i ][ j ]);
    }
    printf("\n");
  }
  printf("\n");
  

  /* Intersection */
  // Create order list by bubble sort
  int length_order_list[ query_num ];
  for (i = 0; i < query_num; i++) {
    length_order_list[ i ] = i;
  }
  for (i = 0; i < query_num; i++) {
    int j;
    for (j = 1; j < query_num - i; j++) {

      if (df[ length_order_list[j] ] <
	  df[ length_order_list[j - 1] ]) {

	// swap i and j
	int swap = length_order_list[ j ];
	length_order_list[ j ] = length_order_list[ j - 1 ];
	length_order_list[ j - 1 ] = swap;
      }
    }
  }

  // Print ordered list
  printf("ordered list===\n");
  for (i = 0; i < query_num; i++) {
    int idx = length_order_list[ i ];
    int j;
    printf("%s -> ", query_list[ idx ]);
    printf("%d: ", df[ idx ]);

    for (j = 0; j < df[ idx ]; j++) {
      printf("%d ", invlist[ idx ][ j ]);
    }
    printf("\n");
  }
  printf("\n");


  // Intersection by linear merge
  int result_num = 0;
  if (query_num == 1) {
    result_num = df[ 0 ];
  }
  for (i = 1; i < query_num; i++) {

    int idx1 = length_order_list[ i - 1 ];
    int idx2 = length_order_list[ i ];


    // Get intersection between invlist[ idx1 ] and invlist[ idx2 ]
    // and store result into invlist[ idx1 ]


    // DEBUG PRINT
    if (i == 1) {
      printf("Intersection of %s and %s: ", query_list[ idx1 ], query_list[ idx2 ]);
    } else {
      printf("Intersection of last result and %s: ", query_list[ idx2 ]);
    }

    int i1 = 0; // pointer for invlist[ idx1 ]
    int i2 = 0; // pointer for invlist[ idx2 ]
    int r  = 0; // pointer for result (invlist[ idx2 ])

    while (i1 < df[ idx1 ] &&
	   i2 < df[ idx2 ]) {

      if (invlist[ idx1 ][ i1 ] <
	  invlist[ idx2 ][ i2 ]) {
	i1++;

      } else if (invlist[ idx1 ][ i1 ] >
		 invlist[ idx2 ][ i2 ]) {
	i2++;

      } else {
	// Hit
	invlist[ idx2 ][ r ] = invlist[ idx2 ][ i2 ];
	r++;

	i1++;
	i2++;
      }
    }

    result_num = r;
    df[ idx2 ] = result_num;


    // DEBUG PRINT
    int j;
    for (j = 0; j < result_num; j++) {
      printf("%d ", invlist[ idx2 ][ j ]);
    }
    printf("\n");
  }
  printf("\n");

  
  // Print result
  printf("search result ===\n");
  printf("result num: %d\n", result_num);

  for (i = 0; i < result_num; i++) {
    printf("%d ", invlist[ length_order_list[query_num - 1] ][ i ]);
  }
  printf("\n");



  for (i = 0; i < query_num; i++) {
    free(query_list[ i ]);
    free(invlist[ i ]);
  }
  free(buff);


  return 0;
}
