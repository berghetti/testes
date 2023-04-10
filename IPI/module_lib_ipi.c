


#ifndef _IPI_H_
#define _IPI_H_

enum ipi_vector {
    IPI_VECTOR_TEST,
    /* Add more IPI vectors here... */
};

int ipi_send(enum ipi_vector vector);

#endif /* _IPI_H_ */



#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/ipi.h>

#include "ipi.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("IPI Library");

static void ipi_handler(unsigned int vector, struct pt_regs *regs)
{
    /* Handle IPI interrupts if needed */
}

static int __init ipi_init(void)
{
    int ret;

    /* Register IPI interrupt handler */
    ret = request_irq(IRQ_IPI_VECTOR, ipi_handler, IRQF_PERCPU | IRQF_NOBALANCING, "ipi_library", NULL);
    if (ret) {
        pr_err("Failed to register IPI handler: %d\n", ret);
        return ret;
    }

    return 0;
}

static void __exit ipi_exit(void)
{
    /* Unregister IPI interrupt handler */
    free_irq(IRQ_IPI_VECTOR, NULL);
}

int ipi_send(enum ipi_vector vector)
{
    if (vector >= 0 && vector < NR_IPI_VECTORS) {
        /* Send IPI */
        apic->send_IPI_self(IRQ_IPI_VECTOR + vector);
        return 0;
    } else {
        return -EINVAL;
    }
}

EXPORT_SYMBOL(ipi_send);

module_init(ipi_init);
module_exit(ipi_exit);

Usage
#include <stdio.h>
#include <dlfcn.h>
#include "ipi.h"

int main(int argc, char **argv)
{
    void *ipi_library;
    int (*ipi_send_fn)(enum ipi_vector);
    int ret;

    /* Load ipi_library */
    ipi_library = dlopen("./libipi.so", RTLD_LAZY);
    if (!ipi_library) {
        fprintf(stderr, "Failed to load ipi_library: %s\n", dlerror());
        return 1;
    }

    /* Get ipi_send function pointer */
    ipi_send_fn = dlsym(ipi_library, "ipi_send");
    if (!ipi_send_fn) {
        fprintf(stderr, "Failed to get ipi_send function: %s\n", dlerror());
        dlclose(ipi_library);
        return 

