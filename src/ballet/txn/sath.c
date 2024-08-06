#include "fd_txn.h"
#include "../../util/sanitize/fd_sanitize.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Hand-crafted invalid instruction that would trigger an out-of-bounds
   write in a previous version. It is not a valid transaction. */
// FD_IMPORT_BINARY( transaction5, "fixtures/transaction5.bin" );

#define RED_ZONE_SZ (8UL)

int
main( int     argc,
      char ** argv ) {
  fd_boot( &argc, &argv );

  fd_rng_t _rng[1]; fd_rng_t * rng = fd_rng_join( fd_rng_new( _rng, 0U, 0UL ) );
  // //get payload from argv[1]
  // uchar const             * payload =
  // ulong                     payload_sz = 

    // Check if the payload argument is provided
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <payload>\n", argv[0]);
        return 1;
    }

int TESTING = 0;

if (TESTING) {
// Open the payload file
    FILE *file = fopen(argv[1], "rb");
    if (!file) {
        perror("fopen");
        return 1;
    }

 // Determine the file size
    fseek(file, 0, SEEK_END);
    long payload_sz = ftell(file);
    fseek(file, 0, SEEK_SET);

 // Allocate buffer for the payload
    uchar *payload = (uchar *)malloc(payload_sz);
    if (!payload) {
        perror("malloc");
        fclose(file);
        return 1;
    }

    // Read the payload into the buffer
    fread(payload, 1, payload_sz, file);
    fclose(file);
}

    // Get payload from argv[1]
    uchar const *payload = (uchar const *)argv[1];
    ulong payload_sz = strlen((char *)payload);

    // Print the payload size
    printf("Payload size: %ld bytes\n", payload_sz);

        // Print the payload contents
    printf("Payload contents:\n");
    // for (long i = 0; i < payload_sz; i++) {
    //     printf("%02x ", payload[i]);
    // }
        fwrite(payload, 1, payload_sz, stdout);

    printf("\n");


// uchar out_buf[ FD_TXN_MAX_SZ+RED_ZONE_SZ ];
    uchar out_buf[4];

  FD_TEST( 0UL                          != fd_txn_parse( payload, payload_sz, out_buf, NULL ) );

    // Print out_buf contents
    printf("out_buf contents:\n");
    for (size_t i = 0; i < payload_sz; i++) {
      //only buggy if payload_sz == 100
      if (payload_sz == 1198 || i< sizeof(out_buf)) 
        printf("%02x ", out_buf[i]);
    }
    printf("\n");


  fd_rng_delete( fd_rng_leave( rng ) );

  FD_LOG_NOTICE(( "pass" ));
  fd_halt();
  return 0;
}
