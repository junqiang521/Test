#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <ctype.h>
#include <malloc.h>
#include <stdint.h>
#include <sys/mman.h>

#define _TNVME_H_

#include "../dnvme_interface.h"
#include "../dnvme_ioctls.h"

#include "test_metrics.h"
#include "test_irq.h"

#define DEVICE_FILE_NAME "/dev/nvme0"
#define RANDOM_VAL      10
/*
 * Functions for the ioctl calls
*/
void ioctl_read_data(int file_desc)
{
    int ret_val;
    int i;
    struct rw_generic test_data;

    test_data.type = NVMEIO_BAR01;
    test_data.offset = 8;
    test_data.nBytes = 0x4;
    test_data.acc_type = BYTE_LEN;

    test_data.buffer = malloc(sizeof(char) * test_data.nBytes);
    if (test_data.buffer == NULL) {
        printf("Malloc Failed");
        return;
    }
    printf("Reading Test Application...\n");
    ret_val = ioctl(file_desc, NVME_IOCTL_READ_GENERIC, &test_data);

        if (ret_val < 0) {
            printf("ioctl_set_msg failed:%d\n", ret_val);
            exit(-1);
        }

    i = 0;

    printf("Reading::offset:Data\n");
    while (test_data.nBytes) {
        printf("0x%x:0x%x\n", i, test_data.buffer[i]);
    i++;
    test_data.nBytes--;
    }
    free(test_data.buffer);

}

/*
 * Functions for the ioctl calls
*/
void ioctl_write_data(int file_desc)
{
    int ret_val;
    //int message;
    struct rw_generic test_data;

    test_data.type = NVMEIO_BAR01;
    test_data.offset = 0x14;
    test_data.nBytes = 4;
    test_data.acc_type = DWORD_LEN;
    test_data.buffer = NULL;

    test_data.buffer = malloc(test_data.nBytes);
    if (test_data.buffer == NULL) {
        printf("malloc failed!");
        goto err;
    }
    test_data.buffer[0] = 0x01;
    test_data.buffer[1] = 0x00;
    test_data.buffer[2] = 0x46;
    test_data.buffer[3] = 0x00;

    printf("\nwriting Test Application...\n");
    ret_val = ioctl(file_desc, NVME_IOCTL_WRITE_GENERIC, &test_data);

    if (ret_val < 0) {
            printf("ioctl_set_msg failed:%d\n", ret_val);
            exit(-1);
   }

   free(test_data.buffer);

err:
    return;
}

void ioctl_create_acq(int file_desc)
{
    int ret_val = -1;
    struct nvme_create_admn_q aq_data;

    aq_data.elements = 640; // 2.5 Pages
    aq_data.type = ADMIN_CQ;

    printf("\tACQ No. of Elements = %d\n", aq_data.elements);

    ret_val = ioctl(file_desc, NVME_IOCTL_CREATE_ADMN_Q, &aq_data);
    if(ret_val < 0)
        printf("\tCreation of ACQ Failed!\n");
    else
        printf("\tACQ Creation SUCCESS\n");
}

void ioctl_create_asq(int file_desc)
{
    int ret_val = -1;
    struct nvme_create_admn_q aq_data;

    aq_data.elements = 640;
    aq_data.type = ADMIN_SQ;

    //printf("User Call to Create Admin SQ:\n");
    printf("\tAdmin SQ No. of Elements = %d\n", aq_data.elements);

    ret_val = ioctl(file_desc, NVME_IOCTL_CREATE_ADMN_Q, &aq_data);
    if(ret_val < 0)
        printf("\tCreation of ASQ Failed!\n");
    else
        printf("\tASQ Creation SUCCESS\n");
}

void ioctl_check_device(int file_desc)
{
    int ret_val = -1;
    int status = -1;

    printf("Inside checking device\n");

    ret_val = ioctl(file_desc, NVME_IOCTL_ERR_CHK, &status);
    if(status < 0)
        printf("Device Status FAILED!\n");
    else
        printf("Device Status SUCCESS\n");
}

