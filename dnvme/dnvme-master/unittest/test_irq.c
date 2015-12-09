#include <stdio.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdlib.h>

#include "../dnvme_interface.h"
#include "../dnvme_ioctls.h"

#include "test_metrics.h"
#include "test_irq.h"

void test_interrupts(int fd) /* Call with 16 number */
{
    int ret_val;
    struct interrupts new_irq;

    printf("Setting MSI X IRQ Type...\n");
    printf("Press any key to continue...\n");
    getchar();

    /* MSI SINGLE Testing */
    new_irq.num_irqs = 32;
    new_irq.irq_type = INT_MSIX;

    ret_val = ioctl(fd, NVME_IOCTL_SET_IRQ, &new_irq);
    if(ret_val < 0) {
        printf("\nSet IRQ MSI-X failed!\n");
    }
    else {
        printf("\nSet IRQ MSI-X success!!\n");
    }
    printf("Press any key to continue...\n");
    getchar();

    printf("Setting NONE IRQ Type...\n");
    /* Setting to INT_NONE Testing */
    new_irq.num_irqs = 1;
    new_irq.irq_type = INT_NONE;

    ret_val = ioctl(fd, NVME_IOCTL_SET_IRQ, &new_irq);
    if(ret_val < 0) {
        printf("\nSet IRQ NONE failed!\n");
    }
    else {
        printf("\nSet IRQ NONE success!!\n");
    }
    printf("Press any key to continue...\n");
    getchar();
}

void set_irq_none(int fd)
{
    int ret_val;
    struct interrupts new_irq;

    printf("Setting NONE IRQ Type...\n");
    /* Setting to INT_NONE Testing */
    new_irq.num_irqs = 0;
    new_irq.irq_type = INT_NONE;

    ret_val = ioctl(fd, NVME_IOCTL_SET_IRQ, &new_irq);
    if(ret_val < 0) {
        printf("\nSet IRQ NONE failed!\n");
    }
    else {
        printf("\nSet IRQ NONE success!!\n");
    }
}

void set_irq_msix(int fd)
{
    int ret_val;
    struct interrupts new_irq;
    printf("Setting MSI X IRQ Type...\n");

    printf("Enter desired num_irqs = ");
    fflush(stdout);
    scanf("%hu", &new_irq.num_irqs);

    new_irq.irq_type = INT_MSIX;

    ret_val = ioctl(fd, NVME_IOCTL_SET_IRQ, &new_irq);
    if(ret_val < 0) {
        printf("\nSet IRQ MSI-X failed!\n");
    }
    else {
        printf("\nSet IRQ MSI-X success!!\n");
    }
}

void test_irq_review568(int fd)
{
    int i;

    i = 10000;

    while(i) {
        printf("\nIRQ Loop Test = %d\n", i + 1);
        set_irq_msix(fd);
        i--;
    }
    set_irq_none(fd);
    printf("\nCalling Dump Metrics to irq_loop_test\n");
    ioctl_dump(fd, "/tmp/test_rev568.txt");
    printf("\nPressAny key..\n");
    getchar();
}

void test_loop_irq(int fd)
{
    int i, cmds, num;
    void *rd_buffer;
    if (posix_memalign(&rd_buffer, 4096, READ_BUFFER_SIZE)) {
        printf("Memalign Failed");
        return;
    }
    num = ioctl_reap_inquiry(fd, 0);
    /* Submit 10 cmds */
    printf("\nEnter no of commands in ACQ:");
    fflush(stdout);
    scanf ("%d", &cmds);
    for (i = 0; i < cmds; i++) {
        ioctl_send_identify_cmd(fd, rd_buffer);
    }

    ioctl_tst_ring_dbl(fd, 0); /* Ring Admin Q Doorbell */
    while(ioctl_reap_inquiry(fd, 0) != (num + cmds));

    ioctl_dump(fd, "/tmp/irq_loop_test.txt");
    free(rd_buffer);
}

