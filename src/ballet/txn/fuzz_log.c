#include "fd_txn.h"
#include "../../util/sanitize/fd_sanitize.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED_ZONE_SZ (8UL)

int main(int argc, char **argv)
{
    fd_boot(&argc, &argv);

    fd_rng_t _rng[1];
    fd_rng_t *rng = fd_rng_join(fd_rng_new(_rng, 0U, 0UL));

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <payload>\n", argv[0]);
        return 1;
    }

    int TESTING = 0;
    uchar const *payload;
    ulong payload_sz;

    if (TESTING)
    {
        FILE *file = fopen(argv[1], "rb");
        if (!file)
        {
            perror("fopen");
            return 1;
        }

        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        fseek(file, 0, SEEK_SET);

        payload = (uchar *)malloc(size);
        if (!payload)
        {
            perror("malloc");
            fclose(file);
            return 1;
        }

        fread((void *)payload, 1, size, file);
        fclose(file);
        payload_sz = (ulong)size;
    }
    else
    {
        payload = (uchar const *)argv[1];
        payload_sz = strlen((char *)payload);
    }

    printf("Payload size: %lu bytes\n", payload_sz);
    printf("Payload contents:\n");
    fwrite(payload, 1, payload_sz, stdout);
    printf("\n");

    uchar out_buf[FD_TXN_MAX_SZ + RED_ZONE_SZ];

    int result = fd_txn_parse(payload, payload_sz, out_buf, NULL);

    // Log output buffer contents
    FILE *log_file = fopen("fuzz_log.txt", "a");
    fprintf(log_file, "Payload: ");
    fwrite(payload, 1, payload_sz, log_file);
    fprintf(log_file, "\nResult: %d\n", result);
    fprintf(log_file, "out_buf contents: ");
    for (size_t i = 0; i < sizeof(out_buf); i++)
    {
        fprintf(log_file, "%02x ", out_buf[i]);
    }
    fprintf(log_file, "\n\n");
    fclose(log_file);

    fd_rng_delete(fd_rng_leave(rng));

    FD_LOG_NOTICE(("pass"));
    fd_halt();
    return 0;
}