void ioctl_enable_ctrl(int file_desc)
{
    int ret_val = -1;
    enum nvme_state new_state = ST_ENABLE;

    printf("User Call to Enable Ctrlr:\n");

    ret_val = ioctl(file_desc, NVME_IOCTL_DEVICE_STATE, new_state);
    if(ret_val < 0)
        printf("enable Failed!\n");
    else
        printf("enable SUCCESS\n");
}

void ioctl_disable_ctrl(int file_desc, enum nvme_state new_state)
{
    int ret_val = -1;
    printf("User Call to Disable Ctrlr:\n");

    ret_val = ioctl(file_desc, NVME_IOCTL_DEVICE_STATE, new_state);
    if(ret_val < 0)
        printf("Disable Failed!\n");
    else
        printf("Disable SUCCESS\n");
}

void test_admin(int file_desc)
{
    /* Test Case 1 */
    printf("\nTEST 1.1: Create Admin CQ...\n");
    ioctl_create_acq(file_desc);
    printf("\nTEST 1.2: Create Admin SQ...\n");
    ioctl_create_asq(file_desc);
}

void test_prep_sq(int file_desc)
{
    printf("\nTEST 2.2.1: Allocating 3 IO Contiguous SQs with different sizes...\n");
    printf("\n\tSD_ID : CQ ID = 1 : 1\n");
    ioctl_prep_sq(file_desc, 1, 1, 256, 0);

    printf("\nPress any key to continue..");
    getchar();
    printf("\n\tSD_ID : CQ ID = 2 : 3\n");
    ioctl_prep_sq(file_desc, 2, 3, 200, 1);
    printf("\nPress any key to continue..");
    getchar();
    printf("\n\tSD_ID : CQ ID = 3 : 6\n");
    ioctl_prep_sq(file_desc, 3, 6, 120, 1);
    printf("\nPress any key to continue..");
    getchar();
    printf("\nTEST 2.2.2: Allocating 3 Non-Contiguous IO SQs with different sizes...\n");
    printf("\n\tSD_ID : CQ ID = 4 : 6\n");
    ioctl_prep_sq(file_desc, 4, 6, 10, 0);
    printf("\nPress any key to continue..");
    getchar();
    printf("\n\tSD_ID : CQ ID = 5 : 1\n");
    ioctl_prep_sq(file_desc, 5, 1, 15, 0);
    printf("\nPress any key to continue..");
    getchar();
    printf("\n\tSD_ID : CQ ID = 6 : 3\n");
    ioctl_prep_sq(file_desc, 6, 3, 32, 0);
    printf("\nPress any key to continue..");
    getchar();

}

void test_prep_cq(int file_desc)
{
    printf("\nTEST 2.3.1: Preparing IO Contiguous CQ's with 100 elements each...\n");
    printf("\n\tCQ ID = 1\n");
    ioctl_prep_cq(file_desc, 1, 100, 1);
    printf("\nPress any key to continue..");
    getchar();

    printf("\n\tCQ ID = 2\n");
    ioctl_prep_cq(file_desc, 2, 100, 1);
    printf("\nPress any key to continue..");
    getchar();

    printf("\n\tCQ ID = 3\n");
    ioctl_prep_cq(file_desc, 3, 100, 1);
    printf("\nPress any key to continue..");
    getchar();

    printf("\n\tCQ ID = 4\n");
    ioctl_prep_cq(file_desc, 4, 100, 1);
    printf("\nPress any key to continue..");
    getchar();

    printf("\n\tCQ ID = 5\n");
    ioctl_prep_cq(file_desc, 5, 100, 1);
    printf("\nPress any key to continue..");
    getchar();

    printf("\n\tCQ ID = 6\n");
    ioctl_prep_cq(file_desc, 6, 100, 1);
    printf("\nPress any key to continue..");
    getchar();

    printf("\nTEST 2.3.2: Preparing Non-Contiguous IO CQ's with different sizes...\n");
    printf("\n\tCQ ID = 16\n");
    ioctl_prep_cq(file_desc, 16, 10, 0);
    printf("\nPress any key to continue..");
    getchar();
    printf("\n\tCQ ID = 15\n");
    ioctl_prep_cq(file_desc, 15, 15, 0);
    printf("\nPress any key to continue..");
    getchar();
}