int irq_for_io_discontig(int file_desc, int cq_id, int irq_no, int cq_flags,
    uint16_t elem, void *addr)
{
    int ret_val = -1;
    struct nvme_64b_send user_cmd;
    struct nvme_create_cq create_cq_cmd;

    /* Fill the command for create discontig IOSQ*/
    create_cq_cmd.opcode = 0x05;
    create_cq_cmd.cqid = cq_id;
    create_cq_cmd.qsize = elem;
    create_cq_cmd.cq_flags = cq_flags;
    create_cq_cmd.irq_no = irq_no;
    create_cq_cmd.rsvd1[0] = 0x00;

    /* Fill the user command */
    user_cmd.q_id = 0;
    user_cmd.bit_mask = MASK_PRP1_LIST;
    user_cmd.cmd_buf_ptr = (u_int8_t *) &create_cq_cmd;
    user_cmd.data_buf_size = PAGE_SIZE_I * 16;
    user_cmd.data_buf_ptr = addr;
    user_cmd.data_dir = 0;

    printf("User Call to send command\n");

    ret_val = ioctl(file_desc, NVME_IOCTL_SEND_64B_CMD, &user_cmd);
    if (ret_val < 0) {
        printf("Sending of Command Failed!\n");
    } else {
        printf("Command sent succesfully\n");
    }
    return ret_val;
}

int irq_for_io_contig(int file_desc, int cq_id, int irq_no,
    int cq_flags, uint16_t elems)
{
    int ret_val = -1;
    struct nvme_64b_send user_cmd;
    struct nvme_create_cq create_cq_cmd;

    /* Fill the command for create contig IOSQ*/
    create_cq_cmd.opcode = 0x05;
    create_cq_cmd.cqid = cq_id;
    create_cq_cmd.qsize = elems;
    create_cq_cmd.cq_flags = cq_flags;
    create_cq_cmd.irq_no = irq_no;
    create_cq_cmd.rsvd1[0] = 0x00;

    /* Fill the user command */
    user_cmd.q_id = 0;
    user_cmd.bit_mask = (MASK_PRP1_PAGE);
    user_cmd.cmd_buf_ptr = (u_int8_t *) &create_cq_cmd;
    user_cmd.data_buf_size = 0;
    user_cmd.data_buf_ptr = NULL;
    user_cmd.data_dir = 0;

    printf("User Call to send command\n");

    ret_val = ioctl(file_desc, NVME_IOCTL_SEND_64B_CMD, &user_cmd);
    if (ret_val < 0) {
        printf("Sending of Command Failed!\n");
    } else {
        printf("Command sent succesfully\n");
    }
    return ret_val;
}

void send_nvme_read(int file_desc, int sq_id, void* addr)
{
    int ret_val = -1;
    struct nvme_64b_send user_cmd;
    struct nvme_user_io nvme_read;

    /* Fill the command for create discontig IOSQ*/
    nvme_read.opcode = 0x02;
    nvme_read.flags = 0;
    nvme_read.control = 0;
    nvme_read.nsid = 1;
    nvme_read.metadata = 0;
    nvme_read.slba = 0;
    nvme_read.nlb = 15;
    nvme_read.cmd_flags = 0;
    nvme_read.dsm = 0;
    nvme_read.ilbrt = 0;
    nvme_read.lbat = 0;
    nvme_read.lbatm = 0;

    /* Fill the user command */
    user_cmd.q_id = sq_id;
    user_cmd.bit_mask = (MASK_PRP1_PAGE | MASK_PRP1_LIST |
        MASK_PRP2_PAGE | MASK_PRP2_LIST);
    user_cmd.cmd_buf_ptr = (u_int8_t *) &nvme_read;
    user_cmd.data_buf_size = READ_BUFFER_SIZE;
    user_cmd.data_buf_ptr = addr;
    user_cmd.data_dir = 0;

    printf("User Call to send command\n");

    ret_val = ioctl(file_desc, NVME_IOCTL_SEND_64B_CMD, &user_cmd);
    if (ret_val < 0) {
        printf("Sending of Command Failed!\n");
    } else {
        printf("Command sent succesfully\n");
    }
}

