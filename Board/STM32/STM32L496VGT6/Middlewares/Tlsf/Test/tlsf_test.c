    struct tlsf_stat_t tlsf_stat;

    tlsf_stat_r( &HEAP_SRAM, &tlsf_stat);

    printf("free memory available = %d, used memory = %d \r\n",tlsf_stat.mfree, tlsf_stat.mused);