void test_metrics(int file_desc)
{
    printf("\nTEST 4: Get metrics\n");
    /* ACQ Metrics */
    printf("Get ACQ Metrics:\n\n");
    ioctl_get_q_metrics(file_desc, 0, 0, sizeof(struct nvme_gen_cq));
    printf("\nPress any key to continue..");
    getchar();

    /* ASQ Metrics */
    printf("Get ASQ Metrics:\n\n");
    ioctl_get_q_metrics(file_desc, 0, 1, sizeof(struct nvme_gen_sq));
    printf("\nPress any key to continue..");
    getchar();

    printf("Get IO_SQ = 2 (exists) Metrics: \n\n");
    ioctl_get_q_metrics(file_desc, 2, 1, sizeof(struct nvme_gen_sq) + 10);
    printf("\nPress any key to continue..");
    getchar();

    printf("Get IO_SQ = 1 (exists) Metrics: \n\n");
    ioctl_get_q_metrics(file_desc, 1, 1, sizeof(struct nvme_gen_sq));
    printf("\nPress any key to continue..");
    getchar();

    printf("Get IO_SQ = 6 (does not exist. No metrics): \n\n");
    ioctl_get_q_metrics(file_desc, 6, 1, sizeof(struct nvme_gen_sq));
    printf("\nPress any key to continue..");
    getchar();

    printf("Get IO_SQ = 3 (exists but no space to copy. No metrics): \n\n");
    ioctl_get_q_metrics(file_desc, 3, 1, sizeof(struct nvme_gen_sq) - 5);
    printf("\nPress any key to continue..");
    getchar();
}

void tst_ring_dbl(int file_desc)
{
    ioctl_tst_ring_dbl(file_desc, 1);
    ioctl_tst_ring_dbl(file_desc, 10);
    ioctl_tst_ring_dbl(file_desc, 5);
    ioctl_tst_ring_dbl(file_desc, 0);
}

void ioctl_dump(int file_desc, char *tmpfile)
{
    int ret_val = -1;
    struct nvme_file pfile;

    pfile.flen = strlen(tmpfile);

    //printf("size = %d\n", pfile.flen);

    pfile.file_name = malloc(pfile.flen);
    strcpy((char *)pfile.file_name, tmpfile);

    printf("File name = %s\n", pfile.file_name);

    ret_val = ioctl(file_desc, NVME_IOCTL_DUMP_METRICS, &pfile);
    if(ret_val < 0)
        printf("Dump Metrics failed!\n");
    else
        printf("Dump Metrics SUCCESS\n");

}

int test_prp(int file_desc)
{
    ioctl_create_prp_one_page(file_desc);
    ioctl_create_prp_less_than_one_page(file_desc);
    ioctl_create_prp_more_than_two_page(file_desc);
    ioctl_create_list_of_prp(file_desc);
    ioctl_create_fill_list_of_prp(file_desc);
    return 0;
}

void test_reap_inquiry(int file_desc)
{
    //printf("\tReap inquiry on Admin CQ...\n");
    //ioctl_reap_inquiry(file_desc, 0);
    printf("\tReap inquiry on CQ = 1...\n");
    ioctl_reap_inquiry(file_desc, 1);
    printf("\tReap inquiry on CQ = 2...\n");
    ioctl_reap_inquiry(file_desc, 2);
    printf("\tReap inquiry on CQ = 3...\n");
    ioctl_reap_inquiry(file_desc, 3);
    printf("\tReap inquiry on CQ = 4...\n");
    ioctl_reap_inquiry(file_desc, 4);
    printf("\tReap inquiry on CQ = 5...\n");
    ioctl_reap_inquiry(file_desc, 5);
    printf("\tReap inquiry on CQ = 6...\n");
    ioctl_reap_inquiry(file_desc, 6);
}

void display_contents(uint8_t *kadr, int elem)
{
    int i;
    for (i = 0; i < elem; i += 16) {
        printf("%x ", *kadr);
        // display_cq_data((unsigned char *)kadr, 1);
        // kadr += 16;
        kadr++;
    }
}

void set_reap_cq(int file_desc, int cq_id, int elements, int size, int disp)
{
    ioctl_reap_cq(file_desc, cq_id, elements, size, disp);
}