void send_nvme_read_mb(int file_desc, int sq_id, void* addr, uint32_t meta_id)
{
    int ret_val = -1;
    struct nvme_64b_send user_cmd;
    struct nvme_user_io nvme_read;

    /* Fill the command for create discontig IOSQ*/
    nvme_read.opcode = 0x02;
    nvme_read.flags = 0;
    nvme_read.control = 0;
    nvme_read.nsid = 1;
    nvme_read.metadata = 0;
    nvme_read.slba = 0;
    nvme_read.nlb = 15;
    nvme_read.cmd_flags = 0;
    nvme_read.dsm = 0;
    nvme_read.ilbrt = 0;
    nvme_read.lbat = 0;
    nvme_read.lbatm = 0;

    /* Fill the user command */
    user_cmd.q_id = sq_id; /* Contig SQ ID */
    user_cmd.bit_mask = (MASK_PRP1_PAGE | MASK_PRP1_LIST |
        MASK_PRP2_PAGE | MASK_PRP2_LIST);
    user_cmd.cmd_buf_ptr = (u_int8_t *) &nvme_read;
    user_cmd.data_buf_size = READ_BUFFER_SIZE;
    user_cmd.data_buf_ptr = addr;
    user_cmd.meta_buf_id = meta_id;
    user_cmd.data_dir = 0;

    printf("User Call to send command\n");

    ret_val = ioctl(file_desc, NVME_IOCTL_SEND_64B_CMD, &user_cmd);
    if (ret_val < 0) {
        printf("Sending of Command Failed!\n");
    } else {
        printf("Command sent succesfully\n");
  }
}

int admin_create_iocq_irq(int fd, int cq_id, int irq_no, int cq_flags)
{
     struct nvme_64b_send user_cmd;
     struct nvme_create_cq create_cq_cmd;
     int ret_val;

     ioctl_prep_cq(fd, cq_id, 20, 1);

     /* Fill the command for create contig IOSQ*/
     create_cq_cmd.opcode = 0x05;
     create_cq_cmd.cqid = cq_id;
     create_cq_cmd.qsize = 20;
     create_cq_cmd.cq_flags = cq_flags;
     create_cq_cmd.irq_no = irq_no;

     /* Fill the user command */
     user_cmd.q_id = 0;
     user_cmd.bit_mask = (MASK_PRP1_PAGE);
     user_cmd.cmd_buf_ptr = (u_int8_t *) &create_cq_cmd;
     user_cmd.data_buf_size = 0;
     user_cmd.data_buf_ptr = NULL;
     user_cmd.data_dir = 0;

     ret_val = ioctl(fd, NVME_IOCTL_SEND_64B_CMD, &user_cmd);
     if (ret_val < 0) {
         printf("Sending Admin Command Create IO Q Failed!\n");
     } else {
         printf("Admin Command Create IO Q SUCCESS\n\n");
     }
     return ret_val;
}


void set_cq_irq(int fd, void *p_dcq_buf)
{
    int ret_val;
    int cq_id;
    int irq_no;
    int cq_flags;
    int num;

    /* Discontig case */
    cq_id = 20;
    irq_no = 1;
    cq_flags = 0x2;
    num = ioctl_reap_inquiry(fd, 0);

    ret_val = ioctl_prep_cq(fd, cq_id, PAGE_SIZE_I, 0);
    if (ret_val < 0) exit(-1);
    ret_val = irq_for_io_discontig(fd, cq_id, irq_no, cq_flags,
        PAGE_SIZE_I, p_dcq_buf);
    ioctl_tst_ring_dbl(fd, 0);
   /* contig case */
    cq_id = 21;
    irq_no = 2;
    cq_flags = 0x3;

    ret_val = ioctl_prep_cq(fd, cq_id, PAGE_SIZE_I, 1);
    if (ret_val < 0) exit(-1);
    ret_val= irq_for_io_contig(fd, cq_id, irq_no, cq_flags, PAGE_SIZE_I);

    cq_id = 22;
    irq_no = 2;
    cq_flags = 0x3;

    ret_val = ioctl_prep_cq(fd, cq_id, PAGE_SIZE_I, 1);
    if (ret_val < 0) exit(-1);

    ret_val= irq_for_io_contig(fd, cq_id, irq_no, cq_flags, PAGE_SIZE_I);

    cq_id = 23;
    irq_no = 3;
    cq_flags = 0x3;

    ret_val = ioctl_prep_cq(fd, cq_id, PAGE_SIZE_I, 1);
    if (ret_val < 0) exit(-1);

    ret_val= irq_for_io_contig(fd, cq_id, irq_no, cq_flags, PAGE_SIZE_I);

    ioctl_tst_ring_dbl(fd, 0);
}

int irq_cr_contig_io_sq(int fd, int sq_id, int assoc_cq_id, uint16_t elems)
{
    int ret_val = -1;
    struct nvme_64b_send user_cmd;
    struct nvme_create_sq create_sq_cmd;

    /* Fill the command for create discontig IOSQ*/
    create_sq_cmd.opcode = 0x01;
    create_sq_cmd.sqid = sq_id;
    create_sq_cmd.qsize = elems;
    create_sq_cmd.cqid = assoc_cq_id;
    create_sq_cmd.sq_flags = 0x01;

    /* Fill the user command */
    user_cmd.q_id = 0;
    user_cmd.bit_mask = (MASK_PRP1_PAGE);
    user_cmd.cmd_buf_ptr = (u_int8_t *) &create_sq_cmd;
    user_cmd.data_buf_size = 0;
    user_cmd.data_buf_ptr = NULL;
    user_cmd.data_dir = 2;

    printf("User Call to send command\n");

    ret_val = ioctl(fd, NVME_IOCTL_SEND_64B_CMD, &user_cmd);
    if (ret_val < 0) {
        printf("Sending of Command Failed!\n");
    } else {
        printf("Command sent succesfully\n");
    }
    return ret_val;
}

int irq_cr_disc_io_sq(int fd, void *addr,int sq_id,
    int assoc_cq_id, uint16_t elems)
{
    int ret_val = -1;
    struct nvme_64b_send user_cmd;
    struct nvme_create_sq create_sq_cmd;

    /* Fill the command for create discontig IOSQ*/
    create_sq_cmd.opcode = 0x01;
    create_sq_cmd.sqid = sq_id;
    create_sq_cmd.qsize = elems;
    create_sq_cmd.cqid = assoc_cq_id;
    create_sq_cmd.sq_flags = 0x00;

    /* Fill the user command */
    user_cmd.q_id = 0;
    user_cmd.bit_mask = MASK_PRP1_LIST;
    user_cmd.cmd_buf_ptr = (u_int8_t *) &create_sq_cmd;
    user_cmd.data_buf_size = PAGE_SIZE_I * 64;
    user_cmd.data_buf_ptr = addr;
    user_cmd.data_dir = 2;

    ret_val = ioctl(fd, NVME_IOCTL_SEND_64B_CMD, &user_cmd);
    if (ret_val < 0) {
        printf("Sending of Command Failed!\n");
    } else {
        printf("Command sent succesfully\n");
    }
    return ret_val;
}

void set_sq_irq(int fd, void *addr)
{
    int sq_id;
    int assoc_cq_id;
    int num;
    int ret_val;

    num = ioctl_reap_inquiry(fd, 0);
    sq_id = 31;
    assoc_cq_id = 20;
    ret_val = ioctl_prep_sq(fd, sq_id, assoc_cq_id, PAGE_SIZE_I, 0);
    if (ret_val < 0) return;
    ret_val= irq_cr_disc_io_sq(fd, addr, sq_id, assoc_cq_id, PAGE_SIZE_I);
    ioctl_tst_ring_dbl(fd, 0);
    /* Contig SQ */
    sq_id = 32;
    assoc_cq_id = 21;
    ret_val = ioctl_prep_sq(fd, sq_id, assoc_cq_id, PAGE_SIZE_I, 1);
    if (ret_val < 0) return;
    ret_val = irq_cr_contig_io_sq(fd, sq_id, assoc_cq_id, PAGE_SIZE_I);

    sq_id = 33;
    assoc_cq_id = 22;
    ret_val = ioctl_prep_sq(fd, sq_id, assoc_cq_id, PAGE_SIZE_I, 1);
    if (ret_val < 0) return;
    ret_val = irq_cr_contig_io_sq(fd, sq_id, assoc_cq_id, PAGE_SIZE_I);

    sq_id = 34;
    assoc_cq_id = 23;
    ret_val = ioctl_prep_sq(fd, sq_id, assoc_cq_id, PAGE_SIZE_I, 1);
    if (ret_val < 0) return;
    ret_val = irq_cr_contig_io_sq(fd, sq_id, assoc_cq_id, PAGE_SIZE_I);

    ioctl_tst_ring_dbl(fd, 0);
}