int main()
{
    int file_desc, test_case, i, ret_val;
    char *tmpfile1 = "/tmp/temp_file1.txt";
    char *tmpfile2 = "/tmp/temp_file2.txt";
    char *tmpfile3 = "/tmp/temp_file3.txt";
    char *tmpfile4 = "/tmp/temp_file4.txt";
    char *tmpfile5 = "/tmp/temp_file5.txt";
    char *tmpfile6 = "/tmp/temp_file6.txt";
    char *tmpfile7 = "/tmp/temp_file7.txt";
    char *tmpfile8 = "/tmp/temp_file8.txt";
    char *tmpfile14 = "/tmp/temp_file14.txt";
    char *tmpfile15 = "/tmp/temp_file15.txt";
    char *tmpfile17 = "/tmp/temp_file17.txt";
    char *tmpfile18 = "/tmp/temp_file18.txt";
    char *tmpfile27 = "/tmp/temp_file27.txt";
    char *tmpfile28 = "/tmp/temp_file28.txt";
    char *tmpfilei = "/tmp/irq_test.txt";
    char *closefile = "/tmp/closefile.txt";
    /* Maximum possible entries */
    void *read_buffer, *read_irq_buffer_1, *read_irq_buffer_2, *read_irq_buffer_3;
    void *identify_buffer;
    void *discontg_sq_buf, *discontg_cq_buf;
    void *write_buffer, *write_buffer_wo_meta;
    void *irq_dcq_buf, *irq_dsq_buf;


    uint8_t *kadr;
    uint32_t offset,meta_id,cmds,index;
    uint16_t cq_id, int_vec;

    printf("\n*****\t Demo \t*****\n");

    printf("Starting Test Application...\n");

    file_desc = open(DEVICE_FILE_NAME, 0);
    if (file_desc < 0) {
        printf("Can't open device file: %s\n", DEVICE_FILE_NAME);
        exit(-1);
    }
    printf("Device File Successfully Opened = %d\n", file_desc);

    /* Allocating buffer for Read */
    read_buffer = malloc(READ_BUFFER_SIZE);
    /* Allocating buffer for IRQ */
    read_irq_buffer_1 = malloc(READ_BUFFER_SIZE);
    /* Allocating buffer for IRQ */
    read_irq_buffer_2 = malloc(READ_BUFFER_SIZE);
    /* Allocating buffer for IRQ */
    read_irq_buffer_3 = malloc(READ_BUFFER_SIZE);

    /* Allocating buffer for Identify command */
    identify_buffer = malloc(4096);
    if (identify_buffer == NULL) {
        printf("Malloc Failed");
        return 0;
    }
    /* Allocating buffer for Discontiguous IOSQ and setting to 0 */
    if (posix_memalign(&discontg_sq_buf, 4096, DISCONTIG_IO_SQ_SIZE)) {
        printf("Memalign Failed");
        return 0;
    }
    memset(discontg_sq_buf, 0, DISCONTIG_IO_SQ_SIZE);

    /* Allocating buffer for Discontiguous IOCQ and setting to 0 */
    if (posix_memalign(&discontg_cq_buf, 4096, DISCONTIG_IO_CQ_SIZE)) {
        printf("Memalign Failed");
        return 0;
    }
    memset(discontg_cq_buf, 0, DISCONTIG_IO_CQ_SIZE);

    /* Allocating buffer for write buffer wo merta data */
    if (posix_memalign(&write_buffer_wo_meta, 16, READ_BUFFER_SIZE)) {
        printf("Memalign Failed");
        return 0;
    }

    write_buffer = malloc(READ_BUFFER_SIZE);
    if (write_buffer == NULL) {
        printf("Malloc Failed");
        return 0;
    }
    /* Writing values's to first page */
    for (i =0 ; i<READ_BUFFER_SIZE ; i++)
    {
        *(char *)(write_buffer + i) = i;
        *(char *)(write_buffer_wo_meta + i) = i;
    }

    /* Allocating buffer for Discontiguous IOCQ and setting to 0 */
    if (posix_memalign(&irq_dcq_buf, 4096, PAGE_SIZE_I * 16)) {
        printf("Memalign Failed");
        return 0;
    }


    /* Allocating buffer for Discontiguous IOSQ and setting to 0 */
    if (posix_memalign(&irq_dsq_buf, 4096, PAGE_SIZE_I * 64)) {
        printf("Memalign Failed");
        return 0;
    }

    test_drv_metrics(file_desc);
    test_dev_metrics(file_desc);

    do {
        printf("\nEnter a valid test case number:");
        fflush(stdout);
        scanf ("%d", &test_case);
        switch(test_case) {
        case 1: /* Create Admin */
            printf("Test1: Initializing the state of the device to Run tests\n");

            /* This disable is commented here to make this test case
             * just not to disable controller here */
            /*
             * printf("Calling Contoller State to set to Disable state\n");
             * ioctl_disable_ctrl(file_desc, ST_DISABLE);
             */
            test_admin(file_desc);
            printf("\n.Test PASS if creation is success.");
            printf("\nCalling Controller State to set to Enable state\n");
            ioctl_enable_ctrl(file_desc);
            printf("\nWriting the Registers of NVME space\n");
            ioctl_write_data(file_desc);
            printf("\nTest to initialize the state of controller done\n");

            printf("\nCalling Dump Metrics to tmpfile1\n");
            ioctl_dump(file_desc, tmpfile1);
            break;
        case 2:
            printf("Test2: Disabling the controller completely\n");
            ioctl_disable_ctrl(file_desc, ST_DISABLE_COMPLETELY);
            ioctl_dump(file_desc, "/tmp/temp_irq2.txt");
            break;
        case 3:
            printf("Test3: Sending Create Discontig IOSQ with ID 1"
            " and discontig IOCQ with ID 2\n");
            printf("\n Preparing discontig CQ with ID 2\n");
            printf("\n\tCQ ID = 2\n");
            ioctl_prep_cq(file_desc, 2, 65280, 0);

            printf("\n Preparing Discontig SQ with ID 1\n");
            printf("\n\tSD_ID : CQ ID = 1 : 2\n");
            ioctl_prep_sq(file_desc, 1, 2, 65472, 0);

            printf("Executing SEND 64 byte command both for SQ and CQ\n");
            ioctl_create_discontig_iocq(file_desc, discontg_cq_buf);
            ioctl_create_discontig_iosq(file_desc, discontg_sq_buf);

            printf("\nCalling Dump Metrics to tmpfile1\n");
            ioctl_dump(file_desc, tmpfile1);

            printf("Ringing Doorbell for SQID 0\n");
            ioctl_tst_ring_dbl(file_desc, 0);

            printf("Test to Create Discontig IO Queues Done\n");
            break;
        case 4:
            printf("Test4: Sending Create contig IOSQ with ID 2 and contig"
            "CQ with ID1\n");

            printf("\n Preparing contig CQ with ID 1\n");
            printf("\n\tCQ ID = 1\n");
            ioctl_prep_cq(file_desc, 1, 20, 1);

            printf("\n Preparing contig SQ with ID 2\n");
            printf("\n\tSD_ID : CQ ID = 2 : 1\n");
            ioctl_prep_sq(file_desc, 2, 1, 256, 1);

            printf("Executing SEND 64 byte command\n");
            ioctl_create_contig_iocq(file_desc);
            ioctl_create_contig_iosq(file_desc);

            printf("\nCalling Dump Metrics to tmpfile2\n");
            ioctl_dump(file_desc, tmpfile2);
            printf("Ringing Doorbell for SQID 0\n");
            ioctl_tst_ring_dbl(file_desc, 0);
            printf("Test to Create contig IOSQ Done\n");
        break;
        case 5: /* Delete the Queues */
            printf("Test5: Sending Delete IO Q command to remove all Q");
            printf("Executing SEND 64 byte commands 4 at a time!\n");
            printf("Deleting IOSQ 1\n");
            ioctl_delete_ioq(file_desc, 0x00, 1);
            printf("Deleting IOSQ 2\n");
            ioctl_delete_ioq(file_desc, 0x00, 2);
            printf("Deleting IOCQ 1\n");
            ioctl_delete_ioq(file_desc, 0x04, 1);
            printf("Deleting IOCQ 2\n");
            ioctl_delete_ioq(file_desc, 0x04, 2);

            printf("Ringing Doorbell for SQID 0\n");
            ioctl_tst_ring_dbl(file_desc, 0);
            printf("Test to Delete IOQ's Done\n");
            printf("\nCalling Dump Metrics to tmpfile3\n");
            ioctl_dump(file_desc, tmpfile3);
            break;
        case 6: /* Send the identify command */
            printf("Test6: Sending Identify Command\n");
            ioctl_send_identify_cmd(file_desc, identify_buffer);
            printf("Ringing Doorbell for SQID 0\n");
            ioctl_tst_ring_dbl(file_desc, 0);
            printf("Test to send identify command Done\n");
            printf("\nCalling Dump Metrics to tmpfile4\n");
            ioctl_dump(file_desc, tmpfile4);
            break;
        case 7: /* Send an IO write command through discontig IOQ*/
            printf("Test7: Sending IO Write Command through Discontig IOQ\n");
            ioctl_send_nvme_write(file_desc, write_buffer_wo_meta);
            printf("Ringing Doorbell for SQID 1\n");
            ioctl_tst_ring_dbl(file_desc, 1);
            printf("\nCalling Dump Metrics to tmpfile5\n");
            ioctl_dump(file_desc, tmpfile5);
            printf("Test to send IO Write command Done\n");
            break;
        case 8: /* Send an IO read command through Discontig IOQ*/
            printf("Test8: Sending IO Read Command through Discontig IOQ\n");
            ioctl_send_nvme_read(file_desc, read_buffer);
            printf("Ringing Doorbell for SQID 1\n");
            ioctl_tst_ring_dbl(file_desc, 1);
            printf("\nCalling Dump Metrics to tmpfile6\n");
            ioctl_dump(file_desc, tmpfile6);
            printf("Test to send IO Read command Done\n");
            break;
        case 9: /* Reading contents of the read buffer */
            printf("\nReadin Data:\n");
            for (i = 0; i < READ_BUFFER_SIZE; i++) {
               printf("%x ",*(uint8_t *)(read_buffer +i) );
            }
            break;
        case 10: /* reap on Admin CQ for 2 elems */
            printf("\nCalling Dump Metrics to tmpfile14\n");
            ioctl_dump(file_desc, tmpfile14);
            set_reap_cq(file_desc, 0, 2, 32, 1);
            printf("\nCalling Dump Metrics to tmpfile15\n");
            ioctl_dump(file_desc, tmpfile15);
            break;
        case 11: /* reap on IO CQ for 2 elems */
            set_reap_cq(file_desc, 1, 2, 32, 1);
            set_reap_cq(file_desc, 2, 2, 32, 1);
            break;
        case 12: /* Display ACQ Contents */
            // ioctl_ut_mmap(file_desc);
            kadr = mmap(0, 4096 * 3, PROT_READ, MAP_SHARED, file_desc, 0x60000000);
            if ((int64_t)kadr == -1) {
                printf("mapping failed\n");
                break;
            } else {
                display_contents(kadr, 3 * 4096);
            }
            if ((int64_t)kadr != -1) {
                munmap(kadr, 4096);
            }
            break;
        case 13: /* Reading contents of the Identify buffer */
            printf("\nIdentify Data:\n");
            for (i = 0; i < READ_BUFFER_SIZE/2; i++) {
                 printf("%x ",*(uint8_t *)(identify_buffer +i));
            }
            break;
        case 15: /* Test Meta */
            printf("Test case for testing Meta buffers\
                    creation, allocation and deletion\n");
            test_meta(file_desc, 1);
            break;
        case 16:
            printf("Test to Create CQ's with IRQ flag set...\n");
            printf("Enter Number of CQs = ");
            fflush(stdout);
            scanf("%hu", &cq_id);
            int_vec = 1;
            for (i = 1; i <= cq_id; i++) {
                admin_create_iocq_irq(file_desc, i + RANDOM_VAL, int_vec,
                        0x03);
                int_vec++;
                printf("Ring Doorbell for SQID 0\n");
                ioctl_tst_ring_dbl(file_desc, 0);
            }
            printf("\nCalling Dump Metrics to tmpfile1\n");
            ioctl_dump(file_desc, tmpfilei);
            break;
        case 17:
            printf("Setup MSI-X interrupt scheme.\n");
            set_irq_msix(file_desc);
            printf("\nCalling Dump Metrics to tmpfile17\n");
            ioctl_dump(file_desc, tmpfile17);
            printf("Calling Device Metrics....");
            test_dev_metrics(file_desc);
            break;
        case 18:
            printf("Setup interrupt scheme to INT_NONE.\n");
            set_irq_none(file_desc);
            printf("\nCalling Dump Metrics to tmpfile18\n");
            ioctl_dump(file_desc, tmpfile18);
            break;
        case 19: /* call to disable only */
            printf("call to DISABLE only.\n");
            ioctl_disable_ctrl(file_desc, ST_DISABLE);
            ioctl_dump(file_desc, "/tmp/temp_irq19.txt");
            break;
        case 22: /* Reading contents of Discontig SQ page wise */
            printf("\nReading contents of Discontig SQ page wise:\n");
            printf("\nEnter a Page number:");
            fflush(stdout);
            scanf ("%d", &test_case);
            offset = 4096 * test_case;
            if (offset < (DISCONTIG_IO_SQ_SIZE - 4096)) {
                for (i = 0; i < 4096; i++) {
                    printf("%x ",*(uint8_t *)(discontg_sq_buf + i + offset));
                }
            } else {
              printf("\nIllelgal Page number!\n");
            }
            break;
        case 23: /* Reading contents of Discontig CQ page wise */
            printf("\nReading contents of Discontig CQ page wise:\n");
            printf("\nEnter a Page number:");
            fflush(stdout);
            scanf ("%d", &test_case);
            offset = 4096 * test_case;
            if (offset < (DISCONTIG_IO_CQ_SIZE - 4096)) {
                for (i = 0; i < 4096; i++) {
                    printf("%x ",*(uint8_t *)(discontg_cq_buf + i + offset));
                }
            } else {
              printf("\nIllelgal Page number!\n");
            }
            break;
        case 24: /* Test to check meta buffer support through contig Q */
            printf("Test20: Test to check meta buffer write command support"
                "through contig Q\n");
            printf("\nEnter meta_id:\n");
            fflush(stdout);
            scanf ("%d", &meta_id);
            test_meta_buf(file_desc, meta_id);
            /* Sending the write command through Contig SQ 2 using meta_buff */
            printf("\n Sending write IO through Contig SQ 2 using meta_buff \n");
            ioctl_send_nvme_write_using_metabuff(file_desc, meta_id, write_buffer);
            printf("Ringing Doorbell for SQID 2\n");
            ioctl_tst_ring_dbl(file_desc, 2);

            printf("\nCalling Dump Metrics to tmpfile7\n");
            getchar();
            ioctl_dump(file_desc, tmpfile7);
            break;
        case 25: /*Test to check meta buffer support through contig Q */
            printf("Test21: Test to check meta buffer read cmd support"
                "through contig Q\n");
            printf("\nEnter meta_id:\n");
            fflush(stdout);
            scanf ("%d", &meta_id);
            test_meta_buf(file_desc, meta_id);
            /* Sending the read command through Contig SQ 2 using meta_buff */
            printf("\n Sending read IO through Contig SQ 2 using meta_buff \n");
            ioctl_send_nvme_read_using_metabuff(file_desc, read_buffer, meta_id);
            printf("Ringing Doorbell for SQID 2\n");
            ioctl_tst_ring_dbl(file_desc, 2);
            printf("\nCalling Dump Metrics to tmpfile8\n");
            ioctl_dump(file_desc, tmpfile8);
            break;
        case 26: /*Delete the meta data ID */
            printf("Deleting the Meta ID's\n");
            printf("\nEnter metaid:\n");
            fflush(stdout);
            scanf ("%d", &meta_id);
            ret_val = ioctl(file_desc, NVME_IOCTL_METABUF_DELETE, meta_id);
            if(ret_val < 0) {
                printf("\nMeta Id = %d deletion failed!\n", meta_id);
            } else {
                printf("Meta Id = %d deletion success!!\n", meta_id);
            }
            break;
        case 27:
            printf("\nPerform Reap Inquiry on required CQ Id\n");
            printf("\nEnter cq id: ");
            fflush(stdout);
            scanf ("%hu", &cq_id);
            ioctl_reap_inquiry(file_desc, cq_id);
            printf("\nCalling Dump Metrics to tmpfile27\n");
            ioctl_dump(file_desc, tmpfile27);
            break;
        case 28: /* Test looping for IRQ */
            printf("IRQ loop test, sends multiple cmds, then write doorbell\n");
            test_loop_irq(file_desc);
            printf("\nCalling Dump Metrics to tmpfile28\n");
            ioctl_dump(file_desc, tmpfile28);
            break;
        case 29:
            printf("IRQ Loop Test for Memory Leak checks...Rev 568");
            test_irq_review568(file_desc);
            break;
        case 30:
            /* Test to Create 4 IO queues with IRQ's enabled */
            printf("Test to Create 4 IO queues with IRQ's enabled\n");
           /* First Create CQ before assoc CQ */
            printf("Create IO CQ's with ID (20:Discontig"
                "21-23:Contig and Interrupt Enabl.\n");
            memset(irq_dcq_buf, 0, PAGE_SIZE_I * 16);
            memset(irq_dsq_buf, 0, PAGE_SIZE_I * 64);
            set_cq_irq(file_desc, irq_dcq_buf);
            printf("Create IO SQ's with ID (31:Discontig"
		"32-34:Contig\n");
            set_sq_irq(file_desc, irq_dsq_buf);
            ioctl_dump(file_desc, "/tmp/temp_irq30_2.txt");
            break;
        case 31:
            /* ISR test to send commands through previously created 4 IO Q's */
            printf("ISR test to send commands through previously created 4 IO Q's");
            ioctl_dump(file_desc, "/tmp/temp_irq31_1.txt");
            printf("\nEnter no of commands in each CQ:");
            fflush(stdout);
            scanf ("%d", &cmds);
            for(index = 0; index < cmds; index++) {
                test_contig_threeio_irq(file_desc, read_irq_buffer_1,
                    read_irq_buffer_2, read_irq_buffer_3);
                test_discontig_io_irq(file_desc, read_buffer);
            }
            while ((uint32_t) ioctl_reap_inquiry(file_desc, 21) != cmds);
            ioctl_reap_cq(file_desc, 21, cmds, 16, 0);
            while ((uint32_t) ioctl_reap_inquiry(file_desc, 22) != cmds);
            ioctl_reap_cq(file_desc, 22, cmds, 16, 0);
	    ioctl_dump(file_desc, "/tmp/temp_irq31_2.txt");
            break;
        case 32:
            printf("IRQ Test for Deleting the IOQs");
            ioctl_dump(file_desc, "/tmp/temp_irq32_1.txt");
            test_irq_delete(file_desc);
            ioctl_dump(file_desc, "/tmp/temp_irq32_2.txt");
            break;
        case 33:
            printf("Calling Device Metrics....");
            test_dev_metrics(file_desc);
            break;
        case 34:
            printf("Testing Public Q metrics...");
            test_metrics(file_desc);
            break;
        case 35: /* call to enable only */
            printf("\nCalling Controller State to set to Enable state\n");
            ioctl_enable_ctrl(file_desc);
            ioctl_write_data(file_desc);
            ioctl_dump(file_desc, "/tmp/temp_irq35.txt");
            break;
        default:
            printf("\nUndefined case!\n");
        }
    } while (test_case < 36);

    printf("\nCalling Dump Metrics to closefile\n");
    ioctl_dump(file_desc, closefile);

    /* Exit gracefully */
    printf("\nNow Exiting gracefully....\n");
    ioctl_disable_ctrl(file_desc, ST_DISABLE_COMPLETELY);
    set_irq_none(file_desc);

    free(read_buffer);
    free(read_irq_buffer_1);
    free(read_irq_buffer_2);
    free(read_irq_buffer_3);
    free(discontg_sq_buf);
    free(discontg_cq_buf);
    free(write_buffer);
    free(write_buffer_wo_meta);
    free(irq_dcq_buf);
    free(irq_dsq_buf);
    free(identify_buffer);

    printf("\n\n****** END OF DEMO ******\n\n");
    close(file_desc);
    return 0;
}