void test_contig_threeio_irq(int fd, void *addr0, void *addr1, void *addr2)
{
  static uint32_t meta_index = 10;
  int num, sq_id, assoc_cq_id;
    /* Send read command through 3 IO SQ's */

    /* SQ:CQ 32:21 */
    sq_id = 32;
    assoc_cq_id = 21;
    test_meta_buf(fd, meta_index);
    send_nvme_read_mb(fd, sq_id, addr0, meta_index);
    ioctl_tst_ring_dbl(fd, sq_id);

    /* SQ:CQ 33:22 */
    sq_id = 33;
    assoc_cq_id = 22;
    send_nvme_read(fd, sq_id, addr1);
    ioctl_tst_ring_dbl(fd, sq_id);

    /* SQ:CQ 34:23 */

    sq_id = 34;
    assoc_cq_id = 23;
    num = ioctl_reap_inquiry(fd, assoc_cq_id);
    send_nvme_read(fd, sq_id, addr2);
    ioctl_tst_ring_dbl(fd, sq_id);
    while (ioctl_reap_inquiry(fd, assoc_cq_id) != num + 1);
    ioctl_reap_cq(fd, assoc_cq_id, 1, 16, 0);

    meta_index++;
}

void test_discontig_io_irq(int fd, void *addr)
{
    int sq_id, num,assoc_cq_id;
    /* SQ:CQ 31:20 */
    sq_id = 31;
    assoc_cq_id = 20;
    num = ioctl_reap_inquiry(fd, assoc_cq_id);
    send_nvme_read(fd, sq_id, addr);
    ioctl_tst_ring_dbl(fd, sq_id);
    while (ioctl_reap_inquiry(fd, assoc_cq_id) != num + 1);
    ioctl_reap_cq(fd, assoc_cq_id, 1, 16, 0);

}

void test_irq_delete(int fd)
{
    int op_code;
    int q_id;
    int num;

    /*
     * Host software shall delete all associated Submission Queues
     * prior to deleting a Completion Queue.
     */

    /* SQ Case */
    op_code = 0x0;
    q_id = 32;
    num = ioctl_reap_inquiry(fd, 0);
    ioctl_delete_ioq(fd, op_code, q_id);
    ioctl_tst_ring_dbl(fd, 0);
    while (ioctl_reap_inquiry(fd, 0) != num + 1);

     /* SQ Case */
    op_code = 0x0;
    q_id = 31;
    num = ioctl_reap_inquiry(fd, 0);
    ioctl_delete_ioq(fd, op_code, q_id);
    ioctl_tst_ring_dbl(fd, 0);
    while (ioctl_reap_inquiry(fd, 0) != num + 1);
    /* SQ Case */
    op_code = 0x0;
    q_id = 33;
    num = ioctl_reap_inquiry(fd, 0);
    ioctl_delete_ioq(fd, op_code, q_id);
    ioctl_tst_ring_dbl(fd, 0);
    while (ioctl_reap_inquiry(fd, 0) != num + 1);

    /* SQ Case */
    op_code = 0x0;
    q_id = 34;
    num = ioctl_reap_inquiry(fd, 0);
    ioctl_delete_ioq(fd, op_code, q_id);
    ioctl_tst_ring_dbl(fd, 0);
    while (ioctl_reap_inquiry(fd, 0) != num + 1);

    /* CQ case */
    op_code = 0x4;
    q_id = 21;
    num = ioctl_reap_inquiry(fd, 0);
    ioctl_delete_ioq(fd, op_code, q_id);
    ioctl_tst_ring_dbl(fd, 0);
    while (ioctl_reap_inquiry(fd, 0) != num + 1);

    /* CQ case */
    op_code = 0x4;
    q_id = 22;
    num = ioctl_reap_inquiry(fd, 0);
    ioctl_delete_ioq(fd, op_code, q_id);
    ioctl_tst_ring_dbl(fd, 0);
    while (ioctl_reap_inquiry(fd, 0) != num + 1);
    /* CQ case */
    op_code = 0x4;
    q_id = 23;
    num = ioctl_reap_inquiry(fd, 0);
    ioctl_delete_ioq(fd, op_code, q_id);
    ioctl_tst_ring_dbl(fd, 0);
    while (ioctl_reap_inquiry(fd, 0) != num + 1);
    /* CQ case */
    op_code = 0x4;
    q_id = 20;
    num = ioctl_reap_inquiry(fd, 0);
    ioctl_delete_ioq(fd, op_code, q_id);
    ioctl_tst_ring_dbl(fd, 0);
    while (ioctl_reap_inquiry(fd, 0) != num + 1);

}
